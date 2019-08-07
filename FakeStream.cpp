/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "FakeStream.h"

#include "GlobalDefines.h"

#ifdef OFFBOARD

#include <assert.h>

void Stream::write(unsigned char byte) {
    //Interrompe l'applicazione se il buffer non e' in grado di contenere tutti i dati
    if(bytesCount >= FAKE_STREAM_BUFFER_LENGTH) {
        assert(false);
    }

    buffer[bytesCount] = byte;
    ++bytesCount;
}

unsigned char *Stream::getBuffer() {
    return buffer;
}

unsigned long Stream::getBytesCount() {
    return bytesCount;
}

bool Stream::available() {
    return bytesLeft > 0;
}

unsigned char Stream::read() {
    //Interrompe l'applicazione se viene chiamato read senza byte da leggere
    if(!available()) {
        assert(false);
    }

    unsigned char readByte = 0;

    readByte = receptionBuffer[receptionIndex];
    ++receptionIndex;
    --bytesLeft;

    return readByte;
}

Stream::Stream() {
    bytesCount = 0;
    receptionIndex = 0;
    bytesLeft = 0;
}

Stream::Stream(unsigned char *_receptionBuffer, unsigned long _size) {
    bytesCount = 0;
    receptionIndex = 0;
    bytesLeft = _size;

    for(unsigned long i = 0; i < _size; ++i) {
        receptionBuffer[i] = _receptionBuffer[i];
    }
}

#endif // OFFBOARD
