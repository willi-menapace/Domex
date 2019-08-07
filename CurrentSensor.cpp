/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo <simone@lorengo.org> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Simone Lorengo <simone@lorengo.org>
 */

#include "GlobalDefines.h"

#ifdef ONBOARD

#include "CurrentSensor.h"
#include "EmonLib.h"

SensorType CurrentSensor::getType() {
    return type;
}

long CurrentSensor::readValue() {
	long ampRms = ((sensor.calcIrms(1480) - offset) * 1000);
	return ampRms;
}

long CurrentSensor::getRaw() {
	return analogRead(sensorPin);
}

void CurrentSensor::setOffset(long calibOffset){
	offset = calibOffset;
}

CurrentSensor::CurrentSensor(int sensorAnalogPin) {
	type = CURRENT;

	sensorPin = sensorAnalogPin;
	sensor.current(sensorPin, 11.8);
	offset = 0;
}

#endif //ONBOARD
