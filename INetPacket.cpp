/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "INetPacket.h"

#include "GlobalDefines.h"

#include <assert.h>

INetPacket::INetPacket(INetPacket *packet) {
    assert(packet != 0);

    //Copia lo stato del pacchetto
    data = packet->data;
    length = packet->length;
}

INetPacketType INetPacket::getType() {
    return (INetPacketType) data[0];
}

unsigned char INetPacket::getLength() {
    return length;
}

unsigned char *INetPacket::getData() {
    return data;
}

INetPacket::INetPacket(unsigned char *_data, unsigned char _length) {

    assert(_data != 0);

    data = _data;
    length = _length;
}
