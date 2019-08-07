/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_FRAMEABLE_PACKET_INCLUDED
#define XBEE_FRAMEABLE_PACKET_INCLUDED

#include "GlobalDefines.h"

#include "XBeePacket.h"

/**
* Pacchetto contenente un frame ID identificativo
*/
class XBeeFrameablePacket : public XBeePacket {
private:
    static const unsigned char FRAME_ID_INDEX;
public:
    /**
    * Imposta il frame ID del pacchetto
    *
    * @param frameId il frame ID del pacchetto
    */
    void setFrameId(unsigned char frameId);
    /**
    * Ottiene il frame ID del pacchetto.
    * Richiamabile solo se il frame ID e' stato impostato
    *
    * @return il frame ID del pacchetto
    */
    unsigned char getFrameId();

    XBeeFrameablePacket();

    /**
    * Implementa il chaining con il costruttore di copia di XBeePacket
    * per permettere alle sottoclassi di usufruire di questa funzionalita'
    */
    XBeeFrameablePacket(XBeePacket *packet);

    virtual ~XBeeFrameablePacket() {}
};

#endif // XBEE_FRAMEABLE_PACKET_INCLUDED
