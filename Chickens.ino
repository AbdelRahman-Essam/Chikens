#include "Chikens_init.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
FirebaseData firebaseData;
WiFiManager  wifiManager;
PN532_HSU pn532hsu(Serial2);
PN532 nfc(pn532hsu);

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\nVoid Setup\n\n\n\n");
  /////////////////////////////////////////////////////////////////////

  //wifiManager.resetSettings();

  ////////////////////////////////////////////////////////////////////
  pinSetup();
  DHTSetup();
  wifiSetup();
  EEPROMSetup();
  firbaseSetup();
  timeServerSetup();
  RFID_Success = RFID_Setup();
  parameterSetup();
}

void loop()
{
  //delay(1000);
  Serial.println("\n\n\n\nVoid Loop\n\n\n\n");
  firebaseErrorDetect();
  resetCheck();
  tempFn();
  detectGas();
  controlStatments();
  firebaseStatments();
  gooogleSheetStatments();
  timeUpgrade();
  errorStatments();
  RFID_Read(RFID_Success);
  //serialPrints();
}





