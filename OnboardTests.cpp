/* Copyright (C) 2015 Willi Menapace <willi.menapace@gmail.com>, Simone Lorengo - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Willi Menapace <willi.menapace@gmail.com>
 */

/**
* Esegue tutti i test cases delle seguenti librerie disponibili in modalita' onboard:
* -Core
* -IO
* -INet
*
* Alcuni test richiedono il controllo manuale dei risultati
*
* Hardware richiesto:
* -Shield Ethernet
* -Shield SD
*
* Configurazione pre caricamento:
* -Assicurarsi che la scheda SD sia presente
*
* Seguire la procedura di esportazione per il caricamento
*/

#ifdef ONBOARD

#include <Action.h>
#include <Condition.h>
#include <ConnectionParameters.h>
#include <Connector.h>
#include <ConstantCondition.h>
#include <ConstantOutputAction.h>
#include <CoreLibraryTestCases.h>
#include <DataManager.h>
#include <DateTime.h>
#include <DeviceManager.h>
#include <DirectFileReader.h>
#include <DirectFileWriter.h>
#include <FakeOutput.h>
#include <FakeSensor.h>
#include <FakeStream.h>
#include <FileLock.h>
#include <FileReader.h>
#include <FileWriter.h>
#include <GlobalDefines.h>
#include <INetDescriptor.h>
#include <INetLibraryTestCases.h>
#include <INetMappedValues.h>
#include <INetOutputRequest.h>
#include <INetPacket.h>
#include <INetSensorValues.h>
#include <MemoryManager.h>
#include <NetworkDevice.h>
#include <NetworkDeviceStatus.h>
#include <NetworkReader.h>
#include <NetworkResources.h>
#include <NetworkWriter.h>
#include <Output.h>
#include <RequestHandler.h>
#include <Rule.h>
#include <SecureFileReader.h>
#include <SecureFileWriter.h>
#include <Sensor.h>
#include <Settings.h>
#include <StreamReader.h>
#include <StreamWriter.h>
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

#include <CoreLibraryTestCases.h>

#include <Stream.h>
#include <Arduino.h>
#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>

void setup() {
  pinMode(53, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Serial.begin(9600);
  Serial.println("Initializing");

  SD.begin(4);

}

void loop() {
  // put your main code here, to run repeatedly:
   Serial.println("Starting tests");
   executeAllCoreLibraryTests();
   executeAllINetLibraryTests();
   Serial.println("All done");
}

#endif //ONBOARD
