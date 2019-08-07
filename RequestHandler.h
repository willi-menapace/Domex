/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef REQUEST_HANDLER_INCLUDED
#define REQUEST_HANDLER_INCLUDED

#include "GlobalDefines.h"

#include "Connector.h"
#include "Settings.h"

/**
* Gestore delle richieste effettuate
*/
class RequestHandler {
private:

    static const unsigned long CONNECTION_RETRY_PERIOD; //Tempo in millisecondi dopo cui ritentare la connessione

    static const unsigned long DEVICE_SEND_TIMEOUT;     //Tempo in millisecondi dopo cui si ritiene ci sia stato un errore durante l'invio della lista di device
    static const unsigned long SETTINGS_UPDATE_TIMEOUT; //Tempo in millisecondi dopo cui si ritiene ci sia stato un errore durante la ricezione delle nuove impostazioni

    Settings *settings;
    Connector connector;    //Gestore della connessione con il server delle richieste

    unsigned long lastConnectionAttempt; //Millisecondi in cui e' avvenuto l'ultimo tentativo di connessione

    NetworkReader *netReader; //Reader per il dialogo con il server delle richieste
    NetworkWriter *netWriter; //Writer per il dialogo con il server delle richieste

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
    * Risponde a tutte le richieste in arrivo
    * Deve essere presente la connessione al momento della chiamata
    */
    void respondToAllRequests();
    /**
    * Effettua l'invio delle informazioni sui device disponibili sulla rete
    * Deve essere presente la connessione al momento della chiamata
    *
    * @return true in caso di successo, false in caso di errori di comunicazione con il server
    */
    bool sendDeviceList();
    /**
    * Effettua la sovrascrittura delle impostazioni con quelle inviate dal server
    * Deve essere presente la connessione al momento della chiamata
    * L'operazione e' potenzialmente dispendiosa e puo' provocare l'abbattimento delle connessioni
    * e il loro ripristino in caso di riuscito aggiornamento
    *
    * @return true in caso di successo, false in caso di errori di comunicazione con il server
    */
    bool updateSettings();
public:

    enum RequestProtocolConstants {
        DEVICE_LIST_REQUEST = 1,  //Richiesta di invio dei descrittori dei device conosciuti
        DEVICE_LIST_RECEIVED = 1, //Segnalazione di corretta ricezione della lista di device

        SETTINGS_UPDATE_REQUEST = 2,  //Richiesta di aggiornamento delle impostazioni
        SETTINGS_UPDATE_RECEIVED = 1  //Segnalazione di corretto aggiornamento delle impostazioni
    };

    /**
    * Effettua la chiusura di tutte le risorse di rete
    */
    void closeConnection();

    /**
    * Soddisfa tutte le richieste effettuate
    * dal server delle richieste.
    * L'operazione e' potenzialmente dispendiosa
    */
    void processRequests();

    /**
    * Costruisce un gestore delle richieste che funziona
    * secondo le impostazioni fornite
    *
    * @param _settings le impostazioni di funzionamento.
    *                  Possono essere modificate su richiesta del server
    */
    RequestHandler(Settings *_settings);

};

#endif // REQUEST_HANDLER_INCLUDED
