/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef MEMORY_MANAGER_INCLUDED
#define MEMORY_MANAGER_INCLUDED

#include "GlobalDefines.h"

#include "ConstantCondition.h"
#include "ConstantOutputAction.h"
#include "InstancePool.h"

#define CONSTANT_CONDITION_INSTANCES 15
#define CONSTANT_OUTPUT_ACTION_INSTANCES 10

/**
* Classe che si occupa di gestire un pool di memoria
* contenente istanze di classi usate comunemente nel programma
*
* IMPORTANTE: L'allocazione iniziale e' effettuata dimanicamente, dunque
*             e' possibile istanziare questa classe solo prima dell'esecuzione del programma
*/
class MemoryManager {
private:
    InstancePool<ConstantCondition> constantConditions;       //Pool delle istanze di ConstantCondition

    InstancePool<ConstantOutputAction> constantOutputActions; //Pool delle istanze di ConstantOutputAction

public:
    MemoryManager();
    /**
    * Restituisce un'istanza non inizializzata di ConstantCondition
    */
    ConstantCondition *allocateConstantCondition();
    /**
    * Restituisce un'istanza non inizializzata di ConstantOutputAction
    */
    ConstantOutputAction *allocateConstantOutputAction();
    /**
    * Dealloca l'istanza di Condition spacificata rendendo la memoria occupata disponibile
    *
    * @param condition la condizione da deallocare
    * @return true se la condizione e' stata deallocata, false in caso la condizione non sia stata istanziata da questo manager
    */
    bool deallocateCondition(Condition *condition);
    /**
    * Dealloca l'istanza di Action specificata rendendo la memoria occupara disponibile
    *
    * @param action l'azione da deallocare
    * @return true se l'azione e' stata deallocata, false in caso l'azione non sia stata istanziata da questo manager
    */
    bool deallocateAction(Action *action);
    /**
    * Dealloca tutte le istanze
    */
    void clear();

};

#endif // MEMORY_MANAGER_INCLUDED
