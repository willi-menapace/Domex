/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "DateTime.h"

#include "GlobalDefines.h"

#include <assert.h>

#define SECONDS_PER_YEAR 31536000
#define SECONDS_PER_DAY 86400
#define SECONDS_PER_HOUR 3600

//Inizializza le lunghezze dei mesi
unsigned char DateTime::monthLength[12] = {31, 28, 31, 30, 31, 30,
                                            31, 31, 30, 31, 30, 31};

void DateTime::initialize(unsigned char _second, unsigned char _minute, unsigned char _hour,
                          unsigned char _dayOfMonth, unsigned char _dayOfWeek, unsigned char _month, unsigned char _year) {

    assert(_second < 60);
    assert(_minute < 60);
    assert(_hour < 24);
    assert(_dayOfWeek <= 7 && _dayOfWeek > 0);
    assert(_month <= 12 && _month > 0);
    if(_month != 2) {
        assert(_dayOfMonth <= monthLength[_month - 1] && _dayOfMonth > 0);
    } else { //Febbraio puo' essere bisestile
        assert(_dayOfMonth <= monthLength[_month - 1] + 1 && _dayOfMonth > 0);
    }
    assert(_year < 99);

    second = _second;
    minute = _minute;
    hour = _hour;
    dayOfMonth = _dayOfMonth;
    dayOfWeek = _dayOfWeek;
    month = _month;
    year = _year;

    validOffsetCache = false;
}

void DateTime::initialize(DateTime *otherDate) {

    assert(otherDate != 0);

    initialize(otherDate->second, otherDate->minute, otherDate->hour,
               otherDate->dayOfMonth, otherDate->dayOfWeek,
               otherDate->month, otherDate->year);
}

void DateTime::initialize(FileReader *fileReader) {
    second = fileReader->readByte();
    minute = fileReader->readByte();
    hour = fileReader->readByte();
    dayOfMonth = fileReader->readByte();
    dayOfWeek = fileReader->readByte();
    month = fileReader->readByte();
    year = fileReader->readByte();

    assert(second < 60);
    assert(minute < 60);
    assert(hour < 24);
    assert(dayOfWeek <= 7 && dayOfWeek > 0);
    assert(month <= 12 && month > 0);
    if(month != 2) {
        assert(dayOfMonth <= monthLength[month - 1] && dayOfMonth > 0);
    } else { //Febbraio puo' essere bisestile
        assert(dayOfMonth <= monthLength[month - 1] + 1 && dayOfMonth > 0);
    }
    assert(year < 99);

    validOffsetCache = fileReader->readByte();
    timeOffset = fileReader->readLong();
}

unsigned long DateTime::getTimeOffset() {
    if(validOffsetCache) {
        return timeOffset;
    }

    timeOffset = 0;

    //Aggiunge i secondi di tutti gli anni passati senza considerare i bisestili
    timeOffset += year * SECONDS_PER_YEAR;

    //Aggiunge i secondi dei mesi passati senza considerare i bisestili
    for(int i = 0; i < (month - 1); ++i) {
        timeOffset += DateTime::monthLength[i] * SECONDS_PER_DAY;
    }

    //Aggiunge i secondi dei giorni del mese corrente
    timeOffset += (dayOfMonth - 1) * SECONDS_PER_DAY;

    timeOffset += hour * SECONDS_PER_HOUR;

    timeOffset += minute * 60;

    timeOffset += second;

    //Calcola i giorni bisestili
    unsigned char additionalDays = 0;
    additionalDays += (year / 4) + 1;
    //Se l'anno corrente e' bisestile e non si e' superato febbraio non bisogna contare il giorno
    if(year % 4 == 0 && month <= 2) {
        --additionalDays;
    }

    timeOffset += additionalDays * SECONDS_PER_DAY;

    validOffsetCache = true;

    return timeOffset;

}


void DateTime::writeToFile(FileWriter *fileWriter) {

    assert(fileWriter != 0);
    assert(fileWriter->isOpen());

    //Scrive le informazioni di base
    fileWriter->writeByte(second);
    fileWriter->writeByte(minute);
    fileWriter->writeByte(hour);
    fileWriter->writeByte(dayOfMonth);
    fileWriter->writeByte(dayOfWeek);
    fileWriter->writeByte(month);
    fileWriter->writeByte(year);

    //Scrive la cache del timeoffset
    fileWriter->writeByte(validOffsetCache);
    fileWriter->writeLong(timeOffset);

}

bool DateTime::equals(DateTime *otherDate) {
    if(otherDate == 0) {
        return false;
    }

    //Un'istanze e' uguale a se stessa
    if(this == otherDate) {
        return true;
    }

    if(second != otherDate->second) {
        return false;
    }
    if(minute != otherDate->minute) {
        return false;
    }
    if(hour != otherDate->hour) {
        return false;
    }
    if(dayOfWeek != otherDate->dayOfWeek) {
        return false;
    }
    if(dayOfMonth != otherDate->dayOfMonth) {
        return false;
    }
    if(month != otherDate->month) {
        return false;
    }
    if(year != otherDate->year) {
        return false;
    }

    return true;
}
