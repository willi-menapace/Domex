/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef SETTINGS_INCLUDED
#define SETTINGS_INCLUDED

#include "GlobalDefines.h"

#define MAX_RULES 10

#include "MemoryManager.h"
#include "Rule.h"
#include "FileReader.h"
#include "ConnectionParameters.h"

class Settings {
private:

    MemoryManager memoryManager;  //Responsabile dell'allocazione della memoria per tutte le condizioni e le azioni presenti nelle regole

    unsigned char rulesCount; //Il numero di regole attualmente presenti
    Rule rules[MAX_RULES];    //Le regole impostate

    ConnectionParameters dbParameters; //Parametri per la connessione al DB
    ConnectionParameters requestServerParameters; //Parametri per la connessione al server delle richieste

    /**
    * Azzera l'istanza
    */
    void initialize();
public:

    /**
    * @return parametri per la connessione autenticata al DB
    */
    ConnectionParameters *getDbParameters();
    /**
    * @return parametri per la connessione autenticata al server delle richieste
    */
    ConnectionParameters *getRequestServerParameters();
    /**
    * Verifica ed esegue le regole
    *
    * @param networkResources il contesto di rete
    * @param now data e ora correnti
	* @param xBee il modulo XBee da utilizzare per l'eventuale esecuzione di azioni
    */
    void verifyAndExecuteRules(NetworkResources *networkResources, DateTime *now, XBee *xBee);
    /**
    * Inizializza l'istanza con un file di configurazione
    * L'operazione e' potenzialmente dispendiosa e comporta l'abbattimento delle connessioni
    * con l'esterno e il tentativo di riconnessione
    *
    * @param fileReader lettore del file contenente la configurazione
    */
    void initialize(FileReader *fileReader);
    /**
    * Crea un'istanza inizializzata
    */
    Settings();

};

#endif // SETTINGS_INCLUDED
