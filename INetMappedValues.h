/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef INET_MAPPED_VALUES_INCLUDED
#define INET_MAPPED_VALUES_INCLUDED

#include "GlobalDefines.h"

#include "INetPacket.h"

/**
* Pacchetto contenente valori mappati a degli ID
* Non e' un tipo di pacchetto direttamente utilizzabile quindi non ha un tipo
*/
class INetMappedValues : public INetPacket {
protected:

    static const int VALUES_COUNT_INDEX; //Indice in cui si trova il numero di valori

public:

    /**
    * @return il numero di valori
    */
    unsigned char getValuesCount();
    /**
    * @return mappa tra indici dell'array dei valori e gli ID
    *         a cui i valori fanno riferimento
    */
    unsigned char *getIds();
    /**
    * @return i valori
    */
    long *getValues();
    /**
    * Costruisce un pacchetto contenente valori
    *
    * @param buffer array in cui sara' costruito il pacchetto.
    *               Deve essere in grado di contenere tutti gli altri parametri + 1 byte
    * @param valuesCount il numero di valori letti
    * @param ids mappa tra indici dell'array values e gli ID
    *            a cui i valori fanno riferimento
    * @param values i valori
    */
    INetMappedValues(unsigned char *buffer, unsigned char valuesCount,
                    unsigned char *ids, long *values);
    /**
    * Trasforma il pacchetto generico nel pacchetto della classe corrente
    *
    * @param packet il pacchetto da convertire.
    */
    INetMappedValues(INetPacket *packet);
};

#endif // INET_MAPPED_VALUES_INCLUDED
