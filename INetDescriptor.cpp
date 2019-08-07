/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "INetDescriptor.h"

#include "GlobalDefines.h"

#include <assert.h>

unsigned const int INetDescriptor::SENSORS_DESCRIPTION_INDEX = 1;

unsigned char INetDescriptor::getSensorsCount() {
    return data[SENSORS_DESCRIPTION_INDEX]; //Il numero di sensori e' all'inizio della loro descrizione
}

unsigned char INetDescriptor::getOutputsCount() {
    return data[outputsDescriptionIndex]; //Il numero di output e' all'inizio della loro descrizione
}

SensorType INetDescriptor::getSensorById(unsigned char id) {
    //L'ID deve essere in range
    assert(id < data[SENSORS_DESCRIPTION_INDEX]);

    return (SensorType) data[SENSORS_DESCRIPTION_INDEX + 1 + id];
}

OutputType INetDescriptor::getOutputById(unsigned char id) {
    //L'ID deve essere in range
    assert(id < data[outputsDescriptionIndex]);

    return (OutputType) data[outputsDescriptionIndex + 1 + id];
}

unsigned long INetDescriptor::getMaxSilencePeriod() {
    return (unsigned long) data[maxSilencePeriodIndex];
}

INetDescriptor::INetDescriptor(INetPacket *packet) : INetPacket(packet) {
    assert(packet->getType() == DESCRIPTOR);

    //Si trova un indice dopo quello in cui e' posizionato il numero di sensori aumentato del numero di sensori
    outputsDescriptionIndex = data[SENSORS_DESCRIPTION_INDEX] + SENSORS_DESCRIPTION_INDEX + 1;
    maxSilencePeriodIndex = data[outputsDescriptionIndex] + outputsDescriptionIndex + 1;
}

INetDescriptor::INetDescriptor(unsigned char *buffer,
                               unsigned char sensorsCount, SensorType *sensorTypes,
                               unsigned char outputsCount, OutputType *outputTypes,
                               unsigned long maxSilencePeriod)
                               //Calcola le dimensioni del pacchetto, 1 in piu' per il byte di tipo
                               : INetPacket(buffer, 2 + sensorsCount + outputsCount + sizeof(unsigned long) + 1) {

    data[0] = DESCRIPTOR; //Imposta il tipo di pacchetto

    data[1] = sensorsCount; //Imposta il numero di sensori
    //Scrive i tipi di sensori
    for(int i = 0; i < sensorsCount; ++i) {
        data[i + 2] = (unsigned char) sensorTypes[i];
    }

    outputsDescriptionIndex = 2 + sensorsCount;

    //Imposta la descrizione degli output subito dopo la descrizione dei sensori
    data[outputsDescriptionIndex] = outputsCount;
    for(int i = 0; i < outputsCount; ++i) {
        data[i + outputsDescriptionIndex + 1] = (unsigned char) outputTypes[i];
    }

    //Imposta maxSilencePeriod subito dopo la descrizione degli output
    maxSilencePeriodIndex = outputsDescriptionIndex + outputsCount + 1;
    unsigned char *currentByte = (unsigned char *) &maxSilencePeriod;
    for(unsigned int i = 0; i < sizeof(unsigned long); ++i) {
        data[i + maxSilencePeriodIndex] = *currentByte;
        ++currentByte;
    }
}
