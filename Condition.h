/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef CONDITION_INCLUDED
#define CONDITION_INCLUDED

#include "GlobalDefines.h"

#include "NetworkResources.h"
#include "DateTime.h"

#include "FileWriter.h"
#include "ConditionTypes.cpp"

/**
* Classe astratta rappresentante una condizione verificabile
*
* @author Willi Menapace
*/
class Condition {
public:

    /**
    * @return il tipo di condizione
    */
    virtual ConditionTypes getType() = 0;
    /**
    * Verifica se la condizione e' soddisfatta
    *
    * @param networkResources il contesto di rete
    * @param now data e ora correnti
    * @return true se la condizione e' soddisfatta
    */
    virtual bool test(NetworkResources *networkResources, DateTime *now) = 0;
    /**
    * Effettua l'output della configurazione su file
    *
    * @param fileWriter il file su cui effettuare l'output
    */
    virtual void writeToFile(FileWriter *fileWriter) = 0;
    virtual ~Condition() {};
};

#endif // CONDITION_INCLUDED
