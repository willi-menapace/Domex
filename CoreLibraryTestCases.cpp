/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "CoreLibraryTestCases.h"

#include "GlobalDefines.h"

#include "Rule.h"
#include "NetworkResources.h"
#include "DateTime.h"
#include "DeviceManager.h"
#include "FakeSensor.h"
#include "FakeOutput.h"
#include "DirectFileReader.h"
#include "DirectFileWriter.h"
#include "Settings.h"
#include "FileLock.h"
#include "SecureFileReader.h"
#include "SecureFileWriter.h"
#include "ConnectionParameters.h"
#include "RequestHandler.h"

#ifdef OFFBOARD
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#endif

#include <assert.h>

#define SHARED_TEST_FILENAME "shartst.bin"

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

/**
* Effettua uno sleep
*
* @param millis durata dello sleep in millisecondi
*/
void performSleep(unsigned long millis) {
    #ifdef OFFBOARD
    Sleep(millis);
    #endif // OFFBOARD

    #ifdef ONBOARD
    delay(millis);
    #endif // ONBOARD
}

void executeAllCoreLibraryTests() {
    print("--------------------------------------\n");
    print("Now testing ConstantOutputAction\n");
    print("--------------------------------------\n");
    constantOutputActionTest();
    print("TEST ENDED, CHECK RESULTS\n");
    print("\n--------------------------------------\n");
    print("Now testing ConstantCondition\n");
    print("--------------------------------------\n");
    constantConditionTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing InstanceIterator\n");
    print("--------------------------------------\n");
    instanceIteratorTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing InstancePool\n");
    print("--------------------------------------\n");
    instancePoolTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing MemoryManager\n");
    print("--------------------------------------\n");
    memoryManagerTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing NetworkDevice\n");
    print("--------------------------------------\n");
    networkDeviceTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing DateTime\n");
    print("--------------------------------------\n");
    dateTimeTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing NetworkResources\n");
    print("--------------------------------------\n");
    networkResourcesTest();
    print("TEST SUCCESSFUL\n");
    print("--------------------------------------\n");
    print("Now testing Rule\n");
    print("--------------------------------------\n");
    ruleTest();
    print("TEST ENDED, CHECK RESULTS\n");
    print("\n--------------------------------------\n");
    print("Now testing DeviceManager\n");
    print("--------------------------------------\n");
    deviceManagerTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing DirectFileReader and DirectFileWriter\n");
    print("--------------------------------------\n");
    directFileReaderAndWriterTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing SecureFileReader and SecureFileWriter\n");
    print("--------------------------------------\n");
    secureFileReaderAndWriterTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing Settings\n");
    print("--------------------------------------\n");
    writeReadConfFile();
    print("TEST ENDED, CHECK RESULTS\n");
    print("\n--------------------------------------\n");
    print("Now testing FileLock\n");
    print("--------------------------------------\n");
    fileLockTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing ConnectionParameters\n");
    print("--------------------------------------\n");
    connectionParametersTest();
    print("TEST SUCCESSFUL\n");
    #ifdef OFFBOARD
    print("\n--------------------------------------\n");
    print("Now testing Connector\n");
    print("--------------------------------------\n");
    connectorTest();
    print("TEST SUCCESSFUL\n");
    print("\n--------------------------------------\n");
    print("Now testing RequestHandler\n");
    print("--------------------------------------\n");
    requestHandlerTest1();
    requestHandlerTest2();
    print("TEST SUCCESSFUL\n");
    #endif // OFFBOARD
}

void constantOutputActionTest() {

    unsigned char address[] = {23, 44, 21, 244, 12, 34, 52, 23};

    const unsigned char OUTPUT_ID = 3;
    const long OUTPUT_VALUE = -33223444;

    ConstantOutputAction constantOutputAction;
    constantOutputAction.initialize(address, OUTPUT_ID, OUTPUT_VALUE);

    NetworkResources emptyResources;

    print("Should print:\n");
    print("Constant output executed: ");
    print(OUTPUT_VALUE);
    print(" on output ID ");
    print(OUTPUT_ID);
    print("\n");

    ((Action *) &constantOutputAction)->execute(&emptyResources, 0);
}

void constantConditionTest() {
    DateTime now;
    now.initialize(0, 0, 0, 1, 1, 1, 0); //1 gennaio 2000

    unsigned char address[] = {23, 44, 21, 244, 12, 34, 52, 23};

    const unsigned char SENSOR_ID = 1;
    const long REFERENCE_VALUE = 457985442;

    const unsigned char SENSOR_COUNT = 2;
    SensorType sensorTypes[2] = {TEMPERATURE, TEMPERATURE};

    //Inizializza una rete fasulla con un sensore
    NetworkResources networkResources;
    networkResources.addDevice(address, SENSOR_COUNT, sensorTypes, 0, 0, 3600, &now);

    //Inizializza una condizione che lavora con il sensore fasullo appena creato
    ConstantCondition constantCondition;
    constantCondition.initialize(address, SENSOR_ID, REFERENCE_VALUE, LOWER_THAN);

    Condition *condition = (Condition *) &constantCondition;

    //Effettua il test della condizione con diversi operatori di confronto
    networkResources.getDeviceByAddress(address)->setLatestReading(SENSOR_ID, REFERENCE_VALUE - 1);
    assert(condition->test(&networkResources, &now));

    constantCondition.initialize(address, SENSOR_ID, REFERENCE_VALUE, HIGHER_THAN);
    networkResources.getDeviceByAddress(address)->setLatestReading(SENSOR_ID, REFERENCE_VALUE + 1);
    assert(condition->test(&networkResources, &now));

    constantCondition.initialize(address, SENSOR_ID, REFERENCE_VALUE, EQUAL_TO);
    networkResources.getDeviceByAddress(address)->setLatestReading(SENSOR_ID, REFERENCE_VALUE);
    assert(condition->test(&networkResources, &now));

    //Effettua un test che deve dare risultato falso
    networkResources.getDeviceByAddress(address)->setLatestReading(SENSOR_ID, REFERENCE_VALUE - 5);
    assert(condition->test(&networkResources, &now) == false);

}

