/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "ConstantCondition.h"

#include "GlobalDefines.h"

#include "ConditionTypes.cpp"

#include <assert.h>

ConditionTypes ConstantCondition::getType() {
    return CONSTANT_CONDITION;
}

bool ConstantCondition::test(NetworkResources *networkResources, DateTime *now) {

    assert(networkResources != 0);
    assert(now != 0);

    //Ottiene il device di cui si deve verificare il valore
    NetworkDevice *device = networkResources->getDeviceByAddress(sensorAddress);
    //Se il device non e' presente nella rete allora la condizione fallisce
    if(device == 0) {
        return false;
    }

    long latestValue = device->getLatestReading(sensorId);
    //Se nessun valore e' disponibile allora la condizione e' considerata falsa
    if(latestValue == NetworkDevice::READING_UNAVAILABLE) {
        return false;
    }

    switch(comparisonOperator) {
    case LOWER_THAN:
        if(latestValue < testValue) {
            return true;
        }
        break;
    case EQUAL_TO:
        if(latestValue == testValue) {
            return true;
        }
        break;
    case HIGHER_THAN:
        if(latestValue > testValue) {
            return true;
        }
        break;
    default:
        assert(false);
        break;
    }
    return false;
}

void ConstantCondition::initialize(unsigned char _sensorAddress[ADDRESS_BYTES], unsigned char _sensorId, long _testValue, ComparisonOperator _comparisonOperator) {

    assert(_sensorAddress != 0);

    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        sensorAddress[i] = _sensorAddress[i];
    }
    sensorId = _sensorId;
    testValue = _testValue;
    comparisonOperator = _comparisonOperator;
}

void ConstantCondition::initialize(FileReader *fileReader) {
    assert(fileReader != 0);
    assert(fileReader->isOpen());

    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        sensorAddress[i] = fileReader->readByte();
    }

    sensorId = fileReader->readByte();
    testValue = fileReader->readLong();
    comparisonOperator = (ComparisonOperator) fileReader->readByte();
}

void ConstantCondition::writeToFile(FileWriter *fileWriter) {

    assert(fileWriter != 0);
    assert(fileWriter->isOpen());

    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        fileWriter->writeByte(sensorAddress[i]);
    }
    fileWriter->writeByte(sensorId);
    fileWriter->writeLong(testValue);
    fileWriter->writeByte((unsigned char) comparisonOperator);
}

ConstantCondition::~ConstantCondition() {
}
