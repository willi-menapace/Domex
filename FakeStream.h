/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef FAKE_STREAM_INCLUDED
#define FAKE_STREAM_INCLUDED

#include "GlobalDefines.h"

#ifdef OFFBOARD

#define FAKE_STREAM_BUFFER_LENGTH (10 * 1024)

/**
* Classe che emula uno stream definito dalle librerie Arduino a scopi di test
*/
class Stream {
private:
    unsigned char buffer[FAKE_STREAM_BUFFER_LENGTH];           //Buffer in cui bengono salvati i byte scritti sullo stream
    unsigned long bytesCount;                                  //Numero di byte contenuti nel buffer

    unsigned char receptionBuffer[FAKE_STREAM_BUFFER_LENGTH];  //Buffer da cui vengono letti i byte
    unsigned long bytesLeft;                                   //Il numero di byte rimasti da leggere
    unsigned long receptionIndex;                              //Indice da cui leggere il prossimo byte
public:
    /**
    * Scrive un byte sullo stream
    *
    * @param byte il byte da scrivere
    */
    void write(unsigned char byte);
    /**
    * Ottiene il buffer contenente i byte scritti finora sullo stream
    *
    * @return il buffer contenente i byte scritti finora sullo stream
    */
    unsigned char *getBuffer();
    /**
    * Ottiene il numero di byte contenuti nel buffer
    *
    * @return il numero di byte contenuti nel buffer
    */
    unsigned long getBytesCount();
    /**
    * @return true se sono disponibili byte da leggere sullo stream
    */
    bool available();
    /**
    * Effettua la lettura di un byte dallo stream
    * Deve essere available() == true
    *
    * @return il byte letto dallo stream
    */
    unsigned char read();
    /**
    * Costruisce uno stream vuoto
    */
    Stream();
    /**
    * Costruisce uno stream contenente dati leggibili
    *
    * @param _receptionBuffer array contenente i byte che saranno leggibili dallo stream
    * @param _size il numero di byte leggibili
    */
    Stream(unsigned char *_receptionBuffer, unsigned long _size);
};

#endif // OFFBOARD
#endif // FAKE_STREAM_INCLUDED
