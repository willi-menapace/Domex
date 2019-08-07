/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "INetSensorValues.h"

#include "GlobalDefines.h"

#include "INetPacketType.cpp"

#include <assert.h>

INetSensorValues::INetSensorValues(INetPacket *packet) : INetMappedValues(packet) {
    assert(packet->getType() == SENSOR_VALUES);
}

INetSensorValues::INetSensorValues(unsigned char *buffer, unsigned char valuesCount,
                                   unsigned char *sensorIds, long *sensorValues)
                                   : INetMappedValues(buffer, valuesCount, sensorIds, sensorValues) {


    data[0] = SENSOR_VALUES; //Deve solo inserire il tipo di pacchetto
}
