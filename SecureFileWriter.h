/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef SECURE_FILE_WRITER_INCLUDED
#define SECURE_FILE_WRITER_INCLUDED

#include "GlobalDefines.h"

#include "FileWriter.h"
#include "FileLock.h"

/**
* Writer in grado di effettuare la sovrascrittura di file e l'append in maniera sicura.
* Le modifiche ai file vengono apportate in maniera atomica al momento della
* chiusura.
*
* File creati in questo modo possono essere manipolati solo dalle classi SecureReader e SecureWriter
* File creati con altre classi non possono essere manipolati da SecureReader e SecureWriter
*/
class SecureFileWriter : public FileWriter {
private:
    FileLock fileLock;        //Lock che permette la scrittura atomica
    FileWriter *fileWriter;   //Writer usato per scrivere sul file bloccato
protected:
    #ifdef OFFBOARD
    std::ostream *getStream();
    #endif // ONFFBOARD
    #ifdef ONBOARD
    Stream *getStream();
    #endif // ONFFBOARD
public:

    bool open(const char *_fileName, bool _append);

    bool isOpen();

    void close();

    /**
    * Elimina tutte le modifiche effettuate al file dal momento dell'open.
    * Dopo la chiamata al metodo il file viene chiuso
    */
    void abort();

    SecureFileWriter();
};

#endif // SECURE_WRITER_INCLUDED
