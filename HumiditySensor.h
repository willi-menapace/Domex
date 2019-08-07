/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo <simone@lorengo.org> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Simone Lorengo <simone@lorengo.org>
 */

#ifndef HUMIDITY_SENSOR_INCLUDED
#define HUMIDITY_SENSOR_INCLUDED

#include "GlobalDefines.h"

#ifdef ONBOARD

#include "GroundHumidity.cpp"
#include "Sensor.h"

/**
* Rappresenta un sensore di umidità del terreno analogico
* Modello di riferimento: Moisture Sensor
*/

class HumiditySensor : public Sensor {

private:
    SensorType type;
	int sensorPin;
	long offset;

	/**
	* @return valore non elaborato
	*/
	long getRaw();

public:

    SensorType getType();

	/**
	* @return valore elaborato del sensore
	*/
    long readValue();

	/**
	* @return valore del sensore con enumeratore GroundHumidity;
	*/
	GroundHumidity readRange();

	/**
	* Verifica che il sensore sia collegato basandosi sui valori letti. Non completamente affidabile e genera un blocco di circa 2 secondi
	* @return corretto funzionamento sensore
	*/
	bool check();

	/**
	* Imposta un offset per la calibrazione del sensore
	*/
	void setOffset(long calibOffset);

	/**
	* Crea un sensore di tipo HUMIDITY
	*/
	HumiditySensor(int sensorAnalogPin);

};

#endif //ONBOARD

#endif // HUMIDITY_SENSOR_INCLUDED
