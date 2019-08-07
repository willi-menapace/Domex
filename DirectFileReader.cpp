/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "DirectFileReader.h"

#include "GlobalDefines.h"

#include <assert.h>

#ifdef OFFBOARD
std::istream *DirectFileReader::getStream() {
    //Se lo stream non e' disponibile ritorna NULL
    if(!isOpen()) {
        return 0;
    }
    return &inFile;
}
#endif // OFFBOARD
#ifdef ONBOARD
Stream *DirectFileReader::getStream() {
    //Se lo stream non e' disponibile ritorna NULL
    if(!isOpen()) {
        return 0;
    }
    return &inFile;
}
#endif // ONBOARD

unsigned long DirectFileReader::getSize() {
    assert(isOpen());

    #ifdef OFFBOARD
    std::streampos previuosPosition = inFile.tellg();
    inFile.seekg(0, inFile.end);
    std::streamsize length = inFile.tellg();
    inFile.seekg(previuosPosition);
    return (unsigned long) length;
    #endif

    #ifdef ONBOARD
    return inFile.size();
    #endif // ONBOARD
}

bool DirectFileReader::open(const char *_fileName) {

    #ifdef OFFBOARD
    inFile.open(_fileName, std::ios::in | std::ios::binary);
    return inFile.is_open();
    #endif

    #ifdef ONBOARD
    inFile = SD.open((char *) _fileName, FILE_READ);
    return (inFile == true);
    #endif // ONBOARD
}

bool DirectFileReader::isOpen() {
    #ifdef OFFBOARD
    return inFile.is_open();
    #endif

    #ifdef ONBOARD
    return (inFile == true);
    #endif // ONBOARD
}

void DirectFileReader::close() {
    #ifdef OFFBOARD
    inFile.close();
    #endif

    #ifdef ONBOARD
    inFile.close();
    #endif // ONBOARD
}

void DirectFileReader::seek(unsigned long position) {
    assert(isOpen());
    assert(position <= getSize());

    #ifdef OFFBOARD
    inFile.seekg(position);
    #endif // OFFBOARD

    #ifdef ONBOARD
    inFile.seek(position);
    #endif // ONBOARD
}
