/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef INET_OUTPUT_REQUEST_INCLUDED
#define INET_OUTPUT_REQUEST_INCLUDED

#include "GlobalDefines.h"

#include "INetMappedValues.h"

/**
* Pacchetto INet contenente una serie di richieste di
* attivazione di dispositivi di output
*/
class INetOutputRequest : public INetMappedValues {

public:

    /**
    * Trasforma il pacchetto generico nel pacchetto della classe corrente
    *
    * @param packet il pacchetto da convertire.
    *        Deve essere packet->getType() == OUTPUT_REQUEST
    */
    INetOutputRequest(INetPacket *packet);
    /**
    * Costruisce un nuovo pacchetto
    *
    * @param buffer array in cui sara' costruito il pacchetto.
    *               Deve essere in grado di contenere tutti gli altri parametri + 1
    * @param valuesCount il numero di richieste di output
    * @param outputIds mappa tra indici dell'array outputValues e gli ID degli output
    *                  a cui i valori fanno riferimento
    * @param outputValues i valori da inviare ai rispettivi output
    */
    INetOutputRequest(unsigned char *buffer, unsigned char valuesCount,
                      unsigned char *outputIds, long *outputValues);
};

#endif // INET_OUTPUT_REQUEST_INCLUDED
