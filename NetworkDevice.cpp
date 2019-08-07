/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "NetworkDevice.h"

#include "GlobalDefines.h"

#include <assert.h>

const long NetworkDevice::READING_UNAVAILABLE = -2147483647;

void NetworkDevice::initialize(unsigned char _address[ADDRESS_BYTES],
                               unsigned char _sensorsCount,
                               SensorType _sensorTypes[],
                               unsigned char _outputsCount, OutputType _outputTypes[],
                               unsigned long _maxSilencePeriod) {

    assert(_address != 0);
    assert(_sensorsCount <= NETWORK_DEVICE_MAX_SENSORS);
    assert(_outputsCount <= NETWORK_DEVICE_MAX_OUTPUTS);
    if(_sensorsCount > 0) {
        assert(_sensorTypes != 0);
    }
    if(_outputsCount > 0) {
        assert(_outputTypes != 0);
    }

    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        address[i] = _address[i];
    }

    sensorsCount = _sensorsCount;
    for(int i = 0; i < sensorsCount; ++i) {
        sensorTypes[i] = _sensorTypes[i];
        //Nessun valore e' ancora stato letto dal sensore
        latestReadings[i] = READING_UNAVAILABLE;
    }

    outputsCount = _outputsCount;
    for(int i = 0; i < outputsCount; ++i) {
        outputTypes[i] = _outputTypes[i];
    }

    maxSilencePeriod = _maxSilencePeriod;
}

void NetworkDevice::initialize(FileReader *fileReader) {
    //Legge l'indirizzo
    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        address[i] = fileReader->readByte();
    }

    //Legge numero di sensori, tipo e ultime letture
    sensorsCount = fileReader->readByte();
    assert(sensorsCount <= NETWORK_DEVICE_MAX_SENSORS);
    for(int i = 0; i < sensorsCount; ++i) {
        sensorTypes[i] = (SensorType) fileReader->readByte();
    }
    for(int i = 0; i < sensorsCount; ++i) {
        latestReadings[i] = (long) fileReader->readLong();
    }

    //Legge numero e tipo di output
    outputsCount = fileReader->readByte();
    assert(outputsCount <= NETWORK_DEVICE_MAX_OUTPUTS);
    for(int i = 0; i < outputsCount; ++i) {
        outputTypes[i] = (OutputType) fileReader->readByte();
    }

    //Legge massimo periodo di silenzio
    maxSilencePeriod = fileReader->readLong();
}

void NetworkDevice::setLatestReading(unsigned char sensorId, long value) {
    assert(sensorId < sensorsCount);

    latestReadings[sensorId] = value;
}

long NetworkDevice::getLatestReading(unsigned char sensorId) {
    assert(sensorId < sensorsCount);

    return latestReadings[sensorId];
}

unsigned char *NetworkDevice::getAddress() {
    return address;
}

unsigned long NetworkDevice::getMaxSilencePeriod() {
    return maxSilencePeriod;
}

void NetworkDevice::writeToFile(FileWriter *fileWriter) {

    assert(fileWriter != 0);
    assert(fileWriter->isOpen());

    //Scrive l'indirizzo
    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        fileWriter->writeByte(address[i]);
    }

    //Scrive numero di sensori, tipo e ultime letture
    fileWriter->writeByte(sensorsCount);
    for(int i = 0; i < sensorsCount; ++i) {
        fileWriter->writeByte((unsigned char) sensorTypes[i]);
    }
    for(int i = 0; i < sensorsCount; ++i) {
        fileWriter->writeLong((unsigned long) latestReadings[i]);
    }

    //Scrive numero e tipo di output
    fileWriter->writeByte(outputsCount);
    for(int i = 0; i < outputsCount; ++i) {
        fileWriter->writeByte((unsigned char) outputTypes[i]);
    }

    fileWriter->writeLong(maxSilencePeriod);
}

bool NetworkDevice::equals(NetworkDevice *otherDevice) {
    if(otherDevice == 0) {
        return false;
    }

    //Un'istanza e' uguale a se stessa
    if(otherDevice == this) {
        return true;
    }

    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        if(address[i] != otherDevice->address[i]) {
            return false;
        }
    }

    if(sensorsCount != otherDevice->sensorsCount) {
        return false;
    }

    for(int i = 0; i < sensorsCount; ++i) {
        if(sensorTypes[i] != otherDevice->sensorTypes[i]) {
            return false;
        }
        if(latestReadings[i] != otherDevice->latestReadings[i]) {
            return false;
        }
    }

    if(outputsCount != otherDevice->outputsCount) {
        return false;
    }

    for(int i = 0; i < outputsCount; ++i) {
        if(outputTypes[i] != otherDevice->outputTypes[i]) {
            return false;
        }
    }

    if(maxSilencePeriod != otherDevice->maxSilencePeriod) {
        return false;
    }

    return true;
}
