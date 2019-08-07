/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef FILE_WRITER_INCLUDED
#define FILE_WRITER_INCLUDED

#include "GlobalDefines.h"

#include "StreamWriter.h"

#ifdef OFFBOARD
#include <iostream>
#include <fstream>
#endif

#ifdef ONBOARD
#include <SD.h>
#include <SPI.h>
#endif

/**
* Writer di file di testo in grado di effettuare la scrittura
* di tipi primitivi.
*
* I nomi dei file devono essere espressi nel formato nome.ext dove nome ha
* dimensione massima di 7 caratteri e ext di 3 caratteri. I nomi possono contenere percorsi.
*/
class FileWriter : public StreamWriter {

public:
    /**
    * Apre un file.
    *
    * @param _fileName il nome del file da aprire
    * @param _append true per aggiungere i dati alla fine di un file esistente, false per sovrascriverlo
    * @return true se il file e' stato aperto con successo
    */
    virtual bool open(const char *_fileName, bool _append) = 0;
    /**
    * @return true se il file e' aperto
    */
    virtual bool isOpen() = 0;
    /**
    * Effettua la chiusura del file
    */
    virtual void close() = 0;

    virtual ~FileWriter() {}
};

#endif // FILE_WRITER_INCLUDED
