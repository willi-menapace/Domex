/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBeeLibraryTestCases.h"

#include "GlobalDefines.h"

#ifdef OFFBOARD

#include "XBeeSerialManager.h"
#include "XBeeATCommand.h"
#include "XBeeTransmitRequest.h"
#include "XBeeRemoteATCommand.h"

#include <fstream>
#include <assert.h>

/**
* Ritorna true se i contenuti del file e dello stream coincidono
*
* @param stream lo stream contenente i dati da verificare, checksum esclusa
* @param verificationFile il file contenente i byte che dovrebbero trovarsi nello stream
*
* @return true se lo stream contiene i byte come elencato nel file di verifica
*/
bool verifyStream(Stream *stream, std::string verificationFile) {
    std::ifstream verification(verificationFile.c_str());

    unsigned char *buffer = stream->getBuffer();
    unsigned long bytesCount = stream->getBytesCount();

    int theoreticalByte;
    for(unsigned int i = 0; i < bytesCount - 1; ++i) {
        verification >> theoreticalByte;
        if((unsigned char) theoreticalByte != buffer[i]) {
            verification.close();
            return false; //Un byte non coincide
        }

    }

    verification.close();
    return true; //Tutti i byte coincidono
}

/**
* Effettua la lettura di un buffer da file contenente i dati in formato numerico
* con dati separati da spazio es. 23 41 143 233
*
* @param file il file da cui leggere i dati
* @param size la dimensione del buffer da riempire
* @param buffer il buffer da riempire
*/
void fillByteBuffer(std::ifstream *file, int size, unsigned char *buffer) {
    int readNumber;
    for(int i = 0; i < size; ++i) {
        *file >> readNumber;
        buffer[i] = (unsigned char) readNumber;
    }
}

/**
* Verifica che un pacchetto sia stato costruito secondo quanto descritto
* dal file di verifica
*
* @param packet il pacchetto da verificare
* @param verification il file contenente i dati per la verifica
*
* @return true se il pacchetto e' stato costruito come descritto dal file di verifica
*/
bool verifyPacket(XBeePacket *packet, std::string verification) {
    //Crea uno stream in cui contenere i dati del pacchetto
    Stream fakeStream;
    XBeeSerialManager manager(&fakeStream, 0);

    //Vengono scritti i byte sul buffer
    packet->send(&manager);

    return verifyStream(&fakeStream, verification); //Verifica che lo stream combaci con il file
}

void xBeeATCommandTest(std::string parameters, std::string verificationFile) {

    //Carica i parametri da file
    std::ifstream parametersFile(parameters.c_str());

    unsigned char atCommand[2];

    parametersFile >> atCommand[0];
    parametersFile >> atCommand[1];

    int parametersSize;

    parametersFile >> parametersSize;

    unsigned char parametersArray[parametersSize];

    fillByteBuffer(&parametersFile, parametersSize, parametersArray);

    parametersFile.close();

    //Crea il pacchetto
    XBeeATCommand packet(atCommand, parametersSize, parametersArray);

    assert(verifyPacket(&packet, verificationFile));
}

void xBeeTransmitRequestTest(std::string parameters, std::string verification) {

    std::ifstream parametersFile(parameters.c_str());

    unsigned char address[ADDRESS_BYTES];
    unsigned char netAddress[2];

    fillByteBuffer(&parametersFile, ADDRESS_BYTES, address);
    fillByteBuffer(&parametersFile, 2, netAddress);

    int dataSize;
    parametersFile >> dataSize;

    unsigned char data[dataSize];
    fillByteBuffer(&parametersFile, dataSize, data);

    parametersFile.close();

    XBeeTransmitRequest packet(dataSize, data, address, netAddress);

    assert(verifyPacket(&packet, verification));
}

void xBeeRemoteATCommandTest(std::string parameters, std::string verification) {

    //Carica i parametri da file
    std::ifstream parametersFile(parameters.c_str());

    unsigned char atCommand[2];

    parametersFile >> atCommand[0];
    parametersFile >> atCommand[1];

    int parametersSize;

    parametersFile >> parametersSize;

    unsigned char parametersArray[parametersSize];

    fillByteBuffer(&parametersFile, parametersSize, parametersArray);

    unsigned char address[ADDRESS_BYTES];
    unsigned char netAddress[2];

    fillByteBuffer(&parametersFile, ADDRESS_BYTES, address);
    fillByteBuffer(&parametersFile, 2, netAddress);

    parametersFile.close();

    //Effettua la verifica dello stream prodotto
    XBeeRemoteATCommand packet(atCommand, parametersSize, parametersArray, address, netAddress);

    assert(verifyPacket(&packet, verification));

}

#endif //OFFBOARD
