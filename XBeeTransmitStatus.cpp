/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBeeTransmitStatus.h"

#include "GlobalDefines.h"

const unsigned char XBeeTransmitStatus::NET_ADDRESS_INDEX = 5;
const unsigned char XBeeTransmitStatus::TRANSMIT_RETRIES_INDEX = 7;
const unsigned char XBeeTransmitStatus::DELIVERY_STATUS_INDEX = 8;
const unsigned char XBeeTransmitStatus::DISCOVERY_STATUS_INDEX = 9;

bool XBeeTransmitStatus::receivedCorrectly() {
    unsigned char *buffer = getData();

    return buffer[DELIVERY_STATUS_INDEX] == 0;
}

unsigned char *XBeeTransmitStatus::getDestinationNetAddress() {
    unsigned char *buffer = getData();

    return &buffer[NET_ADDRESS_INDEX];
}

XBeeTransmitStatus::XBeeTransmitStatus(XBeePacket *packet) : XBeeFrameablePacket(packet) {}
