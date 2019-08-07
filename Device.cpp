/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

/**
* Software devices
*
* Hardware richiesto
* -Modulo XBee Series 2 con firmware Router API o End Node API
*
* Configurazione pre caricamento:
* -Inizializzare e impostare all'interno di setup() il collegamento seriale utilizzato per
*  il dialogo con il modulo XBee
* -Settare CTS_PIN al numero del pin utilizzato per il controllo CTS del modulo XBee
* -Istanziare e registrare Sensori e Output all'interno di setup()
* -Settare MAX_SILENCE_PERIOD al tempo massimo di inattivita' del device
* -Settare DESCRIPTOR_BUFFER_SIZE in base al numero
*  di Sensori e Output ad una dimensione congrua a contenere il descrittore
* -Settare SENSOR_VALUES_MAX_COUNT al numero di sensori installati
*
* Seguire la procedura di esportazione per il caricamento
*/

#include "GlobalDefines.h"

#ifdef ONBOARD

#include <assert.h>

#include <XBeeSerialManager.h>
#include <XBee.h>
#include <XBeeTransmitRequest.h>
#include <XBeeReceivePacket.h>

#include <DeviceManager.h>
#include <OutputType.cpp>
#include <SensorType.cpp>
#include <FakeSensor.h>
#include <FakeOutput.h>

#include <INetDescriptor.h>
#include <INetSensorValues.h>
#include <INetOutputRequest.h>

//Pin da utilizzare per il controllo seriale in invio sul modulo XBee
#define CTS_PIN 12

//Puo'contenere al massimo da descrizione di ~15 sensori e output
#define DESCRIPTOR_BUFFER_SIZE 40

//Tempo massimo espresso in secondi che dovrebbe trascorrere tra due comunicazioni successive del device
#define MAX_SILENCE_PERIOD 120
//Numero di letture di sensori inviabili simultaneamente
#define SENSOR_VALUES_MAX_COUNT 10
//Dimensione in byte del buffer del pacchetto letture dei sensori. Calcolato  secondo le specifiche di INetSensorValues
#define SENSOR_VALUES_BUFFER_SIZE (SENSOR_VALUES_MAX_COUNT * (sizeof(long) + sizeof(unsigned char)) + 2)

//Inizializzazione modulo XBee
XBeeSerialManager serialManager(&Serial1, CTS_PIN);
XBee xBee(&serialManager);
XBeePacket xBeeReceptionPacket; //Pacchetto utilizzato per la ricezione dei messaggi

//Indirizzo di destinazione dell'unita' centrale
unsigned char coordinatorAddress[ADDRESS_BYTES] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char coordinatorNetAddress[NET_ADDRESS_BYTES] = {0xFF, 0xFE};

unsigned char deviceDescriptorLength = 0; //Il numero di byte dell'array descrittore effettivamente utilizzati
unsigned char deviceDescriptor[DESCRIPTOR_BUFFER_SIZE];
XBeeTransmitRequest *deviceDescriptorPacket;

DeviceManager deviceManager;
//Buffer per l'invio delle letture dei sensori
unsigned char sensorValuesIds[SENSOR_VALUES_MAX_COUNT];
long sensorValues[SENSOR_VALUES_MAX_COUNT];
unsigned char sensorValuesBuffer[SENSOR_VALUES_BUFFER_SIZE];

//Inizializzazione sensori e output
FakeSensor fakeSensor(TEMPERATURE, -2344421);
FakeOutput fakeOutput(RELAY);


void setup() {
    pinMode(CTS_PIN, INPUT); //Inizializza il pin di CTS per effettuare letture

    //Inizializza le connessioni seriali
    Serial.begin(9600);
    Serial1.begin(9600);

    #ifdef DEBUG
    Serial.println("Registrazione e output");
    #endif //DEBUG
    //Registrazione di tutti i sensori e di tutti gli output
    deviceManager.registerSensor(&fakeSensor, 100);
    deviceManager.registerOutput(&fakeOutput);

    #ifdef DEBUG
    Serial.println("Sensori e output registrati");
    Serial.println("Calculating descriptor");
    #endif //DEBUG

    //Calcola il descrittore che rimane inalterato per tutta l'esecuzione
    INetDescriptor descriptorCalculator(deviceDescriptor, deviceManager.getSensorsCount(),
                                        deviceManager.getSensorTypes(), deviceManager.getOutputsCount(),
                                        deviceManager.getOutputTypes(), MAX_SILENCE_PERIOD);
    deviceDescriptorLength = descriptorCalculator.getLength();

    //Costruisce il paccehtto descrittore. Viene messo in memoria dinamica essendo necessaria una sola inizializzazione
    deviceDescriptorPacket = new XBeeTransmitRequest(deviceDescriptorLength, deviceDescriptor,
                                                     coordinatorAddress, coordinatorNetAddress);

    #ifdef DEBUG
    Serial.println("Descrittore calcolato");
    #endif //DEBUG
}

