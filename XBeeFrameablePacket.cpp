/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "XBeeFrameablePacket.h"

#include "GlobalDefines.h"

const unsigned char XBeeFrameablePacket::FRAME_ID_INDEX = 4;

void XBeeFrameablePacket::setFrameId(unsigned char frameId) {
    unsigned char *buffer = getData();

    buffer[FRAME_ID_INDEX] = frameId;
}

unsigned char XBeeFrameablePacket::getFrameId() {
    unsigned char *buffer = getData();

    return buffer[FRAME_ID_INDEX];
}

XBeeFrameablePacket::XBeeFrameablePacket() {}

XBeeFrameablePacket::XBeeFrameablePacket(XBeePacket *packet) : XBeePacket(packet) {}
