/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "StreamWriter.h"

#include "GlobalDefines.h"

#include <assert.h>

void StreamWriter::writeByte(unsigned char value) {
    //Si assicura che sia disponibile uno stream
    assert(getStream() != 0);

    #ifdef OFFBOARD
    getStream()->write((char *) &value, sizeof(unsigned char));
    #endif // OFFBOARD

    #ifdef ONBOARD
    getStream()->write(value);
    #endif // ONBOARD
}

void StreamWriter::writeLong(unsigned long value) {
    unsigned char *byteToWrite;
    byteToWrite = ((unsigned char *) &value) + (sizeof(value) - 1); //Si fa puntare all'MSB

    for(unsigned int i = 0; i < sizeof(unsigned long); ++i) {
        writeByte(*byteToWrite);
        --byteToWrite; //Fa scorrere verso l'LSB il puntatore
    }
}

void StreamWriter::writeBytes(unsigned char buffer[], unsigned long length) {
    assert(getStream() != 0);

    #ifdef OFFBOARD
    getStream()->write((char *) buffer, length);
    assert(!getStream()->fail());
    #endif // OFFBOARD

    #ifdef ONBOARD
    unsigned long writtenBytes = getStream()->write((char *) buffer, length);
    assert(writtenBytes == length);
    #endif // ONBOARD
}
