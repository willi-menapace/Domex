/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef STREAM_READER_INCLUDED
#define STREAM_READER_INCLUDED

#include "GlobalDefines.h"

#ifdef OFFBOARD
#include <iostream>
#endif // OFFBOARD

#ifdef ONBOARD
#include <Stream.h>
#endif // ONBOARD

/**
* Effettua la lettura di tipi primitivi da uno stream
* I byte sono ricevuti a partire dall'MSB
*/
class StreamReader {
private:
    /**
    * Legge il prossimo byte
    *
    * @return il byte letto
    */
    unsigned char getNextByte();
protected:
    /**
    * @return lo stream da cui effettuare la lettura. NULL se non
    *         e' disponibile lo stream da cui effettuare la lettura
    */
    #ifdef OFFBOARD
    virtual std::istream *getStream() = 0;
    #endif // OFFBOARD
    #ifdef ONBOARD
    virtual Stream *getStream() = 0;
    #endif // ONBOARD

public:
    /**
    * @return il numero di byte disponibili per la lettura
    */
    unsigned long isAvailable();
    /**
    * Legge un byte dallo stream.
    * Lo stream deve essere disponibile e devono essere disponibili
    * byte da leggere
    *
    * @return il byte letto
    */
    unsigned char readByte();
    /**
    * Legge un numero a 4 byte dallo stream
    * Lo stream deve essere disponibile e devono essere disponibili
    * byte da leggere
    *
    * @return i 4 byte letti
    */
    unsigned long readLong();
    /**
    * Legge un numero di bytes dallo stream
    * Lo stream deve essere disponibile e devono essere disponibili
    * i byte da leggere
    *
    * @param buffer buffer in cui inserire i bytes letti
    * @param length il numero di bytes da leggere
    *
    * @return numero di bytes effettivamente letti
    */
    unsigned long readBytes(unsigned char buffer[], unsigned long length);
    virtual ~StreamReader() {}

    //Permette ai reader di accedere ai reciproci stream
    friend class SecureFileReader;
    friend class NetworkReader;
};

#endif // STREAM_READER_INCLUDED
