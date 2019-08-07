/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

#ifndef XBEE_PACKET_INCLUDED
#define XBEE_PACKET_INCLUDED

#include "GlobalDefines.h"

#include "XBeePacketType.cpp"
#include "XBeeSerialManager.h"

/**
* Pacchetto trasmittibile e ricevibile tramite modulo XBee
*/
class XBeePacket {
private:
    unsigned char data[XBEE_PACKET_BUFFER_SIZE];
    /**
    * Calcola l'LSB della somma dei byte del payload
    * Il buffer deve essere completamente inizializzato salvo checksum
    * prima della chiamata di questo metodo.
    *
    * @return LSB della somma dei byte del payload
    *
    */
    unsigned char getPayloadSumLSB();
    /**
    * Calcola la checksum del pacchetto dati. Il pacchetto dati
    * deve essere gia' costruito salvo la checksum prima di richiamare questo metodo.
    * La checksum non viene inserita nel pacchetto dati da questo metodo
    *
    * @return il checksum calcolato del pacchetto
    */
    unsigned char generateChecksum();
protected:
    static const int PAYLOAD_INDEX;
    /**
    * Ritorna il buffer di dimensione XBEE_PACKET_BUFFER_SIZE che verra' inviato al modulo XBee.
    * La sottoclasse e' responsabile di assicurarsi che esso
    * sia totalmente inizializzato prima di richiamare metodi di invio
    *
    * @return il buffer che verra' inviato
    */
    unsigned char *getData();
    /**
    * Ottiene la lunghezza del payload.
    * IMPORTANTE: Si suppone che esso non superi i 255 byte
    *
    * @return la lunghezza del payload del pacchetto
    */
    unsigned char getLength();
    /**
    * Costruttore di copia, copia solo lo stato del pacchetto tralasciando i byte superflui
    *
    * @param packet il pacchetto da copiare
    */
    XBeePacket(XBeePacket *packet);
public:
    /**
    * @return il tipo di pacchetto
    */
    XBeePacketType getType();
    /**
    * @return true se la checksum del pacchetto e' corretta
    */
    bool validateChecksum();

    /**
    * Invia il pacchetto attraverso il manager della connessione fornito.
    * Prima dell'invio effettua automaticamente il calcolo della checksum.
    *
    * @param manager il manager della connessione con il modulo XBee
    */
    void send(XBeeSerialManager *manager);
    /**
    * Costruttore di default
    */
    XBeePacket();

    virtual ~XBeePacket() {}

    friend class XBee; //Permette al modulo di ricezione di inserire i dati ricevuti nel pacchetto
};

#endif // XBEE_PACKET_INCLUDED
