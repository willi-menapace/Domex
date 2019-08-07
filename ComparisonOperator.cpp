/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef COMPARISON_OPERATOR_INCLUDED
#define COMPARISON_OPERATOR_INCLUDED

#include "GlobalDefines.h"

/**
* Rappresenta i possibili operatori di confronto utilizzabili
* in una condizione
*/
enum ComparisonOperator {
    //Si assegnano valori precisi perche' devono essere
    //conosciuti a priori nei file di configurazione
    LOWER_THAN = 0,
    EQUAL_TO = 1,
    HIGHER_THAN = 2
};

#endif // COMPARISON_OPERATORS_INCLUDED
