/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "Rule.h"

#include "GlobalDefines.h"

#include <assert.h>

bool Rule::verifyConditions(NetworkResources *networkResources, DateTime *now) {

    assert(networkResources != 0);
    assert(now != 0);

    for(int i = 0; i < nextConditionIndex; ++i) {
        assert(conditions[i] != 0);

        if(conditions[i]->test(networkResources, now) == false) {
            return false;
        }
    }
    return true;
}

void Rule::executeActions(NetworkResources *networkResources, XBee *xBee) {
	assert(networkResources != 0);

	for (int i = 0; i < nextActionIndex; ++i) {
		assert(actions[i] != 0);

		actions[i]->execute(networkResources, xBee);
	}
}

void Rule::executeNotActions(NetworkResources *networkResources, XBee *xBee) {
	assert(networkResources != 0);

	for (int i = 0; i < nextNotActionIndex; ++i) {
		assert(notActions[i] != 0);

		notActions[i]->execute(networkResources, xBee);
	}
}

void Rule::verifyAndExecute(NetworkResources *networkResources, DateTime *now, XBee *xBee) {

    assert(networkResources != 0);
    assert(now != 0);

    //Verifica che sia trascorso abbastanza tempo per una nuova esecuzione
    if(now->getTimeOffset() - lastActionsExecution.getTimeOffset() < minimumExecutionInterval) {
        return;
    }

    //Effettua la verifica potenzialmente dispendiosa delle condizioni
    if(verifyConditions(networkResources, now)) {
		//Solo se ci sono azioni da eseguire
		if(nextActionIndex != 0) {
			//Le azioni sono appena state eseguite
			lastActionsExecution.initialize(now);
			executeActions(networkResources, xBee);
		}
	} else {
		//Solo se ci sono azioni da eseguire
		if(nextNotActionIndex != 0) {
			//Le azioni not sono appena state eseguite
			lastActionsExecution.initialize(now);
			executeNotActions(networkResources, xBee);
		}
	}
}

void Rule::initialize(unsigned long _minimumExecutionInterval) {

    minimumExecutionInterval = _minimumExecutionInterval;

    //Non sono mai state eseguite azioni, si inizializza a 1 gennaio 2000
    lastActionsExecution.initialize(0, 0, 0, 1, 1, 1, 0);

    //Elimina azioni e condizioni contenute
    nextConditionIndex = 0;
    nextActionIndex = 0;
	nextNotActionIndex = 0;
}

void Rule::initialize(FileReader *fileReader, MemoryManager *memoryManager) {
    assert(fileReader != 0);
    assert(memoryManager != 0);

    minimumExecutionInterval = fileReader->readLong();
    //Non sono mai state eseguite azioni, si inizializza a 1 gennaio 2000
    lastActionsExecution.initialize(0, 0, 0, 1, 1, 1, 0);

    //Legge tutte le condizioni della regola corrente
    nextConditionIndex = fileReader->readByte();
    for(int i = 0; i < nextConditionIndex; ++i) {
        unsigned char conditionType = fileReader->readByte();

        //Le condizione da configurare e' una ConstantCondition
        if(conditionType == CONSTANT_CONDITION) {

            //Alloca, inizializza ed inserisce nella regola la condizione
            ConstantCondition *constantCondition = memoryManager->allocateConstantCondition();
            constantCondition->initialize(fileReader);

            conditions[i] = constantCondition;
        }
        //Se il tipo di condizione e' sconosciuto
        else {
            assert(false);
        }
    }

	//Legge tutte le azioni della regola corrente
	nextActionIndex = fileReader->readByte();
	for (int i = 0; i < nextActionIndex; ++i) {
		unsigned char actionType = fileReader->readByte();

		//Se l'azione da configurare e' una ConstantOutputAction
		if (actionType == CONSTANT_OUTPUT_ACTION) {

			//Alloca, inizializza ed inserisce nella regola l'azione
			ConstantOutputAction *constantAction = memoryManager->allocateConstantOutputAction();
			constantAction->initialize(fileReader);

			actions[i] = constantAction;
		}
		//Se il tipo di azione e' sconosciuto
		else {
			assert(false);
		}
	}

	//Legge tutte le azioni not
	nextNotActionIndex = fileReader->readByte();
	for (int i = 0; i < nextNotActionIndex; ++i) {
		unsigned char actionType = fileReader->readByte();

		//Se l'azione da configurare e' una ConstantOutputAction
		if (actionType == CONSTANT_OUTPUT_ACTION) {

			//Alloca, inizializza ed inserisce nella regola l'azione
			ConstantOutputAction *constantAction = memoryManager->allocateConstantOutputAction();
			constantAction->initialize(fileReader);

			notActions[i] = constantAction;
		}
		//Se il tipo di azione e' sconosciuto
		else {
			assert(false);
		}
	}
}

void Rule::addCondition(Condition *condition) {
    assert(condition != 0);
    assert(nextConditionIndex < MAX_CONDITIONS);

    conditions[nextConditionIndex] = condition; //Inserisce la condizione
    ++nextConditionIndex;
}

void Rule::addAction(Action *action) {
	assert(action != 0);
	assert(nextActionIndex < MAX_ACTIONS);

	actions[nextActionIndex] = action;  //Inserisce l'azione
	++nextActionIndex;
}

void Rule::addNotAction(Action *action) {
	assert(action != 0);
	assert(nextNotActionIndex < MAX_ACTIONS);

	notActions[nextNotActionIndex] = action;  //Inserisce l'azione
	++nextNotActionIndex;
}

void Rule::writeToFile(FileWriter *fileWriter) {

    assert(fileWriter != 0);

    fileWriter->writeLong(minimumExecutionInterval);

    //Scrive la configurazione delle condizioni
    fileWriter->writeByte(nextConditionIndex);
    for(int i = 0; i < nextConditionIndex; ++i) {
        fileWriter->writeByte((unsigned char) conditions[i]->getType());
        conditions[i]->writeToFile(fileWriter);
    }

	//Scrive la configurazione delle azioni
	fileWriter->writeByte(nextActionIndex);
	for (int i = 0; i < nextActionIndex; ++i) {
		fileWriter->writeByte((unsigned char)actions[i]->getType());
		actions[i]->writeToFile(fileWriter);
	}

	//Scrive la configurazione delle azioni not
	fileWriter->writeByte(nextNotActionIndex);
	for (int i = 0; i < nextNotActionIndex; ++i) {
		fileWriter->writeByte((unsigned char)notActions[i]->getType());
		notActions[i]->writeToFile(fileWriter);
	}

}
