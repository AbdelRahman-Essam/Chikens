#include "Chikens_init.h"

WiFiServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
FirebaseData firebaseData;
//FirebaseConfig config;
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
  currentmillis = millis();
//  int Stime = currentmillis;
//  Serial.print("Start Time "); Serial.println(Stime);
  WDT_Feed();
  if (((EEPROMReadByte(506)%11) < 10)||(esp_reset_reason() != ESP_RST_TASK_WDT))
  {
   firebaseErrorDetect();
  }
  ResetReason_Update();
  
//  if (currentmillis > 0x05265C00 ) //Reset every 24h //0xFFFFFFF0 overflow 49 days
//  {
//  ESP.restart();
//  }  

  if (App_mode==0)
  {
  tempFn();
  detectGas();
  LCD_setup();
  LCD_weather();
  LCD_Fast_Update();
  if (((EEPROMReadByte(506)%11) < 10)||(esp_reset_reason() != ESP_RST_TASK_WDT))
  {
    timeUpgrade();
    WiFiCheck();
  }
  else if ( currentmillis == safe_mode_prev + 600000 )
  {
    // wdt +1
    EEPROMWriteByte(506, (EEPROMReadByte(506)+1));
    // reset
    ESP.restart();
  }
  CreditionalsConfig();
  resetCheck();
  controlStatments();
  if (((EEPROMReadByte(506)%11) < 10)||(esp_reset_reason() != ESP_RST_TASK_WDT))
  {
    firebaseStatments();
    gooogleSheetStatments();
  }
  //RFID_Read(RFID_Success);
//  serialPrints();
  buttonCheck();
  }
  else 
  {
    SettingMode();
  }
//  int endtime = millis() - Stime;
//  Serial.print("End Time "); Serial.println(endtime);
}





