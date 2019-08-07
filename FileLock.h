/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef FILE_LOCK_INCLUDED
#define FILE_LOCK_INCLUDED

#include "GlobalDefines.h"

#include "DirectFileWriter.h"
#include "DirectFileReader.h"

#define FILENAME_MAX_LENGTH 30

/**
* Salvataggio dello stato di un file che permette
* il rollback e il commit delle modifiche apportate ad esso successivamente.
* I file devono essere creati, acceduti e modificati utilizzando unicamente i meccanismi
* forniti da questa classe e lo stesso file non puo' mai essere aperto contemporaneamente in lettura e scrittura.
* Non possono essere aperti contemporaneamente piu' di un file in lettura e scrittura.
* I nomi dei file devono rispettare le condizioni imposte da FileWriter e FileReader.
*/
class FileLock {
private:

    //Prefissi identificativi del tipo di file
    enum FilePrefix {
        LOCK_PREFIX = '0',         //Il prefisso che si aggiunge al nome dei file di lock
        FIRST_DATA_PREFIX = '1',   //Il prefisso che si aggiunge al nome del primo file di dati
        SECOND_DATA_PREFIX = '2'  //Il prefisso che si aggiunge al nome del secondo file di dati
    };

    //Tipi di lock effettuabili
    enum LockType {
        VOID = 0,
        OVERWRITE = 1,
        APPEND = 2
    };

    DirectFileWriter lockedFileWriter; //Writer usato per la scrittura su file bloccato
    DirectFileReader fileReader;       //Reader usato per la lettura di file gestiti attraverso i meccanismi di lock

    char memorizedFileName[30];        //Nome del file bloccato

    /**
    * Ottiene il nome del file con l'aggiunta del prefisso specificato
    *
    * @param fileName nome del file a cui aggiungere il prefisso specificato. Puo' contenere un percorso
    * @param prefixedFileName stringa in cui verra' inserito il nome del file con aggiunta del prefisso.
    *        Deve poter contenere almeno una stringa di un carattere piu' lunga di fileName.
    *        In caso fileName contenesse un percorso anche prefixedFileName lo conservera'
    * @param prefix il prefisso da aggiungere a fileName
    */
    static void getPrefixedFilename(const char *fileName, char *prefixedFileName, FilePrefix prefix);

    /**
    * Effettua il rollback delle modifiche effettuate al file dopo il lock.
    * Richiamabile solo se il file non e' al momento aperto
    *
    * @param fileName il nome del file di cui eseguire il rollback
    */
    static void solveErrors(const char *fileName);

public:
    /**
    * Effettua il salvataggio dello stato del file e ottiene un writer per modificarlo.
    * Lo stato corrente e' quello che verra' ripristinato in caso di un rollback.
    *
    * @param fileName nome del file di cui salvare lo stato
    * @param append true per aggiungere dati alla fine del file, false per sovrascrivere
    *
    * @return writer per la scrittura sul file di cui si e' salvato lo stato, NULL in caso di errore
    *         Per rendere effettive le modifiche deve essere effettuato il commit.
    *         Non deve essere aperto e non deve essere chiuso
    */
    FileWriter *writeLock(const char *fileName, bool append);
    /**
    * Effettua il commit delle modifiche
    */
    void writeUnlock();
    /**
    * Effettua il rollback delle modifiche
    */
    void writeAbort();
    /**
    * Ottiene un reader per la lettura di un file gestito attraverso i meccanismi di lock.
    *
    * @param fileName il nome del file da aprire in lettura.
    *
    * @return reader per il file specificato. Non deve essere aperto e non deve essere
    *         chiuso. NULL in caso il file non esista
    */
    FileReader *getReader(const char *fileName);
    /**
    * Effettua la chiusura e il rilascio delle risorse associate ad un
    * file precedentemente aperto in lettura.
    */
    void releaseReader();

};

#endif // FILE_LOCK_INCLUDED
