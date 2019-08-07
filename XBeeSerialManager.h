/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_SERIAL_MANAGER_INCLUDED
#define XBEE_SERIAL_MANAGER_INCLUDED

#include "GlobalDefines.h"

#ifdef OFFBOARD
#include "FakeStream.h"
#endif // OFFBOARD

#ifdef ONBOARD
#include <Arduino.h>
#endif //ONBOARD

/**
* Gestore della connessione seriale con il modulo XBee
*/
class XBeeSerialManager {
private:
    Stream *stream;       //Il canale di comunicazione con il modulo
    unsigned char ctsPin; //Il bit da verificare per sapere se e' possibile inviare un'altro byte
public:
    /**
    * Invia un byte al modulo
    *
    * @param _byte il byte da inviare
    */
    void writeByte(unsigned char _byte);
    /**
    * @return true se e' disponibile un byte da leggere
    */
    bool available();
    /**
    * Effettua la lettura di un byte dalla connessione seriale.
    * Richiamabile se available() == true
    *
    * @return il byte letto
    */
    unsigned char readByte();
    /**
    * Costruisce manager per l'invio di dati attraverso uno stream
    *
    * @param _stream lo stream attraverso cui inviare i dati
    * @param _ctsPin il pin CTS che se alto indica che e' possibile inviare un byte sul canale
    */
    XBeeSerialManager(Stream *_stream, unsigned char _ctsPin);
};

#endif // XBEE_SERIAL_MANAGER_INCLUDED
