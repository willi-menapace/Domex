/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo <simone@lorengo.org> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Simone Lorengo <simone@lorengo.org>
 */

#include "GlobalDefines.h"

#ifdef ONBOARD

#include <Arduino.h>

#include "HumiditySensor.h"
#include "GroundHumidity.cpp"

SensorType HumiditySensor::getType() {
    return type;
}

long HumiditySensor::readValue() {
	long humidity = ((getRaw() + offset) * 100) / 1023;
	return humidity;
}

GroundHumidity HumiditySensor::readRange() {
	long humidity = getRaw() + offset;
	const int	MAX = 1023 + offset,
				MIN = 0 + offset;

	if (humidity >= MIN         && humidity < DRY)         return COMPLETELY_DRY;
	else if (humidity >= DRY         && humidity < QUITE_HUMID) return DRY;
	else if (humidity >= QUITE_HUMID && humidity < HUMID)       return QUITE_HUMID;
	else if (humidity >= HUMID       && humidity < QUITE_WET)   return HUMID;
	else if (humidity >= QUITE_WET   && humidity < WET)         return QUITE_WET;
	else if (humidity >= WET         && humidity < MAX)         return WET;
	else return ERROR;
}

long HumiditySensor::getRaw() {
	return analogRead(sensorPin);
}

void HumiditySensor::setOffset(long calibOffset){
	offset = calibOffset;
}

bool HumiditySensor::check() {
	const int READINGS = 10; //Numero di letture da eseguire
	int values[READINGS],    //Array di valori letti
		deltaValues = 0;     //Differenza tra un valore e un altro
	values[0] = analogRead(sensorPin);

	for (int i = 1; i < READINGS; i++) {
		values[i] = analogRead(sensorPin);
		deltaValues += values[i - 1] - values[i];
		delay(200);
	}

	if (deltaValues >(READINGS / 2)) return false;
	else return true;
}

HumiditySensor::HumiditySensor(int sensorAnalogPin) {
	type = HUMIDITY;

	sensorPin = sensorAnalogPin;
	offset = 0;
}

#endif //ONBAORD
