/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "GlobalDefines.h"

#include "Rule.h"
#include "NetworkResources.h"
#include "DateTime.h"
#include "FileReader.h"
#include "Settings.h"

/**
* Lancia in esecuzione tutti i test case disponibili
* e genera un'asserzione in caso di fallimento
*/
void executeAllCoreLibraryTests();

/**
* Effettua un test di ConstantOutputAction
* che comprende la logica di base ma non invia
* i dati fisicamente ai trasmettitori
*/
void constantOutputActionTest();

/**
* Effettua un test completo di ConstantCondition
*/
void constantConditionTest();

/**
* Effettua un test di InstanceIterator
*/
void instanceIteratorTest();

/**
* Effettua un test di InstancePool
* Allocando e deallocando in maniera casuale
*/
void instancePoolTest();

/**
* Effettua un test di MemoryManager
*/
void memoryManagerTest();

/**
* Test completo delle funzionalita' di NetworkDevice
*/
void networkDeviceTest();

/**
* Effettua un test sommario di DateTime
*/
void dateTimeTest();

/**
* Effettua un test di NetworkResources
*/
void networkResourcesTest();

/**
* Effettua un test di rule
*/
void ruleTest();

/**
* Effettua un test di DeviceManager
*/
void deviceManagerTest();

/**
* Effettua un test di lettura/scrittura di tipi primitivi su file
*
* @param reader implementazione di FileReader da testare
* @param writer implementazione di FileWriter da testare
*/
void fileReaderAndWriterTest(FileReader *reader, FileWriter *writer);

/**
* Effettua un test di DirectFileReader e DirectFileWriter
*/
void directFileReaderAndWriterTest();

/**
* Effettua un test di SecureFileReader e SecureFileWriter
*/
void secureFileReaderAndWriterTest();

/**
* Effettua un test di scrittura e caricamento di file di configurazione
*/
void writeReadConfFile();

/**
* Effettua un test di FileLock
*/
void fileLockTest();

/**
* Effettua un test di ConnectionParameters
*/
void connectionParametersTest();

#ifdef OFFBOARD
/**
* Effettua un test di Connector
*/
void connectorTest();

/**
* Effettua un test di RequestHandler per il trasferimento della lista di device
*/
void requestHandlerTest1();
/**
* Effettua un test di RequestHandler per il cambio delle impostazioni
*/
void requestHandlerTest2();
#endif //OFFBOARD
