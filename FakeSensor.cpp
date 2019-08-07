/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "FakeSensor.h"

#include "GlobalDefines.h"

SensorType FakeSensor::getType() {
    return type;
}

long FakeSensor::readValue() {
    return defaultValue;
}

FakeSensor::FakeSensor(SensorType _type, long _defaultValue) {
    type = _type;
    defaultValue = _defaultValue;
}
