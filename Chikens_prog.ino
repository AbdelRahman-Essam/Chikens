#include "Chikens_init.h"


/********************************************** update Functions *********************************************************/
void WiFiCheck(void)
{
  if((currentmillis - WiFiCheck_previousMillis >= WiFiCheck_interval)||(currentmillis < WiFiCheck_previousMillis))
  {
    //Serial.println("will start WiFiCheck"); 
    WiFiCheck_previousMillis = currentmillis; 
    if ((Error <= 0))
    {
      LED_Status = ! LED_Status;
      digitalWrite(BlueLed, LED_Status);
    }
    if (((currentmillis - WiFi_Reconnect_previousMillis >= WiFi_Reconnect_interval)||(currentmillis < WiFi_Reconnect_previousMillis)) && (firebaseErrorDetectNoPrint() < 0))
    {
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
  if((currentmillis - CreditionalsConfig_previousMillis >= CreditionalsConfig_interval)||(currentmillis < CreditionalsConfig_previousMillis))
  { 
    //Serial.println("will start cred config");
    CreditionalsConfig_previousMillis = currentmillis; 
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW)
    {
      delay(2000);
      buttonState = digitalRead(buttonPin);
      if (buttonState == LOW)
      {
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
          set_access_point();
          while (1)
          {
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
void resetCheck(void)
{
  if((currentmillis - resetCheck_previousMillis >= resetCheck_interval)||(currentmillis < resetCheck_previousMillis))
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
  if((currentmillis - temp_previousMillis >= temp_interval)||(currentmillis < temp_previousMillis))
  { 
    //Serial.println("will start temp");
    temp_previousMillis = currentmillis; 
    int tempTotalNumber = 3;
    int humTotalNumber  = 3;
    Temperature1 = dht1.readTemperature(); // Gets the values of the temperature
    Humidity1 = dht1.readHumidity(); // Gets the values of the humidity
  
    Temperature2 = dht2.readTemperature(); // Gets the values of the temperature
    Humidity2 = dht2.readHumidity(); // Gets the values of the humidity
  
    Temperature3 = dht3.readTemperature(); // Gets the values of the temperature
    Humidity3 = dht3.readHumidity(); // Gets the values of the humidity
  
    /////////////////////////////////protection/////////////////////////////////////
    if (isnan(Temperature1))
    {
      Temperature1 = 00.0;
      tempTotalNumber--;
    }
    if (isnan(Temperature2))
    {
      Temperature2 = 00.0;
      tempTotalNumber--;
    }
    if (isnan(Temperature3))
    {
      Temperature3 = 00.0;
      tempTotalNumber--;
    }
    if (isnan(Humidity1))
    {
      Humidity1 = 00.0;
      humTotalNumber--;
    }
    if (isnan(Humidity2))
    {
      Humidity2 = 00.0;
      humTotalNumber--;
    }
    if (isnan(Humidity3))
    {
      Humidity3 = 00.0;
      humTotalNumber--;
    }
    Temperature = (( Temperature1 + Temperature2 + Temperature3) / tempTotalNumber);
    Humidity    = ((Humidity1 + Humidity2 + Humidity3) / humTotalNumber);
  
  
  
    val = (int)(Temperature * 10 + 0.5);
    Temperature = (float) val / 10;
    if (isnan(Temperature))
      Temperature = 00.0;
    val = (int)(Humidity * 10 + 0.5);
    Humidity = (float) val / 10;
    if (isnan(Humidity))
      Humidity = 00.0;
    if (Temperature > 80)
      Temperature = 80;
    if (Humidity > 120)
      Humidity = 120;
  
    Serial.print("Temperature: "); Serial.println(Temperature);
    Serial.print("Humidity: "); Serial.println(Humidity);
  }
}
void detectGas()
{
  if((currentmillis - gas_previousMillis >= gas_interval)||(currentmillis < gas_previousMillis))
  {
    //Serial.println("will start detectGas");
    gas_previousMillis = currentmillis;  
    gas = analogRead(GASA1);
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
  if((currentmillis - control_previousMillis >= control_interval)||(currentmillis < control_previousMillis))
  {
    //Serial.println("will start control");
    control_previousMillis = currentmillis;
    if (FanAuto=="ON")
    {
      if (((gas > MaxVent_Trigger) && (Temperature > MinTemp_Trigger)) || (Temperature > MaxTemp_Trigger + Temp_variance_Cool))
      {
        digitalWrite(Cooler, control_ON);
        cooler_status = "ON";
      }
      else if ((Temperature < MinTemp_Trigger) || ((gas < MinVent_Trigger)&&((Temperature < MaxTemp_Trigger))))
      {
        digitalWrite(Cooler, control_OFF);
        cooler_status = "OFF";
      }
    }
    else
    {
      if (Set_ManualF == "ON")
      {
        digitalWrite(Cooler, control_ON);
        cooler_status = "ON";
      }
      else if (Set_ManualF == "OFF")
      {
        digitalWrite(Cooler, control_OFF);
        cooler_status = "OFF";
      }
    }
  
    if ((heaterAauto == "OFF") && (heaterBAuto == "OFF"))
    {
      //HeaterA
      if(Set_ManualHA == "ON")
      {
      digitalWrite(Heater1, control_ON);
      digitalWrite(Heater2, control_ON);
      heaterA_status = Set_ManualHA;
      }
      else if(Set_ManualHA == "OFF")
      {
      digitalWrite(Heater1, control_OFF);
      digitalWrite(Heater2, control_OFF);
      heaterA_status = Set_ManualHA;
      }
      //HeaterB
      if(Set_ManualHB == "ON")
      {
      digitalWrite(Heater3, control_ON);
      digitalWrite(Heater4, control_ON);
      heaterB_status = Set_ManualHB;
      }
      else if(Set_ManualHA == "OFF")
      {
      digitalWrite(Heater3, control_OFF);
      digitalWrite(Heater4, control_OFF);
      heaterB_status = Set_ManualHB;
      }
    }
    else
    {
      if (Temperature <= MinTemp_Trigger)
      {
        //HeaterA
        digitalWrite(Heater1, control_ON);
        digitalWrite(Heater2, control_ON);
        //HeaterB
        digitalWrite(Heater3, control_ON);
        digitalWrite(Heater4, control_ON);
        heaterA_status = "ON";
        heaterB_status = "ON";
      }
      else if ((Temperature > MinTemp_Trigger) && (Temperature < MaxTemp_Trigger))
      {
        if (HeaterFlag == "OFF")
        {
          if ((WhichHeater == "A"))
          {
            //HeaterA
            digitalWrite(Heater1, control_OFF);
            digitalWrite(Heater2, control_OFF);
            heaterA_status = "OFF";
            //HeaterB
            digitalWrite(Heater3, control_ON);
            digitalWrite(Heater4, control_ON);
            heaterB_status = "ON";
            WhichHeater = "B";
            heaterB_Stime = currentmillis/(HeaterSwitch_interval);
          }
          else if ((WhichHeater == "B"))
          {
            //HeaterA
            digitalWrite(Heater1, control_ON);
            digitalWrite(Heater2, control_ON);
            heaterA_status = "ON";
            WhichHeater = "A";
            //HeaterB
            digitalWrite(Heater3, control_OFF);
            digitalWrite(Heater4, control_OFF);
            heaterB_status = "OFF";
            heaterA_Stime = currentmillis/(HeaterSwitch_interval);
          }
          HeaterFlag = "ON";
        }
        else
        {
          if (((heaterA_status == "ON") && (currentmillis/(HeaterSwitch_interval) != (heaterA_Stime))) || ((heaterB_status == "ON") && (currentmillis/(HeaterSwitch_interval) != (heaterB_Stime))))
          {
            HeaterFlag = "OFF";
          }
        }
      }
      else if (Temperature > MaxTemp_Trigger)
      {
        //HeaterA
        digitalWrite(Heater1, control_OFF);
        digitalWrite(Heater2, control_OFF);
        //HeaterB
        digitalWrite(Heater3, control_OFF);
        digitalWrite(Heater4, control_OFF);
        heaterA_status = "OFF";
        heaterB_status = "OFF";
      }
    }
    //Serial.println("will stop control");
  }
}
void firebaseStatments(void)
{
  if(((currentmillis - firebase_previousMillis >= firebase_interval*60000)||(currentmillis < firebase_previousMillis)) && (firebaseErrorDetectNoPrint() > 0))
  {
    firebase_previousMillis = currentmillis;
    Serial.println("will start firebase");
    int Stime=millis()/1000;
    Serial.print("Start Time ");Serial.println(Stime);
    /***************************************/
///////////////////////////// Getters ////////////////////////////
    if (WhichHeater_prev != WhichHeater) // the 3 is a number to avoid setting the 0 value to firebase
    {
      Serial.print("WhichHeater: "); Serial.println(WhichHeater);
      WhichHeater_prev = WhichHeater;
      Firebase.setString(firebaseData, username + "/Heaters/WhichHeater", WhichHeater);
    }
    if ((temp_prev1 != Temperature1)&&(Temperature1 > 3))
    {
      Serial.print("temp1: "); Serial.println(Temperature1);
      temp_prev1 = Temperature1;
      Firebase.setFloat(firebaseData, username + "/Temp/temp1", Temperature1);
    }
    if ((temp_prev2 != Temperature2)&&(Temperature2 > 3))
    {
      temp_prev2 = Temperature2;
      Serial.print("temp2: "); Serial.println(Temperature2);
      Firebase.setFloat(firebaseData, username + "/Temp/temp2", Temperature2);
    }
    if ((temp_prev3 != Temperature3)&&(Temperature3 > 3))
    {
      temp_prev3 = Temperature3;
      Serial.print("temp3: "); Serial.println(Temperature3);
      Firebase.setFloat(firebaseData, username + "/Temp/temp3", Temperature3);
    }
    if ((hum_prev1 != Humidity1)&&(Humidity1 > 3))
    {
      hum_prev1 = Humidity1;
      Serial.print("Humidity1: "); Serial.println(Humidity1);
      Firebase.setFloat(firebaseData, username + "/Hum/hum1", Humidity1);
    }
    if ((hum_prev2 != Humidity2)&&(Humidity2 > 3))
    {
      hum_prev2 = Humidity2;
      Serial.print("Humidity2: "); Serial.println(Humidity2);
      Firebase.setFloat(firebaseData, username + "/Hum/hum2", Humidity2);
    }
    if ((hum_prev3 != Humidity3)&&(Humidity3 > 3))
    {
      hum_prev3 = Humidity3;
      Serial.print("Humidity3: "); Serial.println(Humidity3);
      Firebase.setFloat(firebaseData, username + "/Hum/hum3", Humidity3);
    }
    if ((gas_prev != gas)&&(gas > 3))
    { 
      // the String will be produced in the android program
      gas_prev = gas;
      Serial.print("gas: "); Serial.println(gas);
      Firebase.setInt(firebaseData, username + "/airQuality", gas);
    }
    if (heaterA_status_prev != heaterA_status)
    {
      heaterA_status_prev = heaterA_status;
      Serial.print("heaterA_status: "); Serial.println(heaterA_status);
      Firebase.setString(firebaseData, username + "/Heaters/heaterA_status", heaterA_status);
      Firebase.setString(firebaseData, username + "/Heaters/Get_ManualHA", heaterA_status);// temporary because the android is working with this variable
    }
    if (heaterB_status_prev != heaterB_status)
    {
      heaterB_status_prev = heaterB_status;
      Serial.print("heaterB_status: "); Serial.println(heaterB_status);
      Firebase.setString(firebaseData, username + "/Heaters/heaterB_status", heaterB_status);
      Firebase.setString(firebaseData, username + "/Heaters/Get_ManualHB", heaterB_status);// temporary
    }
    if (cooler_status_prev != cooler_status)
    {
      cooler_status_prev = cooler_status;
      Serial.print("cooler_status: "); Serial.println(cooler_status);
      Firebase.setString(firebaseData, username + "/Heaters/Cooler_status", cooler_status);
      Firebase.setString(firebaseData, username + "/Heaters/Get_ManualF", cooler_status);// temporary
    }
    ///////////////////////////// Setters ////////////////////////////
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/Heaters/FanAuto");
    string =firebaseData.stringData();
    if ((FanAuto_prev != string)&&(string== "ON"||string== "OFF"))
    {
        Serial.println("FanAuto Changed!");
        FanAuto_prev = string;
        FanAuto = FanAuto_prev;
        Serial.print("FanAuto: "); Serial.println(FanAuto);
        Firebase.setString(firebaseData, username + "/Heaters/Get_FanAuto", FanAuto);
    }
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/Heaters/heaterAauto");  
    string =firebaseData.stringData();
    if ((heaterAauto_prev != string)&&(string== "ON"||string== "OFF"))
    {
        Serial.println("heaterAauto Changed!");
        heaterAauto_prev = string;
        heaterAauto = heaterAauto_prev;
        Serial.print("heaterAauto: "); Serial.println(heaterAauto);
        Firebase.setString(firebaseData, username + "/Heaters/Get_heaterAauto", heaterAauto);
    }
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/Heaters/heaterBAuto");
    string =firebaseData.stringData();
    if ((heaterBAuto_prev != string)&&(string== "ON"||string== "OFF"))
    {
        Serial.println("heaterBAuto Changed!");
        heaterBAuto_prev = string;
        heaterBAuto = heaterBAuto_prev;
        Serial.print("heaterBAuto: "); Serial.println(heaterBAuto);
        Firebase.setString(firebaseData, username + "/Heaters/Get_heaterBAuto", heaterBAuto);
    }
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/Heaters/Set_ManualHA");
    string =firebaseData.stringData();
    if ((Set_ManualHA_prev != string)&&(string== "ON"||string== "OFF"))
    {
        Serial.println("ManualHA Changed!");
        Set_ManualHA_prev = string;
        Set_ManualHA = Set_ManualHA_prev;
        Serial.print("Set_ManualHA: "); Serial.println(Set_ManualHA);
        //Firebase.setString(firebaseData, username + "/Heaters/Get_ManualHA", Set_ManualHA);// temporary
    }
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/Heaters/Set_ManualHB");
    string =firebaseData.stringData();
    if ((Set_ManualHB_prev != string)&&(string== "ON"||string== "OFF"))
    {
        Serial.println("ManualHB Changed!");
        Set_ManualHB_prev = string;
        Set_ManualHB = Set_ManualHB_prev;
        Serial.print("Set_ManualHB: "); Serial.println(Set_ManualHB);
        // Firebase.setString(firebaseData, username + "/Heaters/Get_ManualHB", Set_ManualHB);//temporary
    }
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/Heaters/Set_ManualF");
    string =firebaseData.stringData();
    if ((Set_ManualF_prev != string)&&(string== "ON"||string== "OFF"))
    {
        Serial.println("ManualF Changed!");
        Set_ManualF_prev = string;
        Set_ManualF = Set_ManualF_prev;
        Serial.print("Set_ManualF: "); Serial.println(Set_ManualF);
        //Firebase.setString(firebaseData, username + "/Heaters/Get_ManualF", Set_ManualF); //temporary
    }
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/resetFlag");
    string =firebaseData.stringData();
    if ((ResetFlag_prev != string)&&(string== "ON"||string== "OFF"))
    {
        Serial.println("ResetFlag Changed!");
        ResetFlag_prev = string;
        ResetFlag = ResetFlag_prev;
    }
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/Lights/Set_Led1");
    string =firebaseData.stringData();
    if ((LED_prev != string)&&(string== "ON"||string== "OFF"))
    {
        Serial.println("BlueLed Changed!");
        LED_prev = string;
        LED_Status_Str = LED_prev;
        Firebase.setString(firebaseData, username + "/Lights/Get_Led1", LED_Status_Str);
        if(LED_Status_Str=="ON")
          LED_Status=1;
        else if(LED_Status_Str=="OFF")
          LED_Status=0;
        digitalWrite(BlueLed, LED_Status);
    }
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/Lights/Set_Light");
    string =firebaseData.stringData();
    if ((Light_prev != string)&&(string== "ON"||string== "OFF"))
    {
        Serial.println("Light Changed!");
        Light_prev = string;
        Light_Status = Light_prev;
        Serial.print("Light_Status: "); Serial.println(Light_Status);
        if (Light_Status == "OFF")
          digitalWrite(Light, light_OFF);
        else if(Light_Status == "ON")
          digitalWrite(Light, light_ON);
        Firebase.setString(firebaseData, username + "/Lights/Get_Light", Light_Status);
    }
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/valueRanges/vent");
    string =firebaseData.stringData();
    if ((VentRange_prev != string))
    {
      VentRange_prev = string;
      String sbstring = VentRange_prev.substring(0, 3);
      MinVent_Trigger = sbstring.toInt();
      if (((MinVent_Trigger >= 5) && (MinVent_Trigger <= 1000))&&(EEPROMReadInt(300)!= MinVent_Trigger))
      {
        EEPROMWriteInt(300, MinVent_Trigger);
        Serial.print("MinVent_Trigger :"); Serial.println(MinVent_Trigger);      
      }
      else
        MinVent_Trigger = EEPROMReadInt(300);
      sbstring = "";
      sbstring = VentRange_prev.substring(4, 7);
      MaxVent_Trigger = sbstring.toInt();
      if (((MaxVent_Trigger >= 5) && (MaxVent_Trigger <= 1000))&&(EEPROMReadInt(310)!= MaxVent_Trigger))
      {
        EEPROMWriteInt(310, MaxVent_Trigger);
        Serial.print("MaxVent_Trigger :"); Serial.println(MaxVent_Trigger);
      }
      else
        MaxVent_Trigger = EEPROMReadInt(310);
    }
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/valueRanges/temp");
    string =firebaseData.stringData();
    if ((TempRange_prev != string))
    {
      TempRange_prev = string;
      String sbstring = TempRange_prev.substring(0, 3);
      MinTemp_Trigger = sbstring.toInt();
      if (((MinTemp_Trigger >= 5) && (MinTemp_Trigger <= 1000))&&(EEPROMReadInt(330)!= MinTemp_Trigger))
      {
        EEPROMWriteInt(330, MinTemp_Trigger);
        Serial.print("MinTemp_Trigger :"); Serial.println(MinTemp_Trigger);
      }
      else
        MinTemp_Trigger = EEPROMReadInt(330);
      sbstring = "";
      sbstring = TempRange_prev.substring(3, 7);
      MaxTemp_Trigger = sbstring.toInt();
      if (((MaxTemp_Trigger >= 5) && (MaxTemp_Trigger <= 1000))&&(EEPROMReadInt(320)!= MaxTemp_Trigger))
      {
        EEPROMWriteInt(320, MaxTemp_Trigger);
        Serial.print("MaxTemp_Trigger :"); Serial.println(MaxTemp_Trigger);
      }
      else
        MaxTemp_Trigger = EEPROMReadInt(320);
    }
    firebaseData.clearData();
    string.clear();
    Firebase.get(firebaseData, username + "/valueRanges/delay");
    string =firebaseData.stringData();
    if ((firebase_interval_prev != string))
    {
      firebase_interval_prev = string;
      firebase_interval = firebase_interval_prev.toInt();
      if (!((firebase_interval >= 0) && (firebase_interval <= 5000)))
      {
        firebase_interval = 1;
      Serial.print("firebase_interval :"); Serial.println(firebase_interval);
      }
    }
    timeClient.update();
    Firebase.setInt(firebaseData, username + "/Time/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, username + "/Time/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, username + "/Time/Seconds", timeClient.getSeconds());
    Serial.println("Done Firebase");
    int endtime =millis()/1000 - Stime;
    Serial.print("End Time ");Serial.println(endtime);
  }
}
void gooogleSheetStatments(void)
{
  if(((currentmillis - googleSheet_previousMillis >= googleSheet_interval*60000)||(currentmillis < googleSheet_previousMillis)) && (firebaseErrorDetectNoPrint() > 0))
  {
    googleSheet_previousMillis = currentmillis;
    Serial.println("will start googleSheets");
    sendDataToSheet();
  }
}
void timeUpgrade(void)
{
  if(((currentmillis - timeUpgrade_previousMillis >= timeUpgrade_interval)||(currentmillis < timeUpgrade_previousMillis)) && (firebaseErrorDetectNoPrint() > 0))
  {
    //Serial.println("will start timeupgrade");
    timeUpgrade_previousMillis = currentmillis;
    timeClient.update();
    timeClient.getSeconds();
    timeClient.getMinutes();
    timeClient.getHours();
  }
}
void RFID_Read(boolean RFID_Success)
{
//  if(((currentmillis - RFID_Read_previousMillis >= RFID_Read_interval)||(currentmillis < RFID_Read_previousMillis)) && (firebaseErrorDetectNoPrint() > 0))
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
  if((currentmillis - serial_previousMillis >= serial_interval)||(currentmillis < serial_previousMillis))
  {
    serial_previousMillis = currentmillis;
    Serial.println("\n\n\n\n\n\n\n\n\n\nSerialPrint\n\n");
    Serial.print("cooler_status: "); Serial.println(cooler_status);
    Serial.print("heaterA_status: "); Serial.println(heaterA_status);
    Serial.print("heaterB_status: "); Serial.println(heaterB_status);
    Serial.print("WhichHeater: "); Serial.println(WhichHeater);
//    Serial.print("Temperature1: "); Serial.println(Temperature1);
//    Serial.print("Temperature2: "); Serial.println(Temperature2);
//    Serial.print("Temperature3: "); Serial.println(Temperature3);
//    Serial.print("Temperature: "); Serial.println(Temperature);
//    Serial.print("Humidity: "); Serial.println(Humidity);
//    Serial.print("Humidity1: "); Serial.println(Humidity1);
//    Serial.print("Humidity2: "); Serial.println(Humidity2);
//    Serial.print("Humidity3: "); Serial.println(Humidity3);
//    Serial.print("gas: "); Serial.println(gas);
    Serial.print("Set_ManualHA: "); Serial.println(Set_ManualHA);
    Serial.print("Set_ManualHB: "); Serial.println(Set_ManualHB);
    Serial.print("Set_ManualF: "); Serial.println(Set_ManualF);
    Serial.print("ResetFlag: "); Serial.println(ResetFlag);
    Serial.print("LED_Status: "); Serial.println(LED_Status);
    Serial.print("Light_Status: "); Serial.println(Light_Status);
//    Serial.print("MinVent_Trigger: "); Serial.println(MinVent_Trigger);
//    Serial.print("MaxVent_Trigger: "); Serial.println(MaxVent_Trigger);
//    Serial.print("MinTemp_Trigger: "); Serial.println(MinTemp_Trigger);
//    Serial.print("MaxTemp_Trigger: "); Serial.println(MaxTemp_Trigger);
//    Serial.print("firebase_interval: "); Serial.println(firebase_interval);
    Serial.print("FanAuto: "); Serial.println(FanAuto);
    Serial.print("heaterAauto: "); Serial.println(heaterAauto);
    Serial.print("heaterBAuto: "); Serial.println(heaterBAuto);
//    Serial.print("timeClient.getSeconds(): "); Serial.println(timeClient.getSeconds());
//    Serial.print("timeClient.getMinutes(): "); Serial.println(timeClient.getMinutes());
//    Serial.print("timeClient.getHours(): "); Serial.println(timeClient.getHours());
  }
}
void LCD_weather() 
{
  if(((currentmillis - LCD_previousMillis >= LCD_interval)||(currentmillis < LCD_previousMillis)))
  {
    Serial.println("will start LCD");
    lcd.clear(); //clear lcd to print next vlaues
    LCD_previousMillis = currentmillis;
    line1 = String("Tem:") + String(" ") + String("Hum:") + String(" ") + String("Gas:") + String(" ");
    line2 = String(int(Temperature)) + "." + String((int(Temperature * 10)) % 10) + " " + String(int(Humidity)) + "." + String((int(Humidity * 10)) % 10) + " " + String(int(gas));
    // set cursor to first column, first row
    lcd.setCursor(0, 0);
    // print line1 in first row
    lcd.print(line1);
    // set cursor to first column, second row
    lcd.setCursor(0, 1);
    lcd.print(line2);
    // set cursor to second row and sixteenth column to print wifi connection
    lcd.setCursor(15, 1);
    if(WiFi.status() != WL_CONNECTED) lcd.write(2);
    else lcd.write(1);
  }
}


/********************************************** Init Functions *********************************************************/
void pinSetup(void)
{
  pinMode(GASA1, INPUT);
  //pinMode(GASA2, INPUT);
  pinMode(DHT1Pin, INPUT);
  pinMode(DHT2Pin, INPUT);
  pinMode(DHT3Pin, INPUT);
  pinMode(Heater1, OUTPUT);
  pinMode(Heater2, OUTPUT);
  pinMode(Heater3, OUTPUT);
  pinMode(Heater4, OUTPUT);
  pinMode(Cooler, OUTPUT);
  pinMode(BlueLed, OUTPUT);
  pinMode(Light, OUTPUT);
  pinMode(buttonPin, INPUT);

  digitalWrite(Heater3, control_ON);
  digitalWrite(Heater4, control_ON);
  digitalWrite(Heater1, control_OFF);
  digitalWrite(Heater2, control_OFF);
}
void DHTSetup(void)
{
  dht1.begin();
  dht2.begin();
  dht3.begin();
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
    eeprom_Write(100, password.c_str(), stringLength);
    eeprom_Write(200, username.c_str(), stringLength);
  }
  else
  {
    ssid = eeprom_read(0, stringLength);
    password = eeprom_read(100, stringLength);
    username = eeprom_read(200, stringLength);
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
  //it tells the project to reconnect to wifi when it's able to, but it keep spaming
  Firebase.get(firebaseData, username + "/Heaters/whichHeater");
  WhichHeater = firebaseData.stringData();
  if ((WhichHeater != "A") || (WhichHeater != "B"))
    WhichHeater = "A";


  Error = firebaseData.httpCode();
  Error_prev = Error;
  Firebase.reconnectWiFi(false);
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
}
void parameterSetup(void)
{
  heaterA_status = "OFF";
  heaterB_status = "ON";
  
  MinVent_Trigger = EEPROMReadInt(300);
  MaxVent_Trigger = EEPROMReadInt(310);
  MaxTemp_Trigger = EEPROMReadInt(320);
  MinTemp_Trigger = EEPROMReadInt(330);
  
  Serial.print("MinVent_Trigger ");Serial.println(MinVent_Trigger);
  Serial.print("MaxVent_Trigger ");Serial.println(MaxVent_Trigger);
  Serial.print("MinTemp_Trigger ");Serial.println(MinTemp_Trigger);
  Serial.print("MaxTemp_Trigger ");Serial.println(MaxTemp_Trigger);
}
void UpdateCheck(void)
{
  firebaseData.clearData();     
  Firebase.get(firebaseData, "/Admin/updateCode");
  string = " ";
  string = firebaseData.stringData();
  if (UpdateCode_prev != string)
  {
    if(string == "ON"||string == "OFF")
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
  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  byte wifi_customChar[8] = {B00000, B00000, B00000, B00000, B00001, B00011, B00111, B01111};
  byte no_wifi[8] ={B00000, B10100, B01000, B10100, B0001, B00011, B00111, B01111};
  lcd.createChar(1 , wifi_customChar);   //Creating custom characters in CG-RAM
  lcd.createChar(2, no_wifi );
  // maximum character can be stored in LCD is 8
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
  String DATA = String(String((float)Temperature) + "|||" + String((int)Humidity) + "|||" + String((int)gas) + "|||" + (float)Temperature1) + "|||" + String((float)Temperature2) + "|||" + String((float)Temperature3) + "|||" + String((int)Humidity1) + "|||" + String((int)Humidity2) + "|||" + String((int)Humidity3) + "|||" + heaterA_status + "|||" + heaterB_status + "|||" + cooler_status + "|||" + Light_Status + "|||" + LED_Status_Str + "|||" + String((int)MinTemp_Trigger) + "|||" + String((int)MaxTemp_Trigger) + "|||" + String((int)MinVent_Trigger) + "|||" + String((int)MaxVent_Trigger) + "|||" + ResetFlag + "|||" + UpdateCode + "|||" + Set_ManualHA + "|||" + Set_ManualHB + "|||" + Set_ManualF + "|||" + FanAuto + "|||" + heaterAauto + "|||" + heaterBAuto + "|||" + String((unsigned int)heaterA_Stime) + "|||" + String((unsigned int)heaterB_Stime) + "|||" + WhichHeater;     
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
  Firebase.setInt(firebaseData, username + "/keepAlive", timeClient.getSeconds());
  Error = firebaseData.httpCode();
  return (Error);
}
int firebaseErrorDetect(void)//uses NTP & firebase
{
  Firebase.setInt(firebaseData, username + "/keepAlive", timeClient.getSeconds());
  Error = firebaseData.httpCode();
  Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); Serial.println(Error);
  //if ((Error != Error_prev) && (Error != 200))  ResetFlag = 1;
  return (Error);
}




