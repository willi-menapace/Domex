/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef INSTANCE_ITERAROR_INCLUDED
#define INSTANCE_ITERATOR_INCLUDED

#include "GlobalDefines.h"

/**
* Iteratore su pool di istanze
*
* Supporta la cancellazione di tutti gli elementi gia' ritornati da next
* ma non di quelli ancora da ritornare
*/
template <class E>
class InstanceIterator {
private:
    E *instancePool;            //Array delle istanze di dimensione size
    E *lastReturnedInstance;
    bool *instanceMap;          //Array che mappa le istanza valide di instancePool. true = istanza valida
    unsigned char currentIndex; //L'indice della prossima istanza da ritornare. currentIndex == size se non ci sono piu' istanza da ritornare
    unsigned char size;         //Le dimensioni dell'array delle istanza

public:
    /**
    * Inizializza l'iteratore per iterare sul dato array di istanze
    *
    * @param SIZE la dimensione di _instancePool
    * @param _instancePool array di istanze su cui iterare
    * @param _instanceMap mappa ad _instancePool che segnala ponendo a true le istanza valide
    */
    void initialize(const unsigned char SIZE, E *_instancePool, bool *_instanceMap);
    /**
    * @return true se la prossima chiamata a next restituira' una nuova istanza
    */
    bool hasNext();
    /**
    * @return una nuova istanza se hasNext() == true, altrimenti l'ultima ritornata
    *         Se nessuna istanza e' ancora stata ritornara e hasNext() == false riturna 0
    */
    E *next();
};

template <class E>
void InstanceIterator<E>::initialize(const unsigned char SIZE, E *_instancePool, bool *_instanceMap) {
    lastReturnedInstance = 0;
    size = SIZE;
    instancePool = _instancePool;
    instanceMap = _instanceMap;
    //Sposta currentIndex alla prima istanza da ritornare
    for(currentIndex = 0; currentIndex < size; ++currentIndex) {
        if(instanceMap[currentIndex] == true) {
            break;
        }
    }
}

template <class E>
bool InstanceIterator<E>::hasNext() {
    //currentIndex == size segnala che non ci sono istanze da ritornare
    if(currentIndex < size) {
        return true;
    }
    return false;
}

template <class E>
E *InstanceIterator<E>::next() {
    //Se ci sono istanze da ritornare
    if(currentIndex < size) {
        //Memorizza l'istanza da ritornare
        lastReturnedInstance = &instancePool[currentIndex];
        //Ricerca l'istanza da ritornare alla prossima chiamata
        for(currentIndex = currentIndex + 1; currentIndex < size; ++currentIndex) {
            if(instanceMap[currentIndex] == true) {
                break;
            }
        }
    }

    return lastReturnedInstance;
}

#endif // INSTANCE_ITERAROR_INCLUDED
