/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef DIRECT_FILE_WRITER_INCLUDED
#define DIRECT_FILE_WRITER_INCLUDED

#include "GlobalDefines.h"

#include "FileWriter.h"

/**
* Writer in grado di effettuare la scrittura di file di testo
* in maniera diretta, senza garanzia di integrita' dei file
* in caso di arresti improvvisi o mancata chiusura.
*/
class DirectFileWriter : public FileWriter {
private:
    #ifdef OFFBOARD
    std::ofstream outFile;
    #endif

    #ifdef ONBOARD
    File outFile;
    #endif

protected:
    #ifdef OFFBOARD
    std::ostream *getStream();
    #endif // ONFFBOARD
    #ifdef ONBOARD
    Stream *getStream();
    #endif // ONFFBOARD
public:

    /**
    * Apre un file.
    * Se il file viene aperto con _append == false l'eventuale file esistente viene immediatamente eliminato e sovrascritto
    *
    * @param _fileName il nome del file da aprire
    * @param _append true per aggiungere i dati alla fine di un file esistente, false per sovrascriverlo
    * @return true se il file e' stato aperto con successo
    */
    bool open(const char *_fileName, bool _append);

    bool isOpen();

    void close();

};

#endif // DIRECT_FILE_WRITER_INCLUDED
