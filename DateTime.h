/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef DATETIME_INCLUDED
#define DATETIME_INCLUDED

#include "GlobalDefines.h"

#include "FileReader.h"
#include "FileWriter.h"

/**
* Classe rappresentante una data e un orario
*/
class DateTime {
private:
    static unsigned char monthLength[12];

    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char dayOfMonth;
    unsigned char dayOfWeek;
    unsigned char month;
    unsigned char year;

    unsigned long timeOffset; //I secondi trascorsi dal 1 gennaio 2000
    bool validOffsetCache;    //True se il valore di timeOffset non deve essere ricalcolato

public:

    /**
    * Inizializza l'istanza corrente
    *
    * @param _second secondi 0-59
    * @param _minute minuti 0-59
    * @param _hour ora 0-23
    * @param _dayOfMonty giorno del mese 1-31
    * @param _dayOfWeek giorno della settimana 1-7
    * @param _month mese 1-12
    * @param _year anno 0-99
    */
    void initialize(unsigned char _second, unsigned char _minute, unsigned char _hour,
                    unsigned char _dayOfMonth, unsigned char _dayOfWeek, unsigned char _month, unsigned char _year);
    void initialize(DateTime *otherDate);
    /**
    * Inizializza l'istanza corrente con una scritta su file
    *
    * @param fileReader il file su cui e' scritta l'istanza
    */
    void initialize(FileReader *fileReader);
    /**
    * @return secondi trascorsi dal 1 gennaio 2000 alla data corrente
    */
    unsigned long getTimeOffset();

    /**
    * Memorizza la data su un file
    *
    * @param fileWriter il file su cui memorizzare la data
    */
    void writeToFile(FileWriter *fileWriter);
    /**
    * @return true se le due istanze sono logicamente equivalenti
    */
    bool equals(DateTime *otherDate);
};

#endif // DATETIME_INCLUDED
