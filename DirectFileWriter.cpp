/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "GlobalDefines.h"

#include "DirectFileWriter.h"

#include <assert.h>

#ifdef OFFBOARD
std::ostream *DirectFileWriter::getStream() {
    //Se lo stream non e' disponibile ritorna 0
    if(!isOpen()) {
        return 0;
    }

    return &outFile;
}
#endif // ONFFBOARD
#ifdef ONBOARD
Stream *DirectFileWriter::getStream() {
    //Se lo stream non e' disponibile ritorna 0
    if(!isOpen()) {
        return 0;
    }

    return &outFile;
}
#endif // ONFFBOARD

bool DirectFileWriter::open(const char *_fileName, bool _append) {

    #ifdef OFFBOARD
    if(_append == true) {
        //Aggiunge i dati a fine file
        outFile.open(_fileName, std::ios::out | std::ios::app | std::ios::binary);
    } else {
        //Sovrascrive il file esistente
        outFile.open(_fileName, std::ios::out | std::ios::binary);
    }
    return outFile.is_open();
    #endif // OFFBOARD

    #ifdef ONBOARD
    if(_append == false) {
        //Se il file esiste gia' lo rimuove per sovrascriverlo
        if(SD.exists((char *) _fileName)) {
            SD.remove((char *) _fileName);
        }
    }

    outFile = SD.open((char *) _fileName, FILE_WRITE);
    return (outFile == true);
    #endif // ONBOARD
}

bool DirectFileWriter::isOpen() {
    #ifdef OFFBOARD
    return outFile.is_open();
    #endif // OFFBOARD
    #ifdef ONBOARD
    return outFile == true;
    #endif // ONBOARD
}

void DirectFileWriter::close() {
    #ifdef OFFBOARD
    outFile.close();
    #endif // OFFBOARD

    #ifdef ONBOARD
    outFile.close();
    #endif // ONBOARD
}
