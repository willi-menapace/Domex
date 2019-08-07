/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "FakeOutput.h"

#include "GlobalDefines.h"

OutputType FakeOutput::getType() {
    return type;
}

void FakeOutput::performOutput(long value) {
    lastOutputValue = value;
}

long FakeOutput::getLastOutputValue() {
    return lastOutputValue;
}

FakeOutput::FakeOutput(OutputType _type) {
    lastOutputValue = 0;
    type = _type;
}
