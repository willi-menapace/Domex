/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_TRANSMIT_STATUS_INCLUDED
#define XBEE_TRANSMIT_STATUS_INCLUDED

#include "GlobalDefines.h"

#include "XBeeFrameablePacket.h"

/**
* Pacchetto rappresentante un frame di tipo Transmit Status
*/
class XBeeTransmitStatus : public XBeeFrameablePacket {
private:
    static const unsigned char NET_ADDRESS_INDEX;
    static const unsigned char TRANSMIT_RETRIES_INDEX;
    static const unsigned char DELIVERY_STATUS_INDEX;
    static const unsigned char DISCOVERY_STATUS_INDEX;

public:

    /**
    * @return true se il pacchetto e' stato consegnato correttamente
    */
    bool receivedCorrectly();
    /**
    * Ottiene l'indirizzo di rete del dispositivo a cui si e' inviato il messaggio
    * in caso non fosse gia' noto.
    * Richiamabile solo se receivedCorrectly() == true
    *
    * @return indirizzo di rete del destinatario
    */
    unsigned char *getDestinationNetAddress();

    /**
    * Trasforma il pacchetto di risposta generico in un Transmit Status
    *
    * @param packet il pacchetto di risposta generica
    *        deve essere packet->getType() == TRANSMIT_STATUS
    */
    XBeeTransmitStatus(XBeePacket *packet);

};

#endif // XBEE_TRANSMIT_STATUS_INCLUDED
