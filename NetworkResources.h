/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef NETWORK_RESOURCES_INCLUDED
#define NETWORK_RESOURCES_INCLUDED

#include "GlobalDefines.h"

#define MAX_NETWORK_DEVICES 20

#include "NetworkDeviceStatus.h"
#include "InstancePool.h"

/**
* Rappresenta risorse della rete
*
* IMPORTANTE: questa classe utilizza membri che allocano memoria dinamicamente, quindi puo'
*             essere istanziata solo prima dell'esecuzione del programma
*/
class NetworkResources {
private:
    InstancePool<NetworkDeviceStatus> networkDevices; //Pool delle istanze

    /**
    * Ottiene lo stato di un device in base al suo indirizzo di rete
    *
    * @param address l'indirizzo del device di rete
    * @return lo stato del device cercato, 0 se nessuno e' stato trovato
    */
    NetworkDeviceStatus *getDeviceStatusByAddress(unsigned char address[ADDRESS_BYTES]);
public:
    /**
    * Aggiunge un nuovo device alla rete con cui si abbia appena avuto un contatto
    *
    * @param address l'indirizzo di rete del device
    * @param sensorsCount il numero di sensori installati
    * @param sensorTypes array di dimensione _sensorsCount contenente il tipo di ogni sensore installato
    * @param outputsCount il numero di output installati
    * @param outputTypes array di dimensione _outputsCount contenente il tipo di ogni output installato
    * @param maxSilencePeriod Intervallo di tempo espresso in secondi all'interno del quale dovrebbe
    *                         verificarsi almeno una comunicazione con l'esterno
    * @param now Ora e Data correnti
    */
    void addDevice(unsigned char address[ADDRESS_BYTES], unsigned char sensorsCount, SensorType sensorTypes[],
                   unsigned char outputsCount, OutputType outputTypes[], unsigned long maxSilencePeriod, DateTime *now);
    /**
    * Rimuove i dispositivi condiderati persi
    *
    * @param now data e ora correnti
    *
    * @return numero di dispositivi rimossi
    */
    unsigned char removeDeadDevices(DateTime *now);
    /**
    * Ottiene un device in base al suo indirizzo di rete
    *
    * @param address l'indirizzo di rete del device
    * @return il device ricercato, 0 se nessun device e' stato trovato
    */
    NetworkDevice *getDeviceByAddress(unsigned char address[ADDRESS_BYTES]);
    /**
    * Crea un'istanza vuota e inizializzata
    */
    NetworkResources();
    /**
    * Carica uno stato salvato delle risorse di rete
    * Se uno stato e' gia' presente nell'istanza corrente lo sovrascrive
    *
    * @param fileReader il file contenente lo stato da caricare
    */
    void initialize(FileReader *fileReader);
    /**
    * Salva lo stato di tutte le risorse di rete su file
    *
    * @param fileWriter il file su cui salvare lo stato delle risorse di rete
    */
    void writeToFile(FileWriter *fileWriter);
    /**
    * @return true se le due istanze sono logicamente equivalenti
    */
    bool equals(NetworkResources *otherResources);

};

#endif // NETWORK_RESOURCES_INCLUDED
