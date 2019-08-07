/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef DEVICE_MANAGER_INCLUDED
#define DEVICE_MANAGER_INCLUDED

#include "GlobalDefines.h"

#include "Sensor.h"
#include "Output.h"

#define DEVICE_MANAGER_MAX_SENSORS 5
#define DEVICE_MANAGER_MAX_OUTPUTS 5

/**
* Manager dei sensori e degli output disponibili ad un device
*/
class DeviceManager {
private:

    /**
    * Manager di un sensore registrato sul device
    */
    class SensorManager {
    private:
        Sensor *sensor;                  //Il sensore registrato
        unsigned long sensorRefreshTime; //Tempo espresso in millisecondi dopo il quale e' possibile effettuare un'ulteriore lettura del sensore
        long lastReadValue;     //Ultimo valore letto dal sensore
        bool newValueAvailable;          //True se readValue contiene un valore che non e' ancora stato letto
        unsigned long lastReadingTime;   //Tempo di sistema espresso in millisecondi al quale e' stata effettuata l'ultima lettura
    public:
        /**
        * Effetua la lettura di un nuovo valore se
        * e' passato il tempo minimo impostato
        *
        * @param currentTime il tempo di sistema corrente espresso in millisecondi
        */
        void refresh(unsigned long currentTime);
        /**
        * @return true se e' disponibile un valore del sensore non ancora letto
        */
        bool isAvailable();
        /**
        * @return l'ultimo valore letto dal sensore
        */
        long getValue();
        /**
        * @return il sensore registrato
        */
        Sensor *getSensor();
        /**
        * Inizializza il manager di sensore
        *
        * @param _sensor il sensore da monitorare
        * @param _sensorRefreshTime intervallo minimo espresso in millisecondi che separa
        *                           due letture del sensore
        */
        void initialize(Sensor *_sensor, unsigned long _sensorRefreshTime);
    };

    SensorManager sensors[DEVICE_MANAGER_MAX_SENSORS];  //Sensori disponibili al device. L'ID assegnato ad un sensore e' uguale al suo indice
    SensorType sensorTypes[DEVICE_MANAGER_MAX_SENSORS]; //Tipi dei sensori disponibili al device. Evita continue allocazioni di array nelle chiamate a getSensorType()
    unsigned char sensorsCount;                         //Numero di sensori disponibili

    Output *outputs[DEVICE_MANAGER_MAX_OUTPUTS];         //Output disponibili al device. L'ID assegnato ad un output e' uguale al suo indice
    OutputType outputTypes[DEVICE_MANAGER_MAX_SENSORS];  //Tipi di output disponibili al device. Evita continue allocazioni di array nelle chiamate a getOutputType();
    unsigned char outputsCount;                          //Numero di output disponibili

public:

    DeviceManager();

    /**
    * Registra un sensore per essere gestito dal manager
    *
    * @param sensor il sensore da registrare
    * @param sensorRefreshTime intervallo minimo espresso in millisecondi tra due letture successive del sensore
    */
    void registerSensor(Sensor *sensor, unsigned long sensorRefreshTime);
    /**
    * Registra un output per essere gestito dal manger
    *
    * @param output l'output da registrare
    */
    void registerOutput(Output *output);
    /**
    * @return il numero di sensori registrati
    */
    unsigned char getSensorsCount();
    /**
    * @return il numero di output registrati
    */
    unsigned char getOutputsCount();
    /**
    * Ottiene i tipi di sensori registrati
    *
    * @return  array di dimensione getSensorsCount() contenente i tipi di sensori.
    *          L'indice dell'array corrisponde all'ID del sensore
    */
    SensorType *getSensorTypes();
    /**
    * Ottiene i tipi di output registrati
    *
    * @return array di dimensione getOutputsCount() contenente i tipi di sensori
    *                    L'indice dell'array corrisponde all'ID dell'output
    */
    OutputType *getOutputTypes();
    /**
    * Effettua la lettura dei sensori per cui e' trascorso
    * il tempo minimo di refresh
    */
    void refreshSensors();
    /**
    * @return il numero di valori disponibili non ancora letti
    */
    unsigned char getAvailableValuesCount();
    /**
    * Ottiene i valori disponibili non ancora letti
    *
    * @param ids mapping tra indici e ID dei sensori a cui i valori fanno riferimento
    *            Deve avere almeno dimensioni = getAvailableValuesCount()
    * @param values i valori letti dai sensori
    *               Deve avere almeno dimensioni = getAvailableValuesCount()
    */
    void getAvailableValues(unsigned char *ids, long *values);
    /**
    * Invia in output il valore specificato all'output richiesto
    *
    * @param outputId l'ID dell'output a cui inviare il valore
    * @param value il valore da inviare all'output
    */
    void performOutput(unsigned char outputId, long value);
};

#endif // DEVICE_MANAGER_INCLUDED
