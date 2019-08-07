/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBeeReceivePacket.h"

#include "GlobalDefines.h"

const int XBeeReceivePacket::ADDRESS_INDEX = 4;
const int XBeeReceivePacket::NET_ADDRESS_INDEX = 12;
const int XBeeReceivePacket::OPTIONS_INDEX = 14;
const int XBeeReceivePacket::DATA_INDEX = 15;

unsigned char XBeeReceivePacket::getApplicationDataSize() {
    return getLength() - (DATA_INDEX - PAYLOAD_INDEX);
}

unsigned char *XBeeReceivePacket::getApplicationData() {
    unsigned char *buffer = getData();

    return &buffer[DATA_INDEX];
}

unsigned char *XBeeReceivePacket::getSourceAddress() {
    unsigned char *buffer = getData();

    return &buffer[ADDRESS_INDEX];
}

unsigned char *XBeeReceivePacket::getSourceNetAddress() {
    unsigned char *buffer = getData();

    return &buffer[NET_ADDRESS_INDEX];
}

XBeeReceivePacket::XBeeReceivePacket(XBeePacket *packet) : XBeePacket(packet) {}
