/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef STREAM_WRITER_INCLUDED
#define STREAM_WRITER_INCLUDED

#include "GlobalDefines.h"

#ifdef OFFBOARD
#include <iostream>
#endif // OFFBOARD

#ifdef ONBOARD
#include <Stream.h>
#endif // ONBOARD

/**
* Effettua la scrittura di tipi primitivi su uno stream
* I byte sono trasmessi a partire dall'MSB
*/
class StreamWriter {
protected:
    /**
    * @return lo stream su cui effettuare la scrittura.
    *         NULL se non e' disponibile
    */
    #ifdef OFFBOARD
    virtual std::ostream *getStream() = 0;
    #endif // ONFFBOARD
    #ifdef ONBOARD
    virtual Stream *getStream() = 0;
    #endif // ONFFBOARD
public:
    /**
    * Scrive un byte sullo stream.
    * Lo stream deve essere disponibile.
    *
    * @param value il byte da scrivere
    */
    void writeByte(unsigned char value);
    /**
    * Scrive un numero da 4 byte sullo stream
    * Lo stream deve essere disponibile
    *
    * @param value i 4 byte da scrivere
    */
    void writeLong(unsigned long value);
    /**
    * Scrive un array di bytes sullo stream
    * Lo stream deve essere disponibile
    *
    * @param buffer array da scrivere sullo stream
    * @param length numero di bytes da scrivere sullo stream
    */
    void writeBytes(unsigned char buffer[], unsigned long length);
    virtual ~StreamWriter() {}

    //Permette l'accesso reciproco agli stream
    friend class SecureFileWriter;
    friend class NetworkWriter;

};

#endif // STREAM_WRITER_INCLUDED
