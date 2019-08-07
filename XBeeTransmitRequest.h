/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_TRANSMIT_REQUEST
#define XBEE_TRANSMIT_REQUEST

#include "GlobalDefines.h"

#include "XBeeFrameablePacket.h"

/**
* Pacchetto rappresentante un frame Transmit Request
*/
class XBeeTransmitRequest : public XBeeFrameablePacket {
private:
    static const int ADDRESS_INDEX;
    static const int NET_ADDRESS_INDEX;
    static const int BROADCAST_RADIUS_INDEX;
    static const int DATA_INDEX;
    static const int OPTIONS_INDEX;

    unsigned char frameId;  //Id del frame corrente
    unsigned char options;  //Opzioni di trasmissione
    unsigned char broadcastRadius;  //Raggio massimo di broadcast

public:
    /**
    * Crea un pacchetto destinato all'indirizzo specificato
    *
    * @param _dataSize la lunghezza in byte del messaggio da trasmettere
    * @param _data il messaggio da trasmettere
    * @param _destinationAddress indirizzo fisico del destinatario
    * @param _destinationNetAddress indirizzo di rete del destinatario, 0xFFFE se sconosciuto
    */
    XBeeTransmitRequest(unsigned char _dataSize, unsigned char *_data,
                        unsigned char *_destinationAddress, unsigned char *_destinationNetAddress);

};

#endif // XBEE_TRANSMIT_REQUEST
