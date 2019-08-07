/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo <simone@lorengo.org> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Simone Lorengo <simone@lorengo.org>
 */

#include "VisibleLightSensor.h"

#include "GlobalDefines.h"

#ifdef ONBOARD

#include "Wire.h"
#include "TSL2561.h"

SensorType VisibleLightSensor::getType() {
    return type;
}

long VisibleLightSensor::readValue() {
	/* TIPO DI OUTPUT:
	* TSL2561_INFRARED:		Solo luce infrarossa
	* TSL2561_VISIBLE:		Solo luce visibile
	* TSL2561_FULLSPECTRUM:	Luce visibile e infrarossa
	*/
	long lux = sensor.getLuminosity(TSL2561_VISIBLE) + offset;
	return lux;
}

void VisibleLightSensor::setOffset(long calibOffset){
	offset = calibOffset;
}

bool VisibleLightSensor::check() {
	return checkPass;
}

/* INDIRIZZO DEVICE:
* FLOAT:	Pin di indirizzamento non collegato
* LOW:		Pin di indirizzamento collegato a terra
* HIGH:	Pin di indirizzamento collegato a +5V
*/
VisibleLightSensor::VisibleLightSensor() : sensor(TSL2561_ADDR_FLOAT) {
	type = VISIBLE_LIGHT;

	checkPass = sensor.begin();

	/* GAIN LUCE:
	* TSL2561_GAIN_0X	Nessun Gain
	* TSL2561_GAIN_16X	Gain di 16x
	*/
	sensor.setGain(TSL2561_GAIN_0X);
	/* TEMPO DI INTEGRAZIONE/ESPOSIZIONE:
	* TSL2561_INTEGRATIONTIME_13M		13ms
	* TSL2561_INTEGRATIONTIME_101MS		101ms
	* TSL2561_INTEGRATIONTIME_402MS		402ms
	*/
	sensor.setTiming(TSL2561_INTEGRATIONTIME_13MS);

	offset = 0;
}

VisibleLightSensor::VisibleLightSensor(int address) : sensor(address) {
	type = VISIBLE_LIGHT;

	checkPass = sensor.begin();

	sensor.setGain(TSL2561_GAIN_0X);
	sensor.setTiming(TSL2561_INTEGRATIONTIME_13MS);

	offset = 0;
}

#endif //ONBAORD
