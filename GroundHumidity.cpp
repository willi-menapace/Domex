// Definisce lo stato di umidità del terreno

#ifndef GROUND_HUMIDITY_INCLUDED
#define GROUND_HUMIDITY_INCLUDED

#include "GlobalDefines.h"

enum GroundHumidity {
    COMPLETELY_DRY  = 0,
    DRY             = 200,
    QUITE_HUMID     = 300,
    HUMID           = 450,
    QUITE_WET       = 650,
    WET             = 750,
    ERROR
};

#endif // GROUND_HUMIDITY_INCLUDED
