/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef ACTION_INCLUDED
#define ACTION_INCLUDED

#include "GlobalDefines.h"

#include "NetworkResources.h"

#include "FileWriter.h"
#include "ActionTypes.cpp"
#include "XBee.h"

/**
* Classe astratta che rappresenta un'azione eseguibile
* all'interno di una rete
*
* @author Willi Menapace
*/
class Action {

public:

    /**
    * @return il tipo di azione
    */
    virtual ActionTypes getType() = 0;
    /**
    * Esegue l'azione all'interno del contesto di rete specificato
    *
    * @param networkResources il contesto di rete
    * @param xBee modulo per la trasmissione
    */
    virtual void execute(NetworkResources *networkResources, XBee *xBee) = 0;
    /**
    * Effettua l'output della configurazione su file
    *
    * @param fileWriter il file su cui effettuare l'output
    */
    virtual void writeToFile(FileWriter *fileWriter) = 0;
    virtual ~Action() {};
};

#endif // ACTION_INCLUDED
