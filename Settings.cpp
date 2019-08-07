/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "Settings.h"

#include "GlobalDefines.h"

#include "ConditionTypes.cpp"
#include "ActionTypes.cpp"

#include <assert.h>

ConnectionParameters *Settings::getDbParameters() {
    return &dbParameters;
}

ConnectionParameters *Settings::getRequestServerParameters() {
    return &requestServerParameters;
}

void Settings::initialize() {
    //Dealloca tutta la memoria usata dalle regole che stanno
    //per essere sovrascritte
    memoryManager.clear();
    rulesCount = 0;

}

void Settings::verifyAndExecuteRules(NetworkResources *networkResources, DateTime *now, XBee *xBee) {
    for(int i = 0; i < rulesCount; ++i) {
        rules[i].verifyAndExecute(networkResources, now, xBee);
    }
}

void Settings::initialize(FileReader *fileReader) {

    assert(fileReader != 0);

    //Dealloca tutta la memoria usata dalle regole che stanno
    //per essere sovrascritte
    memoryManager.clear();

    //Legge le impostazioni per la connessione con il DB
    dbParameters.initialize(fileReader);
    //Legge le impostazioni per la connessione con il server delle richieste
    requestServerParameters.initialize(fileReader);

    //Legge ogni regola contenuta
    rulesCount = fileReader->readByte();
    assert(rulesCount <= MAX_RULES);
    for(int i = 0; i < rulesCount; ++i) {

        //Inizializza la regola che si sta leggendo
        rules[i].initialize(fileReader, &memoryManager);

    }
}

Settings::Settings() {
    initialize();
}
