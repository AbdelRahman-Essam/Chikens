#include "Chikens_init.h"

WiFiServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
FirebaseData firebaseData;
//PN532_HSU pn532hsu(Serial2);
//PN532 nfc(pn532hsu);

void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n\n\nVoid Setup \n\n\n\n");
  pinSetup();
  LCD_setup();
  DHTSetup();
  EEPROMSetup();
  RestoreData();
  WifiSetup();
  firbaseSetup();
  timeServerSetup();
//  RFID_Success = RFID_Setup();
  parameterSetup();
  UpdateCheck();
}

void loop()
{
  firebaseErrorDetect();
  currentmillis = millis();
  if (App_mode==0)
  {
  tempFn();
  detectGas();
  LCD_setup();
  LCD_weather();
  timeUpgrade();
  WiFiCheck();
  CreditionalsConfig();
  resetCheck();
  controlStatments();
  firebaseStatments();
  gooogleSheetStatments();
  //RFID_Read(RFID_Success);
//  serialPrints();
  buttonCheck();
  }
  else 
  {
    SettingMode();
  }
}





