/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef DIRECT_FILE_READER_INCLUDED
#define DIRECT_FILE_READER_INCLUDED

#include "GlobalDefines.h"

#include "FileReader.h"

/**
* Reader in grado di effettuare la lettura diretta di file
* senza supporto per meccanismi di controllo dell'integrita'
*/
class DirectFileReader : public FileReader {

private:
    #ifdef OFFBOARD
    std::ifstream inFile;
    #endif

    #ifdef ONBOARD
    File inFile;
    #endif

protected:
    #ifdef OFFBOARD
    std::istream *getStream();
    #endif // OFFBOARD
    #ifdef ONBOARD
    Stream *getStream();
    #endif // ONBOARD
public:

    unsigned long getSize();

    bool open(const char *_fileName);

    bool isOpen();

    void close();

    void seek(unsigned long position);

};

#endif // DIRECT_FILE_READER_INCLUDED
