/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "INetLibraryTestCases.h"

#include "GlobalDefines.h"

#include "INetPacketType.cpp"
#include "INetPacket.h"
#include "INetMappedValues.h"
#include "INetSensorValues.h"
#include "INetOutputRequest.h"
#include "INetDescriptor.h"

#include "SensorType.cpp"
#include "OutputType.cpp"

#include <assert.h>

#ifdef OFFBOARD
#include <iostream>
#endif // OFFBOARD

#ifdef ONBOARD
#include <Arduino.h>
#endif // ONBOARD

/**
* Stampa informazioni a monitor in maniera indipendente dalla
* piattaforma di esecuzione
*
* @param thingToWrite l'elemento da stampare a schermo
*/
template <typename E>
void print(E thingToWrite) {
    #ifdef OFFBOARD
    std::cout << thingToWrite;
    #endif // OFFBOARD

    #ifdef ONBOARD
    Serial.print(thingToWrite);
    #endif // ONBOARD
}

void executeAllINetLibraryTests() {
    print("\n--------------------------------------\n");
    print("Now testing INetPacket\n");
    print("--------------------------------------\n");
    iNetPacketTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing INetMappedValues\n");
    print("--------------------------------------\n");
    iNetMappedValuesTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing INetSensorValues\n");
    print("--------------------------------------\n");
    iNetSensorValuesTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing INetOutputRequest\n");
    print("--------------------------------------\n");
    iNetOutputRequestTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing INetDescriptor\n");
    print("--------------------------------------\n");
    iNetDescriptorTest();
    print("TEST SUCCESSFUL\n");
}

void iNetPacketTest() {
    unsigned const char LENGTH = 5;
    unsigned char data[LENGTH] = {(unsigned char) SENSOR_VALUES, 0x34, 0x45, 0x23, 0x34};

    //Crea un pacchetto con dati casuali
    INetPacket packet(data, LENGTH);

    unsigned char *retrievedData = packet.getData();

    //Verifica che non siano stati alterati i dati
    assert(packet.getType() == SENSOR_VALUES);
    assert(packet.getLength() == LENGTH);

    for(unsigned int i = 0; i < LENGTH; ++i) {
        assert(data[i] == retrievedData[i]);
    }
}

void iNetMappedValuesTest() {
    unsigned const char VALUES_COUNT = 3;
    unsigned char ids[VALUES_COUNT] = {2, 4, 5};
    long values[VALUES_COUNT] = {3456213, 3457894, 0};

    unsigned const char BUFFER_LENGTH = 1 + VALUES_COUNT + VALUES_COUNT * sizeof(long) + 1; //L'ultimo e' per il byte di tipo pacchetto
    unsigned char buffer[BUFFER_LENGTH];

    //Inizializza un pacchetto con un mapping
    INetMappedValues mappedValues(buffer, VALUES_COUNT, ids, values);

    //Verifica che la lunghezza sia calcolata correttamente
    assert(mappedValues.getLength() == BUFFER_LENGTH);

    unsigned char retrievedValuesCount = mappedValues.getValuesCount();
    assert(retrievedValuesCount == VALUES_COUNT);

    //Verifica che non siano stati alterati i dati
    unsigned char *retrievedIds = mappedValues.getIds();
    long *retrievedValues = mappedValues.getValues();
    for(unsigned int i = 0; i < VALUES_COUNT; ++i) {
        assert(ids[i] == retrievedIds[i]);
        assert(values[i] == retrievedValues[i]);
    }

    INetPacket packet(mappedValues.getData(), mappedValues.getLength());

    INetMappedValues recastedMappedValues(&packet);

    //Verifica che non siano stati alterati i dati
    retrievedIds = recastedMappedValues.getIds();
    retrievedValues = recastedMappedValues.getValues();
    for(unsigned int i = 0; i < VALUES_COUNT; ++i) {
        assert(ids[i] == retrievedIds[i]);
        assert(values[i] == retrievedValues[i]);
    }

}

