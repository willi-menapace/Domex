/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo <simone@lorengo.org> - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Simone Lorengo <simone@lorengo.org>
 */

#ifndef TEMPERATURE_SENSOR_INCLUDED
#define TEMPERATURE_SENSOR_INCLUDED

#include "GlobalDefines.h"

#ifdef ONBOARD

#include "Sensor.h"

/**
* Rappresenta un sensore di temperatura digitale
* Modello di riferimento: TMP 102
*/

class TemperatureSensor : public Sensor {

private:
    SensorType type;
	int sensorAddress;
	long offset;

	/**
	* @return valore non elaborato
	*/
	long getRaw();

	/**
	* @return bits letti dalla porta seriale
	*/
	long getBits();


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
	* Crea un sensore di tipo TEMPERATURE
	*/
	TemperatureSensor();

    /**
    * OVERLOAD DEL COSTRUTTORE
    *
    * @param address Indirizzo SPI del sensore
    */
	TemperatureSensor(int address);

};

#endif //ONBOARD

#endif // TEMPERATURE_SENSOR_INCLUDED