void instanceIteratorTest() {
    const unsigned char SIZE = 10;

    int instances[SIZE];
    bool map[SIZE];

    for(int i = 0; i < SIZE; ++i) {
        int allocate = rand() % 2;
        if(allocate == 1) {
            map[i] = true;
            instances[i] = i;
        } else {
            map[i] = false;
            map[i] = -1;
        }
    }

    InstanceIterator<int> iterator;
    iterator.initialize(SIZE, instances, map);

    while(iterator.hasNext()) {
        int *instance = iterator.next();
        assert(*instance != -1);
    }

}

void instancePoolTest() {
    const unsigned char POOL_SIZE = 8;

    //Contiene i riferimenti alle istanze allocate
    ConstantCondition *references[POOL_SIZE];
    for(int i = 0; i < POOL_SIZE; ++i) {
        references[i] = 0;
    }

    InstancePool<ConstantCondition> instancePool(POOL_SIZE);

    unsigned char currentInstances = 0;
    for(int i = 0; i < 10000; ++i) {

        int randomNumber = rand() % 2;
        //Se si deve allocare un'istanza
        if(randomNumber == 1) {
            //Verifica che ci sia spazio
            if(currentInstances < POOL_SIZE) {
                ++currentInstances;
                ConstantCondition *reference = instancePool.allocate();
                //Verifica che l'allocazione sia avvenuta
                assert(reference != 0);
                //Memorizza in una posizione libera il reference
                for(int i = 0; i < POOL_SIZE; ++i) {
                    if(references[i] == 0) {
                        references[i] = reference;
                        break;
                    }
                }
            }
        }
        //Se si deve deallocare un'istanza
        if(randomNumber == 0) {
            //Se ci sono istanza da deallocare
            if(currentInstances > 0) {
                //Finche' non si trova un reference da deallocare
                while(true) {
                    int randomIndex = rand() % POOL_SIZE;
                    if(references[randomIndex] != 0) {
                        assert(instancePool.deallocate(references[randomIndex]));
                        references[randomIndex] = 0;
                        --currentInstances;
                        break;
                    }
                }
            }
        }

    }

    ConstantCondition externalInstance;

    //Se si dealloca una posizione invalida l'errore deve essere segnalato
    assert(instancePool.deallocate(&externalInstance) == false);

    //Dealloca tutte le istanze rimanenti
    instancePool.clear();
    assert(instancePool.getInstanceCount() == 0);
}

void memoryManagerTest() {
    MemoryManager memoryManager;

    memoryManager.deallocateCondition(memoryManager.allocateConstantCondition());
    memoryManager.deallocateAction(memoryManager.allocateConstantOutputAction());

    ConstantOutputAction externalAction;
    ConstantCondition externalCondition;

    assert(memoryManager.deallocateAction(&externalAction) == false);
    assert(memoryManager.deallocateCondition(&externalCondition) == false);

    memoryManager.clear();
}

void networkDeviceTest() {

    const int ITERATIONS = 4; //Il numero di sovrascrittura complete dei valori dei sensori

    unsigned char address[8] = {23, 65, 34, 23, 34, 45, 244, 77};

    const unsigned char SENSOR_COUNT = 5;
    SensorType sensorTypes[SENSOR_COUNT] = {TEMPERATURE, VISIBLE_LIGHT, HUMIDITY, CURRENT, HUMIDITY};

    const unsigned char OUTPUT_COUNT = 5;
    OutputType outputTypes[OUTPUT_COUNT] = {RELAY, RELAY, RELAY, RELAY, RELAY};

    NetworkDevice networkDevice;
    networkDevice.initialize(address, SENSOR_COUNT, sensorTypes, OUTPUT_COUNT, outputTypes, 3600);

    //Ci si assicura che da appena inizializzato venga segnalato che non ci siano valori disponibili
    for(int i = 0; i < SENSOR_COUNT; ++i) {
        assert(networkDevice.getLatestReading(i) == NetworkDevice::READING_UNAVAILABLE);
    }

    for(int j = 0; j < ITERATIONS; ++j) {
        for(int i = 0; i < SENSOR_COUNT; ++i) {
            long randomNumber = rand();
            networkDevice.setLatestReading(i, randomNumber);
            assert(networkDevice.getLatestReading(i) == randomNumber);
        }
    }

    unsigned char *deviceAddress = networkDevice.getAddress();
    for(int i = 0; i < ADDRESS_BYTES; ++i) {
        assert(address[i] == deviceAddress[i]);
    }

    //Effettua il test di scrittura e lettura da file
    DirectFileWriter writer;
    writer.open(SHARED_TEST_FILENAME, false);
    networkDevice.writeToFile(&writer);
    writer.close();

    DirectFileReader reader;
    reader.open(SHARED_TEST_FILENAME);

    NetworkDevice readDevice;
    readDevice.initialize(&reader);
    reader.close();

    assert(networkDevice.equals(&readDevice) == true);
}

