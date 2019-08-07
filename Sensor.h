/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef SENSOR_INCLUDED
#define SENSOR_INCLUDED

#include "GlobalDefines.h"

#include "SensorType.cpp"

/**
* Sensore in grado di effettuare la lettura di un valore
*/
class Sensor {
public:
    /**
    * @return il tipo di sensore
    */
    virtual SensorType getType() = 0;
    /**
    * Effettua la lettura del valore
    *
    * @return valore a 64 bit rappresentante la lettura
    */
    virtual long readValue() = 0;
    virtual ~Sensor() {}
};

#endif // SENSOR_INCLUDED
