/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef NETWORK_READER_INCLUDED
#define NETWORK_READER_INCLUDED

#include "GlobalDefines.h"

#include "StreamReader.h"

#ifdef OFFBOARD
#include <iostream>
#include "FakeClient.h"
#endif // OFFBOARD

#ifdef ONBOARD
#include <Ethernet.h>
#endif // ONBOARD

/**
* Reader in grado di effettuare la
* lettura da un socket
*/
class NetworkReader : public StreamReader {
private:
    Client *client;
protected:
    #ifdef OFFBOARD
    std::istream *getStream();
    #endif // ONFFBOARD
    #ifdef ONBOARD
    Stream *getStream();
    #endif // ONFFBOARD
public:
    /**
    * Imposta il socket da cui effettuare la lettura
    *
    * @param _client il socket da cui effettuare la lettura.
    *                Se impostato a 0 rilascia il socket precedentemente impostato
    *                ma non ne effettua la chiusura
    */
    void initialize(Client *_client);
    /**
    * @return true se il socket e' connesso, false se
    *         non e' connesso o non e' stato impostato.
    *         I client di test sono sempre connessi
    */
    bool isConnected();

    /**
    * Crea un reader vuoto senza nessun socket impostato
    */
    NetworkReader();
};

#endif // NETWORK_READER_INCLUDED
