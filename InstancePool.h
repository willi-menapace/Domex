/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef INSTANCE_POOL_INCLUDED
#define INSTANCE_POOL_INCLUDED

#include "GlobalDefines.h"

#include "InstanceIterator.h"

#include <assert.h>

/**
* Classe template rappresentante un set di istanze
* di cui se ne puo' richiedere l'allocazione o la
* deallocazione.
*
* IMPORTANTE: L'allocazione iniziale e' effettuata dimanicamente, dunque
*             e' possibile istanziare questa classe solo prima dell'esecuzione del programma
*/
template <class E>
class InstancePool {
private:
    E *instancePool;        //Il pool delle istanze
    bool *instanceMap;      //true indica che una data posizione e' allocata
    unsigned char poolSize;           //Il numero di istanze contenibili nel pool
    unsigned char instanceCount;      //Il numero di istanza attualmente allocate nel pool
    unsigned char maxAllocatedIndex;  //Il massimo indice di instancePool che contiene un'istanza allocata
    InstanceIterator<E> iterator;
public:
    /**
    * Alloca la memoria e restituisce un'istanza non inizializzata di E
    *
    * @return istanza non inizializzata di E
    */
    E *allocate();
    /**
    * Dealloca l'istanza
    *
    * @param instance l'istanza da deallocare
    * @return true se l'istanza appartiene al pool corrente ed e' stata deallocata o se vi apparteneva in passato ed era gia' deallocata,
              false se l'istanza non appartiene al pool corrente e non e' stata deallocata
    */
    bool deallocate(E *instance);
    /**
    * Dealloca tutte le istanze
    */
    void clear();
    /**
    * Ritorna un iteratore sulle istanza del pool
    * NOTA: resetta tutti gli iteratori ottenuti dall'oggetto corrente
    *
    */
    InstanceIterator<E> *getInstanceIterator();
    /**
    * @return il numero di istanze attualmente allocate nel pool
    */
    unsigned char getInstanceCount();
    /**
    * Crea una pool in grado di contenere _poolSize istanze di E
    *
    * @param _poolSize il numero di istanza di E contenibili nella pool
    */
    InstancePool(unsigned char _poolSize);
    ~InstancePool();
};

template <class E>
E *InstancePool<E>::allocate() {

    unsigned char allocatedIndex = 0;
    //Ricerca il primo indice libero
    for(int i = 0; i < poolSize; ++i) {
        if(instanceMap[i] == false) {
            allocatedIndex = i;
            //Se allochiamo una porzione di memoria maggiore del massimo indice lo aggiorniamo
            //in modo che le future ricerche includano anche il device appena allocato
            if(allocatedIndex > maxAllocatedIndex) {
                maxAllocatedIndex = allocatedIndex;
            }
            instanceMap[allocatedIndex] = true;
            ++instanceCount;
            return &instancePool[i];
        }
    }

    //E' necessaria una dimensione maggiore del pool corrente
    assert(false);
    return 0;
}

template <class E>
bool InstancePool<E>::deallocate(E *instance) {
    if(instance == 0) {
        return false;
    }

    //Se l'istanza non e' contenuta nella pool e' impossibile deallocarla
    if(!(instance >= instancePool && instance <= &instancePool[poolSize - 1])) {
        return false;
    }

    int index = instance - instancePool;
    assert(index >= 0);

    //Se si rimuove l'ultima istanza allocata, allora si decrementa l'indice di allocazione massimo
    if(index == maxAllocatedIndex) {
        --maxAllocatedIndex;
    }

    //Solo se non era gia' stata deallocata si riduce il numero di istanze
    if(instanceMap[index] == true) {

        assert(instanceCount > 0);

        --instanceCount;
        instanceMap[index] = false;
    }

    return true;
}

template <class E>
void InstancePool<E>::clear() {
    instanceCount = 0;
    maxAllocatedIndex = 0;

    for(int i = 0; i < poolSize; ++i) {
        instanceMap[i] = false;
    }
}

template <class E>
InstanceIterator<E> *InstancePool<E>::getInstanceIterator() {
    iterator.initialize(maxAllocatedIndex + 1, instancePool, instanceMap);
    return &iterator;
}

template <class E>
unsigned char InstancePool<E>::getInstanceCount() {
    return instanceCount;
}

template <class E>
InstancePool<E>::InstancePool(unsigned char _poolSize) {
    instanceCount = 0;
    poolSize = _poolSize;
    maxAllocatedIndex = 0;

    instancePool = new E[poolSize];
    instanceMap = new bool[poolSize];

    //Nessuna istanza e' stata allocata
    for(int i = 0; i < poolSize; ++i) {
        instanceMap[i] = false;
    }

}

template <class E>
InstancePool<E>::~InstancePool() {
    delete[] instancePool;
    delete[] instanceMap;
}

#endif // INSTANCE_POOL_INCLUDED
