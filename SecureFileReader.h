/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef SECURE_FILE_READER_INCLUDED
#define SECURE_FILE_READER_INCLUDED

#include "GlobalDefines.h"

#include "FileReader.h"
#include "FileLock.h"

/**
* Reader in grado di effettuare la lettura di file manipolati da
* SecureFileWriter
*/
class SecureFileReader : public FileReader {

private:
    FileLock fileLock;       //Permette la lettura di file manipolati attraverso meccenismi di blocco
    FileReader *fileReader;  //Reader per la lettura del file manipolato tramite meccanismi di blocco
protected:
    #ifdef OFFBOARD
    std::istream *getStream();
    #endif // OFFBOARD
    #ifdef ONBOARD
    Stream *getStream();
    #endif // ONBOARD
public:

    bool open(const char *_fileName);

    bool isOpen();

    void close();

    void seek(unsigned long position);

    unsigned long getSize();

    SecureFileReader();

};

#endif // SECURE_FILE_READER_INCLUDED
