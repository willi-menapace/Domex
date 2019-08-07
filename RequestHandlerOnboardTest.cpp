/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

/**
* Test su scheda RequestHandler
* Risponde a tutte le richieste inviate dal server delle richieste
*
* Hardware richiesto:
* -Shield Ethernet
* -Shield SD
*
* Configurazione pre caricamento:
* -Inizializzare l'array ethernetMac con l'indirizzo mac dello shield ethernet
* -Impostare i parametri per la connessione al server
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

#include <assert.h>

#include <SecureFileReader.h>
#include <SecureFileWriter.h>

#include <DateTime.h>
#include <Settings.h>
#include <NetworkResources.h>
#include <RequestHandler.h>

unsigned char ethernetMac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x6F, 0x1D};

//Data e ora corrente
DateTime now;

//Parametri per la connessione del gestore di dati
unsigned char ipAddress[IP_ADDRESS_SIZE] = {192, 168, 1, 179};
const unsigned long PORT = 25565;
const char *username = "authuser";
const char *password = "authpwd";

//Impostazioni e risorse di rete
Settings settings;

//Gestore delle richieste
RequestHandler *requestHandler = 0;

long value = 0;
unsigned char deviceAddress[] = {32, 65, 165, 227, 12, 63, 183, 195};
unsigned char sensorId = 0;


void setup() {

    //Inizializza le connessioni seriali
    Serial.begin(9600);

    pinMode(53, OUTPUT); //Attiva il bus SPI

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    #ifdef DEBUG
    Serial.println("Inizializzazione SD");
    #endif //DEBUG
    assert(SD.begin(4) == true); //Inizializza la memoria SD
    #ifdef DEBUG
    Serial.println("SD inizializzata con successo");
    Serial.println("Inizializzazione Ethernet");
    #endif //DEBUG
    assert(Ethernet.begin(ethernetMac) == true); //Inizializza l'ethernet e ottiene un'indirizzo tramite DHCP
    #ifdef DEBUG
    Serial.println("Ethernet inizializzata con successo");
    #endif //DEBUG

    SecureFileWriter secureWriter;
    SecureFileReader secureReader;

    //Costruisce una lista di devices
    const int DEVICES = 4;

    unsigned char addresses[DEVICES][ADDRESS_BYTES] = {{23, 45, 123, 56, 231, 34, 35, 81},
                                                       {34, 56, 123, 34, 73, 34, 56, 81},
                                                       {23, 45, 45, 55, 66, 14, 35, 28},
                                                       {72, 45, 38, 56, 231, 34, 35, 3}};

    unsigned long maxSilencePeriods[DEVICES] = {60, 3600, 100, 40};

    const unsigned char SENSOR_COUNT = 2;
    SensorType sensorTypes[SENSOR_COUNT] = {TEMPERATURE, HUMIDITY};

    const unsigned char OUTPUT_COUNT = 3;
    OutputType outputTypes[OUTPUT_COUNT] = {RELAY, RELAY, RELAY};

    NetworkResources networkResources;
    now.initialize(1, 1, 1, 1, 1, 1, 1);

    for(int i = 0; i < DEVICES; ++i) {
        networkResources.addDevice(addresses[i], SENSOR_COUNT, sensorTypes, OUTPUT_COUNT, outputTypes, maxSilencePeriods[i], &now);
    }

    #ifdef DEBUG
    Serial.println("Scrittura file Devices e Settings");
    #endif //DEBUG

    //Scrive il file dei devices
    secureWriter.open(DEVICES_FILENAME, false);
    networkResources.writeToFile(&secureWriter);
    secureWriter.close();

    //Inizializza i parametri di connessione
    ConnectionParameters connectionParameters;
    connectionParameters.setIpAddress(ipAddress);
    connectionParameters.setPort(PORT);
    connectionParameters.setUsername(username);
    connectionParameters.setPassword(password);

    //Inizializza un file di impostazini ad hoc
    secureWriter.open(SETTINGS_FILENAME, false);
    connectionParameters.writeToFile(&secureWriter); //Scrive le stesse impostazioni anche per il server delle richieste che non viene usato
    connectionParameters.writeToFile(&secureWriter);
    secureWriter.writeByte(0); //Nessuna regola nelle impostazioni
    secureWriter.close();

    //Inizializza il le impostazioni
    secureReader.open(SETTINGS_FILENAME);
    assert(secureReader.isOpen());
    settings.initialize(&secureReader);
    secureReader.close();

    #ifdef DEBUG
    Serial.println("Scrittura completata");
    #endif //DEBUG

    //Inizializza con le impostazioni caricate il gestore delle richieste
    //L'inizializzazione avviene una sola nel setup quindi si puo' utilizzare la memoria dinamica
    requestHandler = new RequestHandler(&settings);

}

//Invia valori all'infinito
void loop() {
    #ifdef DEBUG
    Serial.println("In attesa di richieste");
    #endif //DEBUG
    requestHandler->processRequests();
    #ifdef DEBUG
    Serial.println("Tutte le richieste sono state servite");
    #endif //DEBUG
}

#endif //ONBOARD
