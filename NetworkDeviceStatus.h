/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef NETWORK_DEVICE_STATUS_INCLUDED
#define NETWORK_DEVICE_STATUS_INCLUDED

#include "GlobalDefines.h"

#include "GlobalDefines.h"

#include "NetworkDevice.h"
#include "DateTime.h"
#include "ComputedDeviceStatus.cpp"
#include "FileReader.h"
#include "FileWriter.h"

/**
* Lo stato corrente di un device interno alla rete
*/
class NetworkDeviceStatus {
private:
    NetworkDevice networkDevice;
    DateTime lastContact;
public:
    /**
    * Inizializza l'istanza corrente con le informazioni di un device
    * con cui si suppone si abbia appena avuto un contatto
    *
    * @param address l'indirizzo di rete del device
    * @param sensorsCount il numero di sensori installati
    * @param sensorTypes array di dimensione _sensorsCount contenente il tipo di ogni sensore installato
    * @param outputsCount il numero di output installati
    * @param outputTypes array di dimensione _outputsCount contenente il tipo di ogni output installato
    * @param maxSilencePeriod Intervallo di tempo espresso in secondi all'interno del quale dovrebbe
    *                         verificarsi almeno una comunicazione con l'esterno
    * @param _now data e ora correnti. Si suppone che al momento della
    *        creazione si abbia anche avuto un contatto con il device
    */
    void initialize(unsigned char address[ADDRESS_BYTES], unsigned char sensorsCount, SensorType sensorTypes[],
                    unsigned char outputsCount, OutputType outputTypes[], unsigned long maxSilencePeriod, DateTime *_now);
    /**
    * Inizializza l'istanza corrente con le informazioni presenti su file
    *
    * @param fileReader il file con cui inizializzare l'istanza
    */
    void initialize(FileReader *fileReader);
    /**
    * @return il device a cui lo stato fa riferimento
    */
    NetworkDevice *getNetworkDevice();
    /**
    * Restituisce lo stato probabile in cui si trova il device
    *
    * @param now data e ora di adesso
    * @return lo stato del device
    */
    ComputedDeviceStatus getComputedStatus(DateTime *now);
    /**
    * Scrive l'istanza corrente su un file
    *
    * @param fileWriter il file su cui scrivere l'istanza
    */
    void writeToFile(FileWriter *fileWriter);
    /**
    * @return true se le due istanze sono logicamente equivalenti
    */
    bool equals(NetworkDeviceStatus *otherStatus);
};

#endif // NETWORK_DEVICE_STATUS_INCLUDED
