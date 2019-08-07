/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef INET_DESCRIPTOR_INCLUDED
#define INET_DESCRIPTOR_INCLUDED

#include "GlobalDefines.h"

#include "INetPacket.h"

#include "INetPacketType.cpp"
#include "SensorType.cpp"
#include "OutputType.cpp"

/**
* Pacchetto INet contenente le risorse disponibili
* ad un device.
* Ogni sensore e output ha un ID che va da 0 a N - 1
* dove N e' il numero rispettivamente di sensori e output
*/
class INetDescriptor : public INetPacket {
private:
    static unsigned const int SENSORS_DESCRIPTION_INDEX; //Indice in cui inizia la descrizione dei sensori
    unsigned char outputsDescriptionIndex;               //Indice in cui inizia la descrizione degli output
    unsigned char maxSilencePeriodIndex;                 //Indice in cui e' scritto il maxSilencePeriod
public:
    /**
    * @return il numero di sensori disponibili
    */
    unsigned char getSensorsCount();
    /**
    * @return il numero di output disponibili
    */
    unsigned char getOutputsCount();
    /**
    * Ritorna il tipo di un sensore
    *
    * @param di l'ID del sensore di cui si vuole conoscere il tipo
    *
    * @return tipo del sensore con ID specificato
    */
    SensorType getSensorById(unsigned char id);
    /**
    * Ritorna il tipo di un output
    *
    * @param di l'ID dell'output di cui si vuole conoscere il tipo
    *
    * @return tipo dell'output con ID specificato
    */
    OutputType getOutputById(unsigned char id);
    /**
    * @return intervallo di tempo espresso in secondi che dovrebbe
    *         trascorrere al massimo tra due successivi messaggi del device
    */
    unsigned long getMaxSilencePeriod();
    /**
    * Trasforma il pacchetto generico nel pacchetto della classe corrente
    *
    * @param packet il pacchetto da convertire.
    *        Deve essere packet->getType() == DESCRIPTOR
    */
    INetDescriptor(INetPacket *packet);
    /**
    * Costruisce un nuovo pacchetto
    *
    * @param buffer array in cui sara' costruito il pacchetto
    *               Deve essere in grado di contenere tutti gli altri parametri + 1 byte
    * @param sensorsCount il numero di sensori installati
    * @param sensorTypes array contenente i tipi di sensori installati.
    *                    All'indice I corrisponde il tipo del sensore con ID=I
    * @param outputsCount il numero di output installati
    * @param outputTypes array contenente i tipi di output installati
    *                    All'indice I corrisponde il tipo di output con ID=I
    * @param maxSilencePeriod intervallo di tempo espresso in secondi che dovrebbe trascorrere
    *                         al massimo tra due successivi messaggi del device
    */
    INetDescriptor(unsigned char *buffer,
                   unsigned char sensorsCount, SensorType *sensorTypes,
                   unsigned char outputsCount, OutputType *outputTypes,
                   unsigned long maxSilencePeriod);
};

#endif // INET_DESCRIPTOR_INCLUDED
