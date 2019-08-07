/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "SecureFileReader.h"

#include "GlobalDefines.h"

#include <assert.h>

#ifdef OFFBOARD
std::istream *SecureFileReader::getStream() {
    if(!isOpen()) {
        return 0;
    }
    return fileReader->getStream();
}
#endif // OFFBOARD
#ifdef ONBOARD
Stream *SecureFileReader::getStream() {
    if(!isOpen()) {
        return 0;
    }
    return fileReader->getStream();
}
#endif // ONBOARD

bool SecureFileReader::open(const char *_fileName) {
    //Ottiene il reader per la lettura sul file in questione
    fileReader = fileLock.getReader(_fileName);

    return fileReader != 0;
}

bool SecureFileReader::isOpen() {
    if(fileReader == 0) {
        return false;
    }
    return fileReader->isOpen();
}

void SecureFileReader::close() {
    fileLock.releaseReader();
    fileReader = 0;
}

void SecureFileReader::seek(unsigned long position) {
    assert(fileReader != 0);

    fileReader->seek(position);
}

unsigned long SecureFileReader::getSize() {
    assert(fileReader != 0);

    return fileReader->getSize();
}

SecureFileReader::SecureFileReader() {
    fileReader = 0;
}
