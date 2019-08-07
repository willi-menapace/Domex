/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef FAKE_SENSOR_INCLUDED
#define FAKE_SENSOR_INCLUDED

#include "GlobalDefines.h"

#include "Sensor.h"

/**
* Sensore simulato con tipo e valore restituito di default
* impostabili
*/
class FakeSensor : public Sensor {

private:
    SensorType type;
    long defaultValue;

public:

    SensorType getType();
    long readValue();

    /**
    * Crea un sensore di un determinato tipo che
    * restituisce sempre lo stesso valore
    *
    * @param _type il tipo di sensore
    * @param _defaultValue il valore che verra' sempre restituito dal sensore
    */
    FakeSensor(SensorType _type, long _defaultValue);

};

#endif // FAKE_SENSOR_INCLUDED
