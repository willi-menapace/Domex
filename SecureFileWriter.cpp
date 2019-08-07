/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "SecureFileWriter.h"

#include "GlobalDefines.h"

#ifdef OFFBOARD
std::ostream *SecureFileWriter::getStream() {
    //Se lo stream non e' disponibile ritorna 0
    if(!isOpen()) {
        return 0;
    }

    return fileWriter->getStream();
}
#endif // ONFFBOARD
#ifdef ONBOARD
Stream *SecureFileWriter::getStream() {
    //Se lo stream non e' disponibile ritorna 0
    if(!isOpen()) {
        return 0;
    }

    return fileWriter->getStream();
}
#endif // ONFFBOARD

bool SecureFileWriter::open(const char *_fileName, bool _append) {
    //Effettua il blocco del file che diventa scrivibile attraverso fileWriter
    fileWriter = fileLock.writeLock(_fileName, _append);

    return fileWriter != 0;
}

bool SecureFileWriter::isOpen() {
    if(fileWriter == 0) {
        return false;
    }
    return fileWriter->isOpen();
}

void SecureFileWriter::close() {
    fileLock.writeUnlock();
    fileWriter = 0;
}

void SecureFileWriter::abort() {
    fileLock.writeAbort();
    fileWriter = 0;
}

SecureFileWriter::SecureFileWriter() {
    fileWriter = 0;
}