void loop() {

    #ifdef DEBUG
    Serial.println("Verifica disponibilita' pacchetti XBee");
    #endif //DEBUG
    //Risponde a tutte le richieste in arrivo
    while(xBee.packetAvailable()) {
        #ifdef DEBUG
        Serial.println("Ricezione pacchetto XBee");
        #endif //DEBUG

		if (!xBee.receivePacket(&xBeeReceptionPacket)) {
            #ifdef DEBUG
            Serial.println("Pacchetto scartato: errore durante la ricezione");
            #endif //DEBUG
            continue;
        }

        XBeePacketType packetType = xBeeReceptionPacket.getType();

        //Se il pacchetto e' un pacchetto di dati applicativi
        if(packetType == RECEIVE_PACKET) {
            #ifdef DEBUG
            Serial.println("Il pacchetto contiene dati applicativi");
            Serial.println("Estrazione dati applicativi");
            #endif //DEBUG
            XBeeReceivePacket xBeeApplicationPacket(&xBeeReceptionPacket);
            //Ricostruisce il pacchetto di dati applicativi
            INetPacket applicationPacket(xBeeApplicationPacket.getApplicationData(), xBeeApplicationPacket.getApplicationDataSize());

            INetPacketType appPacketType = applicationPacket.getType();

            //Viene richiesto l'invio del descrittore
            if(appPacketType == DESCRIPTOR_REQUEST) {
                #ifdef DEBUG
                Serial.println("Richiesta descrittore");
                Serial.println("Invio descrittore");
                #endif //DEBUG
                //Invia il descrittore. Rimanendo inalterato non ha bisogno di ricalcolo
                xBee.send(deviceDescriptorPacket);
                #ifdef DEBUG
                Serial.println("Descrittore inviato");
                #endif //DEBUG
            //Viene richiesto di effettuare output
            } else if(appPacketType == OUTPUT_REQUEST) {

                #ifdef DEBUG
                Serial.println("Richiesta output");
                #endif //DEBUG

                //Ricostruisce il pacchetto di richieste di output
                INetOutputRequest outputRequest(&applicationPacket);

                unsigned char outputCount = outputRequest.getValuesCount();
                unsigned char *outputIds = outputRequest.getIds();
                long *outputValues = outputRequest.getValues();

                #ifdef DEBUG
                Serial.println("Output in corso");
                #endif //DEBUG

                //Effettua tutti gli output richiesti
                for(unsigned char i = 0; i < outputCount; ++i) {
                    deviceManager.performOutput(outputIds[i], outputValues[i]);
                }

                #ifdef DEBUG
                Serial.println("Output effettuato");
                #endif //DEBUG
            }
        }
        #ifdef DEBUG
        else {
            Serial.print("Il pacchetto non contiene dati applicativi: ");
            Serial.println((int) packetType);
        }
        #endif //DEBUG
    }

    #ifdef DEBUG
    Serial.println("Tutti i pacchetti XBee sono stati letti");
    Serial.println("Refresh sensori in corso");
    #endif //DEBUG

    deviceManager.refreshSensors();
    unsigned char valuesCount = deviceManager.getAvailableValuesCount();

    #ifdef DEBUG
    if(valuesCount > 0) {
        Serial.print("Sono stati letti: ");
        Serial.print(valuesCount);
        Serial.println("valori");
    }
    #endif //DEBUG

    if(valuesCount > 0) {
        //Si assicura che non si stia tentando di inviare troppo valori simultaneamente
        assert(valuesCount <= SENSOR_VALUES_MAX_COUNT);

        #ifdef DEBUG
        Serial.println("Estrazione lettura");
        #endif //DEBUG

        deviceManager.getAvailableValues(sensorValuesIds, sensorValues);
        INetSensorValues valuesPacket(sensorValuesBuffer, valuesCount, sensorValuesIds, sensorValues);

        //Costruisce e invia il pacchetto contenente i valori
        XBeeTransmitRequest xBeeValuesPacket(valuesPacket.getLength(), valuesPacket.getData(),
                                         coordinatorAddress, coordinatorNetAddress);

        #ifdef DEBUG
        Serial.println("Invio letture");
        #endif //DEBUG
        xBee.send(&xBeeValuesPacket);
        #ifdef DEBUG
        Serial.println("Letture inviate");
        #endif //DEBUG
    }

}

#endif //ONBOARD
