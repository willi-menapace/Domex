/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef INET_PACKET_INCLUDED
#define INET_PACKET_INCLUDED

#include "GlobalDefines.h"

#include "INetPacketType.cpp"

/**
* Pacchetto a livello applicativo utilizzato nella rete interna
*/
class INetPacket {
private:
    unsigned char length; //Lunghezza dei dati del pacchetto
protected:
    unsigned char *data;  //Dati contenuti dal pacchetto
    /**
    * Costruttore di copia. Il pacchetto copiato condivide lo stesso buffer di dati
    *
    * @param packet il pacchetto da copiare
    */
    INetPacket(INetPacket *packet);
public:
    /**
    * Ottiene il tipo di pacchetto
    *
    * @return il tipo di pacchetto
    */
    INetPacketType getType();
    /**
    * Ottiene la lunghezza in byte del pacchetto
    *
    * @return la lunghezza in byte del pacchetto
    */
    unsigned char getLength();
    /**
    * Ottiene i dati contenuti dal pacchetto
    *
    * @return i dati contenuti dal pacchetto
    */
    unsigned char *getData();
    /**
    * Costruisce un pacchetto contenente i dati specificati
    *
    * @param _data i dati che dovra' contenere il pacchetto.
    *              Rimane comune a tutti i pacchetti costruiti a partire da quello corrente
    * @param _length la lunghezza in byte dei dati
    */
    INetPacket(unsigned char *_data, unsigned char _length);
};

#endif // INET_PACKET_INCLUDED
