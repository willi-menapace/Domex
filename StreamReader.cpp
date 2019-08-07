/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "StreamReader.h"

#include "GlobalDefines.h"

#include <assert.h>

unsigned char StreamReader::getNextByte() {
    //Garantisce anche che lo stream sia != NULL
    assert(isAvailable());

    #ifdef OFFBOARD
    unsigned char byte;
    //Effettua la lettura di un byte.
    getStream()->read((char *) &byte, 1);
    return byte;
    #endif

    #ifdef ONBOARD
    return (unsigned char) getStream()->read();
    #endif // ONBOARD
}

unsigned long StreamReader::isAvailable() {
    //Se non e' disponibile uno stream ritorna false
    if(getStream() == 0) {
        return 0;
    }

    #ifdef OFFBOARD
    std::streampos originalPosition = getStream()->tellg();
    getStream()->seekg(0, std::ios::end);
    std::streamsize length = getStream()->tellg() - originalPosition;
    getStream()->seekg(originalPosition);
    return length;
    #endif

    #ifdef ONBOARD
    return (unsigned long) getStream()->available();
    #endif // ONBOARD
}

unsigned char StreamReader::readByte() {
    return getNextByte();
}

unsigned long StreamReader::readLong() {
    assert(isAvailable() >= sizeof(unsigned long));

    unsigned char byte;
    unsigned long returnValue = 0;
    //Effettua la lettura dei 4 byte che pero' vengono rovesciati
    for(unsigned int i = 0; i < sizeof(unsigned long); ++i) {
        returnValue <<= 8; //Sposta a sinistra di un byte
        byte = getNextByte();
        returnValue |= byte;
    }

    return returnValue;
}

unsigned long StreamReader::readBytes(unsigned char buffer[], unsigned long length) {
    assert(isAvailable() >= length);

    #ifdef OFFBOARD
    //Effettua la lettura dei bytes
    getStream()->read((char *) buffer, length);
    assert(!getStream()->fail());
    return length;
    #endif

    #ifdef ONBOARD
    return getStream()->readBytes((char *) buffer, length);
    #endif // ONBOARD
}
