/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "RequestHandler.h"

#include "GlobalDefines.h"

#include "SecureFileReader.h"
#include "SecureFileWriter.h"

#include <assert.h>

const unsigned long RequestHandler::CONNECTION_RETRY_PERIOD = 120000;

const unsigned long RequestHandler::DEVICE_SEND_TIMEOUT = 5000;
const unsigned long RequestHandler::SETTINGS_UPDATE_TIMEOUT = 10000;

void RequestHandler::tryConnection() {

    connector.tryConnection();
    if(connector.isConnected()) {
        netReader = connector.getReader();
        netWriter = connector.getWriter();
    }

}

bool RequestHandler::isConnected() {
    return netReader != 0 && netWriter != 0 && netReader->isConnected() && netWriter->isConnected();
}

void RequestHandler::respondToAllRequests() {
    while(netReader->isAvailable()) {

        RequestProtocolConstants currentRequest = (RequestProtocolConstants) (netReader->readByte());

        if(currentRequest == DEVICE_LIST_REQUEST) {
            bool result = sendDeviceList();
            //Ci sono stati errori durante l'invio
            if(result == false) {
                closeConnection();
                return;
            }
        } else if(currentRequest == SETTINGS_UPDATE_REQUEST) {
            bool result = updateSettings(); //Puo' provocare la caduta della connessione
            //Ci sono stati errori
            if(result == false) {
                closeConnection();
                return;
            }
        } else {
            //Richiesta sconosciuta
            assert(false);
        }

        //Se cade la connessione chiude tutte le risorse associate
        if(!isConnected()) {
            closeConnection();
            return;
        }
    }
}

bool RequestHandler::sendDeviceList() {
    SecureFileReader devicesReader;
    devicesReader.open(DEVICES_FILENAME);

    assert(isConnected());

    unsigned long bytesToWrite = devicesReader.getSize();
    unsigned long writtenBytes = 0;

    //Invia la dimensione dei dati da ricevere al server
    netWriter->writeLong(bytesToWrite);

    while(devicesReader.isAvailable()) {
        netWriter->writeByte(devicesReader.readByte());
        ++writtenBytes;
    }

    //Forza l'invio di tutti i byte del messaggio
    netWriter->flush();

    devicesReader.close();

    assert(writtenBytes == bytesToWrite);

    unsigned long sendTerminationTime = 0;
    #ifdef ONBOARD
    sendTerminationTime = millis();
    #endif // ONBOARD

    unsigned long currentTime = 0;
    while(true) {
        #ifdef ONBOARD
        currentTime = millis();
        #endif // ONBOARD

        if(netReader->isAvailable()) {
            RequestProtocolConstants response = (RequestProtocolConstants) netReader->readByte();
            if(response == DEVICE_LIST_RECEIVED) {
                return true;
            } else {
                //Se si riceve qualcosa deve essere una risposta positiva
                assert(false);
            }
        }

        //Se e' scaduto il timeout oppure se il contatore si e' resettato
        if(currentTime - sendTerminationTime > DEVICE_SEND_TIMEOUT || currentTime < sendTerminationTime) {
            return false;
        }

        if(!isConnected()) {
            return false;
        }
    }

    return true;
}

bool RequestHandler::updateSettings() {
    SecureFileWriter settingsWriter;
    settingsWriter.open(SETTINGS_FILENAME, false);

    unsigned long receptionStartTime = 0;
    unsigned long currentTime = 0;
    #ifdef ONBOARD
    receptionStartTime = millis();
    currentTime = millis();
    #endif // ONBOARD

    bool sizeReceived = false;
    unsigned long settingsSize = 0;
    unsigned long receivedBytes = 0; //I byte di payload ricevuti (non sono inclusi i byte di dimensione del file)

    //Finche' il timeout non scade e il contatore non viene resettato prova a ricevere il file
    while(currentTime - receptionStartTime  < SETTINGS_UPDATE_TIMEOUT && currentTime >= receptionStartTime) {
        #ifdef ONBOARD
        currentTime = millis();
        #endif // ONBOARD

        if(!isConnected()) {
            return false;
        }

        while(netReader->isAvailable()) {
            if(!sizeReceived) {
                if(netReader->isAvailable() >= sizeof(unsigned long)) {
                    settingsSize = netReader->readLong();
                    sizeReceived = true;
                }
            } else {
                settingsWriter.writeByte(netReader->readByte());
                ++receivedBytes;
                //Se tutti i byte di settings sono stati ricevuti ferma la ricezione
                if(receivedBytes == settingsSize) {
                    break;
                }
            }
        }

        if(receivedBytes == settingsSize) {
            settingsWriter.close();

            //Segnala l'avvenuta ricezione
            netWriter->writeByte(SETTINGS_UPDATE_RECEIVED);
            netWriter->flush();
            //Carica le nuove impostazioni
            SecureFileReader settingsReader;
            settingsReader.open(SETTINGS_FILENAME);

            //Ricarica le impostazioni
            settings->initialize(&settingsReader);

            settingsReader.close();
            return true;
        }
    }

    //Ci sono stati errori durante la ricezione quindi scarta i byte letti
    settingsWriter.abort();
    return false;
}

void RequestHandler::closeConnection() {

    connector.closeConnection();

    netReader = 0;
    netWriter = 0;
}

void RequestHandler::processRequests() {
    if(isConnected()) {
        //La connessione con il server delle richieste e' presente
        respondToAllRequests();
    } else {
        unsigned long currentTime = 0;

        #ifdef ONBOARD
        currentTime = millis();
        #endif // ONBOARD

        //E' passato il tempo di guardia o il contatore si e' resettato
        if(currentTime - lastConnectionAttempt > CONNECTION_RETRY_PERIOD || currentTime < lastConnectionAttempt) {
            //Ritenta la connessione con il server delle richiesete
            tryConnection();

            //Se la connessione ha successo
            if(isConnected()) {
                //La connessione con il server delle richieste e' presente
                respondToAllRequests();
            } else {
                #ifdef ONBOARD
                //Imposta la data dell'ultimo tentativo di connessione
                lastConnectionAttempt = millis();
                #endif // ONBOARD
            }
        }
    }
}

RequestHandler::RequestHandler(Settings *_settings) : connector(_settings->getRequestServerParameters()) {
    assert(_settings != 0);

    netReader = 0;
    netWriter = 0;

    settings = _settings;
    lastConnectionAttempt = 4294967294; //Forza il tentativo di connessione all'avvio
}