void dateTimeTest() {
    DateTime date1;
    DateTime date2;

    //Mezzanotte tra 28 e 29 febbraio 2004
    date1.initialize(0, 0, 0, 29, 1, 2, 4);
    //Esattamente un giorno dopo date1
    date2.initialize(0, 0, 0, 1, 2, 3, 4);

    assert(date2.getTimeOffset() - date1.getTimeOffset() == 3600 * 24);

    //Date separate da 7 anni con due giorni bisestili
    date1.initialize(0, 0, 0, 1, 1, 1, 0);
    date2.initialize(0, 0, 0, 1, 1, 1, 7);

    assert(date2.getTimeOffset() - date1.getTimeOffset() == (7 * 365 + 2) * 24 * 3600);

    //Date separate da 10 giorni
    date1.initialize(0, 0, 0, 1, 1, 1, 7);
    date2.initialize(0, 0, 0, 11, 1, 1, 7);

    assert(date2.getTimeOffset() - date1.getTimeOffset() == 10 * 24 * 3600);

    //Date separate da 10 giorni 4 ore 3 minuti e 2 secondi
    date1.initialize(0, 0, 0, 1, 1, 1, 7);
    date2.initialize(2, 3, 4, 11, 1, 1, 7);

    assert(date2.getTimeOffset() - date1.getTimeOffset() == 10 * 24 * 3600 + 2 + 3 * 60 + 4 * 3600);

    //Effettua il test di scrittura e lettura da file
    DirectFileWriter writer;
    writer.open(SHARED_TEST_FILENAME, false);
    date1.writeToFile(&writer);
    writer.close();

    DirectFileReader reader;
    reader.open(SHARED_TEST_FILENAME);
    DateTime readDate;
    readDate.initialize(&reader);
    reader.close();

    assert(date1.getTimeOffset() == readDate.getTimeOffset());
}

/**
* Ritorna un'istanza di test di network resources
*
* @param networkResources istanza da inizializzare di NetworkResources
* @param DEVICES il numero di device da aggiungere
* @param addresses gli indirizzi da dare ai devices
* @param maxSilencePeriods i periodi di silenzio massimi dei device
* @param creationTime l'ora in cui si ha avuto l'ultimo contatto con i device
*/
void initializeTestNetworkResources(NetworkResources *networkResources, const int DEVICES, unsigned char addresses[][ADDRESS_BYTES], unsigned long *maxSilencePeriods, DateTime *creationTime) {

    const unsigned char SENSOR_COUNT = 2;
    SensorType sensorTypes[SENSOR_COUNT] = {TEMPERATURE, HUMIDITY};

    const unsigned char OUTPUT_COUNT = 3;
    OutputType outputTypes[OUTPUT_COUNT] = {RELAY, RELAY, RELAY};

    for(int i = 0; i < DEVICES; ++i) {
        networkResources->addDevice(addresses[i], SENSOR_COUNT, sensorTypes, OUTPUT_COUNT, outputTypes, maxSilencePeriods[i], creationTime);
    }
}

void networkResourcesTest() {

    DateTime initialDate;
    initialDate.initialize(2, 3, 4, 11, 1, 3, 7);
    //180 secondi dopo initialDate
    DateTime afterSomeTime;
    afterSomeTime.initialize(2, 6, 4, 11, 1, 3, 7);

    const int DEVICES = 4;

    unsigned char addresses[DEVICES][ADDRESS_BYTES] = {{23, 45, 123, 56, 231, 34, 35, 81},
                                                       {34, 56, 123, 34, 73, 34, 56, 81},
                                                       {23, 45, 45, 55, 66, 14, 35, 28},
                                                       {72, 45, 38, 56, 231, 34, 35, 3}};

    unsigned long maxSilencePeriods[DEVICES] = {60, 3600, 100, 40};

    NetworkResources networkResources;

    initializeTestNetworkResources(&networkResources, DEVICES, addresses, maxSilencePeriods, &initialDate);

    for(int i = 0; i < DEVICES; ++i) {
        assert(networkResources.getDeviceByAddress(addresses[i]) != 0);
    }

    //Solo l'ultimo device deve risultare disperso
    networkResources.removeDeadDevices(&afterSomeTime);
    assert(networkResources.getDeviceByAddress(addresses[0]) != 0);
    assert(networkResources.getDeviceByAddress(addresses[1]) != 0);
    assert(networkResources.getDeviceByAddress(addresses[2]) != 0);
    assert(networkResources.getDeviceByAddress(addresses[3]) == 0);

    //Effettua il salvataggio e la rilettura su file
    DirectFileWriter writer;
    writer.open(SHARED_TEST_FILENAME, false);
    networkResources.writeToFile(&writer);
    writer.close();

    DirectFileReader reader;
    reader.open(SHARED_TEST_FILENAME);
    NetworkResources readResources;
    readResources.initialize(&reader);
    reader.close();

    assert(readResources.equals(&networkResources));
    assert(networkResources.equals(&readResources));

}

