/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "DeviceManager.h"

#include "GlobalDefines.h"

#include <assert.h>

#ifdef OFFBOARD
#include <time.h>
#endif // OFFBOARD

#ifdef ONBOARD
#include <Arduino.h>
#endif // ONBOARD

/**
* Ritorna i millisecondi trascorsi dall'avvio del sistema
*
* Una volta arrivato all'overflow il tempo di sistema riparte da 0
*/
unsigned long getSystemTime() {

    #ifdef OFFBOARD
    unsigned long t = clock();
    return ((double) t / CLOCKS_PER_SEC) * 1000;
    #endif // OFFBOARD

    #ifdef ONBOARD
    return (unsigned long) millis();
    #endif // ONBOARD
}

void DeviceManager::SensorManager::refresh(unsigned long currentTime) {

    //Se e' passato il tempo minimo di lettura o se si e' riavviato il contatore di sistema
    if(currentTime >= lastReadingTime + sensorRefreshTime || currentTime < lastReadingTime) {
        lastReadValue = sensor->readValue();
        lastReadingTime = currentTime;
        newValueAvailable = true;
    }
}

bool DeviceManager::SensorManager::isAvailable() {
    return newValueAvailable;
}

long DeviceManager::SensorManager::getValue() {
    newValueAvailable = false;
    return lastReadValue;
}

Sensor *DeviceManager::SensorManager::getSensor() {
    return sensor;
}

void DeviceManager::SensorManager::initialize(Sensor *_sensor, unsigned long _sensorRefreshTime) {
    assert(_sensor != 0);

    sensor = _sensor;
    sensorRefreshTime = _sensorRefreshTime;

    //Non sono mai state effettuate letture
    lastReadValue = 0;
    newValueAvailable = false;
    lastReadingTime = 0;
}

DeviceManager::DeviceManager() {
    sensorsCount = 0;
    outputsCount = 0;
}

void DeviceManager::registerSensor(Sensor *sensor,  unsigned long sensorRefreshTime) {

    assert(sensorsCount < DEVICE_MANAGER_MAX_SENSORS);
    assert(sensor != 0);

    sensors[sensorsCount].initialize(sensor, sensorRefreshTime);
    sensorTypes[sensorsCount] = sensor->getType(); //Imposta il tipo nell'array dedicato
    ++sensorsCount;
}

void DeviceManager::registerOutput(Output *output) {

    assert(outputsCount < DEVICE_MANAGER_MAX_OUTPUTS);
    assert(output != 0);

    outputs[outputsCount] = output;
    outputTypes[outputsCount] = output->getType(); //Imposta il tipo nell'array dedicato
    ++outputsCount;
}

unsigned char DeviceManager::getSensorsCount() {
    return sensorsCount;
}

unsigned char DeviceManager::getOutputsCount() {
    return outputsCount;
}

SensorType *DeviceManager::getSensorTypes() {

    return sensorTypes;
}

OutputType *DeviceManager::getOutputTypes() {

    return outputTypes;
}

void DeviceManager::refreshSensors() {
    unsigned long currentTime = getSystemTime();

    for(unsigned int i = 0; i < sensorsCount; ++i) {
        sensors[i].refresh(currentTime);
    }
}

unsigned char DeviceManager::getAvailableValuesCount() {
    unsigned char availableValues = 0;

    for(unsigned int i = 0; i < sensorsCount; ++i) {
        if(sensors[i].isAvailable()) {
            ++availableValues;
        }
    }

    return availableValues;
}

void DeviceManager::getAvailableValues(unsigned char *ids, long *values) {
    unsigned char readValues = 0;

    for(unsigned int i = 0; i < sensorsCount; ++i) {
        if(sensors[i].isAvailable()) {
            ids[readValues] = i;
            values[readValues] = sensors[i].getValue();
            ++readValues;
        }
    }
}

void DeviceManager::performOutput(unsigned char outputId, long value) {
    assert(outputId < outputsCount);

    outputs[outputId]->performOutput(value);
}
