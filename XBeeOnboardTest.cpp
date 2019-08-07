/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

/**
* Test su scheda della libreria XBee
* E' possibile visualizzare pacchetti dati da altri device XBee connessi alla rete
*
* Hardware richiesto:
* -Modulo XBee Series 2 con firmware Coordinator API
* -Modulo XBee Series 2 con firmware Router o End Node (API mode facoltativa)
*
* Configurazione pre caricamento:
* -Collegare il modulo XBee Coordinator API al device corrente attraverso Serial2
* -Impostare CTS_PIN al pin utilizzato per il controllo CTS del modulo XBee
* -Alimentare il secondo modulo XBee. Il collegamento ad altri device o a PC e' facoltativo
* -Impostare routerAddress all'indirizzo del device con firmware Router o End Node
*
* Seguire la procedura di esportazione per il caricamento
*/

#ifdef ONBOARD

#include <GlobalDefines.h>
#include <XBee.h>
#include <XBeeATCommand.h>
#include <XBeeATResponse.h>
#include <XBeeFrameablePacket.h>
#include <XBeeLibraryTestCases.h>
#include <XBeePacket.h>
#include <XBeeReceivePacket.h>
#include <XBeeRemoteATCommand.h>
#include <XBeeRemoteATResponse.h>
#include <XBeeSerialManager.h>
#include <XBeeTransmitRequest.h>
#include <XBeeTransmitStatus.h>

#include <SD.h>
#include <SPI.h>
#include <Arduino.h>

//Istanzia le classi usate per inviare e ricevere pacchetti
const int CTS_PIN = 12;
XBeeSerialManager serialManager(&Serial, CTS_PIN); //Il modulo XBee e' collegato alla porta seriale 2 e l'input digitale 12 viene utilizzato per il Clear To Send
XBee xBee(&serialManager);

//Definisce gli indirizzi fisici dei device coinvolti
unsigned char coordinatorAddress[ADDRESS_BYTES] = {0x00, 0x13, 0xA2, 0x00, 0x40, 0xBF, 0x57, 0x3E};
unsigned char routerAddress[ADDRESS_BYTES] = {0x00, 0x13, 0xA2, 0x00, 0x40, 0xC2, 0x59, 0x24};

//Definisce l'indirizzo di rete del device router che non e' noto a priori
//Viene impostato per richiedere la scoperta dell'indirizzo di rete
unsigned char routerNetAddress[4] = {0xFF, 0xFE};

//Array di dati utilizzati nelle trasmissioni di test
unsigned char data[4] = {0x43, 0x56, 0xFE, 0xAA};
unsigned char signalStrengthCommand[2] = {'D', 'B'};

//Costruisce i pacchetti che verranno inviati
XBeeTransmitRequest txPacket(4, data, routerAddress, routerNetAddress);
XBeeATCommand signalStrengthPacket(signalStrengthCommand, 0, 0);
XBeeRemoteATCommand remoteSignalStrengthPacket(signalStrengthCommand, 0, 0, routerAddress, routerNetAddress);

void setup() {
  pinMode(CTS_PIN, INPUT);
  Serial.begin(9600);
  //Serial2.begin(9600);
}


void loop() {

  Serial.println("------------------------");
  Serial.println("Sending a Normal data packet, a Local Signal Strength Request and a Remote Signal Strength Request");

  //Invia i pacchetti di test
  xBee.sendWithAck(&txPacket);
  xBee.sendWithAck(&signalStrengthPacket);
  xBee.sendWithAck(&remoteSignalStrengthPacket);

  //Riceve e visualizza i pacchetti in arrivo
  XBeePacket receivePacket;
  while(xBee.packetAvailable()) {
    xBee.receivePacket(&receivePacket);

    //Per ogni tipo possibile di pacchetto stampa delle informazioni riassuntive
    if(receivePacket.getType() == TRANSMIT_STATUS) {
      Serial.println("Received ACK packet");
      XBeeTransmitStatus transmitStatus(&receivePacket);
      Serial.println(transmitStatus.receivedCorrectly());
    } else if(receivePacket.getType() == AT_RESPONSE) {
      Serial.println("Received Local Signal Strength Response packet");
      XBeeATResponse signalStrength(&receivePacket);
      Serial.println(signalStrength.executedCorrectly());
      unsigned char dataSize = signalStrength.getQueriedDataSize();
      Serial.println("Queried Data");
      for(int i = 0 ; i < dataSize; ++i) {
        Serial.print(signalStrength.getQueriedData()[i]);
        Serial.print(' ');
      }
      Serial.println();
    } else if(receivePacket.getType() == REMOTE_AT_COMMAND_RESPONSE) {
      Serial.println("Received Remote Signal Strength Response packet");
      XBeeRemoteATResponse signalStrength(&receivePacket);
      Serial.println(signalStrength.executedCorrectly());
    } else if(receivePacket.getType() == RECEIVE_PACKET) {
      Serial.println("Received Data packet");
      XBeeReceivePacket dataPacket(&receivePacket);
      unsigned char dataSize = dataPacket.getApplicationDataSize();
      Serial.println("Application Data");
      for(int i = 0 ; i < dataSize; ++i) {
        Serial.print((char) dataPacket.getApplicationData()[i]);
        Serial.print(' ');
      }
      Serial.println();
    } else {
      Serial.println("Received Unknown Packet");
    }

  }

  delay(5000);

}

#endif //ONBOARD
