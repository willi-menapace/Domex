/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#include "MemoryManager.h"

#include "GlobalDefines.h"

MemoryManager::MemoryManager() : constantConditions(CONSTANT_CONDITION_INSTANCES), constantOutputActions(CONSTANT_OUTPUT_ACTION_INSTANCES){
}

ConstantCondition *MemoryManager::allocateConstantCondition() {
    return constantConditions.allocate();
}

ConstantOutputAction *MemoryManager::allocateConstantOutputAction() {
    return constantOutputActions.allocate();
}

bool MemoryManager::deallocateCondition(Condition *condition) {
    ConstantCondition *castedCondition = (ConstantCondition *) condition;
    bool result = constantConditions.deallocate(castedCondition);
    return result;
}

bool MemoryManager::deallocateAction(Action *action) {
    ConstantOutputAction *castedAction = (ConstantOutputAction *) action;
    bool result = constantOutputActions.deallocate(castedAction);
    return result;
}

void MemoryManager::clear() {
    constantConditions.clear();
    constantOutputActions.clear();
}
