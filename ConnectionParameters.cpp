/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "ConnectionParameters.h"

#include "GlobalDefines.h"

#include <assert.h>

unsigned char *ConnectionParameters::getIpAddress() {
    return ipAddress;
}

unsigned long ConnectionParameters::getPort() {
    return port;
}

char *ConnectionParameters::getUsername() {
    return username;
}

char *ConnectionParameters::getPassword() {
    return password;
}

void ConnectionParameters::setIpAddress(unsigned char *_ipAddress) {
    assert(_ipAddress != 0);

    for(int i = 0; i < IP_ADDRESS_SIZE; ++i) {
        ipAddress[i] = _ipAddress[i];
    }
}

void ConnectionParameters::setPort(unsigned long _port) {
    port = _port;
}

void ConnectionParameters::setUsername(const char *_username) {
    for(int i = 0; true; ++i) {
        username[i] = _username[i];

        if(username[i] == '\0') {
            break;
        }

        //A questo punto la copia dovrebbe essere terminata
        assert(i != MAX_USERNAME_LENGTH);
    }
}

void ConnectionParameters::setPassword(const char *_password) {
    for(int i = 0; true; ++i) {
        password[i] = _password[i];

        if(password[i] == '\0') {
            break;
        }

        //A questo punto la copia dovrebbe essere terminata
        assert(i != MAX_PASSWORD_LENGTH);
    }
}

void ConnectionParameters::setOnInitListener(Connector *_listener) {
    listener = _listener;
}

void ConnectionParameters::initialize(FileReader *fileReader) {

    assert(fileReader != 0);
    assert(fileReader->isOpen());
    assert(fileReader->isAvailable());

    //Legge l'indirizzo
    for(int i = 0; i < IP_ADDRESS_SIZE; ++i) {
        ipAddress[i] = fileReader->readByte();
    }
    //Legge la porta
    port = fileReader->readLong();
    //Legge l'username
    for(int i = 0; true; ++i) {
        username[i] = (char) fileReader->readByte();
        if(username[i] == '\0') {
            break;
        }

        //A questo punto deve essere gia' stato trovato un carattere di terminazione
        assert(i != MAX_USERNAME_LENGTH);
    }
    //Legge la password
    for(int i = 0; true; ++i) {
        password[i] = (char) fileReader->readByte();
        if(password[i] == '\0') {
            break;
        }

        //A questo punto deve essere gia' stato trovato un carattere di terminazione
        assert(i != MAX_PASSWORD_LENGTH);
    }

    //Si notifica il listener del cambio di configurazione
    if(listener != 0) {
        listener->onParamInit();
    }
}

void ConnectionParameters::writeToFile(FileWriter *fileWriter) {

    assert(fileWriter != 0);
    assert(fileWriter->isOpen());

    //Scrive l'indirizzo
    for(int i = 0; i < IP_ADDRESS_SIZE; ++i) {
        fileWriter->writeByte(ipAddress[i]);
    }
    //Scrive la porta
    fileWriter->writeLong(port);
    //Scrive l'username
    for(int i = 0; true; ++i) {
        fileWriter->writeByte((unsigned char) username[i]);
        if(username[i] == '\0') {
            break;
        }

        //A questo punto deve essere gia' stato trovato un carattere di terminazione
        assert(i != MAX_USERNAME_LENGTH);
    }
    //Scrive la password
    for(int i = 0; true; ++i) {
        fileWriter->writeByte((unsigned char) password[i]);
        if(password[i] == '\0') {
            break;
        }

        //A questo punto deve essere gia' stato trovato un carattere di terminazione
        assert(i != MAX_PASSWORD_LENGTH);
    }
}

bool ConnectionParameters::equals(ConnectionParameters *other) {
    if(other == 0) {
        return false;
    }

    unsigned char *otherIpAddress = other->getIpAddress();
    for(int i = 0; i < IP_ADDRESS_SIZE; ++i) {
        if(ipAddress[i] != otherIpAddress[i]) {
            return false;
        }
    }
    if(port != other->getPort()) {
        return false;
    }
    char *otherUsername = other->getUsername();
    for(int i = 0; true; ++i) {
        if(username[i] != otherUsername[i]) {
            return false;
        }
        if(username[i] == '\0') {
            break;
        }

        assert(i != MAX_USERNAME_LENGTH);
    }
    char *otherPassword = other->getPassword();
    for(int i = 0; true; ++i) {
        if(password[i] != otherPassword[i]) {
            return false;
        }
        if(password[i] == '\0') {
            break;
        }

        assert(i != MAX_PASSWORD_LENGTH);
    }

    return true;
}

ConnectionParameters::ConnectionParameters() {
    for(int i = 0; i < IP_ADDRESS_SIZE; ++i) {
        ipAddress[i] = 0;
    }
    port = 0;
    username[0] = '\0';
    password[0] = '\0';

    listener = 0;
}
