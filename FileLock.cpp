/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "GlobalDefines.h"

#include "FileLock.h"

#include "DirectFileReader.h"
#include "DirectFileWriter.h"

#include <cstring>
#include <assert.h>

#ifdef OFFBOARD
#include <fstream>
#endif // OFFBOARD

#ifdef ONBOARD
#include <SD.h>
#include <SPI.h>
#endif // ONBOARD

//Le dimensioni del buffer da utilizzare per il ripristino dei file
//durante la soluzione degli errori
#ifdef OFFBOARD
#define RESTORE_BUFFER_SIZE 7500
#endif // OFFBOARD
#ifdef ONBOARD
#define RESTORE_BUFFER_SIZE 7500
#endif // ONBOARD

/**
* Effettua la verifica di esistenza di un file
*
* @param fileName il file di cui verificare l'esistenza. Puo' contenere un percorso
*
* @return true se il file esiste
*/
bool exists(const char *fileName) {
    #ifdef OFFBOARD
    std::ifstream file(fileName);
    if (file.good()) {
        file.close();
        return true;
    } else {
        file.close();
        return false;
    }
    #endif // OFFBOARD

    #ifdef ONBOARD
    return SD.exists((char *) fileName);
    #endif // ONBOARD
}

/**
* Rimuove un file
*
* @param fileName il file da rimuovere. Deve esistere. Puo' contenere un percorso
*
* @return true se il file e' stato rimosso con successo
*/
bool removeFile(const char *fileName) {
    #ifdef OFFBOARD
    if(remove(fileName) == 0) {
        return true;
    } else {
        return false;
    }
    #endif // OFFBOARD

    #ifdef ONBOARD
    if(SD.remove((char *) fileName) == true) {
        return true;
    } else {
        return false;
    }
    #endif // ONBOARD

}

void FileLock::getPrefixedFilename(const char *fileName, char *prefixedFileName, FilePrefix prefix) {
    strcpy(prefixedFileName, fileName);

    int lastPathSeparatorIndex = -1;
    unsigned char terminationCharIndex = 0;

    //Effettua la ricerca del carattere di terminazione e dell'ultimo path separator
    for(int i = 0; true; ++i) {
        if(prefixedFileName[i] == '\0') {
            terminationCharIndex = i;
            break;
        }
        if(prefixedFileName[i] == SYSTEM_PATH_SEPARATOR) {
            lastPathSeparatorIndex = i;
        }
    }

    //Effettua lo shift della parte finale del percorso del file di un carattere
    //per fare spazio al prefisso
    for(int i = terminationCharIndex; i > lastPathSeparatorIndex; --i) {
        //prefixedFileName e' per contratto abbastanza grande da contenere un carattere in piu'
        prefixedFileName[i + 1] = prefixedFileName[i];
    }
    prefixedFileName[lastPathSeparatorIndex + 1] = (char) prefix;
}

