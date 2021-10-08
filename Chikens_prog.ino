#include "Chikens_init.h"

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
  url += "&v="+CodeVersion;
  url += "&s=" + getChipId();
  WiFiClient client;
  httpUpdate.update(client, url, CodeVersion);
  //Serial.println("Done!..");
}
void sendDataToSheet(void)
{
  String DATA = String((float)Temperature1) + "|||" + String((float)Temperature2) + "|||" + String((float)Temperature3) + "|||" + String((float)Temperature) + "|||" + String((int)Humidity1) + "|||" + String((int)Humidity2) + "|||" + String((int)Humidity3) + "|||" + String((int)Humidity) + "|||" + String((int)gas) + "|||" + String((int)heaterA_status) + "|||" + String((int)heaterB_status) + "|||" + String((int)cooler_status) + "|||" + String((int)Light_Status) + "|||" + String((int)LED_Status) + "|||" + String((int)MinTemp_Trigger) + "|||" + String((int)MaxTemp_Trigger) + "|||" + String((int)MinVent_Trigger) + "|||" + String((int)MaxVent_Trigger) + "|||" + String((int)ResetFlag) + "|||" + String((int)Set_ForcedHA) + "|||" + String((int)Set_ForcedHB) + "|||" + String((int)Set_ForcedF);
  String url = server + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + DATA;  
  Serial.println(url);
  //Start to send data to IFTTT
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  http.begin(url); //HTTP

  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  // httpCode will be negative on error
  if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

}
void eeprom_read(int address, int stringLength)
{
  int x = 0;
  Serial.println("Starting Reading");
  for (int i = address; i < address + stringLength; i++, x++)
  {
    readeeprom[x] = EEPROM.read(i);
  }
  Serial.print(" readeeprom  ");
  Serial.println(readeeprom);
}
void eeprom_clear(int address, int stringLength)
{
  int x = 0;
  Serial.println("Starting Clearing");
  for (int i = address; i < address + stringLength; i++, x++)
  {
    EEPROM.put(i, "\n");
  }
  Serial.print("Commit : ");
  Serial.println(EEPROM.commit());
}
void eeprom_Write(int address, char*ptr, int stringLength)
{
  Serial.println("Starting Writing");
  for (int i = address, x = 0; i < (address + stringLength); i++, x++)
  {
    EEPROM.put(i, *(ptr + x));
  }
  Serial.print("Commit : ");
  Serial.println(EEPROM.commit());
}
void saveConfigCallback ()
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
void detectGas()
{
  gas = analogRead(GASA1);
  conductivity = round(((float)gas / 1023) * 100);
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
  Serial.print("conductivity: "); Serial.println(conductivity);
  Serial.print("quality: "); Serial.println(quality);
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

/********************************************** Setup Functions *********************************************************/
void wifiSetup(void)
{
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  WiFiManagerParameter custom_Host("Host", "Host", mqtt_server, stringLength);
  WiFiManagerParameter custom_Token("Token", "Token", output, stringLength);
  wifiManager.addParameter(&custom_Host);
  wifiManager.addParameter(&custom_Token);

  ///////////////connecting to WIFI/////////////////////////////
  Serial.println("Conecting....");

  wifiManager.setConfigPortalTimeout(ConfigPortalTimeout);
  wifiManager.autoConnect("Chickens control", "123456789");
  delay(500);
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  }
  else
  {
    Serial.print("Not Connected!!");
  } 
  Host  = custom_Host.getValue();
  Token = custom_Token.getValue();
}
void pinSetup(void)
{
  pinMode(GASA1, INPUT);
  pinMode(GASA2, INPUT);
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

  digitalWrite(Heater3, 1);
  digitalWrite(Heater4, 1);
  digitalWrite(Heater1, 0);
  digitalWrite(Heater2, 0);
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
    for (int i = 0; i < (Host.length()); i++)
    {
      Host_charArray[i] = Host[i];
    }
    for (int i = 0; i < (Token.length()); i++)
    {
      Token_charArray[i] = Token[i];
    }
    Serial.println("Host_charArray & Token_charArray");
    Serial.println(Host_charArray);
    Serial.println(Token_charArray);
    eeprom_clear(0,stringLength);
    eeprom_clear(100,stringLength);
    eeprom_Write(0, Host_charArray,stringLength);
    eeprom_Write(100, Token_charArray,stringLength);
  }
  else
  {
    eeprom_read(0,stringLength);
    for (int i = 0; i < stringLength; i++)
    {
      Host_charArray[i] = readeeprom[i];
    }
    eeprom_read(100,stringLength);
    for (int i = 0; i < stringLength; i++)
    {
      Token_charArray[i] = readeeprom[i];
    }
  }
}
void firbaseSetup(void)
{
  Serial.println("Host_charArray & Token_charArray");
  Serial.println(Host_charArray);
  Serial.println(Token_charArray);

  Firebase.begin(Host_charArray, Token_charArray);
  //it tells the project to reconnect to wifi when it's able to, but it keep spaming

  Firebase.get(firebaseData, "/Chickens/WhichHeater");
  WhichHeater = firebaseData.stringData();
  if ((WhichHeater != "A") || (WhichHeater != "B"))
    WhichHeater = "A";


  Error = firebaseData.httpCode();
  Error_prev = Error;
  if ((Error <= 0))
    Firebase.reconnectWiFi(true);
  else
    Firebase.reconnectWiFi(false);
}
void parameterSetup(void)
{
  heaterA_status = 0;
  heaterB_status = 1;
  MinVent_prev = EEPROMReadInt(200);
  MaxVent_prev = EEPROMReadInt(210);
  MaxTemp_prev = EEPROMReadInt(220);
  MinTemp_prev = EEPROMReadInt(230);

  MinVent_Trigger = EEPROMReadInt(200);
  MaxVent_Trigger = EEPROMReadInt(210);
  MaxTemp_Trigger = EEPROMReadInt(220);
  MinTemp_Trigger = EEPROMReadInt(230);
  Time_prev=timeClient.getMinutes();
}
void timeServerSetup(void)
{
  timeClient.begin();
  timeClient.setTimeOffset(7200);
}