void ruleTest() {

    DateTime now;
    now.initialize(0, 0, 0, 1, 1, 1, 1); //1 gennaio 2001

    const unsigned long MINIMUM_EXECUTION_INTERVAL = 50;
    DateTime afterSomeTime;
    afterSomeTime.initialize(0, 1, 0, 1, 1, 1, 1); //Dopo 60 secondi

    unsigned char address[] = {23, 44, 21, 244, 12, 34, 52, 23};

    const unsigned char SENSOR_ID = 1;
    const long REFERENCE_VALUE = 15;

    const unsigned char SENSOR_COUNT = 2;
    SensorType sensorTypes[2] = {TEMPERATURE, TEMPERATURE};

    //Inizializza una rete fasulla con un sensore
    NetworkResources networkResources;
    networkResources.addDevice(address, SENSOR_COUNT, sensorTypes, 0, 0, 3600, &now);
    networkResources.getDeviceByAddress(address)->setLatestReading(SENSOR_ID, 5);

    //Inizializza la regola con condizioni e azioni soddisfatte dai valori contenuti in networkResources
    MemoryManager memoryManager;
    Rule rule;
    rule.initialize(MINIMUM_EXECUTION_INTERVAL); //devono passare almeno 50 secondi tra due esecuzioni

    ConstantCondition *condition = memoryManager.allocateConstantCondition();
    condition->initialize(address, SENSOR_ID, REFERENCE_VALUE, LOWER_THAN);

    rule.addCondition(condition);

    const unsigned char ACTIONS = 2;
    const unsigned char OUTPUT_ID = 1;
    long outputValues[ACTIONS] = {0, 1};

    for(int i = 0; i < ACTIONS; ++i) {
        ConstantOutputAction *action = memoryManager.allocateConstantOutputAction();
        action->initialize(address, OUTPUT_ID, outputValues[i]);

        rule.addAction(action);
    }

    print("Should print:\n");
    for(int i = 0; i < ACTIONS; ++i) {
        print("Constant output executed: ");
        print(outputValues[i]);
        print(" on output ID ");
        print(OUTPUT_ID);
        print("\n");
    }
    print("Now. Note: order does not matter\n");

    rule.verifyAndExecute(&networkResources, &now, 0);

    //Verifica il rispetto delle restrizioni temporali
    print("Should not execute actions in this section\nBEGIN\n");
    rule.verifyAndExecute(&networkResources, &now, 0);
    print("END\n");

    condition = memoryManager.allocateConstantCondition();
    condition->initialize(address, SENSOR_ID, REFERENCE_VALUE, HIGHER_THAN);

    //Aggiunge una condizione non verificata dai valori di networkResources
    rule.addCondition(condition);

    print("Should not execute actions in this section\nBEGIN\n");
    rule.verifyAndExecute(&networkResources, &afterSomeTime, 0);
    print("END\n");

    for(int i = 0; i < ACTIONS; ++i) {
        ConstantOutputAction *action = memoryManager.allocateConstantOutputAction();
        action->initialize(address, OUTPUT_ID, outputValues[i]);

        rule.addNotAction(action);
    }

    print("Should print:\n");
    for(int i = 0; i < ACTIONS; ++i) {
        print("Constant output executed: ");
        print(outputValues[i]);
        print(" on output ID ");
        print(OUTPUT_ID);
        print("\n");
    }
    print("Now. Note: order does not matter\n");

    rule.verifyAndExecute(&networkResources, &afterSomeTime, 0);
}

void deviceManagerTest() {

    DeviceManager manager;

    unsigned const char SENSORS_COUNT = 3;
    SensorType sensorTypes[SENSORS_COUNT] = {HUMIDITY, TEMPERATURE, VISIBLE_LIGHT};
    long sensorValues[SENSORS_COUNT] = {543, -2345895, -1};
    unsigned long sensorRefreshTimes[SENSORS_COUNT] = {1, 100, 100};
    FakeSensor *sensors[SENSORS_COUNT];

    //Alloca i sensori e li registra con il manager
    for(unsigned int i = 0; i < SENSORS_COUNT; ++i) {
        sensors[i] = new FakeSensor(sensorTypes[i], sensorValues[i]);
        manager.registerSensor(sensors[i], sensorRefreshTimes[i]);
    }

    unsigned const char OUTPUTS_COUNT = 2;
    OutputType outputTypes[OUTPUTS_COUNT] = {RELAY, RELAY};
    FakeOutput *outputs[OUTPUTS_COUNT];

    //Alloca gli output e li registra con il manager
    for(unsigned int i = 0; i < OUTPUTS_COUNT; ++i) {
        outputs[i] = new FakeOutput(outputTypes[i]);
        manager.registerOutput(outputs[i]);
    }

    //Si assicura che sia passato abbastanza tempo per effettaure le letture su tutti i sensori
    performSleep(100);

    //Effettua le letture sui sensori
    manager.refreshSensors();

    //Si assicura che tutti i sensori siano stati letti
    assert(manager.getAvailableValuesCount() == SENSORS_COUNT);

    unsigned char ids[SENSORS_COUNT];
    long readValues[SENSORS_COUNT];

    manager.getAvailableValues(ids, readValues);

    for(int i = 0; i < SENSORS_COUNT; ++i) {
        assert(ids[i] == i);
        assert(readValues[i] == sensorValues[i]);
    }

    //Fa passare il tempo di aggirnamento minimo solo del primo densore
    performSleep(20);

    //Effettua le letture dei sensori
    manager.refreshSensors();

    //Verifica che solo un sensore sia stato letto
    assert(manager.getAvailableValuesCount() == 1);

    unsigned const char OUTPUT_TESTS = 20; //Il numero di output da effettuare

    //Effettua output casuali e ne verifica l'esecuzione
    for(int i = 0; i < OUTPUT_TESTS; ++i) {
        unsigned char outputId = rand() % OUTPUTS_COUNT;

        long outputValue = 0;

        #ifdef OFFBOARD
        outputValue = rand() - 2 * rand();
        #endif // OFFBOARD

        #ifdef ONBOARD
        outputValue = random(20000) - 2 * random(2000);
        #endif // ONBOARD

        manager.performOutput(outputId, outputValue);

        assert(outputs[outputId]->getLastOutputValue() == outputValue);
    }

    //Elimina sensori e output allocati
    for(unsigned int i = 0; i < SENSORS_COUNT; ++i) {
        delete sensors[i];
    }
    for(unsigned int i = 0; i < OUTPUTS_COUNT; ++i) {
        delete outputs[i];
    }
}

