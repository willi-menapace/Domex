/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

/**
* Software unita' centrale
*
* Hardware richiesto:
* -Shield Ethernet
* -Shield SD
* -XBee Series 2 con firmware Coordinator API
* -Modulo RTC
*
* Configurazione pre caricamento:
* -Scrivere sulla scheda SD in modalita' sicura un file di Settings contenente una configurazione valida
* -Inizializzare l'array ethernetMac con l'indirizzo mac dello shield ethernet
* -Inizializzare e impostare all'interno di setup() il collegamento seriale utilizzato per
*  il dialogo con il modulo XBee
* -Settare CTS_PIN al numero del pin utilizzato per il controllo CTS del modulo XBee
*
* Seguire la procedura di esportazione per il caricamento
*/

#include "GlobalDefines.h"

#ifdef ONBOARD

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Ethernet.h>
#include <SD.h>
#include <utility/w5100.h>

#include <assert.h>

#include <XBeeSerialManager.h>
#include <XBee.h>
#include <XBeeReceivePacket.h>
#include <XBeeTransmitRequest.h>

#include <INetPacket.h>
#include <INetDescriptor.h>
#include <INetSensorValues.h>

#include <SecureFileReader.h>
#include <SecureFileWriter.h>

#include <DateTime.h>
#include <Settings.h>
#include <NetworkResources.h>
#include <DataManager.h>
#include <RequestHandler.h>
#include <OutputType.cpp>
#include <SensorType.cpp>

//Pin da utilizzare per il controllo seriale in invio sul modulo XBee
#define CTS_PIN 12

//Il numero massimo di pacchetti XBee da processare ad ogni iterazione del loop principale di programma
#define MAX_XBEE_PACKETS_PER_LOOP 100

unsigned char ethernetMac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x6F, 0x1D};

//Data e ora corrente
DateTime now;

//Inizializzazione modulo XBee
XBeeSerialManager serialManager(&Serial1, CTS_PIN);
XBee xBee(&serialManager);
XBeePacket xBeeReceptionPacket; //Pacchetto utilizzato per la ricezione dei messaggi

//Impostazioni e risorse di rete
Settings settings;
NetworkResources networkResources;

//Gestori di dati e delle richieste
DataManager *dataManager = 0;
RequestHandler *requestHandler = 0;

//Indica se nel ciclo corrente sono stati registrati
bool newValuesReceived = false;
//Indica se nel ciclo corrente sono stati scoperti nuovi device
bool newDevicesDiscovered = false;

//Numero di pacchetti processati nel loop di programma corrente
long processedPackets = 0;

void setup() {

    pinMode(CTS_PIN, INPUT); //Inizializza il pin di CTS per effettuare letture
    pinMode(53, OUTPUT); //Attiva il bus SPI

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    //Inizializza le connessioni seriali
    Serial.begin(115200);
    Serial1.begin(9600);

    #ifdef DEBUG
    Serial.println(F("Inizializzazione SD"));
    #endif //DEBUG
    assert(SD.begin(4) == true); //Inizializza la memoria SD
    #ifdef DEBUG
    Serial.println(F("SD inizializzata"));
    Serial.println(F("Inizializzazione Ethernet"));
    #endif //DEBUG
    assert(Ethernet.begin(ethernetMac) == true); //Inizializza l'ethernet e ottiene un'indirizzo tramite DHCP

	//Imposta un timeout breve per la connessione
	W5100.setRetransmissionTime(0x07D0);
	W5100.setRetransmissionCount(3);

    #ifdef DEBUG
    Serial.println(F("Ethernet inizializzata"));
    #endif //DEBUG

    SecureFileWriter secureWriter;
    SecureFileReader secureReader;

    #ifdef DEBUG
    Serial.println(F("Caricamento impostazioni"));
    #endif //DEBUG

    //Inizializza il le impostazioni
    secureReader.open(SETTINGS_FILENAME);
    assert(secureReader.isOpen());
    settings.initialize(&secureReader);
    secureReader.close();

    #ifdef DEBUG
    Serial.println(F("Impostazioni caricate"));
    #endif //DEBUG

    #ifdef DEBUG
    Serial.println(F("Controllo elenco devices"));
    #endif //DEBUG
    //Se disponibili salvataggi carica le risorse di rete
    secureReader.open(DEVICES_FILENAME);
    if(secureReader.isOpen()) {
        #ifdef DEBUG
        Serial.println(F("Il file esiste. Caricamento elenco devices"));
        #endif //DEBUG
        networkResources.initialize(&secureReader);
        #ifdef DEBUG
        Serial.println(F("Elenco devices caricato"));
        #endif //DEBUG
    }
    secureReader.close();

    //Inizializza con le impostazioni caricate i gestori di dati e di richieste dal server
    //L'inizializzazione avviene una sola nel setup quindi si puo' utilizzare la memoria dinamica
    dataManager = new DataManager(&settings);
    requestHandler = new RequestHandler(&settings);

    now.initialize(1, 1, 1, 1, 1, 1, 1);
}

