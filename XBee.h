/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_INCLUDED
#define XBEE_INCLUDED

#include "GlobalDefines.h"

#include "XBeeSerialManager.h"
#include "XBeePacket.h"
#include "XBeeFrameablePacket.h"

/**
* Modulo XBee in grado di inviare e ricevere messaggi
*/
class XBee {
private:

    XBeeSerialManager *manager;     //Responsabile della comunicazione seriale con il modulo XBee
    unsigned char nextFrameID;      //L'ID da assegnare al prossimo pacchetto che lo richiede
    /**
    * Ottiene il prossimo ID assegnabile ad un pacchetto
    *
    * @return ID assegnabile al prossimo pacchetto
    */
    unsigned char getNextFrameID();
public:
    /**
    * Invia un pacchetto senza effettuarne manipolazioni
    *
    * @param packet il pacchetto da inviare
    */
    void send(XBeePacket *packet);
    /**
    * Invia un pacchetto effettuando la richiesta di un pacchetto XBeeTransmitStatus
    *
    * @param packet il pacchetto da inviare
    * @return l'ID che verra' associato al pacchetto XBeeTransmitStatus relativo al messaggio inviato
    */
    unsigned char sendWithAck(XBeeFrameablePacket *packet);
    /**
    * @return true se e' disponibile un nuovo pacchetto ricevuto
    */
    bool packetAvailable();
    /**
    * Effettua la ricezione di un pacchetto.
    * Richiamabile se packetAvailable() == true
    *
    * @param packet pacchetto in cui memorizzare il pacchetto ricevuto
    *
    * @return true se il pacchetto e' stato ricevuto correttamente
    */
    bool receivePacket(XBeePacket *packet);
    /**
    * Crea un gestore XBee comunicante con la scheda attraverso il
    * manager seriale fornito
    *
    * @param _manager il manager di comunicazione seriale con il modulo XBee
    */
    XBee(XBeeSerialManager *_manager);
};

#endif // XBEE_INCLUDED
