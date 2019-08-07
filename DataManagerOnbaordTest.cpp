/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

/**
* Test su scheda DataManager
* Richiede regolarmente al DataManager la memorizzazione di dati
* generati casualmente
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
#inculde <SPI.h>
#include <Wire.h>
#include <Ethernet.h>
#include <SD.h>

#include <assert.h>/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

/**
* Test su scheda DataManager
* Richiede regolarmente al DataManager la memorizzazione di dati
* generati casualmente
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
#include <DataManager.h>

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

//Gestori di dati
DataManager *dataManager = 0;

long value = 0;
unsigned char deviceAddress[] = {32, 65, 165, 227, 12, 63, 183, 195};
unsigned char sensorId = 0;


void setup() {

    pinMode(53, OUTPUT); //Attiva il bus SPI

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    assert(SD.begin(4) == true); //Inizializza la memoria SD
    assert(Ethernet.begin(ethernetMac) == true); //Inizializza l'ethernet e ottiene un'indirizzo tramite DHCP

    //Inizializza le connessioni seriali
    Serial.begin(9600);

    SecureFileWriter secureWriter;
    SecureFileReader secureReader;

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

    //Inizializza con le impostazioni caricate il gestore di dati
    //L'inizializzazione avviene una sola nel setup quindi si puo' utilizzare la memoria dinamica
    dataManager = new DataManager(&settings);

}

//Invia valori all'infinito
void loop() {
    now.initialize(1, 1, 1, 1, 1, 1, 1); //Inizializza la data a caso
    ++value;
    if(value == 10000) {
        value = 0;
    }

    ++sensorId;
    if(sensorId == 255) {
        sensorId = 0;
    }

    dataManager->store(value, deviceAddress, sensorId, &now);

    delay(500);
}

#endif //ONBOARD


#include "SecureFileReader.h"
#include "SecureFileWriter.h"

#include "DateTime.h"
#include "Settings.h"
#include "DataManager.h"

unsigned char ethernetMac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x6F, 0x1D};

//Data e ora corrente
DateTime now;

//Parametri per la connessione del gestore di dati
unsigned char ipAddress[IP_ADDRESS_SIZE] = {192, 168, 1, 197};
const unsigned long PORT = 25565;
const char *username = "authuser";
const char *password = "authpwd";

//Impostazioni e risorse di rete
Settings settings;

//Gestori di dati
DataManager *dataManager = 0;

long value = 0;
unsigned char deviceAddress[] = {32, 65, 165, 227, 12, 63, 183, 195};
unsigned char sensorId = 0;


void setup() {

    pinMode(53, OUTPUT); //Attiva il bus SPI

    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    assert(SD.begin(4) == true); //Inizializza la memoria SD
    assert(Ethernet.begin(ethernetMac) == true); //Inizializza l'ethernet e ottiene un'indirizzo tramite DHCP

    //Inizializza le connessioni seriali
    Serial.begin(9600);

    SecureFileWriter secureWriter;
    SecureFileReader secureReader;

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

    //Inizializza con le impostazioni caricate il gestore di dati
    //L'inizializzazione avviene una sola nel setup quindi si puo' utilizzare la memoria dinamica
    dataManager = new DataManager(&settings);

}

//Invia valori all'infinito
void loop() {
    now.initialize(1, 1, 1, 1, 1, 1, 1); //Inizializza la data a caso
    ++value;
    if(value == 10000) {
        value = 0;
    }

    ++sensorId;
    if(sensorId == 255) {
        sensorId = 0;
    }

    dataManager->store(value, deviceAddress, sensorId, &now);

    delay(500);
}

#endif //ONBOARD

