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
  WDT_Init();
  pinSetup();
  LCD_StartUp();
  DHTSetup();
  EEPROMSetup();
  First_Use();
  RestoreData();
  ResetReason();
  WDT_Feed();
  parameterSetup();
  if (((EEPROMReadByte(506)%11) < 10)||(esp_reset_reason() != ESP_RST_TASK_WDT))
  {
    WifiSetup();
    WDT_Feed();
    firbaseSetup();
    WDT_Feed();
    timeServerSetup();
    UpdateCheck();
  }
  else
  {
    safe_mode_prev = millis();    
  }
//  RFID_Success = RFID_Setup();
}

void loop()
{
  //  int Stime = currentmillis;
  //  Serial.print("Start Time "); Serial.println(Stime);
  WDT_Feed();
  ResetReason_Update();
  if (App_mode==0)
  {
  tempFn();
  detectGas();
  LCD_setup();
  LCD_weather();
  LCD_Fast_Update();
  CreditionalsConfig();
  resetCheck();
  controlStatments();

  if (((EEPROMReadByte(506)%11) < 10)||(esp_reset_reason() != ESP_RST_TASK_WDT))
  {
    firebaseErrorDetect();
    timeUpgrade();
    WiFiCheck();
    firebaseStatments();
    //gooogleSheetStatments();
  }
  else if ( currentmillis >= safe_mode_prev + 300000 )
  {
    // wdt +1
    EEPROMWriteByte(506, (EEPROMReadByte(506)+1));
    // reset
    ESP.restart();
  }
  buttonCheck();
  //RFID_Read(RFID_Success);
//  serialPrints();
  }
  else 
  {
    SettingMode();
  }
//  int endtime = millis() - Stime;
//  Serial.print("End Time "); Serial.println(endtime);
}