void fileReaderAndWriterTest(FileReader *reader, FileWriter *writer) {

    assert(!writer->isOpen());
    bool writerOpen = writer->open(SHARED_TEST_FILENAME, false);
    assert(writerOpen);
    assert(writer->isOpen());
    //Effettua un test di lettura / scrittura con variabili signed per
    //verificare che il writer/reader sia capace di scrivere e leggere
    //valori binari senza alterarli

    const int LONG_COUNT = 4;
    long longValues[LONG_COUNT] = {3567, -2045692145, 134, -31455};

    const int BYTE_COUNT = 4;
    char byteValues[BYTE_COUNT] = {113, -65, 55, 23};

    for(int i = 0; i < LONG_COUNT; ++i) {
        writer->writeLong((unsigned long) longValues[i]);
    }
    for(int i = 0; i < BYTE_COUNT; ++i) {
        writer->writeByte((unsigned char) byteValues[i]);
    }
    writer->writeBytes((unsigned char *) byteValues, BYTE_COUNT);

    writer->close();

    assert(!reader->isOpen());
    bool readerOpen = reader->open(SHARED_TEST_FILENAME);
    assert(readerOpen);
    assert(reader->isOpen());


    char readByteValues[BYTE_COUNT];
    for(int i = 0; i < LONG_COUNT; ++i) {
        assert((long) reader->readLong() == longValues[i]);
    }
    for(int i = 0; i < BYTE_COUNT; ++i) {
        assert((char) reader->readByte() == byteValues[i]);
    }
    reader->readBytes((unsigned char *)readByteValues, BYTE_COUNT);
    for(int i = 0; i < BYTE_COUNT; ++i) {
        assert((char) readByteValues[i] == byteValues[i]);
    }

    reader->close();

    char appendedByte = -33;

    //Effettua il test di aggiunta dati a fine file
    writerOpen = writer->open(SHARED_TEST_FILENAME, true);
    assert(writerOpen);

    writer->writeByte(appendedByte);

    writer->close();

    reader->open(SHARED_TEST_FILENAME);

    for(int i = 0; i < LONG_COUNT; ++i) {
        assert((long) reader->readLong() == longValues[i]);
    }
    for(int i = 0; i < BYTE_COUNT; ++i) {
        assert((char) reader->readByte() == byteValues[i]);
    }
    reader->readBytes((unsigned char *)readByteValues, BYTE_COUNT);
    for(int i = 0; i < BYTE_COUNT; ++i) {
        assert((char) readByteValues[i] == byteValues[i]);
    }

    assert((char) reader->readByte() == appendedByte);

    reader->close();

    reader->open(SHARED_TEST_FILENAME);

    reader->seek(0);
    assert(reader->isAvailable() == reader->getSize());
    reader->seek(reader->getSize());
    assert(reader->isAvailable() == 0);

    reader->seek(sizeof(unsigned long));
    long secondLong = (long) reader->readLong();
    assert(secondLong == longValues[1]);

    reader->close();
}

void directFileReaderAndWriterTest() {
    DirectFileReader reader;
    DirectFileWriter writer;

    fileReaderAndWriterTest(&reader, &writer);
}

void secureFileReaderAndWriterTest() {
    SecureFileReader reader;
    SecureFileWriter writer;

    fileReaderAndWriterTest(&reader, &writer);
}

