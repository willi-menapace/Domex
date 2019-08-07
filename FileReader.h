/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef FILE_READER_INCLUDED
#define FILE_READER_INCLUDED

#include "GlobalDefines.h"

#include "StreamReader.h"

#ifdef OFFBOARD
#include <fstream>
#include <iostream>
#endif

#ifdef ONBOARD
#include <SD.h>
#include <SPI.h>
#endif

/**
* Parser di file di testo in grado di effettuare la
* lettura di tipi primitivi.
*
* I nomi dei file devono rispettare le stesse condizioni imposte da FileWriter
*/
class FileReader : public StreamReader {

public:
    /**
    * Apre un file
    *
    * @param _fileName il nome del file da aprire
    * @return true se il file e' stato aperto con successo
    */
    virtual bool open(const char *_fileName) = 0;
    /**
    * @return true se il file e' aperto
    */
    virtual bool isOpen() = 0;
    /**
    * Effettua la chiusura del file
    */
    virtual void close() = 0;
    /**
    * Si posiziona ad un certo punto nel file
    *
    * @param position i byte dall'inizio del file dopo cui posizionarsi
    */
    virtual void seek(unsigned long position) = 0;
    /**
    * Ottiene la dimensione in byte del file aperto.
    * Il file deve essere aperto
    *
    * @return la dimenzione in byte del file
    */
    virtual unsigned long getSize() = 0;

    virtual ~FileReader() {}
};

#endif // FILE_READER_INCLUDED
