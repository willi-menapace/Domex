/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBeeSerialManager.h"

#include "GlobalDefines.h"

void XBeeSerialManager::writeByte(unsigned char _byte) {
    #ifdef OFFBOARD
    stream->write( _byte);
    #endif // OFFBOARD
    #ifdef ONBOARD

    while(true) {
        //Verifica la disponibilita' di spazio nel buffer
        //di ricezione seriale del modulo XBee
        if(digitalRead(ctsPin) == LOW) {
            stream->write((byte) _byte); //Evita il richiamo di overload write
                                         //incapaci di gestire 0x00
            break;
        }
    }

    #endif // ONBOARD
}

bool XBeeSerialManager::available() {
    return stream->available();
}

unsigned char XBeeSerialManager::readByte() {
    return stream->read();
}

XBeeSerialManager::XBeeSerialManager(Stream *_stream, unsigned char _ctsPin) {
    stream = _stream;
    ctsPin = _ctsPin;
}