void writeReadConfFile() {
    DateTime now;
    now.initialize(0, 0, 0, 1, 1, 1, 1); //1 gennaio 2001

    const unsigned long MINIMUM_EXECUTION_INTERVAL = 50;
    DateTime afterSomeTime;
    afterSomeTime.initialize(0, 1, 0, 1, 1, 1, 1); //Dopo 60 secondi

    unsigned char address[] = {23, 44, 21, 244, 12, 34, 52, 23};

    const unsigned char SENSOR_ID = 1;
    const long REFERENCE_VALUE = 15;

    const unsigned char SENSOR_COUNT = 2;
    SensorType sensorTypes[2] = {TEMPERATURE, TEMPERATURE};

    //Inizializza una rete fasulla con un sensore
    NetworkResources networkResources;
    networkResources.addDevice(address, SENSOR_COUNT, sensorTypes, 0, 0, 3600, &now);
    networkResources.getDeviceByAddress(address)->setLatestReading(SENSOR_ID, 5);

    //Inizializza la regola con condizioni e azioni soddisfatte dai valori contenuti in networkResources
    MemoryManager memoryManager;
    Rule rule;
    rule.initialize(MINIMUM_EXECUTION_INTERVAL); //devono passare almeno 50 secondi tra due esecuzioni

    ConstantCondition *condition = memoryManager.allocateConstantCondition();
    condition->initialize(address, SENSOR_ID, REFERENCE_VALUE, LOWER_THAN);

    rule.addCondition(condition);

    const unsigned char ACTIONS = 2;
    const unsigned char OUTPUT_ID = 1;
    long outputValues[ACTIONS] = {0, 1};

    for(int i = 0; i < ACTIONS; ++i) {
        ConstantOutputAction *action = memoryManager.allocateConstantOutputAction();
        action->initialize(address, OUTPUT_ID, outputValues[i]);
        rule.addAction(action);
    }

    DirectFileWriter fileWriter;
    fileWriter.open(SHARED_TEST_FILENAME, false);

    //Scrive i parametri di connessione al DB
    ConnectionParameters dbParameters; //Parametri vuoti
    dbParameters.writeToFile(&fileWriter);

    //Scrive i parametri di connessione al server delle richieste
    ConnectionParameters requestServerParameters; //Parametri vuoti
    requestServerParameters.writeToFile(&fileWriter);

    //Scrive il numero di regole e tutte le regole
    fileWriter.writeByte(1);
    rule.writeToFile(&fileWriter);
    fileWriter.close();

    DirectFileReader fileReader;
    fileReader.open(SHARED_TEST_FILENAME);

    Settings settings;
    settings.initialize(&fileReader);
    fileReader.close();

    print("Should print:\n");
    for(int i = 0; i < ACTIONS; ++i) {
        print("Constant output executed: ");
        print(outputValues[i]);
        print(" on output ID ");
        print(OUTPUT_ID);
        print("\n");
    }
    print("Now. Note: order does not matter\n");

    settings.verifyAndExecuteRules(&networkResources, &now, 0);
}

void fileLockTest() {

    const int BYTE_ARRAY_SIZE = 300;

    //Inizializza i byte effettuati per i test di scrittura
    unsigned char byteArray[BYTE_ARRAY_SIZE];
    for(int i = 48; i < BYTE_ARRAY_SIZE; ++i) {
        byteArray[i] = (unsigned char) i % 256;
    }

    FileLock fileLock;

    //Effettua il test di lock di un file inesistente
    assert(fileLock.getReader("null.nll") == 0);

    //Effettua la scrittura di un file
    FileWriter *fileWriter = fileLock.writeLock(SHARED_TEST_FILENAME, false);
    for(int i = 0; i < BYTE_ARRAY_SIZE; ++i) {
        fileWriter->writeByte(byteArray[i]);
    }
    fileLock.writeUnlock();

    //Effettua la rilettura dei byte appena scritti
    FileReader *fileReader = fileLock.getReader(SHARED_TEST_FILENAME);
    for(int i = 0; i < BYTE_ARRAY_SIZE; ++i) {
        assert(fileReader->readByte() == byteArray[i]);
    }
    fileLock.releaseReader();

    //Simula il fallimento di una sovrascrittura del file appena scritto
    fileWriter = fileLock.writeLock(SHARED_TEST_FILENAME, false);
    for(int i = 0; i < BYTE_ARRAY_SIZE / 2; ++i) {
        fileWriter->writeByte(byteArray[i]);
    }
    fileWriter->close(); //Simula il fallimento della scrittora chiudendo il writer

    //Verifica che venga effettuato il ripristino della situazione precedente
    fileReader = fileLock.getReader(SHARED_TEST_FILENAME);
    for(int i = 0; i < BYTE_ARRAY_SIZE; ++i) {
        assert(fileReader->readByte() == byteArray[i]);
    }
    fileLock.releaseReader();

    //Effettua un append al file
    fileWriter = fileLock.writeLock(SHARED_TEST_FILENAME, true);
    for(int i = 0; i < BYTE_ARRAY_SIZE; ++i) {
        fileWriter->writeByte(byteArray[i]);
    }
    fileLock.writeUnlock();

    //Effettua la rilettura del file
    fileReader = fileLock.getReader(SHARED_TEST_FILENAME);
    for(int j = 0; j < 2; ++j) {
        for(int i = 0; i < BYTE_ARRAY_SIZE; ++i) {
            assert(fileReader->readByte() == byteArray[i]);
        }
    }
    fileLock.releaseReader();

    //Simula il fallimento di un append
    fileWriter = fileLock.writeLock(SHARED_TEST_FILENAME, true);
    for(int i = 0; i < BYTE_ARRAY_SIZE / 2; ++i) {
        fileWriter->writeByte(byteArray[i]);
    }
    fileWriter->close(); //Simula il fallimento dell'append

    //Effettua la rilettura del file assicurandosi che siano stati rimossi i byte dell'append fallito
    fileReader = fileLock.getReader(SHARED_TEST_FILENAME);
    for(int j = 0; j < 2; ++j) {
        for(int i = 0; i < BYTE_ARRAY_SIZE; ++i) {
            unsigned char byte = fileReader->readByte();
            assert(byte == byteArray[i]);
        }
    }

    assert(fileReader->isAvailable() == false);
    fileLock.releaseReader();

    //Effettua un test delle funzionalita' di abort
    fileWriter = fileLock.writeLock(SHARED_TEST_FILENAME, false);
    for(int i = 0; i < 10; ++i) {
        fileWriter->writeByte(i);
    }
    //Effettua il rollback delle modifiche
    fileLock.writeAbort();

    //Effettua la rilettura del file per assicurarsi che non sia stato sovrascritto il vecchio file
    fileReader = fileLock.getReader(SHARED_TEST_FILENAME);
    for(int j = 0; j < 2; ++j) {
        for(int i = 0; i < BYTE_ARRAY_SIZE; ++i) {
            unsigned char byte = fileReader->readByte();
            assert(byte == byteArray[i]);
        }
    }

    assert(fileReader->isAvailable() == false);
    fileLock.releaseReader();

    //Effettua un test delle funzionalita' di abort nel caso di append
    fileWriter = fileLock.writeLock(SHARED_TEST_FILENAME, true);
    for(int i = 0; i < 10; ++i) {
        fileWriter->writeByte(i);
    }
    //Effettua il rollback delle modifiche
    fileLock.writeAbort();

    //Effettua la rilettura del file per assicurarsi che non sia stato stato aggiunto nulla al vecchio file
    fileReader = fileLock.getReader(SHARED_TEST_FILENAME);
    for(int j = 0; j < 2; ++j) {
        for(int i = 0; i < BYTE_ARRAY_SIZE; ++i) {
            unsigned char byte = fileReader->readByte();
            assert(byte == byteArray[i]);
        }
    }

    assert(fileReader->isAvailable() == false);
    fileLock.releaseReader();

}

