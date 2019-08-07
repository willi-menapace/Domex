/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef NETWORK_WRITER_INCLUDED
#define NETWORK_WRITER_INCLUDED

#include "GlobalDefines.h"

#include "StreamWriter.h"

#ifdef OFFBOARD
#include "FakeClient.h"
#endif

#ifdef ONBOARD
#include <Ethernet.h>
#endif // ONBOARD

/**
* Writer in grado di effettuare la scrittura
* attraverso un socket
*/
class NetworkWriter : public StreamWriter {
private:
    Client *client;
protected:
    #ifdef OFFBOARD
    std::ostream *getStream();
    #endif // ONFFBOARD
    #ifdef ONBOARD
    Stream *getStream();
    #endif // ONFFBOARD
public:

    /**
    * Imposta il socket attraverso cui effettuare la scrittura
    *
    * @param _client socket su cui effettuare la scrittura
    *                Se _client == 0 rilascia il socket precedentemente impostato
    *                ma non ne effettua la chiusura
    */
    void initialize(Client *_client);
    /**
    * @return true se il socket e' connesso, false se
    *         non e' connesso o non e' stato impostato
    *         I client di test sono sempre connessi
    */
    bool isConnected();
    /**
    * Forza lo svuotamento del buffer di invio
    */
    void flush();
    /**
    * Crea un writer vuoto senza alcun socket impostato
    */
    NetworkWriter();
};

#endif // NETWORK_WRITER_INCLUDED
