/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef CONNECTOR_INCLUDED
#define CONNECTOR_INCLUDED

#include "GlobalDefines.h"

#ifdef OFFBOARD
#include "FakeClient.h"
#endif // OFFBOARD

#ifdef ONBOARD
#include <Ethernet.h>
#endif // ONBOARD

#include "ConnectionParameters.h"
#include "NetworkReader.h"
#include "NetworkWriter.h"

class ConnectionParameters; //Evita errori dovuti a loop di inclusione

/**
* Connessione autenticata con server
*/
class Connector {
private:

    static const unsigned long CONNECTION_TIMEOUT;      //Tempo in millisecondi di durata massima del tentativo di connessione

    ConnectionParameters *parameters; //Parametri da utilizzare per la connessione

    #ifdef OFFBOARD
    Client client; //Il client da utilizzare per il test
    #endif // OFFBOARD
    #ifdef ONBOARD
    EthernetClient client;  //Il client da utilizzare per la connessione
    #endif // ONBOARD

    NetworkReader netReader; //Reader per il dialogo
    NetworkWriter netWriter; //Writer per il dialogo

public:

    enum ConnectionProtocolConstants {
        CONNECTION_REQUEST_PACKET = 3,  //Richiesta connessione al DB
        CONNECTION_UNKNOWN = 2,  //Non e' ancora arrivata risposta
        CONNECTION_ACCEPTED = 1, //Connessione accettata
        CONNECTION_REFUSED = 0   //Connessione rifiutata
    };

    /**
    * Effettua la chiusura di tutte le risorse di rete
    */
    void closeConnection();
    /**
    * Effettua un tentativo di connessione
    */
    void tryConnection();
    /**
    * Verifica lo stato della connessione
    *
    * @return true se e' diponibile la connessione
    */
    bool isConnected();

    /**
    * Callback richiamata nel momento della reinizializzazione
    * del parametri di connessione
    * Effettua un tentativo di riconnessione se il connector era connesso ad un server
    */
    void onParamInit();

    /**
    * @return reader per la comunicazione con il server
    */
    NetworkReader *getReader();
    /**
    * @return writer per la comunicazione con il server
    */
    NetworkWriter *getWriter();

    /**
    * Crea un connettore in grado di effettuare una connessione
    * utilizzando i parametri forniti.
    *
    * @param _parameters parametri da usare per la connessione.
    *                    Possono essere modificati dall'esterno per effettuare connessioni con parametri diversi
    */
    Connector(ConnectionParameters *_parameters);
};

#endif // CONNECTOR_INCLUDED
