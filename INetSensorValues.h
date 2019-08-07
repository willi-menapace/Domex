/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef INET_SENSOR_VALUES_INCLUDED
#define INET_SENSOR_VALUES_INCLUDED

#include "GlobalDefines.h"

#include "INetMappedValues.h"

/**
* Pacchetto INet contenente un set di misurazioni
* di sensori del device corrente
*/
class INetSensorValues : public INetMappedValues {

public:

    /**
    * Trasforma il pacchetto generico nel pacchetto della classe corrente
    *
    * @param packet il pacchetto da convertire.
    *        Deve essere packet->getType() == SENSOR_VALUES
    */
    INetSensorValues(INetPacket *packet);
    /**
    * Costruisce un nuovo pacchetto
    *
    * @param buffer array in cui sara' costruito il pacchetto.
    *               Deve essere in grado di contenere tutti gli altri parametri + 1
    * @param valuesCount il numero di valori letti
    * @param sensorIds mappa tra indici dell'array sensorValues e gli ID dei sensori
    *                  a cui i valori fanno riferimento
    * @param sensorValues i valori letti dai sensori
    */
    INetSensorValues(unsigned char *buffer, unsigned char valuesCount,
                     unsigned char *sensorIds, long *sensorValues);
};

#endif // INET_SENSOR_VALUES_INCLUDED
