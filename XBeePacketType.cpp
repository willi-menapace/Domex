/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_PACKET_TYPE_INCLUDED
#define XBEE_PACKET_TYPE_INCLUDED

#include "GlobalDefines.h"

enum XBeePacketType {
    AT_COMMAND = 0x08,
    AT_RESPONSE = 0x88,
    TRANSMIT_REQUEST = 0x10,
    TRANSMIT_STATUS = 0x8B,
    RECEIVE_PACKET = 0x90,
    IO_DATA_SAMPLE = 0x92,
    REMOTE_AT_COMMAND_REQUEST = 0x17,
    REMOTE_AT_COMMAND_RESPONSE = 0x97,
    UNKNOWN_PACKET
};

#endif // XBEE_PACKET_TYPE_INCLUDED
