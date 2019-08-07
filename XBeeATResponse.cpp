/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBeeATResponse.h"

#include "GlobalDefines.h"

const unsigned char XBeeATResponse::AT_COMMAND_INDEX = 5;
const unsigned char XBeeATResponse::COMMAND_STATUS_INDEX = 7;
const unsigned char XBeeATResponse::COMMAND_DATA_INDEX = 8;

bool XBeeATResponse::executedCorrectly() {
    unsigned char *buffer = getData();

    //Se il byte e' uguale a 0 non ci sono stati problemi nell'esecuzione
    return buffer[COMMAND_STATUS_INDEX] == 0;
}

unsigned char XBeeATResponse::getQueriedDataSize() {
    return getLength() - (COMMAND_DATA_INDEX - PAYLOAD_INDEX);
}

unsigned char *XBeeATResponse::getQueriedData() {
    unsigned char *buffer = getData();

    return &buffer[COMMAND_DATA_INDEX];
}

XBeeATResponse::XBeeATResponse(XBeePacket *packet) : XBeeFrameablePacket(packet) {}