void loop() {

    //Ad inizio loop non sono stati processati pacchetti
    processedPackets = 0;

	//Rinnova il lease DHCP periodicamente
	Ethernet.maintain();

    //TODO inserire il codice per aggiornare la data e ora corrente
    //TODO vedere se e' meglio fare un po' di array e variabili globali
    //TODO le regole vanno anche verificate a intervalli regolari e all'aggiornamento del file di impostazioni

    #ifdef DEBUG
    Serial.println(F("Elaborazione richieste esterne"));
    #endif //DEBUG

    //Processa tutte le richieste provenienti dall'esterno
    requestHandler->processRequests();

    #ifdef DEBUG
    Serial.println(F("Richieste esterne elaborate"));
    #endif //DEBUG

    #ifdef DEBUG
    Serial.println(F("Verifica disponibilita' pacchetti XBee"));
    #endif //DEBUG
    //Processa tutti i pacchetti in arrivo
    while(true) {
		++processedPackets;
		//Se si sono processati il numero massimo di pacchetti nell'iterazione
		//corrente forza l'uscita dal loop
		if (processedPackets == MAX_XBEE_PACKETS_PER_LOOP) {
			break;
		}

		if(!xBee.packetAvailable()) {
			break;
		}

        #ifdef DEBUG
        Serial.println(F("Ricezione pacchetto XBee"));
        #endif //DEBUG
		//In caso di errori di ricezione
		if (!xBee.receivePacket(&xBeeReceptionPacket)) {
			#ifdef DEBUG
			Serial.println(F("Pacchetto scartato: errore in ricezione"));
			#endif //DEBUG
			continue;
		}


        XBeePacketType xBeePacketType = xBeeReceptionPacket.getType();

        //Se si tratta di dati applicativi ne estrae il tipo e li processa
        if(xBeePacketType == RECEIVE_PACKET) {
            #ifdef DEBUG
            Serial.println(F("Estrazione dati applicativi."));
            #endif //DEBUG

            XBeeReceivePacket xBeeReceivePacket(&xBeeReceptionPacket);

            INetPacket applicationPacket(xBeeReceivePacket.getApplicationData(), xBeeReceivePacket.getApplicationDataSize());

            INetPacketType iNetPacketType = applicationPacket.getType();

            if(iNetPacketType == SENSOR_VALUES) {

				#ifdef DEBUG
				Serial.print(F("Bytes in the serial buffer from the arduino: "));
				Serial.println(Serial1.available());
				#endif //DEBUG

                #ifdef DEBUG
                Serial.println(F("Letture di sensori ricevute"));
                Serial.println(F("Estrazione dati"));
                #endif //DEBUG

                INetSensorValues iNetSensorValues(&applicationPacket);

                unsigned char valuesCount = iNetSensorValues.getValuesCount();
                unsigned char *sensorIds = iNetSensorValues.getIds();
                long *values = iNetSensorValues.getValues();

                //Recupera le informazioni sul device che ha inviato i valori
                NetworkDevice *senderDevice = networkResources.getDeviceByAddress(xBeeReceivePacket.getSourceAddress());

                //Se il device che ha inviato il valore e' sconosciuto invia una richiesta di descriptor
                if(senderDevice == 0) {
                    #ifdef DEBUG
                    Serial.println(F("Il descrittore del mittente non e' conosciuto"));
                    Serial.println(F("Invio richiesta descrittore"));
                    #endif //DEBUG
                    unsigned char payload = (unsigned char) DESCRIPTOR_REQUEST; //Il payload e' un singolo byte
                    XBeeTransmitRequest xBeeDescriptorRequest(1, &payload, xBeeReceivePacket.getSourceAddress(),
                                                              xBeeReceivePacket.getSourceNetAddress());
                    xBee.send(&xBeeDescriptorRequest);
                    #ifdef DEBUG
                    Serial.println(F("Richiesta descrittore inviata"));
                    #endif //DEBUG
                } else {
                    #ifdef DEBUG
                    Serial.println(F("Memorizzazione valori nella cache per il controllo delle regole"));
                    #endif //DEBUG
                    //Imposta gli ultimi valori letti
                    for(unsigned char i = 0; i < valuesCount; ++i) {
                        senderDevice->setLatestReading(sensorIds[i], values[i]);
                    }

                    //Segnala che sono stati registrati nuovi valori
                    newValuesReceived = true;
                }

                #ifdef DEBUG
                Serial.println(F("Memorizzazione permanente dei valori"));
                #endif //DEBUG
                //Effettua il salvataggio dei valori letti
                for(unsigned char i = 0; i < valuesCount; ++i) {
                    dataManager->store(values[i], xBeeReceivePacket.getSourceAddress(), sensorIds[i], &now);
                }

            } else if(iNetPacketType == DESCRIPTOR) {
                #ifdef DEBUG
                Serial.println(F("Descrittore ricevuto"));
                #endif //DEBUG
                //Se il descrittore non proviene da un device gia' noto
                if(networkResources.getDeviceByAddress(xBeeReceivePacket.getSourceAddress()) == 0) {
                    #ifdef DEBUG
                    Serial.println(F("Estrazione dati"));
                    #endif //DEBUG
                    INetDescriptor iNetDescriptor(&applicationPacket);

                    unsigned char sensorsCount = iNetDescriptor.getSensorsCount();
                    SensorType sensorTypes[sensorsCount];
                    for(unsigned char i = 0; i < sensorsCount; ++i) {
                        sensorTypes[i] = iNetDescriptor.getSensorById(i);
                    }

                    unsigned char outputsCount = iNetDescriptor.getOutputsCount();
                    OutputType outputTypes[outputsCount];
                    for(unsigned char i = 0; i < outputsCount; ++i) {
                        outputTypes[i] = iNetDescriptor.getOutputById(i);
                    }

                    #ifdef DEBUG
                    Serial.println(F("Aggiunta nuovo descrittore"));
                    #endif //DEBUG
                    networkResources.addDevice(xBeeReceivePacket.getSourceAddress(), sensorsCount, sensorTypes,
                                               outputsCount, outputTypes, iNetDescriptor.getMaxSilencePeriod(), &now);

                    //Indica che sono stati aggiunti nuovi devices
                    newDevicesDiscovered = true;
                }
            }

        }
        #ifdef DEBUG
        else {
            Serial.println(F("Il pacchetto non contiene dati applicativi"));
        }
        #endif //DEBUG
    }

    #ifdef DEBUG
    Serial.println(F("Rimozione device inattivi"));
    #endif //DEBUG
    unsigned char removedDevicesCount = networkResources.removeDeadDevices(&now);
    #ifdef DEBUG
    Serial.print(F("Devices rimossi: "));
    Serial.println(removedDevicesCount);
    #endif //DEBUG


    //Se i valori dei dispositivi sono stati aggiornati verifica ed esegue le regole
    if(newValuesReceived) {
        newValuesReceived = false;
        #ifdef DEBUG
        Serial.println(F("Verifica ed esecuzione regole"));
        #endif //DEBUG
        settings.verifyAndExecuteRules(&networkResources, &now);
        #ifdef DEBUG
        Serial.println(F("Le regole sono state eseguite"));
        #endif //DEBUG
    }

    //Se sono stati scoperti nuovi device o ne sono stati rimossi
    // salva in maniera persistente la nuova lista di device
    if(newDevicesDiscovered || removedDevicesCount != 0) {
        #ifdef DEBUG
        Serial.println(F("Aggiornamento lista permanente dispositivi"));
        #endif //DEBUG

        newDevicesDiscovered = false;
        SecureFileWriter secureWriter;
        secureWriter.open(DEVICES_FILENAME, false);

        networkResources.writeToFile(&secureWriter);

        secureWriter.close();
        #ifdef DEBUG
        Serial.println(F("Lista permanente dispositivi aggiornata"));
        #endif //DEBUG
    }
}

#endif //ONBOARD


