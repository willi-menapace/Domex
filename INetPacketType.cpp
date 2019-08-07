/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef INET_PACKET_TYPE_INCLUDED
#define INET_PACKET_TYPE_INCLUDED

#include "GlobalDefines.h"

/**
* Tipi di pacchetti applicativi della rete interna
*/
enum INetPacketType {
    SENSOR_VALUES = 0,
    OUTPUT_REQUEST = 1,
    DESCRIPTOR = 2,
    DESCRIPTOR_REQUEST
};

#endif // INET_PACKET_TYPE_INCLUDED
