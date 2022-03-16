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
  Serial.println("\n\n\n\nVoid Setup\n\n\n\n");
  pinSetup();
  DHTSetup();
  Wire.begin(SDA0_Pin, SCL0_Pin);
  EEPROMSetup();
  WifiSetup();
  firbaseSetup();
  timeServerSetup();
//  RFID_Success = RFID_Setup();
  parameterSetup();
  UpdateCheck();
  LCD_setup();
}

void loop()
{
  currentmillis = millis();
  timeUpgrade();
  WiFiCheck();
  CreditionalsConfig();
  resetCheck();
  tempFn();
  detectGas();
  controlStatments();
  firebaseStatments();
  gooogleSheetStatments();
  //RFID_Read(RFID_Success);
  LCD_weather();
  //serialPrints();
}





