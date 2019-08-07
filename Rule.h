/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef RULE_INCLUDED
#define RULE_INCLUDED

#include "GlobalDefines.h"

#include "MemoryManager.h"
#include "ConstantOutputAction.h"
#include "ConstantCondition.h"

#define MAX_CONDITIONS 3
#define MAX_ACTIONS 3

/**
* Regola che permette di eseguire un set di azioni al verificarsi o meno di determinate condizioni
*/
class Rule {
private:

    Condition *conditions[MAX_CONDITIONS]; //Puntatori alle condizioni da verificare
    int nextConditionIndex;                //Il prossimo indice in cui allocare la condizione
    Action *actions[MAX_ACTIONS];          //Puntatori alle azioni da eseguire se tutte le condizioni vengono verificate
    int nextActionIndex;                   //Il prossimo indice in cui allocare l'azione
	Action *notActions[MAX_ACTIONS];	   //Puntatori alle azioni da eseguire se non tutte le condizioni vengono verificate
	int nextNotActionIndex;				   //Il prossimo indice in cui allocare l'azione

    DateTime lastActionsExecution;
    unsigned long minimumExecutionInterval; //La pausa in secondi che deve passare come minimo tra due esecuzioni delle azioni

    /**
    * Verifica che le condizioni contenute siano soddisfatte
    *
    * @param networkResources il contesto di rete
    * @param now data e ora correnti
    * @return true se tutte le condizioni sono soddisfatte
    */
    bool verifyConditions(NetworkResources *networkResources, DateTime *now);
    /**
    * Esegue tutte le azioni contenute nella regola
    *
    * @param networkResources il contesto di rete
	* @param xBee il modulo XBee da utilizzare per effettuare eventuali azioni
    */
	void executeActions(NetworkResources *networkResources, XBee *xBee);
	/**
	* Esegue tutte le azioni not contenute nella regola
	*
	* @param networkResources il contesto di rete
	* @param xBee il modulo XBee da utilizzare per effettuare eventuali azioni
	*/
	void executeNotActions(NetworkResources *networkResources, XBee *xBee);
public:
    /**
    * Verifica il soddisfacimento di tutte le condizioni
    * e in caso affermativo esegue tutte le azioni
    *
    * @param networkResources il contesto di rete
    * @param now data e ora correnti
	* @param xBee il modulo XBee da utilizzare per effettuare eventuali azioni
    */
    void verifyAndExecute(NetworkResources *networkResources, DateTime *now, XBee *xBee);
    /**
    * Elimina tutte le condizioni e le azioni contenute
    * nella regola. Deve essere richiamato prima di riutilizzare l'istanza
    *
    * @param _minimumExecutionInterval il tempo minimo in secondi che deve trascorrere
    *                                  tra due esecuzioni delle azioni
    */
    void initialize(unsigned long _minimumExecutionInterval);
    /**
    * Inizializza l'istanza a partire da una scritta su file
    *
    * @param fileReader il file sui cui e' scritta l'istanza
    * @param memoryManager allocatore di memoria condizioni e regole che
    *                      verranno lette
    */
    void initialize(FileReader *fileReader, MemoryManager *memoryManager);
    /**
    * Aggiunge una condizione alla regola
    *
    * @param condition la condizione da aggiungere. Non deve essere null
    */
    void addCondition(Condition *condition);
    /**
    * Aggiunge un'azione alla regola
    *
    * @param action l'azione da aggiungere. Non deve essere null
    */
    void addAction(Action *action);
	/**
	* Aggiunge un'azione not alla regola
	*
	* @param action l'azione not da aggiungere. Non deve essere null
	*/
	void addNotAction(Action *action);
    /**
    * Codifica l'intera regola sul file fornito
    *
    * @param fileWriter il file su cui codificare la regola
    */
    void writeToFile(FileWriter *fileWriter);

};

#endif // RULE_INCLUDED
