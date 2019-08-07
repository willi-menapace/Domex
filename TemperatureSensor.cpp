/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo <simone@lorengo.org> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Simone Lorengo <simone@lorengo.org>
 */

#include "TemperatureSensor.h"

#include "GlobalDefines.h"

#ifdef ONBOARD

#include <Wire.h>

SensorType TemperatureSensor::getType() {
    return type;
}

long TemperatureSensor::readValue() {
	long celsius = getRaw() + offset;
	return celsius;
}

long TemperatureSensor::getBits() {
	Wire.requestFrom(sensorAddress, 2);

	unsigned char MSB = Wire.read();
	unsigned char LSB = Wire.read();

	int temperatureSum = ((MSB << 8) | LSB) >> 4;

	return temperatureSum;
}

long TemperatureSensor::getRaw() {
	long celsius = getBits() * 0.0625;
	return celsius;
}

void TemperatureSensor::setOffset(long calibOffset){
	offset = calibOffset;
}

bool TemperatureSensor::check() {
	int bitVal = getBits();
	if (bitVal == -1) return false;
	else return true;
}

TemperatureSensor::TemperatureSensor() {
    type = TEMPERATURE;

	Wire.begin();
	sensorAddress = 0x48;
	offset = 0;
}

TemperatureSensor::TemperatureSensor(int address) {
	type = TEMPERATURE;

	Wire.begin();
	sensorAddress = address;
	offset = 0;
}

#endif //ONBOARD
