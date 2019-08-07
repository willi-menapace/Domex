/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_AT_COMMAND_INCLUDED
#define XBEE_AT_COMMAND_INCLUDED

#include "GlobalDefines.h"

#include "XBeeFrameablePacket.h"

/**
* Pacchetto rappresentante un frame AT Command
*/
class XBeeATCommand : public XBeeFrameablePacket {
private:
    static const int AT_COMMAND_INDEX;
    static const int PARAMETERS_INDEX;

    unsigned char frameId; //Id del frame corrente

public:
    /**
    * Crea un pacchetto contenente il comando specificato
    *
    * @param _atCommand il comando da eseguire. Deve essere 2 byte
    * @param _parametersSize la dimensione in byte dei parametri
    * @param _parameters i parametri opzionali del comando, MSB nel primo byte
    */
    XBeeATCommand(unsigned char *_atCommand, unsigned char _parametersSize, unsigned char *_parameters);
};

#endif // XBEE_AT_COMMAND_INCLUDED