void FileLock::solveErrors(const char *fileName) {
    unsigned char fileNameSize =  strlen(fileName);

    char lockFileName[fileNameSize + 2]; //L'array deve contenere anche il carattere '\0' di terminazione + il prefisso

    getPrefixedFilename(fileName, lockFileName, LOCK_PREFIX);

    //Se non ci sono file di lock non serve fare nulla
    if(!exists(lockFileName)) {
        return;
    }

    DirectFileReader lockFile;
    lockFile.open(lockFileName);

    //Se il file di lock non e' stato scritto completamente
    //allora nulla era stato modificato dunque e' sifficiente rimuovere il file di lock
    assert(lockFile.isOpen());
    if(lockFile.isAvailable() == 0) {
        lockFile.close();
        removeFile(lockFileName);
        return;
    }

    //Ottiene il tipo di ripristino da effettuare
    unsigned char lockType = lockFile.readByte();

    char firstDataFileName[fileNameSize + 2];
    char secondDataFileName[fileNameSize + 2];

    getPrefixedFilename(fileName, firstDataFileName, FIRST_DATA_PREFIX);
    getPrefixedFilename(fileName, secondDataFileName, SECOND_DATA_PREFIX);

    //Se prima del lock non c'erano file li rimuove
    if(lockType == VOID) {
        if(exists(firstDataFileName)) {
            bool removed = false;
            removed = removeFile(firstDataFileName);
            assert(removed);
        }
        if(exists(secondDataFileName)) {
            bool removed = false;
            removed = removeFile(secondDataFileName);
            assert(removed);
        }
    }

    char oldFilePrefix = (char) lockFile.readByte(); //Il prefisso del file esistente prima del lock
    char *oldFileFileName = 0; //Puntatore al nome del file preesistente al lock
    char *newFileFileName = 0; //Puntatore al nome del file creato successivamente al lock

    //Individua il file preesistente e il file creato successivamente
    if(oldFilePrefix == FIRST_DATA_PREFIX) {
        oldFileFileName = firstDataFileName;
        newFileFileName = secondDataFileName;
    } else {
        oldFileFileName = secondDataFileName;
        newFileFileName = firstDataFileName;
    }

    //Se il lock e' stato creato per sovrascrivere un file preesistente
    if(lockType == OVERWRITE) {

        //Se esiste ancora il file preesistente bisogna cancellare il file scritto successivamente che
        //tentava di sovrascrivere il primo
        if(exists(oldFileFileName)) {
            //Se esiste il file che tentava la sovrascrittura lo rimuove
            if(exists(newFileFileName)) {
                bool removed = false;
                removed = removeFile(newFileFileName);
                assert(removed);
            }
        }
    }
    //Se il lock e' stato creato per aggiungere dati ad un file preesistente
    else if(lockType == APPEND) {

        //Bisogna concludere il ripristino solo se tentativi precedenti non sono arrivati al punto
        //di cancellare il file originale
        if(exists(oldFileFileName)) {

            //Ottiene la dimensione che il file preesistente aveva prima del lock
            unsigned long oldFileSize = lockFile.readLong();

            //Elimina i file creati da tentativi di ripristino precedenti
            if(exists(newFileFileName)) {
                bool removed = false;
                removed = removeFile(newFileFileName);
                assert(removed);
            }

            //Effettua il ripristino del file originale in un nuovo file
            DirectFileWriter restoredFile;
            restoredFile.open(newFileFileName, false);

            DirectFileReader originalFile;
            originalFile.open(oldFileFileName);

            //Buffer da utilizzare per il ripristino
            unsigned char buffer[RESTORE_BUFFER_SIZE];
            unsigned long bytesToReadInBuffer;

            //Estrae dal file originale tutti i byte che aveva prima del lock
            //e li trasferisce nel nuovo file
            while(oldFileSize != 0) {
                bytesToReadInBuffer = originalFile.isAvailable();
                //Se sono disponibili piu' bytes di quelli leggibili ne legge solo quelli leggibili
                if(bytesToReadInBuffer > RESTORE_BUFFER_SIZE) {
                    bytesToReadInBuffer = RESTORE_BUFFER_SIZE;
                }
                //Se si stanno oltrepassando il numero di bytes da leggere
                if(bytesToReadInBuffer > oldFileSize) {
                    bytesToReadInBuffer = oldFileSize;
                }

                unsigned long readBytes = originalFile.readBytes(buffer, bytesToReadInBuffer);

                restoredFile.writeBytes(buffer, readBytes);
                oldFileSize -= readBytes;

                //Non devono mai essere letti troppy bytes
                assert(oldFileSize >= 0);
            }

            restoredFile.close();
            originalFile.close();

            //Rimuove il file di cui abbiamo completato il ripristino
            bool removed = false;
            removed = removeFile(oldFileFileName);
            assert(removed);
        }
    //Il lock deve essere di un tipo conosciuto
    } else {
        assert(false);
    }

    lockFile.close();

    bool removed = false;
    removed = removeFile(lockFileName);
    assert(removed);
}

FileWriter *FileLock::writeLock(const char *fileName, bool append) {

    assert(!lockedFileWriter.isOpen()); //Non possono essere aperti piu' file contemporaneamente

    bool openSuccess = false; //True se il file e' richiesto e' stato aperto con successo

    //Effettua il rollback di eventuali modifiche precedenti per cui non e'
    //stato fatto un commit
    FileLock::solveErrors(fileName);

    unsigned char fileNameSize =  strlen(fileName);

    assert(fileNameSize < FILENAME_MAX_LENGTH - 1);

    //Memorizza il nome del file bloccato in modo da poter effettuare lo sblocco successivo
    strcpy(memorizedFileName, fileName);

    //Ottiene il nome dei file coinvolti
    char lockFileName[fileNameSize + 2]; //L'array deve contenere anche il carattere '\0' di terminazione + il prefisso
    char firstDataFileName[fileNameSize + 2];
    char secondDataFileName[fileNameSize + 2];

    getPrefixedFilename(fileName, lockFileName, LOCK_PREFIX);
    getPrefixedFilename(fileName, firstDataFileName, FIRST_DATA_PREFIX);
    getPrefixedFilename(fileName, secondDataFileName, SECOND_DATA_PREFIX);

    bool firstFileExists = exists(firstDataFileName);
    bool secondFileExists = exists(secondDataFileName);

    DirectFileWriter lockFile;

    lockFile.open(lockFileName, false); //In ogni caso sara' necessario creare un file di lock

    //Se non esistono file si crea un lock tipo VOID
    if(!firstFileExists && !secondFileExists) {
        lockFile.writeByte((unsigned char) VOID);
        openSuccess = lockedFileWriter.open(firstDataFileName, false); //Si crea il file che verra' ritornato
    //Se esiste un file deve essere unico a causa della risoluzione degli errori iniziale
    } else {
        assert(!(firstFileExists && secondFileExists));

        char *oldFileFileName = 0;
        char *newFileFileName = 0;

        //Trova il prefisso del file esistente
        char existingFilePrefix;
        if(firstFileExists) {
            existingFilePrefix = FIRST_DATA_PREFIX;
            oldFileFileName = firstDataFileName;
            newFileFileName = secondDataFileName;
        } else {
            existingFilePrefix = SECOND_DATA_PREFIX;
            oldFileFileName = secondDataFileName;
            newFileFileName = firstDataFileName;
        }

        //Scrive la modalita' di scrittura nel file di lock
        if(append) {
            lockFile.writeByte((unsigned char) APPEND);
        } else {
            lockFile.writeByte((unsigned char) OVERWRITE);
        }
        //Scrive il prefisso del file esistente nel file di lock
        lockFile.writeByte((unsigned char) existingFilePrefix);

        if(append) {
            //Determina la dimensione del file esistente prima dell'aggiunta di dati
            DirectFileReader existingFileReader;
            existingFileReader.open(oldFileFileName);
            unsigned long fileSize = existingFileReader.getSize();
            existingFileReader.close();

            //Scrive la dimensione nel file di lock in caso di necessita' di ripristino
            lockFile.writeLong(fileSize);

            //Apre il file esistente in modalita' append
            openSuccess = lockedFileWriter.open(oldFileFileName, true);
        } else {
            //Crea un nuovo file per evitare la sovrascrittura di quello vecchio fino al commit
            openSuccess = lockedFileWriter.open(newFileFileName, false);
        }
    }

    lockFile.close();

    //Se il file e' stato aperto con successo ritorna il writer,
    if(openSuccess) {
        return (FileWriter *) &lockedFileWriter;
    } else {
        assert(false);
        return 0;
    }
}

