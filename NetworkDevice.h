/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef NETWORK_DEVICE_INCLUDED
#define NETWORK_DEVICE_INCLUDED

#include "GlobalDefines.h"

#include "SensorType.cpp"
#include "OutputType.cpp"
#include "FileReader.h"
#include "FileWriter.h"

#define NETWORK_DEVICE_MAX_SENSORS 5
#define NETWORK_DEVICE_MAX_OUTPUTS 5

/**
* Device connesso alla rete contenente numero variabile di
* sensori e dispositivi di output
*
* Ogni sensore e ogni dispositivo di output hanno un certo ID che va da 0 al numero di
* sensori o dispositivi di output installati - 1
*/
class NetworkDevice {
private:

    unsigned char address[ADDRESS_BYTES]; //L'indirizzo di rete del device

    SensorType sensorTypes[NETWORK_DEVICE_MAX_SENSORS]; //Descrittore del tipo di sensori installati. Ad ogni indice corrisponde un sensore con ID = indice
    long latestReadings[NETWORK_DEVICE_MAX_SENSORS];    //Memorizza le ultime letture effettuate dai sensori. Ad ogni indice corrisponde
                                         //l'ultima lettura del sensore con ID = indice
    unsigned char sensorsCount;          //Il numero di sensori installati

    OutputType outputTypes[NETWORK_DEVICE_MAX_OUTPUTS]; //Descrittore del ripo di dispositivi di output installati. Ad ogni indice
                                         //corrisponde un dispositivo di output con ID = indice
    unsigned char outputsCount;          //Il numero di dispositivi di output installati

    unsigned long maxSilencePeriod;      //L'intervallo di tempo espresso in secondi all'interno del quale dovrebbe verificarsi almeno
                                         //una comunicazione con l'esterno
public:

    static const long READING_UNAVAILABLE;

    /**
    * Inizializza il device corrente
    *
    * @param _address l'indirizzo di rete del device
    * @param _sensorsCount il numero di sensori installati
    * @param _sensorTypes array di dimensione _sensorsCount contenente il tipo di ogni sensore installato
    * @param _outputsCount il numero di output installati
    * @param _outputTypes array di dimensione _outputsCount contenente il tipo di ogni output installato
    * @param _maxSilencePeriod Intervallo di tempo espresso in secondi all'interno del quale dovrebbe
    *                          verificarsi almeno una comunicazione con l'esterno
    */
    void initialize(unsigned char _address[ADDRESS_BYTES], unsigned char _sensorsCount, SensorType _sensorTypes[],
                    unsigned char _outputsCount, OutputType _outputTypes[], unsigned long _maxSilencePeriod);
    /**
    * Inizializza l'istanza corrente da un file
    *
    * @param fileReader il file da cui leggere l'istanza
    */
    void initialize(FileReader *fileReader);
    /**
    * Memorizza l'ultimo valore letto di un sensore
    *
    * @param sensorId l'id del sensore che ha effettuato la lettura. Deve essere essere < sensorsCount
    * @param value il valore letto
    */
    void setLatestReading(unsigned char sensorId, long value);
    /**
    * Ottiene l'ultimo valore letto da un dato sensore
    *
    * @param sensorId l'id del sensore di cui di vuole conoscere l'ultima lettura. Deve essere essere < sensorsCount
    * @return l'ultima lettura effettuata dal sensore, READING_UNAVAILABLE se non e' presente nessun valore
    */
    long getLatestReading(unsigned char sensorId);
    /**
    * Ottiene l'indirizzo del device
    *
    * @return l'indirizzo del device
    */
    unsigned char *getAddress();
    /**
    * @return Intervallo di tempo espresso in secondi all'interno del quale dovrebbe avvenire almeno
    *         una comunicazione con l'esterno
    */
    unsigned long getMaxSilencePeriod();
    /**
    * Scrive l'istanza corrente su un file fornito
    *
    * @param fileWriter il file su cui scrivere l'istanza
    */
    void writeToFile(FileWriter *fileWriter);
    /**
    * Verifica se due due istanze sono logicamente equivalenti
    *
    * @param otherDevice il device di cui verificare l'uguaglianza
    * @return true se otherDevice e' logicamente equivalente all'istanza corrente
    */
    bool equals(NetworkDevice *otherDevice);
};

#endif // NETWORK_DEVICE_INCLUDED
