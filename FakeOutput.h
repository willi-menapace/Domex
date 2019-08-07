/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef FAKE_OUTPUT_INCLUDED
#define FAKE_OUTPUT_INCLUDED

#include "GlobalDefines.h"

#include "Output.h"

/**
* Output simulato con tipo impostabile
*/
class FakeOutput : public Output {

private:
    OutputType type;
    long lastOutputValue;

public:

    OutputType getType();
    void performOutput(long value);

    /**
    * @return l'ultimo valore inviato in output sull'output corrente
    */
    long getLastOutputValue();

    /**
    * Costruisce un output con il tipo specificato
    *
    * @param _type il tipo dell'output
    */
    FakeOutput(OutputType _type);
};

#endif // FAKE_OUTPUT_INCLUDED
