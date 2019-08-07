/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_REMOTE_AT_COMMAND_INCLUDED
#define XBEE_REMOTE_AT_COMMAND_INCLUDED

#include "GlobalDefines.h"

#include "XBeeFrameablePacket.h"

/**
* Pacchetto rappresentante un frame Remote AT Command
*
*/
class XBeeRemoteATCommand : public XBeeFrameablePacket {
private:
    static const int ADDRESS_INDEX;
    static const int NET_ADDRESS_INDEX;
    static const int OPTIONS_INDEX;
    static const int AT_COMMAND_INDEX;
    static const int PARAMETERS_INDEX;

    unsigned char frameId;  //Id del frame corrente
    unsigned char options;  //Opzioni di trasmissione
public:
    /**
    * Crea un pacchetto destinato all'indirizzo specificato
    *
    * @param _atCommand il comando da eseguire. Deve essere 2 byte
    * @param _parametersSize la dimensione in byte dei parametri
    * @param _parameters i parametri opzionali del comando, MSB nel primo byte
    * @param _destinationAddress indirizzo fisico del destinatario
    * @param _destinationNetAddress indirizzo di rete del destinatario, 0xFFFE se sconosciuto
    */
    XBeeRemoteATCommand(unsigned char *_atCommand, unsigned char _parametersSize, unsigned char *_parameters,
                        unsigned char *_destinationAddress, unsigned char *_destinationNetAddress);
};

#endif // XBEE_REMOTE_AT_COMMAND_INCLUDED
