/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo <simone@lorengo.org> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Simone Lorengo <simone@lorengo.org>
 */

#ifndef VISIBLE_LIGHT_SENSOR_INCLUDED
#define VISIBLE_LIGHT_SENSOR_INCLUDED

#include "GlobalDefines.h"

#ifdef ONBOARD

#include <Wire.h>
#include "TSL2561.h"
#include "Sensor.h"

/**
* Rappresenta un sensore di luce visibile digitale
* Modello di riferimento: TLS 2561 (Visibile)
*/

class VisibleLightSensor : public Sensor {

private:
    SensorType type;
	TSL2561 sensor;
	int sensorAddress;
	long offset;
	bool checkPass;

public:

    SensorType getType();

	/**
	* @return valore elaborato del sensore
	*/
    long readValue();

	/**
	* Verifica che il sensore sia acceso, collegato, funzionante e dialoghi in maniera corretta
	* @return corretto funzionamento sensore
	*/
	bool check();

	/**
	* Imposta un offset per la calibrazione del sensore
	*/
	void setOffset(long calibOffset);

	/**
	* Crea un sensore di tipo LIGHT (VISIBILE)
	*/
	VisibleLightSensor();

    /**
    * OVERLOAD DEL COSTRUTTORE
    *
    * @param address Indirizzo SPI del sensore
    */
	VisibleLightSensor(int address);

};

#endif //ONBOARD

#endif // VISIBLE_LIGHT_SENSOR_INCLUDED
