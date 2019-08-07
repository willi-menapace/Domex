/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "INetOutputRequest.h"

#include "GlobalDefines.h"

#include "INetPacketType.cpp"

#include <assert.h>

INetOutputRequest::INetOutputRequest(INetPacket *packet) : INetMappedValues(packet) {
    assert(packet->getType() == OUTPUT_REQUEST);
}

INetOutputRequest::INetOutputRequest(unsigned char *buffer, unsigned char valuesCount,
                                     unsigned char *outputIds, long *outputValues)
                                     : INetMappedValues(buffer, valuesCount, outputIds, outputValues) {


    data[0] = OUTPUT_REQUEST; //Deve solo inserire il tipo di pacchetto
}
