/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_RECEIVE_PACKET_INCLUDED
#define XBEE_RECEIVE_PACKET_INCLUDED

#include "GlobalDefines.h"

#include "XBeePacket.h"

/**
* Pacchetto rappresentante un frame Receive Packet
*/
class XBeeReceivePacket : public XBeePacket {
private:
    static const int ADDRESS_INDEX;
    static const int NET_ADDRESS_INDEX;
    static const int OPTIONS_INDEX;
    static const int DATA_INDEX;
public:
    /**
    * Ottiene la dimensione in byte dei dati applicativi contenuti nel pacchetto
    *
    * @return la dimensione in byte dei dati applicativi
    */
    unsigned char getApplicationDataSize();
    /**
    * Ottiene i dati appicativi contenuti nel pacchetto
    *
    * @return i dati applicativi
    */
    unsigned char *getApplicationData();
    /**
    * Ottiene l'indirizzo del mittente
    *
    * @return indirizzo del mittente
    */
    unsigned char *getSourceAddress();
    /**
    * Ottiene l'indirizzo di rete del mittente
    *
    * @return indirizzo di rete del mittente
    */
    unsigned char *getSourceNetAddress();
    /**
    * Trasforma il pacchetto di risposta generico in un Transmit Status
    *
    * @param packet il pacchetto di risposta generica
    *        deve essere packet->getType() == RECEIVE_PACKET
    */
    XBeeReceivePacket(XBeePacket *packet);
};

#endif // XBEE_RECEIVE_PACKET_INCLUDED