void connectionParametersTest() {

    unsigned char ipAddress[IP_ADDRESS_SIZE] = {54, 77, 21, 244};
    const unsigned long PORT = 25565;
    const char *username = "Leonida";
    const char *password = "SpartaRul3Z_";

    ConnectionParameters connectionParameters;

    connectionParameters.setIpAddress(ipAddress);
    connectionParameters.setPort(PORT);
    connectionParameters.setUsername(username);
    connectionParameters.setPassword(password);

    //Effettua la scrittura dei parametri di connessione su file
    DirectFileWriter fileWriter;
    fileWriter.open(SHARED_TEST_FILENAME, false);
    connectionParameters.writeToFile(&fileWriter);
    fileWriter.close();

    ConnectionParameters readParameters;

    //Effettua la rilettura
    DirectFileReader fileReader;
    fileReader.open(SHARED_TEST_FILENAME);
    readParameters.initialize(&fileReader);
    fileReader.close();

    assert(connectionParameters.equals(&readParameters));
}

#ifdef OFFBOARD
void connectorTest() {
    //Accetta la connessione del client
    DirectFileWriter serverResponseWriter;

    //Il server accetta la richiesta
    serverResponseWriter.open(FAKE_CLIENT_INPUT_FILENAME, false);
    serverResponseWriter.writeByte(Connector::CONNECTION_ACCEPTED);
    serverResponseWriter.close();

    unsigned char ipAddress[IP_ADDRESS_SIZE] = {54, 77, 21, 244};
    const unsigned long PORT = 25565;
    const char *username = "Leonida";
    const char *password = "SpartaRul3Z_";

    ConnectionParameters connectionParameters;

    connectionParameters.setIpAddress(ipAddress);
    connectionParameters.setPort(PORT);
    connectionParameters.setUsername(username);
    connectionParameters.setPassword(password);

    Connector connector(&connectionParameters);
    assert(connector.isConnected() == false);

    connector.tryConnection();
    assert(connector.isConnected());

    connector.closeConnection();

    assert(!connector.isConnected());

    //Il server rifiuta la richiesta
    serverResponseWriter.open(FAKE_CLIENT_INPUT_FILENAME, false);
    serverResponseWriter.writeByte(Connector::CONNECTION_REFUSED);
    serverResponseWriter.close();

    connector.tryConnection();
    assert(!connector.isConnected());

    //Verifica che la richieta sia stata codificata in maniera corretta
    DirectFileReader reader;
    reader.open(FAKE_CLIENT_OUTPUT_FILENAME);

    assert(reader.readByte() == Connector::CONNECTION_REQUEST_PACKET);
    for(unsigned int i = 0; i < strlen(username); ++i) {
        assert(reader.readByte() == username[i]);
    }
    assert(reader.readByte() == '\0');
    for(unsigned int i = 0; i < strlen(password); ++i) {
        assert(reader.readByte() == password[i]);
    }
    assert(reader.readByte() == '\0');
    assert(!reader.isAvailable());
    reader.close();

}

/**
* Inizializza delle impostazioni parzialmente casuali per la connessione ad un server
* Salva le impostazioni sul file SETTINGS_FILENAME
*
* @param settings impostazioni da inizializzare
*
* @return dimensione in byte del file delle impostazioni
*/
unsigned long initializeConnectionSettings(Settings *settings) {

    unsigned char ipAddress[IP_ADDRESS_SIZE] = {(unsigned char) (rand() % 256),(unsigned char) (rand() % 256),(unsigned char) (rand() % 256),(unsigned char) (rand() % 256)};
    const unsigned long PORT = rand() % 30000;
    const char *username = "Username";
    const char *password = "Password";

    ConnectionParameters connectionParameters;

    connectionParameters.setIpAddress(ipAddress);
    connectionParameters.setPort(PORT);
    connectionParameters.setUsername(username);
    connectionParameters.setPassword(password);

    SecureFileWriter settingsWriter; //Le impostazioni vanno lette e scritte in maniera sicura
    settingsWriter.open(SETTINGS_FILENAME, false);

    //Scrive gli stessi parametri di connessione sia per la connessione al db sia per quella al server delle richieste
    connectionParameters.writeToFile(&settingsWriter);
    connectionParameters.writeToFile(&settingsWriter);

    //Nessuna regola
    settingsWriter.writeByte(0);
    settingsWriter.close();

    SecureFileReader settingsReader;
    settingsReader.open(SETTINGS_FILENAME);

    unsigned long fileSize = settingsReader.getSize();

    //Inizializza le impostazini con il file appena scritto
    settings->initialize(&settingsReader);

    settingsReader.close();

    return fileSize;
}

