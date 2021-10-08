/*
 * need to find away to clear string
 * need to make my own wifi manager with html page to take the id & p.w too
 * need to make authentication wich create splitted part in firebase
 * */
#include "Chikens_init.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
FirebaseData firebaseData;
WiFiManager  wifiManager;


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
  parameterSetup();
}

void loop()
{
  delay(1000);
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
  //serialPrints();
}





