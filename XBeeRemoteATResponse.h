/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_REMOTE_AT_RESPONSE_INCLUDED
#define XBEE_REMOTE_AT_RESPONSE_INCLUDED

#include "GlobalDefines.h"

#include "XBeeFrameablePacket.h"

class XBeeRemoteATResponse : public XBeeFrameablePacket {
private:
    static const int ADDRESS_INDEX;
    static const int NET_ADDRESS_INDEX;
    static const int AT_COMMAND_INDEX;
    static const int COMMAND_STATUS_INDEX;
    static const int COMMAND_DATA_INDEX;
public:
    /**
    * @return true se il comando e' stato eseguito correttamente
    */
    bool executedCorrectly();
    /**
    * @return il numero di byte ottenuti in risposta
    */
    unsigned char getQueriedDataSize();
    /**
    * @return i byte ottenuti in risposta
    */
    unsigned char *getQueriedData();
    /**
    * Trasforma il pacchetto di risposta generico in un AT Response
    *
    * @param packet il pacchetto di risposta generica
    *        deve essere packet->getType() == REMOTE_AT_COMMAND_RESPONSE
    */
    XBeeRemoteATResponse(XBeePacket *packet);

};

#endif // XBEE_REMOTE_AT_RESPONSE_INCLUDED