void requestHandlerTest1() {
    DateTime initialDate;
    initialDate.initialize(2, 3, 4, 11, 1, 3, 7);

    const int DEVICES = 4;

    unsigned char addresses[DEVICES][ADDRESS_BYTES] = {{23, 45, 123, 56, 231, 34, 35, 81},
                                                       {34, 56, 123, 34, 73, 34, 56, 81},
                                                       {23, 45, 45, 55, 66, 14, 35, 28},
                                                       {72, 45, 38, 56, 231, 34, 35, 3}};

    unsigned long maxSilencePeriods[DEVICES] = {60, 3600, 100, 40};

    NetworkResources devices;

    initializeTestNetworkResources(&devices, DEVICES, addresses, maxSilencePeriods, &initialDate);

    //Scrive un file di devices
    SecureFileWriter devicesWriter; //I device devono essere scritti e letti in modo sicuro
    devicesWriter.open(DEVICES_FILENAME, false);
    devices.writeToFile(&devicesWriter);
    devicesWriter.close();

    //Ne ottiene la dimensione
    SecureFileReader devicesReader;
    devicesReader.open(DEVICES_FILENAME);
    unsigned long devicesFileSize = devicesReader.getSize();
    devicesReader.close();

    Settings settings;
    //Inizializza le impostazioni per la connessione
    initializeConnectionSettings(&settings);

    //Scrive la risposta del server
    DirectFileWriter serverResponseWriter;
    serverResponseWriter.open(FAKE_CLIENT_INPUT_FILENAME, false);
    serverResponseWriter.writeByte(Connector::CONNECTION_ACCEPTED);
    serverResponseWriter.writeByte(RequestHandler::DEVICE_LIST_REQUEST);
    serverResponseWriter.writeByte(RequestHandler::DEVICE_LIST_RECEIVED);
    serverResponseWriter.close();

    RequestHandler requestHandler(&settings);
    //Processa tutte le richieste
    requestHandler.processRequests();

    //Effettua la chiusura delle risorse di rete in modo da poter verificare che abbia comunicato correttamente col server
    requestHandler.closeConnection();

    DirectFileReader responseReader;
    responseReader.open(FAKE_CLIENT_OUTPUT_FILENAME);

    //Legge il byte di richiesta connessione
    assert(responseReader.readByte() == Connector::CONNECTION_REQUEST_PACKET);
    //Legge nome utente e password di connessione
    while(responseReader.readByte() != '\0') {};
    while(responseReader.readByte() != '\0') {};

    assert(responseReader.readLong() == devicesFileSize);

    //Legge la lista di device inviati al server
    NetworkResources readDevices;
    readDevices.initialize(&responseReader);

    assert(!responseReader.isAvailable());
    responseReader.close();

    assert(devices.equals(&readDevices));
}


void requestHandlerTest2() {

    Settings originalSettings;
    Settings modifiedSettings;
    //Inizializza le impostazioni per la connessione
    initializeConnectionSettings(&originalSettings);
    unsigned long settingsSize = initializeConnectionSettings(&modifiedSettings);



    //Scrive la risposta del server
    DirectFileWriter serverResponseWriter;
    serverResponseWriter.open(FAKE_CLIENT_INPUT_FILENAME, false);
    serverResponseWriter.writeByte(Connector::CONNECTION_ACCEPTED);
    serverResponseWriter.writeByte(RequestHandler::SETTINGS_UPDATE_REQUEST);
    serverResponseWriter.writeLong(settingsSize);

    //Scrive il file di settings da sostituire a quello originale
    SecureFileReader settingsReader;
    settingsReader.open(SETTINGS_FILENAME);
    while(settingsReader.isAvailable()) {
        serverResponseWriter.writeByte(settingsReader.readByte());
    }
    settingsReader.close();

    serverResponseWriter.close();

    RequestHandler requestHandler(&originalSettings);

    //Disattiva la riconnessione automatica al cambio impostazioni per evitare
    //un loop infinito che porta alla rilettura del file di input del server delle richieste infinite volte
    originalSettings.getDbParameters()->setOnInitListener(0);
    originalSettings.getRequestServerParameters()->setOnInitListener(0);

    //Processa tutte le richieste
    requestHandler.processRequests();

    //Effettua la chiusura delle risorse di rete in modo da poter verificare che abbia comunicato correttamente col server
    requestHandler.closeConnection();

    DirectFileReader responseReader;
    responseReader.open(FAKE_CLIENT_OUTPUT_FILENAME);

    //Legge il byte di richiesta connessione
    assert(responseReader.readByte() == Connector::CONNECTION_REQUEST_PACKET);
    //Legge nome utente e password di connessione
    while(responseReader.readByte() != '\0') {};
    while(responseReader.readByte() != '\0') {};

    //assert(responseReader.readByte() == RequestHandler::SETTINGS_UPDATE_RECEIVED);

    responseReader.close();

    //Verifica che i settings siano stati modificati
    assert(originalSettings.getDbParameters()->equals(modifiedSettings.getDbParameters()));
    assert(originalSettings.getRequestServerParameters()->equals(modifiedSettings.getRequestServerParameters()));
}
#endif //OFFBOARD