void iNetSensorValuesTest() {
    unsigned const char VALUES_COUNT = 3;
    unsigned char ids[VALUES_COUNT] = {2, 4, 5};
    long values[VALUES_COUNT] = {3456213, 3457894, 0};

    unsigned const char BUFFER_LENGTH = 1 + VALUES_COUNT + VALUES_COUNT * sizeof(unsigned long) + 1; //L'ultimo e' per il byte di tipo pacchetto
    unsigned char buffer[BUFFER_LENGTH];

    INetSensorValues sensorValues(buffer, VALUES_COUNT, ids, values);

    assert(sensorValues.getType() == SENSOR_VALUES);

    INetPacket packet(sensorValues.getData(), sensorValues.getLength());

    assert(packet.getType() == SENSOR_VALUES);
}

void iNetOutputRequestTest() {
    unsigned const char VALUES_COUNT = 3;
    unsigned char ids[VALUES_COUNT] = {2, 4, 5};
    long values[VALUES_COUNT] = {3456213, 3457894, 0};

    unsigned const char BUFFER_LENGTH = 1 + VALUES_COUNT + VALUES_COUNT * sizeof(unsigned long) + 1; //L'ultimo e' per il byte di tipo pacchetto
    unsigned char buffer[BUFFER_LENGTH];

    INetOutputRequest outputRequest(buffer, VALUES_COUNT, ids, values);

    assert(outputRequest.getType() == OUTPUT_REQUEST);

    INetPacket packet(outputRequest.getData(), outputRequest.getLength());

    assert(packet.getType() == OUTPUT_REQUEST);

}

void iNetDescriptorTest() {
    unsigned const char SENSORS_COUNT = 2;
    unsigned const char OUTPUTS_COUNT = 3;
    unsigned const long MAX_SILENCE_PERIOD = 100;

    SensorType sensorTypes[SENSORS_COUNT] = {INFRARED_LIGHT, TEMPERATURE};
    OutputType outputTypes[OUTPUTS_COUNT] = {RELAY, RELAY, RELAY};

    unsigned const char LENGTH = sizeof(unsigned long) + SENSORS_COUNT + OUTPUTS_COUNT + 3;
    unsigned char buffer[LENGTH];

    //Costruisce un descrittore
    INetDescriptor descriptor(buffer, SENSORS_COUNT, sensorTypes, OUTPUTS_COUNT, outputTypes, MAX_SILENCE_PERIOD);

    //Verifica che i dati contenuti siano quelli impostati
    assert(descriptor.getType() == DESCRIPTOR);
    assert(descriptor.getSensorsCount() == SENSORS_COUNT);
    assert(descriptor.getOutputsCount() == OUTPUTS_COUNT);
    assert(descriptor.getMaxSilencePeriod() == MAX_SILENCE_PERIOD);

    for(unsigned int i = 0; i < SENSORS_COUNT; ++i) {
        assert(descriptor.getSensorById(i) == sensorTypes[i]);
    }

    for(unsigned int i = 0; i < OUTPUTS_COUNT; ++i) {
        assert(descriptor.getOutputById(i) == outputTypes[i]);
    }

    //Effettua una riconversione del descrittore e verifica che i dati rimangano corretti
    INetPacket packet(descriptor.getData(), descriptor.getLength());

    assert(packet.getType() == DESCRIPTOR);

    INetDescriptor recastedDescriptor(&packet);

    assert(recastedDescriptor.getType() == DESCRIPTOR);
    assert(recastedDescriptor.getSensorsCount() == SENSORS_COUNT);
    assert(recastedDescriptor.getOutputsCount() == OUTPUTS_COUNT);
    assert(recastedDescriptor.getMaxSilencePeriod() == MAX_SILENCE_PERIOD);

    for(unsigned int i = 0; i < SENSORS_COUNT; ++i) {
        assert(recastedDescriptor.getSensorById(i) == sensorTypes[i]);
    }

    for(unsigned int i = 0; i < OUTPUTS_COUNT; ++i) {
        assert(recastedDescriptor.getOutputById(i) == outputTypes[i]);
    }
}
