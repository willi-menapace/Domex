/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef COMPUTED_DEVICE_STATUS_INCLUDED
#define COMPUTED_DEVICE_STATUS_INCLUDED

#include "GlobalDefines.h"

enum ComputedDeviceStatus {
    UP_AND_RUNNING,
    TEMPORARILY_NOT_RESPONDING,
    LOST
};

#endif // COMPUTED_DEVICE_STATUS_INCLUDED
