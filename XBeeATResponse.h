/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_AT_RESPONSE_INCLUDED
#define XBEE_AT_RESPONSE_INCLUDED

#include "GlobalDefines.h"

#include "XBeeFrameablePacket.h"

/**
* Pacchetto rappresentante un frame AT Response
*/
class XBeeATResponse : public XBeeFrameablePacket {
private:
    static const unsigned char AT_COMMAND_INDEX;
    static const unsigned char COMMAND_STATUS_INDEX;
    static const unsigned char COMMAND_DATA_INDEX;
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
    *        deve essere packet->getType() == AT_RESPONSE
    */
    XBeeATResponse(XBeePacket *packet);
};

#endif // XBEE_AR_RESPONDE_INCLUDED
