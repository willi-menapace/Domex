/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef SENSOR_TYPE_INCLUDED
#define SENSOR_TYPE_INCLUDED

#include "GlobalDefines.h"

enum SensorType {
    //Si fissano i valori perche' devono essere conosciuti
    //a priori per la scrittura e lettura da file/rete
    TEMPERATURE = 0,
    VISIBLE_LIGHT = 1,
	INFRARED_LIGHT = 2,
    HUMIDITY = 3,
    CURRENT = 4
};

#endif // SENSOR_TYPE_INCLUDED
