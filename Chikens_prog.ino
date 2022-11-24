#include "Chikens_init.h"

//////////////////////////////////////////////// WDT Module ////////////////////////////////////////////////////////////////
void WDT_Init(void)
{
  Serial.println("Configuring WDT...");
  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch
}
void WDT_Feed(void)
{
  if (WDT_Action =="Stuck")
  {
    Serial.println("Will stuck");
    while(1);
  }
  else if (WDT_Action =="Disable")
  {
    WDT_Disable();
  }
  else if (WDT_Action =="Disable_Feed")
  {
    WDT_Disable();
    esp_task_wdt_reset();
  }
  else if (WDT_Action =="Enable")
  {
    WDT_Enable();
  }
  else if (WDT_Action =="Enable_Feed")
  {
    WDT_Enable();
    esp_task_wdt_reset();
  }
  else if (WDT_Action =="NO_Feed")
  {

  }
  else
  {
    esp_task_wdt_reset();
  }
}
void WDT_Disable(void)
{
  esp_task_wdt_init(WDT_TIMEOUT, false);
}
void WDT_Enable(void)
{
  esp_task_wdt_reset();
  esp_task_wdt_init(WDT_TIMEOUT, true);
  esp_task_wdt_reset();
}
/////////////////////////////////////// Reset Module ///////////////////////////////////////////////////
void ResetReason(void)
{
  esp_reset_reason_t reason = esp_reset_reason();
  Serial.print("Last Reset/Boot Reason was: "); Serial.println( reason );
  switch (reason) {
    case ESP_RST_UNKNOWN:
      Serial.println("Reset reason can not be determined");
      EEPROMWriteByte(500, (EEPROMReadByte(500)+1));
      break;

    case ESP_RST_POWERON:
      Serial.println("Reset due to power-on event");
      EEPROMWriteByte(501, (EEPROMReadByte(501)+1));
      break;

    case ESP_RST_EXT:
      Serial.println("Reset by external pin (not applicable for ESP32)");
      EEPROMWriteByte(502, (EEPROMReadByte(502)+1));
      break;

    case ESP_RST_SW:
      Serial.println("Software reset via esp_restart");
      EEPROMWriteByte(503, (EEPROMReadByte(503)+1));
      break;

    case ESP_RST_PANIC:
      Serial.println("Software reset due to exception/panic");
      EEPROMWriteByte(504, (EEPROMReadByte(504)+1));
      break;

    case ESP_RST_INT_WDT:
      Serial.println("Reset (software or hardware) due to interrupt watchdog");
      EEPROMWriteByte(505, (EEPROMReadByte(505)+1));
      break;

    case ESP_RST_TASK_WDT:
      Serial.println("Reset due to task watchdog");
      EEPROMWriteByte(506, (EEPROMReadByte(506)+1));
      break;

    case ESP_RST_WDT:
      Serial.println("Reset due to other watchdogs");
      EEPROMWriteByte(507, (EEPROMReadByte(507)+1));
      break;

    case ESP_RST_DEEPSLEEP:
      Serial.println("Reset after exiting deep sleep mode");
      EEPROMWriteByte(508, (EEPROMReadByte(508)+1));
      break;

    case ESP_RST_BROWNOUT:
      Serial.println("Brownout reset (software or hardware)");
      EEPROMWriteByte(509, (EEPROMReadByte(509)+1));
      break;

    case ESP_RST_SDIO:
      Serial.println("Reset over SDIO");
      EEPROMWriteByte(510, (EEPROMReadByte(510)+1));
      break;

    default:
      break;
  }

}
void SendResetReason(void)
{
  esp_reset_reason_t reason = esp_reset_reason();
  
  switch (reason) {
    case ESP_RST_UNKNOWN:
      Firebase.setString(firebaseData, username + "/Status/Last_Reset", "ESP_RST_UNKNOWN");
      break;

    case ESP_RST_POWERON:
      Firebase.setString(firebaseData, username + "/Status/Last_Reset", "ESP_RST_POWERON");
      break;

    case ESP_RST_EXT:
      Firebase.setString(firebaseData, username + "/Status/Last_Reset", "ESP_RST_EXT");
      break;

    case ESP_RST_SW:
      Serial.println("Software reset via esp_restart");
      Firebase.setString(firebaseData, username + "/Status/Last_Reset", "ESP_RST_SW");
      break;

    case ESP_RST_PANIC:
      Firebase.setString(firebaseData, username + "/Status/Last_Reset", "ESP_RST_PANIC");
      break;

    case ESP_RST_INT_WDT:
      Firebase.setString(firebaseData, username + "/Status/Last_Reset", "ESP_RST_INT_WDT");
      break;

    case ESP_RST_TASK_WDT:
      Firebase.setString(firebaseData, username + "/Status/Last_Reset", "ESP_RST_TASK_WDT");
      break;

    case ESP_RST_WDT:
      Firebase.setString(firebaseData, username + "/Status/Last_Reset", "ESP_RST_WDT");
      break;

    case ESP_RST_DEEPSLEEP:
      Firebase.setString(firebaseData, username + "/Status/Last_Reset", "ESP_RST_DEEPSLEEP");
      break;

    case ESP_RST_BROWNOUT:
      Firebase.setString(firebaseData, username + "/Status/Last_Reset", "ESP_RST_BROWNOUT");
      break;

    case ESP_RST_SDIO:
      Firebase.setString(firebaseData, username + "/Status/Last_Reset", "ESP_RST_SDIO");
      break;

    default:
      break;
  }
  Firebase.setInt(firebaseData, username + "/Status/ESP_RST_UNKNOWN", EEPROMReadByte(500));
  Firebase.setInt(firebaseData, username + "/Status/ESP_RST_POWERON", EEPROMReadByte(501));
  Firebase.setInt(firebaseData, username + "/Status/ESP_RST_EXT",     EEPROMReadByte(502));
  Firebase.setInt(firebaseData, username + "/Status/ESP_RST_SW",      EEPROMReadByte(503));
  Firebase.setInt(firebaseData, username + "/Status/ESP_RST_PANIC",   EEPROMReadByte(504));
  Firebase.setInt(firebaseData, username + "/Status/ESP_RST_INT_WDT", EEPROMReadByte(505));
  Firebase.setInt(firebaseData, username + "/Status/ESP_RST_TASK_WDT",EEPROMReadByte(506));
  Firebase.setInt(firebaseData, username + "/Status/ESP_RST_WDT",     EEPROMReadByte(507));
  Firebase.setInt(firebaseData, username + "/Status/ESP_RST_DEEPSLEEP",EEPROMReadByte(508));
  Firebase.setInt(firebaseData, username + "/Status/ESP_RST_BROWNOUT",EEPROMReadByte(509));
  Firebase.setInt(firebaseData, username + "/Status/ESP_RST_SDIO",    EEPROMReadByte(510));
}
void ClearResetReason(void)
{
  EEPROMWriteByte(500, 0);
  EEPROMWriteByte(501, 0);
  EEPROMWriteByte(502, 0);
  EEPROMWriteByte(503, 0);
  EEPROMWriteByte(504, 0);
  EEPROMWriteByte(505, 0);
  EEPROMWriteByte(506, 0);
  EEPROMWriteByte(507, 0);
  EEPROMWriteByte(508, 0);
  EEPROMWriteByte(509, 0);
  EEPROMWriteByte(510, 0);
  EEPROMWriteByte(511, 0);
}
void ResetReason_Update(void)
{
  static uint8_t Sent =0;
  if (Sent == 0)
  {
    if (firebaseErrorDetectNoPrint() > 0)
    {
      SendResetReason();
      Sent = 1;
    }
  }
  if (Clear_Reset_Reason == "ON")
  {
    ClearResetReason();
    SendResetReason();
    Firebase.setString(firebaseData, username + "/Status/Clear_Reset_Reason", "OFF");
    Clear_Reset_Reason = "OFF";
  }
  
}
/////////////////////////////////////// WiFi Module ///////////////////////////////////////////////////
void WiFiCheck(void)
{
  static unsigned long long int WiFiCheck_previousMillis = 0;
  static unsigned long long int WiFi_Reconnect_previousMillis = 0;

  if ((currentmillis - WiFiCheck_previousMillis >= WiFiCheck_interval) || (currentmillis < WiFiCheck_previousMillis))
  {
    //Serial.println("will start WiFiCheck");
    WiFiCheck_previousMillis = currentmillis;
    if ((Error <= 0))
    {
      LED_Status = ! LED_Status;
      digitalWrite(BlueLed, LED_Status);
    }
    if (((currentmillis - WiFi_Reconnect_previousMillis >= WiFi_Reconnect_interval) || (currentmillis < WiFi_Reconnect_previousMillis)) && (Error < 0))
    {
      WDT_Feed();
      WiFi_Reconnect_previousMillis = currentmillis;
      Serial.println("Inside WiFiReconnect");
      Serial.print(millis());
      Serial.println("Reconnecting to WiFi...");
      WiFi.disconnect();
      WiFi.reconnect();
    }
    //Serial.println("will stop WiFiCheck");
  }
}
void CreditionalsConfig(void)
{
  static unsigned long long int CreditionalsConfig_previousMillis = 0;
  if ((currentmillis - CreditionalsConfig_previousMillis >= CreditionalsConfig_interval) || (currentmillis < CreditionalsConfig_previousMillis))
  {
    //Serial.println("will start cred config");
    CreditionalsConfig_previousMillis = currentmillis;
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW)
    {
      delay(2000);
      WDT_Feed();
      buttonState = digitalRead(buttonPin);
      if (buttonState == LOW)
      {
        WDT_Feed();
        delay(2000);
        buttonState = digitalRead(buttonPin);
        if (buttonState == LOW)
        {
          digitalWrite(BlueLed, 0);
          delay(500);
          digitalWrite(BlueLed, 1);
          delay(500);
          digitalWrite(BlueLed, 0);
          delay(500);
          digitalWrite(BlueLed, LED_Status);
          WDT_Feed();
          set_access_point();
          while (1)
          {
            WDT_Feed();
            client_handle();
            delay(1000);
            if (ConfigResetFlag)
              ESP.restart();
          }
        }
      }
    }
    //Serial.println("will stop cred config");
  }
}
void firebaseStatments(void)
{
  static uint32_t firebase_previousMillis = 0;
  static uint32_t Case = 0;
  if (((currentmillis - firebase_previousMillis) >= (firebase_interval * 60000)) && (Error > 0))
  {
//    Serial.println("will start firebase");
//    int Stime = millis();
//    Serial.print("Start Time "); Serial.println(Stime);
    Serial.print("Case "); Serial.println(Case);
    /***************************************/
    switch (Case) {
      case 0:{//First_FirebaseUse
        First_FirebaseUse();
        Case++;
      break;}
      case 1:{//timeClient
        ///////////////////////////// Getters ////////////////////////////
        timeClient.update();
        Firebase.setInt(firebaseData, username + "/Time/Hour", timeClient.getHours());
        Firebase.setInt(firebaseData, username + "/Time/Minute", timeClient.getMinutes());
        Firebase.setInt(firebaseData, username + "/Time/Seconds", timeClient.getSeconds());
        Case++;
      break;}
      case 2:{//temp1,temp2
        if (temp_prev1 != Temperature1)
        {
          if (Temperature1 <= 1)
          {
//            Firebase.setFloat(firebaseData,  username + "/Temp/T1", 0);
            Firebase.deleteNode(firebaseData,  username + "/Temp/T1");
            temp_prev1 = Temperature1;
          }
          else
          {
            Serial.print("temp1: "); Serial.println(Temperature1);
            temp_prev1 = Temperature1;
            Firebase.setFloat(firebaseData, username + "/Temp/T1", Temperature1);
          }
        }
        if (temp_prev2 != Temperature2)
        {
          if (Temperature2 <= 1)
          {
//            Firebase.setFloat(firebaseData,  username + "/Temp/T2", 0);
            Firebase.deleteNode(firebaseData,  username + "/Temp/T2");
            temp_prev2 = Temperature2;
          }
          else
          {
            Serial.print("temp2: "); Serial.println(Temperature2);
            temp_prev2 = Temperature2;
            Firebase.setFloat(firebaseData, username + "/Temp/T2", Temperature2);
          }
        }
        Case++;
      break;}
      case 3:{//temp3
        if (temp_prev3 != Temperature3)
        {
          if (Temperature3 <= 1)
          {
            //Firebase.setFloat(firebaseData,  username + "/Temp/T3", 0);
            Firebase.deleteNode(firebaseData,  username + "/Temp/T3");
            temp_prev3 = Temperature3;
          }
          else
          {
            Serial.print("temp3: "); Serial.println(Temperature3);
            temp_prev2 = Temperature2;
            Firebase.setFloat(firebaseData, username + "/Temp/T3", Temperature3);
          }
        }
        Case++;
      break;}
      case 4:{//temp4
        if (temp_prev4 != Temperature4)
        {
          if (Temperature4 <= 1)
          {
//            Firebase.setFloat(firebaseData,  username + "/Temp/T4",0);
            Firebase.deleteNode(firebaseData,  username + "/Temp/T4");
            temp_prev4 = Temperature4;
          }
          else
          {
            Serial.print("temp4: "); Serial.println(Temperature4);
            temp_prev4 = Temperature4;
            Firebase.setFloat(firebaseData, username + "/Temp/T4", Temperature4);
          }
        }
        Case++;
      break;}
      case 5:{//temp5,temp @ failure
        if (temp_prev5 != Temperature5)
        {
          if (Temperature5 <= 1)
          {
//            Firebase.setFloat(firebaseData,  username + "/Temp/T5", 0);
            Firebase.deleteNode(firebaseData,  username + "/Temp/T5");
            temp_prev5 = Temperature5;
          }
          else
          {
            Serial.print("temp5: "); Serial.println(Temperature5);
            temp_prev5 = Temperature5;
            Firebase.setFloat(firebaseData, username + "/Temp/T5", Temperature5);
          }
        }
        if (Temperature>75)
        {
          Serial.print("Temperature: "); Serial.println(Temperature);
          Firebase.setFloat(firebaseData, username + "/Temp/T", Temperature);
        }
        Case++;
      break;}
      case 6:{//Hum1
        if (hum_prev1 != Humidity1)
        {
          if (Humidity1 <= 1)
          {
//            Firebase.setFloat(firebaseData,  username + "/Hum/H1", 0);
            Firebase.deleteNode(firebaseData,  username + "/Hum/H1");
            hum_prev1 = Humidity1;
          }
          else
          {
            Serial.print("Humidity1: "); Serial.println(Humidity1);
            hum_prev1 = Humidity1;
            Firebase.setFloat(firebaseData, username + "/Hum/H1", Humidity1);
          }
        }
        Case++;
      break;}
      case 7:{//Hum2
        if (hum_prev2 != Humidity2)
        {
          if (Humidity2 <= 1)
          {
//            Firebase.setFloat(firebaseData,  username + "/Hum/H2", 0);
            Firebase.deleteNode(firebaseData,  username + "/Hum/H2");
            hum_prev2 = Humidity2;
          }
          else
          {
            Serial.print("Humidity2: "); Serial.println(Humidity2);
            hum_prev2 = Humidity2;
            Firebase.setFloat(firebaseData, username + "/Hum/H2", Humidity2);
          }
        }
        Case++;
      break;}
      case 8:{//Hum3
        if (hum_prev3 != Humidity3)
        {
          if (Humidity3 <= 1)
          {
//            Firebase.setFloat(firebaseData,  username + "/Hum/H3", 0);
            Firebase.deleteNode(firebaseData,  username + "/Hum/H3");
            hum_prev3 = Humidity3;
          }
          else
          {
            Serial.print("Humidity3: "); Serial.println(Humidity3);
            hum_prev3 = Humidity3;
            Firebase.setFloat(firebaseData, username + "/Hum/H3", Humidity3);
          }
        }
        Case++;
      break;}
      case 9:{//Hum4
        if (hum_prev4 != Humidity4)
        {
          if (Humidity4 <= 1)
          {
//            Firebase.setFloat(firebaseData,  username + "/Hum/H4", 0);
            Firebase.deleteNode(firebaseData,  username + "/Hum/H4");
            hum_prev4 = Humidity4;
          }
          else
          {
            Serial.print("Humidity4: "); Serial.println(Humidity4);
            hum_prev4 = Humidity4;
            Firebase.setFloat(firebaseData, username + "/Hum/H4", Humidity4);
          }
        }
        Case++;
      break;}
      case 10:{//Hum5
        if (hum_prev5 != Humidity5)
        {
          if (Humidity5 <= 1)
          {
//            Firebase.setFloat(firebaseData,  username + "/Hum/hum5", 0);
            Firebase.deleteNode(firebaseData,  username + "/Hum/H5");
            hum_prev5 = Humidity5;
          }
          else
          {
            Serial.print("Humidity5: "); Serial.println(Humidity5);
            hum_prev5 = Humidity5;
            Firebase.setFloat(firebaseData, username + "/Hum/H5", Humidity5);
          }
        }
        Case++;
      break;}
      case 11:{//Empty
//        if ((gas_prev != gas) && (gas > 3))
//        {
//          // the String will be produced in the android program
//          gas_prev = gas;
//          Serial.print("gas: "); Serial.println(gas);
//          Firebase.setInt(firebaseData, username + "/airQuality", gas);
//        }
        Case++;
      break;}
      case 12:{//AQ1
        if ((gas1_prev != gas1) && (gas1 > 3))
        {
          // the String will be produced in the android program
          gas1_prev = gas1;
          Serial.print("gas1: "); Serial.println(gas1);
          Firebase.setInt(firebaseData, username + "/airQuality/airQuality1", gas1);
        }
        Case++;
      break;}
      case 13:{//AQ2
        if ((gas2_prev != gas2) && (gas2 > 3))
        {
          // the String will be produced in the android program
          gas2_prev = gas2;
          Serial.print("gas2: "); Serial.println(gas2);
          Firebase.setInt(firebaseData, username + "/airQuality/airQuality2", gas2);
        }
        Case++;
      break;}
      case 14:{//heaterA Status
        if (heaterA_status_prev != heaterA_status)
        {
          heaterA_status_prev = heaterA_status;
          Serial.print("heaterA_status: "); Serial.println(heaterA_status);
          Firebase.setString(firebaseData, username + "/states/Heater_1", heaterA_status);
        }
        Case++;
      break;}
      case 15:{//heaterB Status
        if (heaterB_status_prev != heaterB_status)
        {
          heaterB_status_prev = heaterB_status;
          Serial.print("heaterB_status: "); Serial.println(heaterB_status);
          Firebase.setString(firebaseData, username + "/states/Heater_2", heaterB_status);
        }
        Case++;
      break;}
      case 16:{//fanA Status
        if (fanA_status_prev != fanA_status)
        {
          fanA_status_prev = fanA_status;
          Serial.print("fanA_status: "); Serial.println(fanA_status);
          Firebase.setString(firebaseData, username + "/states/Fan_1", fanA_status);
        }
        Case++;
      break;}
      case 17:{//fanB Status
        if (fanB_status_prev != fanB_status)
        {
          fanB_status_prev = fanB_status;
          Serial.print("fanB_status: "); Serial.println(fanB_status);
          Firebase.setString(firebaseData, username + "/states/Fan_2", fanB_status);
        }
        Case++;
      break;}
      case 18:{//Cooler Status
        if (cooler_status_prev != cooler_status)
        {
          cooler_status_prev = cooler_status;
          Serial.print("cooler_status: "); Serial.println(cooler_status);
          Firebase.setString(firebaseData, username + "/states/Cooler_1", cooler_status);
        }
        Case++;
      break;}
      case 19:{//Light Status
        if (Light_prev != Light_Status)
        {
          Light_prev = Light_Status;
          Serial.print("Light_Status: "); Serial.println(Light_Status);
          Firebase.setString(firebaseData, username + "/states/Light_1", Light_Status);
        }
        Case++;
      break;}
      case 20:{//Gas1_Cal
        ///////////////////////////// Setters ////////////////////////////
        firebaseData.clearData();/////////////////////////////////////////////
        string.clear();
        Firebase.get(firebaseData, username + "/config/Gas1_Cal");
        string = firebaseData.stringData();
        if (Gas1_Sen_Cal_Prev != string)
        {
          Gas1_Sen_Cal_Prev = string;
          Gas1_Sen_Cal = Gas1_Sen_Cal_Prev.toInt();
          Serial.println("Gas1_Sen_Cal Changed!");
          Serial.print("Gas1_Sen_Cal: "); Serial.println(Gas1_Sen_Cal);
          if ((EEPROMReadInt(365) != Gas1_Sen_Cal))
          {
            EEPROMWriteInt(365, Gas1_Sen_Cal);
            Serial.print("Gas1_Sen_Cal :"); Serial.println(Gas1_Sen_Cal);
          }
          Firebase.setInt(firebaseData, username + "/config_get/Gas1_Cal", Gas1_Sen_Cal);
        }
        Case++;
      break;}
      case 21:{//Gas2_Cal
        firebaseData.clearData();/////////////////////////////////////////////
        string.clear();
        Firebase.get(firebaseData, username + "/config/Gas2_Cal");
        string = firebaseData.stringData();
        if (Gas2_Sen_Cal_Prev != string)
        {
          Gas2_Sen_Cal_Prev = string;
          Gas2_Sen_Cal = Gas2_Sen_Cal_Prev.toInt();
          Serial.println("Gas2_Sen_Cal Changed!");
          Serial.print("Gas2_Sen_Cal: "); Serial.println(Gas2_Sen_Cal);
          if ((EEPROMReadInt(369) != Gas2_Sen_Cal))
          {
            EEPROMWriteInt(369, Gas2_Sen_Cal);
            Serial.print("Gas2_Sen_Cal :"); Serial.println(Gas2_Sen_Cal);
          }
          Firebase.setInt(firebaseData, username + "/config_get/Gas2_Cal", Gas2_Sen_Cal);
        }
        Case++;
      break;}
      case 22:{//WDT_Action
          firebaseData.clearData();/////////////////////////////////////////////
          string.clear();
          Firebase.get(firebaseData, username + "/Status/WDT_Action");
          string = firebaseData.stringData();
          if (WDT_Action_prev != string)
          {
          WDT_Action_prev = string;
          WDT_Action = WDT_Action_prev;
          Serial.println("WDT_Action Changed!");
          Serial.print("WDT_Action: "); Serial.println(WDT_Action);
          }
          Case++;
      break;}
      case 23:{//Clear_Reset_Reason
        firebaseData.clearData();/////////////////////////////////////////////
        string.clear();
        Firebase.get(firebaseData, username + "/Status/Clear_Reset_Reason");
        string = firebaseData.stringData();
        if (Clear_Reset_Reason_prev != string)
        {
          Clear_Reset_Reason_prev = string;
          Clear_Reset_Reason = Clear_Reset_Reason_prev;
          Serial.println("Clear_Reset_Reason Changed!");
          Serial.print("Clear_Reset_Reason: "); Serial.println(Clear_Reset_Reason);
        }
        Case++;
      break;}
      case 24:{//Empty
        //    firebaseData.clearData();/////////////////////////////////////////////
        //    string.clear();
        //    Firebase.get(firebaseData, username + "/Gas_Cal/Gas2");
        //    string = firebaseData.stringData();
        //    if (Gas2_Sen_Cal_Prev != string)
        //    {
        //      Gas2_Sen_Cal_Prev = string;
        //      Gas2_Sen_Cal = Gas2_Sen_Cal_Prev.toInt();
        //      Serial.println("Gas2_Sen_Cal Changed!");
        //      Serial.print("Gas2_Sen_Cal: "); Serial.println(Gas2_Sen_Cal);
        //      Firebase.setInt(firebaseData, username + "/Gas_Cal/Gas2_get", Gas2_Sen_Cal);
        //    }
        Case++;
      break;}
      case 25:{//Fan_1_Auto
        firebaseData.clearData();/////////////////////////////////////////////
        string.clear();
        Firebase.get(firebaseData, username + "/states/Fan_1_Auto");
        string = firebaseData.stringData();
        if ((FanA_Auto_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("FanA_Auto Changed!");
          FanA_Auto_prev = string;
          FanA_Auto = FanA_Auto_prev;
          Serial.print("FanA_Auto: "); Serial.println(FanA_Auto);
          Firebase.setString(firebaseData, username + "/controls_get/Fan_1_Auto",FanA_Auto);
        }
        Case++;
      break;}
      case 26:{//Fan_2_Auto
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/states/Fan_2_Auto");
        string = firebaseData.stringData();
        if ((FanB_Auto_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("FanB_Auto Changed!");
          FanB_Auto_prev = string;
          FanB_Auto = FanB_Auto_prev;
          Serial.print("FanB_Auto: "); Serial.println(FanB_Auto);
          Firebase.setString(firebaseData, username + "/controls_get/Fan_2_Auto",FanB_Auto);
        }
        Case++;
      break;}
      case 27:{//Cooler_1_Auto
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/states/Cooler_1_Auto");
        string = firebaseData.stringData();
        if ((Cooler_Auto_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("Cooler_Auto Changed!");
          Cooler_Auto_prev = string;
          Cooler_Auto = Cooler_Auto_prev;
          Serial.print("Cooler_Auto: "); Serial.println(Cooler_Auto);
          Firebase.setString(firebaseData, username + "/controls_get/Cooler_1_Auto",Cooler_Auto);
        }
        Case++;
      break;}
      case 28:{//Heater_1_Auto
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/states/Heater_1_Auto");
        string = firebaseData.stringData();
        if ((heaterA_Auto_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("heaterA_Auto Changed!");
          heaterA_Auto_prev = string;
          heaterA_Auto = heaterA_Auto_prev;
          Serial.print("heaterA_Auto: "); Serial.println(heaterA_Auto);
          Firebase.setString(firebaseData, username + "/controls_get/Heater_1_Auto",heaterA_Auto);
        }
        Case++;
      break;}
      case 29:{//Heater_2_Auto
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/states/Heater_2_Auto");
        string = firebaseData.stringData();
        if ((heaterB_Auto_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("heaterBAuto Changed!");
          heaterB_Auto_prev = string;
          heaterB_Auto = heaterB_Auto_prev;
          Serial.print("heaterB_Auto: "); Serial.println(heaterB_Auto);
          Firebase.setString(firebaseData, username + "/controls_get/Heater_2_Auto", heaterB_Auto);
        }
        Case++;
      break;}
      case 30:{//Heater_1
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/controls/Heater_1");
        string = firebaseData.stringData();
        if ((Set_ManualHA_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("ManualHA Changed!");
          Set_ManualHA_prev = string;
          Set_ManualHA = Set_ManualHA_prev;
          Serial.print("Set_ManualHA: "); Serial.println(Set_ManualHA);
          Firebase.setString(firebaseData, username + "/controls_get/Heater_1", Set_ManualHA);
        }
        Case++;
      break;}
      case 31:{//Heater_2
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/controls/Heater_2");
        string = firebaseData.stringData();
        if ((Set_ManualHB_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("ManualHB Changed!");
          Set_ManualHB_prev = string;
          Set_ManualHB = Set_ManualHB_prev;
          Serial.print("Set_ManualHB: "); Serial.println(Set_ManualHB);
          Firebase.setString(firebaseData, username + "/controls_get/Heater_2", Set_ManualHB);
        }
        Case++;
      break;}
      case 32:{//Cooler_1
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/controls/Cooler_1");
        string = firebaseData.stringData();
        if ((Set_ManualC_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("ManualC Changed!");
          Set_ManualC_prev = string;
          Set_ManualC = Set_ManualC_prev;
          Serial.print("Set_ManualC: "); Serial.println(Set_ManualC);
          Firebase.setString(firebaseData, username + "/controls_get/Cooler_1", Set_ManualC);
        }
        Case++;
      break;}
      case 33:{//Fan_1
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/controls/Fan_1");
        string = firebaseData.stringData();
        if ((Set_ManualFA_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("ManualFA Changed!");
          Set_ManualFA_prev = string;
          Set_ManualFA = Set_ManualFA_prev;
          Serial.print("Set_ManualFA: "); Serial.println(Set_ManualFA);
          Firebase.setString(firebaseData, username + "/controls_get/Fan_1", Set_ManualFA);
        }
        Case++;
      break;}
      case 34:{//Fan_2
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/controls/Fan_2");
        string = firebaseData.stringData();
        if ((Set_ManualFB_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("ManualFB Changed!");
          Set_ManualFB_prev = string;
          Set_ManualFB = Set_ManualFB_prev;
          Serial.print("Set_ManualFB: "); Serial.println(Set_ManualFB);
          Firebase.setString(firebaseData, username + "/controls_get/Fan_2", Set_ManualFB);
        }
        Case++;
      break;}
      case 35:{//resetFlag
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/resetFlag");
        string = firebaseData.stringData();
        if ((ResetFlag_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("ResetFlag Changed!");
          ResetFlag_prev = string;
          ResetFlag = ResetFlag_prev;
        }
        Case++;
      break;}
      case 36:{//Led_1
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/controls/Led_1");
        string = firebaseData.stringData();
        if ((LED_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("BlueLed Changed!");
          LED_prev = string;
          LED_Status_Str = LED_prev;
          Firebase.setString(firebaseData, username + "/controls_get/Led_1", LED_Status_Str);
          Firebase.setString(firebaseData, username + "/states/Led_1", LED_Status_Str);
          if (LED_Status_Str == "ON")
            LED_Status = 1;
          else if (LED_Status_Str == "OFF")
            LED_Status = 0;
          digitalWrite(BlueLed, LED_Status);
        }
        Case++;
      break;}
      case 37:{//Light_1
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/controls/Light_1");
        string = firebaseData.stringData();
        if ((Light_prev != string) && (string == "ON" || string == "OFF"))
        {
          Serial.println("Light Changed!");
          Light_prev = string;
          Light_Status = Light_prev;
          Serial.print("Light_Status: "); Serial.println(Light_Status);
          if (Light_Status == "OFF")
            Light_Status_Char = 0;
          else if (Light_Status == "ON")
            Light_Status_Char = 1;
          Firebase.setString(firebaseData, username + "/controls_get/Light_1", Light_Status);
        }
        Case++;
      break;}
      case 38:{//vent
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/valueRanges/vent");
        string = firebaseData.stringData();
        if ((VentRange_prev != string))
        {
          VentRange_prev = string;
          String sbstring = VentRange_prev.substring(0, 3);
          MinVent_Trigger = sbstring.toInt();
          if (((MinVent_Trigger >= 5) && (MinVent_Trigger <= 1000)) && (EEPROMReadInt(300) != MinVent_Trigger))
          {
            EEPROMWriteInt(300, MinVent_Trigger);
            Serial.print("MinVent_Trigger :"); Serial.println(MinVent_Trigger);
          }
          else
            MinVent_Trigger = EEPROMReadInt(300);
          sbstring = "";
          sbstring = VentRange_prev.substring(4, 7);
          MaxVent_Trigger = sbstring.toInt();
          if (((MaxVent_Trigger >= 5) && (MaxVent_Trigger <= 1000)) && (EEPROMReadInt(310) != MaxVent_Trigger))
          {
            EEPROMWriteInt(310, MaxVent_Trigger);
            Serial.print("MaxVent_Trigger :"); Serial.println(MaxVent_Trigger);
          }
          else
            MaxVent_Trigger = EEPROMReadInt(310);
        }
        Case++;
      break;}
      case 39:{//temp
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/valueRanges/temp");
        string = firebaseData.stringData();
        if ((TempRange_prev != string))
        {
          TempRange_prev = string;
          String sbstring = TempRange_prev.substring(0, 3);
          MinTemp_Trigger = sbstring.toInt();
          if (((MinTemp_Trigger >= 5) && (MinTemp_Trigger <= 1000)) && (EEPROMReadInt(330) != MinTemp_Trigger))
          {
            EEPROMWriteInt(330, MinTemp_Trigger);
            Serial.print("MinTemp_Trigger :"); Serial.println(MinTemp_Trigger);
          }
          else
            MinTemp_Trigger = EEPROMReadInt(330);
          sbstring = "";
          sbstring = TempRange_prev.substring(3, 7);
          MaxTemp_Trigger = sbstring.toInt();
          if (((MaxTemp_Trigger >= 5) && (MaxTemp_Trigger <= 1000)) && (EEPROMReadInt(320) != MaxTemp_Trigger))
          {
            EEPROMWriteInt(320, MaxTemp_Trigger);
            Serial.print("MaxTemp_Trigger :"); Serial.println(MaxTemp_Trigger);
          }
          else
            MaxTemp_Trigger = EEPROMReadInt(320);
        }
        Case++;
      break;}
      case 40:{//delay
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/valueRanges/delay");
        string = firebaseData.stringData();
        if ((firebase_interval_prev != string))
        {
          firebase_interval_prev = string;
          firebase_interval = firebase_interval_prev.toInt();
          if (((firebase_interval_prev.toInt() >= 0) && (firebase_interval_prev.toInt() <= 250)))
          {
            firebase_interval = firebase_interval_prev.toInt();
            Serial.print("firebase_interval :"); Serial.println(firebase_interval);
          }
        }
        Case++;
      break;}
      case 41:{//Fan_min_interval
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Fan_min_interval");
        string = firebaseData.stringData();
        if (Fan_min_interval_prev != string)
        {
          Fan_min_interval_prev = string;
          Fan_min_interval = Fan_min_interval_prev.toInt();
          Serial.println("Fan_min_interval Changed!");
          Serial.print("Fan_min_interval: "); Serial.println(Fan_min_interval);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Fan_min_interval", Fan_min_interval);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 42:{//Cool_min_interval
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Cool_min_interval");
        string = firebaseData.stringData();
        if (Cool_min_interval_prev != string)
        {
          Cool_min_interval_prev = string;
          Cool_min_interval = Cool_min_interval_prev.toInt();
          Serial.println("Cool_min_interval Changed!");
          Serial.print("Cool_min_interval: "); Serial.println(Cool_min_interval);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Cool_min_interval", Cool_min_interval);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 43:{//Heat_min_interval
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Heat_min_interval");
        string = firebaseData.stringData();
        if (Heat_min_interval_prev != string)
        {
          Heat_min_interval_prev = string;
          Heat_min_interval = Heat_min_interval_prev.toInt();
          Serial.println("Heat_min_interval Changed!");
          Serial.print("Heat_min_interval: "); Serial.println(Heat_min_interval);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Heat_min_interval", Heat_min_interval);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 44:{//Fan_1_on_time
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Fan_1_on_time");
        string = firebaseData.stringData();
        if (Fan_1_on_time_prev != string)
        {
          Fan_1_on_time_prev = string;
          Fan_1_on_time = Fan_1_on_time_prev.toInt();
          Serial.println("Fan_1_on_time Changed!");
          Serial.print("Fan_1_on_time: "); Serial.println(Fan_1_on_time);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Fan_1_on_time", Fan_1_on_time);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 45:{//Fan_1_off_time
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Fan_1_off_time");
        string = firebaseData.stringData();
        if (Fan_1_off_time_prev != string)
        {
          Fan_1_off_time_prev = string;
          Fan_1_off_time = Fan_1_off_time_prev.toInt();
          Serial.println("Fan_1_off_time Changed!");
          Serial.print("Fan_1_off_time: "); Serial.println(Fan_1_off_time);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Fan_1_off_time", Fan_1_off_time);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 46:{//Fan_on_time
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Fan_on_time");
        string = firebaseData.stringData();
        if (Fan_on_time_prev != string)
        {
          Fan_on_time_prev = string;
          Fan_on_time = Fan_on_time_prev.toInt();
          Serial.println("Fan_on_time Changed!");
          Serial.print("Fan_on_time: "); Serial.println(Fan_on_time);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Fan_on_time", Fan_on_time);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 47:{//Fan_off_time
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Fan_off_time");
        string = firebaseData.stringData();
        if (Fan_off_time_prev != string)
        {
          Fan_off_time_prev = string;
          Fan_off_time = Fan_off_time_prev.toInt();
          Serial.println("Fan_off_time Changed!");
          Serial.print("Fan_off_time: "); Serial.println(Fan_off_time);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Fan_off_time", Fan_off_time);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 48:{//Heater_on_time
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Heater_on_time");
        string = firebaseData.stringData();
        if (Heater_on_time_prev != string)
        {
          Heater_on_time_prev = string;
          Heater_on_time = Heater_on_time_prev.toInt();
          Serial.println("Heater_on_time Changed!");
          Serial.print("Heater_on_time: "); Serial.println(Heater_on_time);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Heater_on_time", Heater_on_time);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 49:{//Heater_off_time
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Heater_off_time");
        string = firebaseData.stringData();
        if (Heater_off_time_prev != string)
        {
          Heater_off_time_prev = string;
          Heater_off_time = Heater_off_time_prev.toInt();
          Serial.println("Heater_off_time Changed!");
          Serial.print("Heater_off_time: "); Serial.println(Heater_off_time);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Heater_off_time", Heater_off_time);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 50:{//Cooler_on_time
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Cooler_on_time");
        string = firebaseData.stringData();
        if (Cooler_on_time_prev != string)
        {
          Cooler_on_time_prev = string;
          Cooler_on_time = Cooler_on_time_prev.toInt();
          Serial.println("Cooler_on_time Changed!");
          Serial.print("Cooler_on_time: "); Serial.println(Cooler_on_time);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Cooler_on_time", Cooler_on_time);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 51:{//Cooler_off_time
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Cooler_off_time");
        string = firebaseData.stringData();
        if (Cooler_off_time_prev != string)
        {
          Cooler_off_time_prev = string;
          Cooler_off_time = Cooler_off_time_prev.toInt();
          Serial.println("Cooler_off_time Changed!");
          Serial.print("Cooler_off_time: "); Serial.println(Cooler_off_time);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Cooler_off_time", Cooler_off_time);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 52:{//Temp_variance_Cool
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Temp_variance_Cool");
        string = firebaseData.stringData();
        if (Temp_variance_Cool_prev != string)
        {
          Temp_variance_Cool_prev = string;
          Temp_variance_Cool = Temp_variance_Cool_prev.toInt();
          Serial.println("Temp_variance_Cool Changed!");
          Serial.print("Temp_variance_Cool: "); Serial.println(Temp_variance_Cool);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Temp_variance_Cool", Temp_variance_Cool);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 53:{//Temp_variance_FanB
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Temp_variance_FanB");
        string = firebaseData.stringData();
        if (Temp_variance_FanB_prev != string)
        {
          Temp_variance_FanB_prev = string;
          Temp_variance_FanB = Temp_variance_FanB_prev.toInt();
          Serial.println("Temp_variance_FanB Changed!");
          Serial.print("Temp_variance_FanB: "); Serial.println(Temp_variance_FanB);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Temp_variance_FanB", Temp_variance_FanB);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 54:{//Temp_variance_FanA
        firebaseData.clearData();
        string.clear();
        Firebase.get(firebaseData, username + "/config/Temp_variance_FanA");
        string = firebaseData.stringData();
        if (Temp_variance_FanA_prev != string)
        {
          Temp_variance_FanA_prev = string;
          Temp_variance_FanA = Temp_variance_FanA_prev.toInt();
          Serial.println("Temp_variance_FanA Changed!");
          Serial.print("Temp_variance_FanA: "); Serial.println(Temp_variance_FanA);
          StoreData();
          Firebase.setInt(firebaseData, username + "/config_get/Temp_variance_FanA", Temp_variance_FanA);
        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 55:{//Empty
//        firebaseData.clearData();
//        string.clear();
//        Firebase.get(firebaseData, username + "/config/Fan_min_interval");
//        string = firebaseData.stringData();
//        if (Fan_min_interval_prev != string)
//        {
//          Fan_min_interval_prev = string;
//          Fan_min_interval = Fan_min_interval_prev.toInt();
//          Serial.println("Fan_min_interval Changed!");
//          Serial.print("Fan_min_interval: "); Serial.println(Fan_min_interval);
//          Firebase.setInt(firebaseData, username + "/config_get/Fan_min_interval", Fan_min_interval);
//        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 56:{//Empty
//        firebaseData.clearData();
//        string.clear();
//        Firebase.get(firebaseData, username + "/config/Fan_min_interval");
//        string = firebaseData.stringData();
//        if (Fan_min_interval_prev != string)
//        {
//          Fan_min_interval_prev = string;
//          Fan_min_interval = Fan_min_interval_prev.toInt();
//          Serial.println("Fan_min_interval Changed!");
//          Serial.print("Fan_min_interval: "); Serial.println(Fan_min_interval);
//          Firebase.setInt(firebaseData, username + "/config_get/Fan_min_interval", Fan_min_interval);
//        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 57:{//Empty
//        firebaseData.clearData();
//        string.clear();
//        Firebase.get(firebaseData, username + "/config/Fan_min_interval");
//        string = firebaseData.stringData();
//        if (Fan_min_interval_prev != string)
//        {
//          Fan_min_interval_prev = string;
//          Fan_min_interval = Fan_min_interval_prev.toInt();
//          Serial.println("Fan_min_interval Changed!");
//          Serial.print("Fan_min_interval: "); Serial.println(Fan_min_interval);
//          Firebase.setInt(firebaseData, username + "/config_get/Fan_min_interval", Fan_min_interval);
//        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      case 58:{//Empty
//        firebaseData.clearData();
//        string.clear();
//        Firebase.get(firebaseData, username + "/config/Fan_min_interval");
//        string = firebaseData.stringData();
//        if (Fan_min_interval_prev != string)
//        {
//          Fan_min_interval_prev = string;
//          Fan_min_interval = Fan_min_interval_prev.toInt();
//          Serial.println("Fan_min_interval Changed!");
//          Serial.print("Fan_min_interval: "); Serial.println(Fan_min_interval);
//          Firebase.setInt(firebaseData, username + "/config_get/Fan_min_interval", Fan_min_interval);
//        }////////////////////////////////////////////////////////////////
        Case++;
      break;}
      default:{
        firebase_previousMillis = currentmillis;
        Case = 0;
      break;}
    }
//    Serial.println("Done Firebase");
//    int endtime = millis() - Stime;
//    Serial.print("End Time "); Serial.println(endtime);
  }
}
void gooogleSheetStatments(void)
{
  if (((currentmillis - googleSheet_previousMillis >= googleSheet_interval * 60000) || (currentmillis < googleSheet_previousMillis)) && (Error > 0))
  {
    googleSheet_previousMillis = currentmillis;
    Serial.println("will start googleSheets");
    sendDataToSheet();
  }
}
void timeUpgrade(void)
{
  if (((currentmillis - timeUpgrade_previousMillis >= timeUpgrade_interval) || (currentmillis < timeUpgrade_previousMillis)) && (Error > 0))
  {
    //Serial.println("will start timeupgrade");
    timeUpgrade_previousMillis = currentmillis;
    timeClient.update();
    timeClient.getSeconds();
    timeClient.getMinutes();
    timeClient.getHours();
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************** update Functions *********************************************************/
void SettingMode(void)
{
  //fail system to safe state here
  LCD_Fast_Update();
  buttonCheck();

  tempFn();
  detectGas();

  LCD_Fast_Update();
  buttonCheck();

  resetCheck();
  controlStatments();

  if (((currentmillis - Setting_previousMillis >= Setting_interval) || (currentmillis < Setting_previousMillis)))
  { // here need to save the new data to the eeprom
    Page_number = 0;
    Choise_number = 0;
    App_mode = 0;
    StoreData();
  }

}
void First_Use(void)
{
  if (EEPROMReadByte(380) != 0x5A)
  {
    Serial.println("First Use Set the Default Values");
    StoreData();
    ClearResetReason();
    EEPROMWriteByte(380, 0x5A);
  }
}
void First_FirebaseUse(void)
{// still need to be implemented
  if (EEPROMReadByte(381) != 0xBB)
  {
    Serial.println("Firebase First Use Set the Default Values");
    //CodeVersion
    Firebase.setString(firebaseData, username + "/CodeVersion", CodeVersion);
    //WDT Data
    Firebase.setString(firebaseData, username + "/Status/Clear_Reset_Reason", "OFF");
    Firebase.setString(firebaseData, username + "/Status/WDT_Action", "NORMAL");
    Firebase.setInt(firebaseData, username + "/Status/ESP_RST_UNKNOWN",  0);
    Firebase.setInt(firebaseData, username + "/Status/ESP_RST_POWERON",  0);
    Firebase.setInt(firebaseData, username + "/Status/ESP_RST_EXT",      0);
    Firebase.setInt(firebaseData, username + "/Status/ESP_RST_SW",       0);
    Firebase.setInt(firebaseData, username + "/Status/ESP_RST_PANIC",    0);
    Firebase.setInt(firebaseData, username + "/Status/ESP_RST_INT_WDT",  0);
    Firebase.setInt(firebaseData, username + "/Status/ESP_RST_TASK_WDT", 0);
    Firebase.setInt(firebaseData, username + "/Status/ESP_RST_WDT",      0);
    Firebase.setInt(firebaseData, username + "/Status/ESP_RST_DEEPSLEEP",0);
    Firebase.setInt(firebaseData, username + "/Status/ESP_RST_BROWNOUT", 0);
    Firebase.setInt(firebaseData, username + "/Status/ESP_RST_SDIO",     0);
    // GAS CAL
    Firebase.setString(firebaseData, username + "/config/Gas1_Cal",0);
    Firebase.setString(firebaseData, username + "/config/Gas2_Cal",0);
    // controls
    //fans
    Firebase.setString(firebaseData, username + "/controls/Fan_1","ON");
    Firebase.setString(firebaseData, username + "/controls/Fan_2","ON");
    //heater
    Firebase.setString(firebaseData, username + "/controls/Heater_1","ON");
    Firebase.setString(firebaseData, username + "/controls/Heater_2","ON");
    //Cooler
    Firebase.setString(firebaseData, username + "/controls/Cooler_1","ON");
    //LED
    Firebase.setString(firebaseData, username + "/controls/Led_1","ON");
    Firebase.setString(firebaseData, username + "/controls/Light_1","ON");

    // states
    //fans
    Firebase.setString(firebaseData, username + "/states/Fan_1","ON");
    Firebase.setString(firebaseData, username + "/states/Fan_1_Auto","ON");
    Firebase.setString(firebaseData, username + "/states/Fan_2","ON");
    Firebase.setString(firebaseData, username + "/states/Fan_2_Auto","ON");
    //heater
    Firebase.setString(firebaseData, username + "/states/Heater_1","ON");
    Firebase.setString(firebaseData, username + "/states/Heater_1_Auto","ON");
    Firebase.setString(firebaseData, username + "/states/Heater_2","ON");
    Firebase.setString(firebaseData, username + "/states/Heater_2_Auto","ON");
    //Cooler
    Firebase.setString(firebaseData, username + "/states/Cooler_1","ON");
    Firebase.setString(firebaseData, username + "/states/Cooler_1_Auto","ON");
    //LED
    Firebase.setString(firebaseData, username + "/states/Led_1","ON");
    Firebase.setString(firebaseData, username + "/states/Light_1","ON");
    //Config
    Firebase.setString(firebaseData, username + "/config/Fan_min_interval","10");
    Firebase.setString(firebaseData, username + "/config/Cool_min_interval","10");
    Firebase.setString(firebaseData, username + "/config/Heat_min_interval","10");
    Firebase.setString(firebaseData, username + "/config/Fan_on_time","240");
    Firebase.setString(firebaseData, username + "/config/Fan_off_time","60");
    Firebase.setString(firebaseData, username + "/config/Heater_on_time","240");
    Firebase.setString(firebaseData, username + "/config/Heater_off_time","60");
    Firebase.setString(firebaseData, username + "/config/Cooler_on_time","60");
    Firebase.setString(firebaseData, username + "/config/Cooler_off_time","240");
    Firebase.setString(firebaseData, username + "/config/Temp_variance_Cool","2");
    Firebase.setString(firebaseData, username + "/config/Temp_variance_FanB","1");
    Firebase.setString(firebaseData, username + "/config/Temp_variance_FanA","0");

    
    EEPROMWriteByte(381, 0xBB);
  }
}
void RestoreData(void)
{
  Light_Status_Char = EEPROMReadByte(371);
  Gas2_Sen_Cal = EEPROMReadInt(369);
  Gas2_Sen_Cal = EEPROMReadInt(369);
  Gas1_Sen_Cal = EEPROMReadInt(365);
  firebase_interval = EEPROMReadInt(361);
  Fan_min_interval = EEPROMReadInt(359);
  Cool_min_interval = EEPROMReadInt(357);
  Heat_min_interval = EEPROMReadInt(355);
  Fan_on_time = EEPROMReadInt(353);
  Fan_off_time = EEPROMReadInt(351);
  Cooler_on_time = EEPROMReadInt(349);
  Cooler_off_time = EEPROMReadInt(347);
  Heater_on_time = EEPROMReadInt(345);
  Heater_off_time = EEPROMReadInt(343);
  Temp_variance_Cool = EEPROMReadByte(342);
  Temp_variance_FanA = EEPROMReadByte(341);
  Temp_variance_FanB = EEPROMReadByte(340);
}
void StoreData(void)
{
  /*
     EEPROM storage positions
     ResetReasons       --> 500-510
     First_FirebaseUse  --> 382
     First_Use          --> 381
     Light_Status       --> 371
     Gas2_Sen_Cal       --> 369
     Gas1_Sen_Cal       --> 365
     firebase_interv    --> 361
     Fan_min_interval   --> 359
     Cool_min_interval  --> 357
     Heat_min_interval  --> 355
     Fan_on_time        --> 353
     Fan_off_time       --> 351
     Cooler_on_time     --> 349
     Cooler_off_time    --> 347
     Heater_on_time     --> 345
     Heater_off_time    --> 343
     Temp_variance_Cool --> 342
     Temp_variance_FanA --> 341
     Temp_variance_FanB --> 340
     MinTemp_Trigger    --> 330
     MaxTemp_Trigger    --> 320
     MaxVent_Trigger    --> 310
     MinVent_Trigger    --> 300
     mobapp_username    --> 120-179
     WiFi_PW            --> 60-119
     WiFi_SSID          --> 0-59
  */



if ((EEPROMReadInt(369) != Gas2_Sen_Cal))
  {
    EEPROMWriteInt(369, Gas2_Sen_Cal);
    Serial.print("Gas2_Sen_Cal :"); Serial.println(Gas2_Sen_Cal);
  }
  else
    Gas2_Sen_Cal = EEPROMReadInt(369);

  if ((EEPROMReadInt(365) != Gas1_Sen_Cal))
  {
    EEPROMWriteInt(365, Gas1_Sen_Cal);
    Serial.print("Gas1_Sen_Cal :"); Serial.println(Gas1_Sen_Cal);
  }
  else
    Gas1_Sen_Cal = EEPROMReadInt(365);
    
  if ((firebase_interval <= 250) && (EEPROMReadInt(361) != firebase_interval))
  {
    EEPROMWriteInt(361, firebase_interval);
    Serial.print("firebase_interval :"); Serial.println(firebase_interval);
  }
  else
    firebase_interval = EEPROMReadInt(361);

  if ((EEPROMReadInt(359) != Fan_min_interval))
  {
    EEPROMWriteInt(359, Fan_min_interval);
    Serial.print("Fan_min_interval :"); Serial.println(Fan_min_interval);
  }
  else
    Fan_min_interval = EEPROMReadInt(359);

  if ((EEPROMReadInt(357) != Cool_min_interval))
  {
    EEPROMWriteInt(357, Cool_min_interval);
    Serial.print("Cool_min_interval :"); Serial.println(Cool_min_interval);
  }
  else
    Cool_min_interval = EEPROMReadInt(357);

  if (EEPROMReadInt(355) != Heat_min_interval)
  {
    EEPROMWriteInt(355, Heat_min_interval);
    Serial.print("Heat_min_interval :"); Serial.println(Heat_min_interval);
  }
  else
    Heat_min_interval = EEPROMReadInt(355);

  if ((EEPROMReadInt(353) != Fan_on_time))
  {
    EEPROMWriteInt(353, Fan_on_time);
    Serial.print("Fan_on_time :"); Serial.println(Fan_on_time);
  }
  else
    Fan_on_time = EEPROMReadInt(353);

  if ((EEPROMReadInt(351) != Fan_off_time))
  {
    EEPROMWriteInt(351, Fan_off_time);
    Serial.print("Fan_off_time :"); Serial.println(Fan_off_time);
  }
  else
    Fan_off_time = EEPROMReadInt(351);

  if ((EEPROMReadInt(349) != Cooler_on_time))
  {
    EEPROMWriteInt(349, Cooler_on_time);
    Serial.print("Cooler_on_time :"); Serial.println(Cooler_on_time);
  }
  else
    Cooler_on_time = EEPROMReadInt(349);

  if ((EEPROMReadInt(347) != Cooler_off_time))
  {
    EEPROMWriteInt(347, Cooler_off_time);
    Serial.print("Cooler_off_time :"); Serial.println(Cooler_off_time);
  }
  else
    Cooler_off_time = EEPROMReadInt(347);

  if ((EEPROMReadInt(345) != Heater_on_time))
  {
    EEPROMWriteInt(345, Heater_on_time);
    Serial.print("Heater_on_time :"); Serial.println(Heater_on_time);
  }
  else
    Heater_on_time = EEPROMReadInt(345);

  if (EEPROMReadInt(343) != Heater_off_time)
  {
    EEPROMWriteInt(343, Heater_off_time);
    Serial.print("Heater_off_time :"); Serial.println(Heater_off_time);
  }
  else
    Heater_off_time = EEPROMReadInt(343);

  if (EEPROMReadByte(342) != Temp_variance_Cool)
  {
    EEPROMWriteByte(342, Temp_variance_Cool);
    Serial.print("Temp_variance_Cool :"); Serial.println(Temp_variance_Cool);
  }
  else
    Temp_variance_Cool = EEPROMReadByte(342);

  if (EEPROMReadByte(341) != Temp_variance_FanA)
  {
    EEPROMWriteByte(341, Temp_variance_FanA);
    Serial.print("Temp_variance_FanA :"); Serial.println(Temp_variance_FanA);
  }
  else
    Temp_variance_FanA = EEPROMReadByte(341);

  if (EEPROMReadByte(340) != Temp_variance_FanB)
  {
    EEPROMWriteByte(340, Temp_variance_FanB);
    Serial.print("Temp_variance_FanB :"); Serial.println(Temp_variance_FanB);
  }
  else
    Temp_variance_FanB = EEPROMReadByte(340);

  if (((MinTemp_Trigger >= 5) && (MinTemp_Trigger <= 100)) && (EEPROMReadInt(330) != MinTemp_Trigger))
  {
    EEPROMWriteInt(330, MinTemp_Trigger);
    Serial.print("MinTemp_Trigger :"); Serial.println(MinTemp_Trigger);
  }
  else
    MinTemp_Trigger = EEPROMReadInt(330);

  if (((MaxTemp_Trigger >= 5) && (MaxTemp_Trigger <= 100)) && (EEPROMReadInt(320) != MaxTemp_Trigger))
  {
    EEPROMWriteInt(320, MaxTemp_Trigger);
    Serial.print("MaxTemp_Trigger :"); Serial.println(MaxTemp_Trigger);
  }
  else
    MaxTemp_Trigger = EEPROMReadInt(320);

  if (((MinVent_Trigger >= 5) && (MinVent_Trigger <= 100)) && (EEPROMReadInt(300) != MinVent_Trigger))
  {
    EEPROMWriteInt(300, MinVent_Trigger);
    Serial.print("MinVent_Trigger :"); Serial.println(MinVent_Trigger);
  }
  else
    MinVent_Trigger = EEPROMReadInt(300);

  if (((MaxVent_Trigger >= 5) && (MaxVent_Trigger <= 1000)) && (EEPROMReadInt(310) != MaxVent_Trigger))
  {
    EEPROMWriteInt(310, MaxVent_Trigger);
    Serial.print("MaxVent_Trigger :"); Serial.println(MaxVent_Trigger);
  }
  else
    MaxVent_Trigger = EEPROMReadInt(310);
}
void resetCheck(void)
{
  if ((currentmillis - resetCheck_previousMillis >= resetCheck_interval) || (currentmillis < resetCheck_previousMillis))
  {
    //Serial.println("will start resetCheck");
    resetCheck_previousMillis = currentmillis;
    if (ResetFlag == "ON")
    {
      Serial.println("ResetFlag on");
      Firebase.setString(firebaseData, username + "/resetFlag", "OFF");
      ESP.restart();
    }
    //Serial.println("will stop resetCheck");
  }
}
void tempFn(void)
{
  static uint32_t temp_previousMillis = 0;
  if (currentmillis - temp_previousMillis >= temp_interval)
  {
    //Serial.println("will start temp");
    temp_previousMillis = currentmillis;
    int tempTotalNumber = 5;
    int humTotalNumber  = 5;
    T1_S = 1;
    T2_S = 1;
    T3_S = 1;
    T4_S = 1;
    T5_S = 1;

    Temperature1 = dht1.readTemperature(); // Gets the values of the temperature
    Humidity1 = dht1.readHumidity(); // Gets the values of the humidity

    Temperature2 = dht2.readTemperature(); // Gets the values of the temperature
    Humidity2 = dht2.readHumidity(); // Gets the values of the humidity

    Temperature3 = dht3.readTemperature(); // Gets the values of the temperature
    Humidity3 = dht3.readHumidity(); // Gets the values of the humidity

    Temperature4 = dht4.readTemperature(); // Gets the values of the temperature
    Humidity4 = dht4.readHumidity(); // Gets the values of the humidity

    Temperature5 = dht5.readTemperature(); // Gets the values of the temperature
    Humidity5 = dht5.readHumidity(); // Gets the values of the humidity

    /////////////////////////////////protection/////////////////////////////////////
    if (isnan(Temperature1))
    {
      Temperature1 = 0;
      tempTotalNumber--;
    }
    if (isnan(Temperature2))
    {
      Temperature2 = 0;
      tempTotalNumber--;
    }
    if (isnan(Temperature3))
    {
      Temperature3 = 0;
      tempTotalNumber--;
    }
    if (isnan(Temperature4))
    {
      Temperature4 = 0.0;
      tempTotalNumber--;
    }
    if (isnan(Temperature5))
    {
      Temperature5 = 0.0;
      tempTotalNumber--;
    }
    if (isnan(Humidity1))
    {
      Humidity1 = 0.0;
      humTotalNumber--;
    }
    if (isnan(Humidity2))
    {
      Humidity2 = 0.0;
      humTotalNumber--;
    }
    if (isnan(Humidity3))
    {
      Humidity3 = 0.0;
      humTotalNumber--;
    }
    if (isnan(Humidity4))
    {
      Humidity4 = 0.0;
      humTotalNumber--;
    }
    if (isnan(Humidity5))
    {
      Humidity5 = 0.0;
      humTotalNumber--;
    }
    if (tempTotalNumber>0)
    {
      Temperature = (( Temperature1 + Temperature2 + Temperature3 + Temperature4 + Temperature5) / tempTotalNumber);
      Humidity    = ((Humidity1 + Humidity2 + Humidity3 + Humidity4 + Humidity5) / humTotalNumber);
    }
    else
    {
    Temperature = 90;
    Humidity = 110;
    }


    val = (int)(Temperature * 10 + 0.5);
    Temperature = (float) val / 10;
    if (isnan(Temperature))
      Temperature = 00.0;
    val = (int)(Humidity * 10 + 0.5);
    Humidity = (float) val / 10;
    if (isnan(Humidity))
      Humidity = 00.0;
    if (Temperature == 0)
      Temperature = 80;
    if (Humidity == 0)
      Humidity = 120;

    //    Serial.print("Temperature: "); Serial.println(Temperature);
    //    Serial.print("Humidity: "); Serial.println(Humidity);
  }
}
void detectGas()
{
  static uint32_t gas_previousMillis = 0;
  if (currentmillis - gas_previousMillis >= gas_interval)
  {
    //Serial.println("will start detectGas");
    gas_previousMillis = currentmillis;
    gas1 = analogRead(GASA1) - Gas1_Sen_Cal;
    gas2 = analogRead(GASA2) - Gas2_Sen_Cal;
    gas = (gas1 + gas2) / 2;
    quality = "Good";
    if (gas <= MinVent_Trigger)
    {
      quality = "Good";
    }
    else if (gas <= MaxVent_Trigger)
    {
      quality = "Moderate";
    }
    else
    {
      quality = "Poor";
    }
    Serial.print("gas: "); Serial.println(gas);
    Serial.print("quality: "); Serial.println(quality);
  }
}
void controlStatments(void)
{
  static uint32_t control_previousMillis = 0;
  if (currentmillis - control_previousMillis >= control_interval)
  {
    //Serial.println("will start control");
    control_previousMillis = currentmillis;
    if (Temperature<75)
    {
      { //FanA
        if (FanA_Auto == "OFF")// if FanA manual
        {
          if (Set_ManualFA == "ON")
          {
            digitalWrite(FanA, control_ON);
            if (fanA_status == "OFF")
            {
              FanAStartTime = currentmillis;
              fanA_status = "ON";
            }
          }
          else if (Set_ManualFA == "OFF")
          {
            digitalWrite(FanA, control_OFF);
            fanA_status = "OFF";
          }
        }
        else
        {
          if (((gas > MaxVent_Trigger) && (Temperature > MinTemp_Trigger)) || (Temperature > MaxTemp_Trigger + Temp_variance_FanA))
          {
            if (0 == FanA_Flag)
            {
              FanA_Flag = 1;
              FanAStartTime = currentmillis;
            }
          }
          else if (((Temperature < (MinTemp_Trigger)) || ((gas < MinVent_Trigger) && ((Temperature < MaxTemp_Trigger))))&&(currentmillis > (FanA_STime_MinInterval + Fan_min_interval*1000)))// here may be require  - Temp_variance_Cool
          {
            FanA_Flag = 0;
            digitalWrite(FanA, control_OFF);
            fanA_status = "OFF";
          }
  
          if (((currentmillis - FanAStartTime) < (Fan_1_on_time * 1000)) && (FanA_Flag == 1))
          {
            digitalWrite(FanA, control_ON);
            fanA_status = "ON";
          }
          else if (((currentmillis - FanAStartTime) < ((Fan_1_off_time + Fan_1_on_time) * 1000)) && (FanA_Flag == 1))
          {
            digitalWrite(FanA, control_OFF);
            fanA_status = "RST";
          }
          else if (((currentmillis - FanAStartTime) > ((Fan_1_off_time + Fan_1_on_time) * 1000)) && (FanA_Flag == 1))
          {
            FanAStartTime = currentmillis;
          }
        }
      }
      { // FanB
        if (FanB_Auto == "OFF")
        {
          if (Set_ManualFB == "ON")
          {
            digitalWrite(FanB, control_ON);
            if (fanB_status == "OFF")
            {
              FanBStartTime = currentmillis;
              fanB_status = "ON";
            }
          }
          else if (Set_ManualFB == "OFF")
          {
            digitalWrite(FanB, control_OFF);
            fanB_status = "OFF";
          }
        }
        else
        {
          if (((gas > MaxVent_Trigger) && (Temperature > MinTemp_Trigger)) || (Temperature > MaxTemp_Trigger + Temp_variance_FanB))
          {
            if ((currentmillis - FanAStartTime >= FanDelayBTWN_Fans_interval) || (currentmillis < FanAStartTime))
            {
              if (0 == FanB_Flag)
              {
                FanB_Flag = 1;
                FanBStartTime = currentmillis;
              }
            }
          }
          else if (((Temperature < (MinTemp_Trigger)) || ((gas < MinVent_Trigger) && ((Temperature < MaxTemp_Trigger))))&&(currentmillis > (FanB_STime_MinInterval + Fan_min_interval*1000)))
          {
            FanB_Flag = 0;
            digitalWrite(FanB, control_OFF);
            fanB_status = "OFF";
          }
  
          
          if ((((currentmillis - FanBStartTime) <= Fan_on_time * 1000) && (FanB_Flag == 1)) || (currentmillis < FanBStartTime))
          {
            digitalWrite(FanB, control_ON);
            fanB_status = "ON";
          }
          else if ((((currentmillis - FanBStartTime) <= ((Fan_off_time + Fan_on_time) * 1000)) && (FanB_Flag == 1)) || (currentmillis < FanBStartTime))
          {
            digitalWrite(FanB, control_OFF);
            fanB_status = "RST";
          }
          else if ((((currentmillis - FanBStartTime) > ((Fan_off_time + Fan_on_time) * 1000)) && (FanB_Flag == 1)) || (currentmillis < FanBStartTime))
          {
            FanBStartTime = currentmillis;
          }
        }
      }
      { //Cooler code
        if (Cooler_Auto == "OFF")
        {
          if (Set_ManualC == "ON")
          {
            digitalWrite(Cooler, control_ON);
            cooler_status = "ON";
          }
          else if (Set_ManualC == "OFF")
          {
            digitalWrite(Cooler, control_OFF);
            cooler_status = "OFF";
          }
        }
        else //ON/OFF time 60/240S
        {
          if (Temperature > MaxTemp_Trigger + Temp_variance_Cool)//cooler will work after max+Temp_variance_Cool
          {
            if (0 == CoolerFlag)
            {
              CoolerFlag = 1;
              CoolerStartTime = currentmillis;
            }
          }
          else if (Temperature < MaxTemp_Trigger )// cooler will close after max+1
          {
            CoolerFlag = 0;
            digitalWrite(Cooler, control_OFF);
            cooler_status = "OFF";
          }
        }
  
        if ((((currentmillis - CoolerStartTime) <= Cooler_on_time * 1000) && (CoolerFlag == 1)) || (currentmillis < CoolerStartTime))
        {
          digitalWrite(Cooler, control_ON);
          cooler_status = "ON";
        }
        else if ((((currentmillis - CoolerStartTime) <= (Cooler_off_time + Cooler_on_time) * 1000) && (CoolerFlag == 1)) || (currentmillis < CoolerStartTime))
        {
          digitalWrite(Cooler, control_OFF);
          cooler_status = "RST";
        }
        else if ((((currentmillis - CoolerStartTime) > (Cooler_off_time + Cooler_on_time) * 1000) && (CoolerFlag == 1)) || (currentmillis < CoolerStartTime))
        {
          CoolerStartTime = currentmillis;
        }
      }
      { //HeaterA
        if (heaterA_Auto == "OFF")// if HeaterA manual
        {
          if (Set_ManualHA == "ON")
          {
            digitalWrite(HeaterA, control_ON);
            if (heaterA_status == "OFF")
            {
              HeaterAStartTime = currentmillis;
              heaterA_status = "ON";
            }
          }
          else if (Set_ManualHA == "OFF")
          {
            digitalWrite(HeaterA, control_OFF);
            heaterA_status = "OFF";
          }
        }
        else
        {
          if (Temperature < MaxTemp_Trigger)
          {
            if (0 == HeaterA_Flag)
            {
              HeaterA_Flag = 1;
              HeaterAStartTime = currentmillis;
              HeaterA_STime_MinInterval = currentmillis;
            }
          }
          else if ((Temperature > MaxTemp_Trigger)&&(currentmillis > (HeaterA_STime_MinInterval + (Heat_min_interval*1000))))
          {
            HeaterA_Flag = 0;
            digitalWrite(HeaterA, control_OFF);
            heaterA_status = "OFF";
          }
  
          if (((currentmillis - HeaterAStartTime) < (Heater_on_time * 1000)) && (HeaterA_Flag == 1))
          {
            digitalWrite(HeaterA, control_ON);
            heaterA_status = "ON";
          }
          else if (((currentmillis - HeaterAStartTime) < ((Heater_off_time + Heater_on_time) * 1000)) && (HeaterA_Flag == 1))
          {
            digitalWrite(HeaterA, control_OFF);
            heaterA_status = "RST";
          }
          else if (((currentmillis - HeaterAStartTime) > (Heater_off_time + Heater_on_time) * 1000) && (HeaterA_Flag == 1))
          {
            HeaterAStartTime = currentmillis;
          }
        }
      }
      { //HeaterB
        if (heaterB_Auto == "OFF")// if HeaterB manual
        {
          if (Set_ManualHB == "ON")
          {
            digitalWrite(HeaterB, control_ON);
            if (heaterB_status == "OFF")
            {
              HeaterBStartTime = currentmillis;
              heaterB_status = "ON";
            }
          }
          else if (Set_ManualHB == "OFF")
          {
            digitalWrite(HeaterB, control_OFF);
            heaterB_status = "OFF";
          }
        }
        else
        {
          if (Temperature < MaxTemp_Trigger)
          {
            if (0 == HeaterB_Flag)
            {
              HeaterB_Flag = 1;
              HeaterBStartTime = currentmillis;
              HeaterB_STime_MinInterval = currentmillis;
            }
          }
          else if ((Temperature > MaxTemp_Trigger)&&(currentmillis > (HeaterB_STime_MinInterval + (Heat_min_interval*1000))))
          {
            HeaterB_Flag = 0;
            digitalWrite(HeaterB, control_OFF);
            heaterB_status = "OFF";
          }
  
          if (((currentmillis - HeaterBStartTime) < Heater_on_time * 1000) && (HeaterB_Flag == 1))
          {
            digitalWrite(HeaterB, control_ON);
            heaterB_status = "ON";
          }
          else if (((currentmillis - HeaterBStartTime) < (Heater_off_time + Heater_on_time) * 1000) && (HeaterB_Flag == 1))
          {
            digitalWrite(HeaterB, control_OFF);
            heaterB_status = "RST";
          }
          else if (((currentmillis - HeaterBStartTime) > (Heater_off_time + Heater_on_time) * 1000) && (HeaterB_Flag == 1))
          {
            HeaterBStartTime = currentmillis;
          }
        }
      }
      {//Light
      digitalWrite(Light, Light_Status_Char);
      }
      //Serial.println("will stop control");
    }
    else
    {
      
    }
  }
}
void RFID_Read(boolean RFID_Success)
{
  //  if(((currentmillis - RFID_Read_previousMillis >= RFID_Read_interval)||(currentmillis < RFID_Read_previousMillis)) && (Error > 0))
  //  {
  //    //Serial.println("will start RFID");
  //    RFID_Read_previousMillis = currentmillis;
  //    if ( RFID_Success )
  //    {
  //      boolean success;
  //      uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  //      uint8_t uidLength;                       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  //      success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  //      if (success) {
  //        Serial.println("Found A Card!");
  //        Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
  //        Serial.print("UID Value: ");
  //        digitalWrite(BlueLed, 0);
  //        delay(500);
  //        digitalWrite(BlueLed, 1);
  //        delay(500);
  //        digitalWrite(BlueLed, 0);
  //        delay(500);
  //        digitalWrite(BlueLed, LED_Status);
  //        for (uint8_t i = 0; i < uidLength; i++)
  //        {
  //          Serial.print(" 0x"); Serial.print(uid[i], HEX);
  //        }
  //        Serial.println("");
  //        if (uidLength == 4)
  //        {
  //          // We probably have a Mifare Classic card ...
  //          uint32_t cardid = uid[0];
  //          cardid <<= 8;
  //          cardid |= uid[1];
  //          cardid <<= 8;
  //          cardid |= uid[2];
  //          cardid <<= 8;
  //          cardid |= uid[3];
  //          String UID_string = (String)cardid ;
  //          Serial.println("ID is : " + UID_string);
  //          RFIDsendData(UID_string + "&persondata=zzzz&index=2");
  //        }
  //      }
  //      else
  //      {
  //        Serial.println("Timed out! Waiting for a card...");
  //      }
  //    }
  //  }
}
void serialPrints(void)
{
  if ((currentmillis - serial_previousMillis >= serial_interval) || (currentmillis < serial_previousMillis))
  {
    serial_previousMillis = currentmillis;
    Serial.println("\n\n\n\n\n\n\n\n\n\nSerialPrint\n\n");
    Serial.print("cooler_status: "); Serial.println(cooler_status);
    Serial.print("heaterA_status: "); Serial.println(heaterA_status);
    Serial.print("heaterB_status: "); Serial.println(heaterB_status);
    Serial.print("Temperature1: "); Serial.println(Temperature1);
    Serial.print("Temperature2: "); Serial.println(Temperature2);
    Serial.print("Temperature3: "); Serial.println(Temperature3);
    Serial.print("Temperature: "); Serial.println(Temperature);
    Serial.print("Humidity: "); Serial.println(Humidity);
    Serial.print("Humidity1: "); Serial.println(Humidity1);
    Serial.print("Humidity2: "); Serial.println(Humidity2);
    Serial.print("Humidity3: "); Serial.println(Humidity3);
    Serial.print("gas: "); Serial.println(gas);
    Serial.print("Set_ManualHA: "); Serial.println(Set_ManualHA);
    Serial.print("Set_ManualHB: "); Serial.println(Set_ManualHB);
    Serial.print("Set_ManualFA: "); Serial.println(Set_ManualFA);
    Serial.print("Set_ManualFB: "); Serial.println(Set_ManualFB);
    Serial.print("ResetFlag: "); Serial.println(ResetFlag);
    Serial.print("LED_Status: "); Serial.println(LED_Status);
    Serial.print("Light_Status: "); Serial.println(Light_Status);
    //    Serial.print("MinVent_Trigger: "); Serial.println(MinVent_Trigger);
    //    Serial.print("MaxVent_Trigger: "); Serial.println(MaxVent_Trigger);
    //    Serial.print("MinTemp_Trigger: "); Serial.println(MinTemp_Trigger);
    //    Serial.print("MaxTemp_Trigger: "); Serial.println(MaxTemp_Trigger);
    //    Serial.print("firebase_interval: "); Serial.println(firebase_interval);
    Serial.print("FanA_Auto: "); Serial.println(FanA_Auto);
    Serial.print("FanB_Auto: "); Serial.println(FanB_Auto);
    Serial.print("heaterA_Auto: "); Serial.println(heaterA_Auto);
    Serial.print("heaterB_Auto: "); Serial.println(heaterB_Auto);
    //    Serial.print("timeClient.getSeconds(): "); Serial.println(timeClient.getSeconds());
    //    Serial.print("timeClient.getMinutes(): "); Serial.println(timeClient.getMinutes());
    //    Serial.print("timeClient.getHours(): "); Serial.println(timeClient.getHours());
  }
}
void LCD_weather()
{
  if (((currentmillis - LCD_previousMillis >= LCD_interval) || (currentmillis < LCD_previousMillis)))
  {
    Serial.println("will start LCD");
    lcd.clear(); //clear lcd to print next vlaues
    LCD_previousMillis = currentmillis;
    line1 = String("Tem:") + String(" ") + String("Hum:") + String(" ") + String("Gas:") + String(" ") + String(" ") + String("L:");
    line2 = String(int(Temperature)) + "." + String((int(Temperature * 10)) % 10) + String(" ") + String(int(Humidity));
    line3 = String("F1:") + String(" ") + String("F2:") + String(" ") + String("C:") + String(" ") + String(" ") + String("H1:") + String(" ") + String("H2:");
    // set cursor to first column, first row
    lcd.setCursor(0, 0);
    lcd.print(line1);    // print line1 in first row
    lcd.setCursor(0, 1); // set cursor to first column, second row
    lcd.print(line2);
    lcd.setCursor(0, 2);
    lcd.print(line3);
    lcd.setCursor(0, 3);
    lcd.print(fanA_status);
    lcd.setCursor(4, 3);
    lcd.print(fanB_status);
    lcd.setCursor(8, 3);
    lcd.print(cooler_status);
    lcd.setCursor(12, 3);
    lcd.print(heaterA_status);
    lcd.setCursor(16, 3);
    lcd.print(heaterB_status);
    lcd.setCursor(16, 1);
    lcd.print(Light_Status);
    lcd.setCursor(10, 1);
    lcd.print(String(int(gas)));
    lcd.setCursor(19, 0);   // set cursor to second row and sixteenth column to print wifi connection
    if (WiFi.status() != WL_CONNECTED)
      lcd.write(2);
    else
      lcd.write(1);
  }
}
void LCD_Fast_Update()
{
  /* shortcuts names
      TP_0 --> firebase_interval

      TP_1 --> Fan_min_interval
      TP_2 --> Cool_min_interval
      TP_3 --> Heat_min_interval

      TP_4 --> Fan_on_time
      TP_5 --> Fan_off_time
      TP_6 --> Cooler_on_time
      TP_7 --> Cooler_off_time
      TP_8 --> Heater_on_time
      TP_9 --> Heater_off_time

      TP_10--> Temp_variance_Cool
      TP_11--> Temp_variance_FanA
      TP_12--> Temp_variance_FanB
      TP_13-->
      TP_14-->
      TP_15-->

  */
  if (((currentmillis - Fast_LCD_previousMillis >= Fast_LCD_interval) || (currentmillis < Fast_LCD_previousMillis)))
  {
    Fast_LCD_previousMillis = currentmillis;
    if (Page_number == 0)
    {
      line1 = String("Tem:") + String(" ") + String("Hum:") + String(" ") + String("Gas:") + String(" ") + String(" ") + String("L:");
      line2 = String(int(Temperature)) + "." + String((int(Temperature * 10)) % 10) + String(" ") + String(int(Humidity));
      line3 = String("F1:") + String(" ") + String("F2:") + String(" ") + String("C:") + String(" ") + String(" ") + String("H1:") + String(" ") + String("H2:");
      // set cursor to first column, first row
      lcd.setCursor(0, 0);
      lcd.print(line1);    // print line1 in first row
      lcd.setCursor(0, 1); // set cursor to first column, second row
      lcd.print(line2);
      lcd.setCursor(0, 2);
      lcd.print(line3);
      lcd.setCursor(0, 3);
      lcd.print("   ");
      lcd.setCursor(0, 3);
      lcd.print(fanA_status);
      lcd.setCursor(4, 3);
      lcd.print("   ");
      lcd.setCursor(4, 3);
      lcd.print(fanB_status);
      lcd.setCursor(8, 3);
      lcd.print("   ");
      lcd.setCursor(8, 3);
      lcd.print(cooler_status);
      lcd.setCursor(12, 3);
      lcd.print("   ");
      lcd.setCursor(12, 3);
      lcd.print(heaterA_status);
      lcd.setCursor(16, 3);
      lcd.print("   ");
      lcd.setCursor(16, 3);
      lcd.print(heaterB_status);
      lcd.setCursor(16, 1);
      lcd.print("   ");
      lcd.setCursor(16, 1);
      lcd.print(Light_Status);
      lcd.setCursor(10, 1);
      lcd.print("   ");
      lcd.setCursor(10, 1);
      lcd.print(String(int(gas)));
    }
    else if (Page_number == 1)
    {
      lcd.setCursor(1, 0);
      lcd.print("Max Temp");
      lcd.setCursor(1, 1);
      lcd.print(String(int(MaxTemp_Trigger)) + "." + String((int(MaxTemp_Trigger * 10)) % 10));
      lcd.setCursor(11, 0);
      lcd.print("Min Temp");
      lcd.setCursor(11, 1);
      lcd.print(String(int(MinTemp_Trigger)) + "." + String((int(MinTemp_Trigger * 10)) % 10));

      lcd.setCursor(1, 2);
      lcd.print("Max Vent");
      lcd.setCursor(1, 3);
      lcd.print(String(int(MaxVent_Trigger)) + "." + String((int(MaxVent_Trigger * 10)) % 10));
      lcd.setCursor(11, 2);
      lcd.print("Min Vent");
      lcd.setCursor(11, 3);
      lcd.print(String(int(MinVent_Trigger)) + "." + String((int(MinVent_Trigger * 10)) % 10));

      if (Choise_number == 1)
      {
        lcd.setCursor(0, 0);
        lcd.write(3);
      }
      else if (Choise_number == 2)
      {
        lcd.setCursor(10, 0);
        lcd.write(3);
        lcd.setCursor(0, 0);
        lcd.print(" ");
      }
      else if (Choise_number == 3)
      {
        lcd.setCursor(0, 2);
        lcd.write(3);
        lcd.setCursor(10, 0);
        lcd.print(" ");
      }
      else if (Choise_number == 4)
      {
        lcd.setCursor(10, 2);
        lcd.write(3);
        lcd.setCursor(0, 2);
        lcd.print(" ");
      }
      else
      {
        lcd.setCursor(10, 2);//last arrow added
        lcd.print(" ");
      }
    }
    else if (Page_number == 2)
    {
      lcd.setCursor(1, 0);
      lcd.print("TP_0");
      lcd.setCursor(6, 0);
      lcd.print(String(firebase_interval));
      lcd.setCursor(11, 0);
      lcd.print("TP_1");
      lcd.setCursor(16, 0);
      lcd.print(String(Fan_min_interval));

      lcd.setCursor(1, 1);
      lcd.print("TP_2");
      lcd.setCursor(6, 1);
      lcd.print(String(Cool_min_interval));
      lcd.setCursor(11, 1);
      lcd.print("TP_3");
      lcd.setCursor(16, 1);
      lcd.print(String(Heat_min_interval));

      lcd.setCursor(1, 2);
      lcd.print("TP_4");
      lcd.setCursor(6, 2);
      lcd.print(String(Fan_on_time));
      lcd.setCursor(11, 2);
      lcd.print("TP_5");
      lcd.setCursor(16, 2);
      lcd.print(String(Fan_off_time));

      lcd.setCursor(1, 3);
      lcd.print("TP_6");
      lcd.setCursor(6, 3);
      lcd.print(String(Cooler_on_time));
      lcd.setCursor(11, 3);
      lcd.print("TP_7");
      lcd.setCursor(16, 3);
      lcd.print(String(Cooler_off_time));

      if (Choise_number == 1)
      {
        lcd.setCursor(0, 0);
        lcd.write(3);
      }
      else if (Choise_number == 2)
      {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(10, 0);
        lcd.write(3);
      }
      else if (Choise_number == 3)
      {
        lcd.setCursor(0, 1);
        lcd.write(3);
        lcd.setCursor(10, 0);
        lcd.print(" ");
      }
      else if (Choise_number == 4)
      {
        lcd.setCursor(10, 1);
        lcd.write(3);
        lcd.setCursor(0, 1);
        lcd.print(" ");
      }
      else if (Choise_number == 5)
      {
        lcd.setCursor(0, 2);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.print(" ");
      }
      else if (Choise_number == 6)
      {
        lcd.setCursor(10, 2);
        lcd.write(3);
        lcd.setCursor(0, 2);
        lcd.print(" ");
      }
      else if (Choise_number == 7)
      {
        lcd.setCursor(0, 3);
        lcd.write(3);
        lcd.setCursor(10, 2);
        lcd.print(" ");
      }
      else if (Choise_number == 8)
      {
        lcd.setCursor(10, 3);
        lcd.write(3);
        lcd.setCursor(0, 3);
        lcd.print(" ");
      }
      else
      {
        lcd.setCursor(10, 3);//last arrow added
        lcd.print(" ");
      }
    }
    else if (Page_number == 3)
    {
      lcd.setCursor(1, 0);
      lcd.print("TP_8");
      lcd.setCursor(6, 0);
      lcd.print(String(Heater_on_time));
      lcd.setCursor(12, 0);
      lcd.print("TP_9");
      lcd.setCursor(17, 0);
      lcd.print(String(Heater_off_time));

      lcd.setCursor(1, 1);
      lcd.print("TP_10");
      lcd.setCursor(7, 1);
      lcd.print(String(Temp_variance_Cool));
      lcd.setCursor(12, 1);
      lcd.print("TP_11");
      lcd.setCursor(18, 1);
      lcd.print(String(Temp_variance_FanA));

      lcd.setCursor(1, 2);
      lcd.print("TP_12");
      lcd.setCursor(7, 2);
      lcd.print(String(Temp_variance_FanB));
      lcd.setCursor(12, 2);
      lcd.print("TP_13");
      lcd.setCursor(18, 2);
      //    lcd.print(String(Fan_off_time));

      lcd.setCursor(1, 3);
      lcd.print("TP_14");
      lcd.setCursor(7, 3);
      //    lcd.print(String(Cooler_on_time));
      lcd.setCursor(12, 3);
      lcd.print("TP_15");
      lcd.setCursor(18, 3);
      //    lcd.print(String(Cooler_off_time));

      if (Choise_number == 1)
      {
        lcd.setCursor(0, 0);
        lcd.write(3);
      }
      else if (Choise_number == 2)
      {
        lcd.setCursor(0, 0);
        lcd.print(" ");
        lcd.setCursor(10, 0);
        lcd.write(3);
      }
      else if (Choise_number == 3)
      {
        lcd.setCursor(0, 1);
        lcd.write(3);
        lcd.setCursor(10, 0);
        lcd.print(" ");
      }
      else if (Choise_number == 4)
      {
        lcd.setCursor(10, 1);
        lcd.write(3);
        lcd.setCursor(0, 1);
        lcd.print(" ");
      }
      else if (Choise_number == 5)
      {
        lcd.setCursor(0, 2);
        lcd.write(3);
        lcd.setCursor(10, 1);
        lcd.print(" ");
      }
      else if (Choise_number == 6)
      {
        lcd.setCursor(10, 2);
        lcd.write(3);
        lcd.setCursor(0, 2);
        lcd.print(" ");
      }
      else if (Choise_number == 7)
      {
        lcd.setCursor(0, 3);
        lcd.write(3);
        lcd.setCursor(10, 2);
        lcd.print(" ");
      }
      else if (Choise_number == 8)
      {
        lcd.setCursor(10, 3);
        lcd.write(3);
        lcd.setCursor(0, 3);
        lcd.print(" ");
      }
      else
      {
        lcd.setCursor(10, 3);//last arrow added
        lcd.print(" ");
      }
    }
  }
}
void buttonUpdate()
{
  if ((setFlag_l) && (digitalRead(SW_S) == 0))
  {
    delay(50);
    if (digitalRead(SW_S) == 0)
    {
      Serial.println("make the process of set");
      Setting_previousMillis = currentmillis;
      if ((Page_number > 0) && (Page_number < 2))
      {
        Choise_number = (Choise_number + 1) % (Number_of_Choices1 + 1);
      }
      else if ((Page_number > 1) && (Page_number < 10))
      {
        Choise_number = (Choise_number + 1) % (Number_of_Choices2 + 1);
      }
      setFlag_l = 0;
      setFlag_h = 0;

      digitalWrite(Buzzer, 1);
      delay (150);
      digitalWrite(Buzzer, 0);
    }
  }
  if ((negFlag_l) && (digitalRead(SW_N) == 0))
  {
    delay(50);
    if (digitalRead(SW_N) == 0)
    {
      Setting_previousMillis = currentmillis;
      Serial.println("make the process of neg");
      if (Choise_number == 0)
      {
        Page_number = (Page_number - 1) % Number_of_Pages;
        if (Page_number < 0)
          Page_number = Number_of_Pages - 1;
      }
      else if ((Choise_number == 1) && (Page_number == 1))
      {
        MaxTemp_Trigger--;
      }
      else if ((Choise_number == 2) && (Page_number == 1))
      {
        MinTemp_Trigger--;
      }
      else if ((Choise_number == 3) && (Page_number == 1))
      {
        MaxVent_Trigger--;
      }
      else if ((Choise_number == 4) && (Page_number == 1))
      {
        MinVent_Trigger--;
      }
      else if ((Choise_number == 1) && (Page_number == 2))
      {
        firebase_interval--;
      }
      else if ((Choise_number == 2) && (Page_number == 2))
      {
        Fan_min_interval--;
      }
      else if ((Choise_number == 3) && (Page_number == 2))
      {
        Cool_min_interval--;
      }
      else if ((Choise_number == 4) && (Page_number == 2))
      {
        Heat_min_interval--;
      }
      else if ((Choise_number == 5) && (Page_number == 2))
      {
        Fan_on_time--;
      }
      else if ((Choise_number == 6) && (Page_number == 2))
      {
        Fan_off_time--;
      }
      else if ((Choise_number == 7) && (Page_number == 2))
      {
        Cooler_on_time--;
      }
      else if ((Choise_number == 8) && (Page_number == 2))
      {
        Cooler_off_time--;
      }
      else if ((Choise_number == 1) && (Page_number == 3))
      {
        Heater_on_time--;
      }
      else if ((Choise_number == 2) && (Page_number == 3))
      {
        Heater_off_time--;
      }
      else if ((Choise_number == 3) && (Page_number == 3))
      {
        Temp_variance_Cool--;
      }
      else if ((Choise_number == 4) && (Page_number == 3))
      {
        Temp_variance_FanA--;
      }
      else if ((Choise_number == 5) && (Page_number == 3))
      {
        Temp_variance_FanB--;
      }
      else if ((Choise_number == 6) && (Page_number == 3))
      {

      }
      else if ((Choise_number == 7) && (Page_number == 3))
      {

      }
      else if ((Choise_number == 8) && (Page_number == 3))
      {

      }

      Serial.println(Page_number);
      negFlag_l = 0;
      negFlag_h = 0;
      digitalWrite(Buzzer, 1);
      delay (150);
      digitalWrite(Buzzer, 0);
      lcd.clear();
      LCD_Fast_Update();
    }
  }
  if ((posFlag_l) && (posFlag_h))
  {
    delay(50);
    if (digitalRead(SW_P) == 0)
    {
      Setting_previousMillis = currentmillis;
      Serial.println("make the process of pos");
      if (Choise_number == 0)
        Page_number = (Page_number + 1) % Number_of_Pages;
      else if ((Choise_number == 1) && (Page_number == 1))
      {
        MaxTemp_Trigger++;
      }
      else if ((Choise_number == 2) && (Page_number == 1))
      {
        MinTemp_Trigger++;
      }
      else if ((Choise_number == 3) && (Page_number == 1))
      {
        MaxVent_Trigger++;
      }
      else if ((Choise_number == 4) && (Page_number == 1))
      {
        MinVent_Trigger++;
      }
      else if ((Choise_number == 1) && (Page_number == 2))
      {
        firebase_interval++;
      }
      else if ((Choise_number == 2) && (Page_number == 2))
      {
        Fan_min_interval++;
      }
      else if ((Choise_number == 3) && (Page_number == 2))
      {
        Cool_min_interval++;
      }
      else if ((Choise_number == 4) && (Page_number == 2))
      {
        Heat_min_interval++;
      }
      else if ((Choise_number == 5) && (Page_number == 2))
      {
        Fan_on_time++;
      }
      else if ((Choise_number == 6) && (Page_number == 2))
      {
        Fan_off_time++;
      }
      else if ((Choise_number == 7) && (Page_number == 2))
      {
        Cooler_on_time++;
      }
      else if ((Choise_number == 8) && (Page_number == 2))
      {
        Cooler_off_time++;
      }
      else if ((Choise_number == 1) && (Page_number == 3))
      {
        Heater_on_time++;
      }
      else if ((Choise_number == 2) && (Page_number == 3))
      {
        Heater_off_time++;
      }
      else if ((Choise_number == 3) && (Page_number == 3))
      {
        Temp_variance_Cool++;
      }
      else if ((Choise_number == 4) && (Page_number == 3))
      {
        Temp_variance_FanA++;
      }
      else if ((Choise_number == 5) && (Page_number == 3))
      {
        Temp_variance_FanB++;
      }
      else if ((Choise_number == 6) && (Page_number == 3))
      {

      }
      else if ((Choise_number == 7) && (Page_number == 3))
      {

      }
      else if ((Choise_number == 8) && (Page_number == 3))
      {

      }

      Serial.println(Page_number);
      App_mode = 1;
      posFlag_l = 0;
      posFlag_h = 0;
      digitalWrite(Buzzer, 1);
      delay (150);
      digitalWrite(Buzzer, 0);
      lcd.clear();
      LCD_Fast_Update();
    }
  }
}
void buttonCheck(void)
{
  if ((currentmillis - button_previousMillis >= button_interval) || (currentmillis < button_previousMillis))
  {
    button_previousMillis = currentmillis;

    if (digitalRead(SW_S) == 1)
    {
      setFlag_h = 1;
    }
    if (digitalRead(SW_N) == 1)
    {
      negFlag_h = 1;
    }
    if (digitalRead(SW_P) == 1)
    {
      posFlag_h = 1;
    }
    buttonUpdate();
  }
}



/********************************************** Init Functions *********************************************************/
void pinSetup(void)
{
  pinMode(GASA1, INPUT);
  pinMode(GASA2, INPUT);

  pinMode(DHT1Pin, INPUT);
  pinMode(DHT2Pin, INPUT);
  pinMode(DHT3Pin, INPUT);
  pinMode(DHT4Pin, INPUT);
  pinMode(DHT5Pin, INPUT);

  pinMode(SW_N, INPUT_PULLUP);
  pinMode(SW_P, INPUT_PULLUP);
  pinMode(SW_S, INPUT_PULLUP);

  pinMode(HeaterA, OUTPUT);
  pinMode(HeaterB, OUTPUT);

  pinMode(Cooler, OUTPUT);

  pinMode(FanA, OUTPUT);
  pinMode(FanB, OUTPUT);

  pinMode(BlueLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Light, OUTPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(Buzzer, control_OFF);
  digitalWrite(HeaterB, control_OFF);
  digitalWrite(HeaterA, control_OFF);

  attachInterrupt(SW_N, SW_N_ISR, FALLING);
  attachInterrupt(SW_P, SW_P_ISR, FALLING);
  attachInterrupt(SW_S, SW_S_ISR, FALLING);
}
void DHTSetup(void)
{
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
}
void EEPROMSetup(void)
{
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM"); delay(1000000);
    ESP.restart();
  }
  if (shouldSaveConfig == true)
  {
    Serial.println("ssid & p.w & username");
    Serial.println(ssid);
    Serial.println(password);
    Serial.println(username);
    eeprom_Write(0, ssid.c_str(), stringLength);
    eeprom_Write(60, password.c_str(), stringLength);
    eeprom_Write(120, username.c_str(), stringLength);//180
    EEPROMWriteByte(381, 0x00);
  }
  else
  {
    ssid = eeprom_read(0, stringLength);
    password = eeprom_read(60, stringLength);
    username = eeprom_read(120, stringLength);
  }
}
void WifiSetup(void)
{
  Serial.print(" ssid  ");
  Serial.println(ssid);
  Serial.print(" password  ");
  Serial.println(password);
  WiFi.begin(ssid.c_str(), password.c_str());
  int x = 0;
  while ((WiFi.status() != WL_CONNECTED) & (x < 20)) {
    x++;
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Faild to Connect to WiFi");
    Serial.println(WiFi.localIP());
  }
}
void firbaseSetup(void)
{
  Serial.println("firebase setup");
  Firebase.begin(Host, Token);
  //Set database read timeout to 10 Sec (max 15 minutes)(in millis)
  Firebase.setReadTimeout(firebaseData,10000);
  Firebase.reconnectWiFi(false);

  Error = firebaseData.httpCode();
  Error_prev = Error;
}
void timeServerSetup(void)
{
  timeClient.begin();
  timeClient.setTimeOffset(7200);
}
boolean RFID_Setup(void)
{
  //  nfc.begin();
  //  uint32_t versiondata = nfc.getFirmwareVersion();
  //  if (! versiondata)
  //  {
  //    Serial.print("Didn't Find PN53x Module");
  //    return false;
  //  }
  //  else
  //  {
  //    Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  //    Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  //    Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
  //    // Configure board to read RFID tags
  //    nfc.SAMConfig();
  //    Serial.println("Waiting for an ISO14443A Card ...");
  //    return true;
  //  }
  return true;
}
void parameterSetup(void)
{
  heaterA_status = "OFF";
  heaterB_status = "OFF";

  MinVent_Trigger = EEPROMReadInt(300);
  MaxVent_Trigger = EEPROMReadInt(310);
  MaxTemp_Trigger = EEPROMReadInt(320);
  MinTemp_Trigger = EEPROMReadInt(330);

  Serial.print("MinVent_Trigger "); Serial.println(MinVent_Trigger);
  Serial.print("MaxVent_Trigger "); Serial.println(MaxVent_Trigger);
  Serial.print("MinTemp_Trigger "); Serial.println(MinTemp_Trigger);
  Serial.print("MaxTemp_Trigger "); Serial.println(MaxTemp_Trigger);
}
void UpdateCheck(void)
{
  firebaseData.clearData();
  Firebase.get(firebaseData, "/Admin/updateCode");
  string = " ";
  string = firebaseData.stringData();
  if (UpdateCode_prev != string)
  {
    if (string == "ON" || string == "OFF")
    {
      UpdateCode_prev = string;
      UpdateCode = UpdateCode_prev;
      if (UpdateCode == "ON")
      {
        Firebase.setString(firebaseData, username + "/CodeVersion", CodeVersion);
        UPDATE();
      }
    }
  }
}
void LCD_setup()
{
  static unsigned long int LCD_previousMillis = 0;
  static int startflag = 1;
  if (((currentmillis - LCD_previousMillis >= LCD_reinit_interval) || (currentmillis < LCD_previousMillis) || startflag))
  {
    startflag = 0;
    LCD_previousMillis = currentmillis;
    // initialize LCD
    Wire.begin(LCD_SDA, LCD_SCL);
    //    Wire.setClock(10000);
    lcd.init();
    // turn on LCD backlight
    lcd.backlight();
    lcd.createChar(1 , wifi_customChar);   //Creating custom characters in CG-RAM
    lcd.createChar(2, no_wifi );
    lcd.createChar(3, arrow );

    // maximum character can be stored in LCD is 8
    lcd.clear(); //clear lcd to print next vlaues
    LCD_previousMillis = currentmillis;
    line1 = String("Tem:") + String(" ") + String("Hum:") + String(" ") + String("Gas:") + String(" ") + String(" ") + String("L:");
    line2 = String(int(Temperature)) + "." + String((int(Temperature * 10)) % 10) + String(" ") + String(int(Humidity));
    line3 = String("F1:") + String(" ") + String("F2:") + String(" ") + String("C:") + String(" ") + String(" ") + String("H1:") + String(" ") + String("H2:");
    // set cursor to first column, first row
    lcd.setCursor(0, 1);
    lcd.setCursor(0, 0);
    lcd.print(line1);    // print line1 in first row
    lcd.setCursor(0, 1);
    lcd.print(line2);    // print line1 in first row
    lcd.setCursor(0, 2);
    lcd.print(line3);
    lcd.setCursor(0, 3);
    lcd.print(fanA_status);
    lcd.setCursor(4, 3);
    lcd.print(fanB_status);
    lcd.setCursor(8, 3);
    lcd.print(cooler_status);
    lcd.setCursor(12, 3);
    lcd.print(heaterA_status);
    lcd.setCursor(16, 3);
    lcd.print(heaterB_status);
    lcd.setCursor(16, 1);
    lcd.print(Light_Status);
    lcd.setCursor(10, 1);
    lcd.print(String(int(gas)));
    lcd.setCursor(19, 0);   // set cursor to second row and sixteenth column to print wifi connection
    if (WiFi.status() != WL_CONNECTED)
      lcd.write(2);
    else
      lcd.write(1);
  }
}
void LCD_StartUp()
{
  static unsigned long int LCD_previousMillis = 0;
  static int startflag = 1;
  if (((currentmillis - LCD_previousMillis >= LCD_reinit_interval) || (currentmillis < LCD_previousMillis) || startflag))
  {
    startflag = 0;
    LCD_previousMillis = currentmillis;
    // initialize LCD
    Wire.begin(LCD_SDA, LCD_SCL);
    //    Wire.setClock(10000);
    lcd.init();
    // turn on LCD backlight
    lcd.backlight();
    lcd.createChar(1 , wifi_customChar);   //Creating custom characters in CG-RAM
    lcd.createChar(2, no_wifi );
    lcd.createChar(3, arrow );

    // maximum character can be stored in LCD is 8
    lcd.clear(); //clear lcd to print next vlaues
    LCD_previousMillis = currentmillis;
    line1 = String("Tem:") + String(" ") + String("Hum:") + String(" ") + String("Gas:") + String(" ") + String(" ") + String("L:");
    line2 = String(int(Temperature)) + "." + String((int(Temperature * 10)) % 10) + String(" ") + String(int(Humidity));
    line3 = String("F1:") + String(" ") + String("F2:") + String(" ") + String("C:") + String(" ") + String(" ") + String("H1:") + String(" ") + String("H2:");
    // set cursor to first column, first row
    lcd.setCursor(0, 1);
    lcd.print("    FarmArt Tech   ");
    if (WiFi.status() != WL_CONNECTED)
      lcd.write(2);
    else
      lcd.write(1);
  }
}
/********************************************** Support Functions *********************************************************/
String getChipId()
{
  String ChipIdHex = String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
  ChipIdHex += String((uint32_t)ESP.getEfuseMac(), HEX);
  return ChipIdHex;
}
void UPDATE()
{
  //Serial.println("updating..");
  String url = "http://otadrive.com/deviceapi/update?";
  url += "k=af436368-9238-4e59-8b71-f4c764e349a7";
  url += "&v=" + CodeVersion;
  url += "&s=" + getChipId();
  WiFiClient client;
  httpUpdate.update(client, url, CodeVersion);
  Serial.println("Done!..");
}
// depends on who will communicate with google sheet may be deleted
void sendDataToSheet(void)
{
  String DATA = String(String((float)Temperature) + "|||" + String((int)Humidity) + "|||" + String((int)gas) + "|||" + (float)Temperature1) + "|||" + String((float)Temperature2) + "|||" + String((float)Temperature3) + "|||" + String((int)Humidity1) + "|||" + String((int)Humidity2) + "|||" + String((int)Humidity3) + "|||" + heaterA_status + "|||" + heaterB_status + "|||" + cooler_status + "|||" + Light_Status + "|||" + LED_Status_Str + "|||" + String((int)MinTemp_Trigger) + "|||" + String((int)MaxTemp_Trigger) + "|||" + String((int)MinVent_Trigger) + "|||" + String((int)MaxVent_Trigger) + "|||" + ResetFlag + "|||" + UpdateCode + "|||" + Set_ManualHA + "|||" + Set_ManualHB + "|||" + Set_ManualFA + "|||" + Set_ManualFB + "|||" + FanA_Auto + "|||" + FanB_Auto + "|||" + heaterA_Auto + "|||" + heaterB_Auto + "|||" + String((unsigned int)HeaterAStartTime) + "|||" + String((unsigned int)HeaterBStartTime);
  String url = ifttt_server + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + DATA;
  Serial.println(url);
  //Start to send data to IFTTT
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  http.begin(url); //HTTP

  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  // httpCode will be negative on error
  if (httpCode > 0)
  {
    // HTTP header has been send and ifttt_server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(payload);
    }
  }
  else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

}
String eeprom_read(int address, int stringLength)
{
  char in[stringLength + 1];
  Serial.println("Starting Reading");
  for (int i = 0; i < address + stringLength; i++)
  {
    in[i] = EEPROM.read(address + i);
    if (in[i] == '\0')
      break;
  }
  Serial.print(" readeeprom  ");
  Serial.println(in);
  return String(in);
}
void eeprom_Write(int address, const char*ptr, int stringLength)
{
  Serial.println("Starting Writing");
  int i = 0;
  for ( ; i < (address + stringLength); i++)
  {
    EEPROM.write((i + address), ptr[i]);
  }
  EEPROM.write(address + i, '\0');
  Serial.print("Commit : ");
  Serial.println(EEPROM.commit());
}
void EEPROMWriteByte(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);

  EEPROM.write(p_address, lowByte);
  Serial.print("Commit : ");
  Serial.println(EEPROM.commit());
}
unsigned int EEPROMReadByte(int p_address)
{
  byte BYTE = EEPROM.read(p_address);

  return (BYTE);
}
void EEPROMWriteInt(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
  Serial.print("Commit : ");
  Serial.println(EEPROM.commit());
}
unsigned int EEPROMReadInt(int p_address)
{
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}
void RFIDsendData(String params)
{
  //
  //  HTTPClient http;
  //  String url = "https://script.google.com/macros/s/AKfycbxjeykhu5BpiraSKXN_ugb-oz6_3nSC51_Oc_-zMcNywjLttXFMXJ-S0Qb3en6Y7hRS/exec?uid=" + username + "&fun=record&cardid=" + params;
  //  Serial.println(url);
  //  Serial.println("Making a request");
  //  http.setFollowRedirects(followRedirects_t::HTTPC_STRICT_FOLLOW_REDIRECTS); /*to act like a typical web browser
  //    because the url is redeirect to another url*/
  //  http.begin(url.c_str());
  //  //while (httpResponseCode != 200)
  //  {
  //    httpResponseCode = http.GET();
  //
  //    if (httpResponseCode > 0) {
  //      Serial.print("HTTP Response code: ");
  //      Serial.println(httpResponseCode);
  //      String payload = http.getString();
  //      Serial.println(payload);
  //    }
  //    else {
  //      Serial.print("Error code: ");
  //      Serial.println(httpResponseCode);
  //    }
  //  }
  //  // Free resources
  //  httpResponseCode = 0;
  //  http.end();
  //
  //  Serial.println("Waiting for another ISO14443A Card ...");
}
void set_access_point(void)// uses local wifi
{
  WiFi.softAP(ssid_ap, password_ap);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();

}
void client_handle(void)// uses local wifi
{
  WiFiClient client = server.available();

  ssid = "";
  password = "";

  if (client) {
    Serial.println("New Client.");

    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {

          if (currentLine.length() == 0) {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /data") >= 0) {

              String url = header;
              String user_name = "";
              String wifi_ssid = "";
              String wifi_pass = "";
              String p2 = "";
              String p3 = "";

              url = url.substring(7);
              url = url.substring(url.indexOf('?'), url.indexOf(' '));
              Serial.println("URL  :  " + url);

              user_name = url;
              user_name = user_name.substring(url.indexOf('=') + 1, url.indexOf('&'));
              Serial.println("Username  :  " + user_name);
              p2 = url.substring(url.indexOf('&') + 1);

              wifi_ssid = p2;
              wifi_ssid = wifi_ssid.substring(p2.indexOf('=') + 1, p2.indexOf('&'));
              Serial.println("wifi_name  :  " + wifi_ssid);

              p3 = p2.substring(p2.indexOf('&') + 1);
              wifi_pass = p3;
              wifi_pass = wifi_pass.substring(p3.indexOf('=') + 1, p3.indexOf('&'));
              Serial.println("wifi_pass  :  " + wifi_pass);

              ssid = wifi_ssid;
              password = wifi_pass;
              username = user_name;

              shouldSaveConfig = true;
              EEPROMSetup();
              ConfigResetFlag = true;

              count = 0;
              Serial.println("before");

              Serial.println(ssid);
              Serial.println(password);
              Serial.println(username);

              ssid.replace("%20", " ");
              password.replace("%20", " ");
              Serial.println("after");
              Serial.println(ssid);
              Serial.println(password);
              Serial.println(username);
              WiFi.begin(ssid.c_str(), password.c_str());
              delay(500);
              Serial.println("Connecting");
              while (WiFi.status() != WL_CONNECTED) {
                delay(700);
                count++;
                if (count >= 10) {
                  client.println("Failed");
                  break;
                }
              }

              if (count < 10) {
                client.println("OK");
                WiFi.mode(WIFI_STA);
                exit_but = true;

              }

            }

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();

  }
}
int firebaseErrorDetectNoPrint(void)//uses NTP & firebase
{
//  Firebase.setInt(firebaseData, username + "/keepAlive", timeClient.getSeconds());
  Error = firebaseData.httpCode();
  return (Error);
}
int firebaseErrorDetect(void)//uses NTP & firebase
{
  if ((currentmillis - ErrorDetect_previousMillis >= ErrorDetect_interval) || (currentmillis < ErrorDetect_previousMillis))
  {
    ErrorDetect_previousMillis = currentmillis;
    Firebase.setInt(firebaseData, username + "/keepAlive", timeClient.getSeconds());
    Error = firebaseData.httpCode();
    Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); Serial.println(Error);
  }
  return (Error);
}
// switches interrupt
void SW_S_ISR(void)
{
  if (setFlag_h == 1)
  {
    setFlag_l = 1;
  }
}
void SW_N_ISR(void)
{
  if (negFlag_h == 1)
  {
    negFlag_l = 1;
  }
}
void SW_P_ISR(void)
{
  if (posFlag_h == 1)
  {
    posFlag_l = 1;
  }
}
void millis_update(void)
{
  currentmillis = millis();
  if (currentmillis > 0xFFFFFFF0 ) //Reset every 49 //0xFFFFFFF0 overflow 49 days (0x05265C00 24h)
  {
    ESP.restart();
  }  
}

