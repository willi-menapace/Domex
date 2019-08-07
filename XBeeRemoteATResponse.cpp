/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBeeRemoteATResponse.h"

#include "GlobalDefines.h"

const int XBeeRemoteATResponse::ADDRESS_INDEX = 5;
const int XBeeRemoteATResponse::NET_ADDRESS_INDEX = 13;
const int XBeeRemoteATResponse::AT_COMMAND_INDEX = 15;
const int XBeeRemoteATResponse::COMMAND_STATUS_INDEX = 17;
const int XBeeRemoteATResponse::COMMAND_DATA_INDEX = 18;

bool XBeeRemoteATResponse::executedCorrectly() {
    unsigned char *buffer = getData();

    return buffer[COMMAND_STATUS_INDEX] == 0;
}

unsigned char XBeeRemoteATResponse::getQueriedDataSize() {
    return getLength() - (COMMAND_DATA_INDEX - PAYLOAD_INDEX);
}

unsigned char *XBeeRemoteATResponse::getQueriedData() {
    unsigned char *buffer = getData();

    return &buffer[COMMAND_DATA_INDEX];
}

XBeeRemoteATResponse::XBeeRemoteATResponse(XBeePacket *packet) : XBeeFrameablePacket(packet) {}