void FileLock::writeUnlock() {
    //Chiude il file scritto durante il lock
    lockedFileWriter.close();

    unsigned char fileNameSize = strlen(memorizedFileName);

    //Ottiene il nome del file di lock
    char lockFileName[fileNameSize + 2]; //L'array deve contenere anche il carattere '\0' di terminazione + il prefisso
    getPrefixedFilename(memorizedFileName, lockFileName, LOCK_PREFIX);

    DirectFileReader lockFile;
    lockFile.open(lockFileName);

    //Ottiene il tipo di lock che era stato effettuato
    unsigned char lockType = lockFile.readByte();

    //Se il tipo di lock era VOID o APPEND non ci sono
    //altri file oltre al lock da eliminare
    if(lockType == OVERWRITE) {

        //Ottiene il nome dei file di dati coinvolti
        char firstDataFileName[fileNameSize + 2];
        char secondDataFileName[fileNameSize + 2];
        getPrefixedFilename(memorizedFileName, firstDataFileName, FIRST_DATA_PREFIX);
        getPrefixedFilename(memorizedFileName, secondDataFileName, SECOND_DATA_PREFIX);

        char *oldFileFileName = 0;

        char oldFilePrefix = lockFile.readByte();

        if(oldFilePrefix == FIRST_DATA_PREFIX) {
            oldFileFileName = firstDataFileName;
        } else {
            oldFileFileName = secondDataFileName;
        }

        //Rimuove il file da sovrascrivere durante il lock
        bool removed = false;
        removed = removeFile(oldFileFileName);
        assert(removed);
    }

    //Chiude il file di lock e lo rimuove
    //segnalando che l'intero processo e' andato a buon fine
    lockFile.close();
    bool removed = false;
    removed = removeFile(lockFileName);
    assert(removed);
}

void FileLock::writeAbort() {
    //Chiude il file scritto durante il lock
    lockedFileWriter.close();

    //Effettua il rollback delle modifiche
    solveErrors(memorizedFileName);
}

FileReader *FileLock::getReader(const char *fileName) {

    //Effettua il rollback di eventuali modifiche precedenti per cui non e'
    //stato fatto un commit
    solveErrors(fileName);

    unsigned char fileNameSize =  strlen(fileName);

    //Il nome con cui il file potrebbe essere stato salvato effettivamente
    char dataFileName[fileNameSize + 2];

    getPrefixedFilename(fileName, dataFileName, FIRST_DATA_PREFIX);

    //Il file associato al nome fileName deve essere unico dopo la risoluzione degli errori
    //quindi appena viene trovato un file esistente viene aperto e ritornato
    if(exists(dataFileName)) {
        fileReader.open(dataFileName);
        return (FileReader *) &fileReader;
    }

    //Se non e' stato trovato il file tenta con un diverso prefisso
    getPrefixedFilename(fileName, dataFileName, SECOND_DATA_PREFIX);
    if(exists(dataFileName)) {
        fileReader.open(dataFileName);
        return (FileReader *) &fileReader;
    }

    //Nessun file associato a fileName e' stato trovato
    return 0;
}

void FileLock::releaseReader() {
    fileReader.close();
}
