/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "INetMappedValues.h"

#include "GlobalDefines.h"

#include <assert.h>

const int INetMappedValues::VALUES_COUNT_INDEX = 1;

unsigned char INetMappedValues::getValuesCount() {
    return data[VALUES_COUNT_INDEX];
}

unsigned char *INetMappedValues::getIds() {
    return &data[VALUES_COUNT_INDEX + 1]; //La mappatura indici-ID segue subito il numero di valori
}

long *INetMappedValues::getValues() {
    return (long *) &data[VALUES_COUNT_INDEX + getValuesCount() + 1]; //I valori seguono subito la mappatura indici-ID
}

INetMappedValues::INetMappedValues(unsigned char *buffer, unsigned char valuesCount,
                                   unsigned char *ids, long *values)
                                   //Calcola le dimensioni del pacchetto, 1 byte in piu' per il tipo pacchetto
                                   : INetPacket(buffer, sizeof(valuesCount) + sizeof(unsigned char) * valuesCount + sizeof(long) * valuesCount + 1) {

    assert(buffer != 0);
    if(valuesCount > 0) {
        assert(ids != 0);
        assert(values != 0);
    }

    //Inserisce il numero di valori
    data[VALUES_COUNT_INDEX] = valuesCount;

    if(valuesCount != 0) {
        //Inserisce la mappatura indici-ID
        for(int i = 0; i < valuesCount; ++i) {
            data[i + VALUES_COUNT_INDEX + 1] = ids[i];
        }

        //Copia tutti i byte dei valori nel buffer
        unsigned char *currentByte = (unsigned char *) &values[0];
        for(unsigned int i = 0; i < valuesCount * sizeof(long); ++i) {
            data[i + VALUES_COUNT_INDEX + 1 + valuesCount] = *currentByte;
            ++currentByte;
        }
    }
}

INetMappedValues::INetMappedValues(INetPacket *packet) : INetPacket(packet) {
    //Deve solo richiamare il costruttore della superclasse
}
