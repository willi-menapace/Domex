/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef OUTPUT_INCLUDED
#define OUTPUT_INCLUDED

#include "GlobalDefines.h"

#include "OutputType.cpp"

/**
* Entita' in grado di effettuare una determinata azione sulla
* base di un valore fornito
*/
class Output {
public:
    /**
    * @return il tipo di dispositivo di output
    */
    virtual OutputType getType() = 0;
    /**
    * Effettua un'azione sulla base del valore fornito
    *
    * @param value valore a 64 bit che descrive l'azione da effettuare
    */
    virtual void performOutput(long value) = 0;
    virtual ~Output() {}
};

#endif // OUTPUT_INCLUDED
