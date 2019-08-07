/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "Connector.h"

#include "GlobalDefines.h"

#include <assert.h>

const unsigned long Connector::CONNECTION_TIMEOUT = 5000;

void Connector::closeConnection() {

    //Resetta i network reader e writer
    netReader.initialize(0);
    netWriter.initialize(0);

    //Termina la connessione
    #ifdef OFFBOARD
    client.close();
    #endif // OFFBOARD
    #ifdef ONBOARD
    client.stop();
    #endif // ONBOARD
}

void Connector::tryConnection() {

    #ifdef ONBOARD
    unsigned char *ipAddress = parameters->getIpAddress();
    IPAddress dbIpAddress(ipAddress[0], ipAddress[1], ipAddress[2], ipAddress[3]);
    unsigned char connectionResult = client.connect(dbIpAddress, parameters->getPort());
    //Se non si puo' stabilire la connessione interrompe il tentativo
    if(connectionResult != 1) {
        client.stop();
        return;
    }
    #endif // ONBOARD
    #ifdef OFFBOARD
    client.initialize(FAKE_CLIENT_INPUT_FILENAME, FAKE_CLIENT_OUTPUT_FILENAME);
    #endif // OFFBOARD

    netReader.initialize(&client);
    netWriter.initialize(&client);

    netWriter.writeByte((unsigned char) CONNECTION_REQUEST_PACKET); //Si richiede una connessione

    unsigned char counter = 0;
    char *username = parameters->getUsername();
    while(true) {
        netWriter.writeByte((unsigned char) username[counter]);
        //Se e' stato scritto l'ultimo carattere esce dal ciclo
        if(username[counter] == '\0') {
            break;
        }
        ++counter;
    }
    counter = 0;
    char *password = parameters->getPassword();
    while(true) {
        netWriter.writeByte((unsigned char) password[counter]);
        //Se e' stato scritto l'ultimo carattere esce dal ciclo
        if(password[counter] == '\0') {
            break;
        }
        ++counter;
    }

    //Forza l'invio
    netWriter.flush();

    //Fuori scheda la connessione non ha timeout
    unsigned long requestTime = 0;
    unsigned long currentTime = 0;

    #ifdef ONBOARD
    requestTime = millis();
    #endif

    ConnectionProtocolConstants connectionRequest = CONNECTION_UNKNOWN;
    while(true) {

        //Si assicura che non siano stati ricevuti valori non previsti dal protocollo
        assert(connectionRequest == CONNECTION_UNKNOWN);

        #ifdef ONBOARD
        currentTime = millis();
        #endif // ONBOARD

        if(netReader.isAvailable()) {
            connectionRequest = (ConnectionProtocolConstants) netReader.readByte();
            if(connectionRequest == CONNECTION_ACCEPTED) {
                return; //La connessione e' stata stabilita con successo
            } else if(connectionRequest == CONNECTION_REFUSED) {
                closeConnection();
            } else {
                assert(false);
            }
        }
        //In caso sia rifiutata la connessione, sia superato il timeout o si sia resettato il contatore intero
        if(connectionRequest == CONNECTION_REFUSED || currentTime - requestTime > CONNECTION_TIMEOUT || currentTime < requestTime) {

            closeConnection();

            return;
        }
    }
}

bool Connector::isConnected() {
    return netReader.isConnected() && netWriter.isConnected();
}

void Connector::onParamInit() {
    //Tenta la riconnessione con i nuovi parametri solamente se
    //il connector era gia' connesso
    if(isConnected()) {
        closeConnection();
        tryConnection();
    }
}

NetworkReader *Connector::getReader() {
    return &netReader;
}

NetworkWriter *Connector::getWriter() {
    return &netWriter;
}

Connector::Connector(ConnectionParameters *_parameters) {
    assert(_parameters != 0);

    parameters = _parameters;

    //Imposta se stesso come listener in modo
    //da essere notificato al cambiamento dei parametri
    parameters->setOnInitListener(this);
}
