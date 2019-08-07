/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo <simone@lorengo.org> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Simone Lorengo <simone@lorengo.org>
 */

#ifndef CURRENT_SENSOR_INCLUDED
#define CURRENT_SENSOR_INCLUDED

#include "GlobalDefines.h"

#ifdef ONBOARD

#include "EmonLib.h"
#include "Sensor.h"

/**
* Rappresenta un sensore di corrente elettrica alternata
* Modello di riferimento: ACS712 20A
*/

class CurrentSensor : public Sensor {

private:
    SensorType type;
	int sensorPin;
	long offset;
	EnergyMonitor sensor;

	/**
	* @return valore non elaborato
	*/
	long getRaw();

public:

    SensorType getType();

	/**
	* @return valore elaborato del sensore in Amper RMS (Irms)
	*/
    long readValue();

	/**
	* Imposta un offset per la calibrazione del sensore
	*/
	void setOffset(long calibOffset);

	/**
	* Crea un sensore di tipo CURRENT
	*/
	CurrentSensor(int sensorAnalogPin);

};
#endif //ONBOARD

#endif // CURRENT_SENSOR_INCLUDED
