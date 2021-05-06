// need to find away to clear string

#include <WiFi.h>
#include"FirebaseESP32.h"
#include <WiFiManager.h>
#include <EEPROM.h>
#include "DHT.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Chickens_priv.h"
#include "config.h"
#include <HTTPClient.h>
#include <HTTPUpdate.h>

String CodeVersion ="1.0.0.1";

#define DHTTYPE DHT11
DHT dht1(DHT1Pin, DHTTYPE);
DHT dht2(DHT2Pin, DHTTYPE);
DHT dht3(DHT3Pin, DHTTYPE);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
FirebaseData firebaseData;
WiFiManager  wifiManager;

String getChipId()
{
  String ChipIdHex = String((uint32_t)(ESP.getEfuseMac() >> 32), HEX);
  ChipIdHex += String((uint32_t)ESP.getEfuseMac(), HEX);
  return ChipIdHex;
}
void update()
{
  ////Serial.println("updating..");
  String url = "http://otadrive.com/deviceapi/update?";
  url += "k=af436368-9238-4e59-8b71-f4c764e349a7";
  url += "&v="+CodeVersion;
  url += "&s=" + getChipId();
  WiFiClient client;
  httpUpdate.update(client, url, CodeVersion);
  ////Serial.println("Done!..");
}
void sendDataToSheet(void)
{
  String DATA = String((float)Temperature1) + "|||" + String((float)Temperature2) + "|||" + String((float)Temperature3) + "|||" + String((float)Temperature) + "|||" + String((int)Humidity1) + "|||" + String((int)Humidity2) + "|||" + String((int)Humidity3) + "|||" + String((int)Humidity) + "|||" + String((int)gas) + "|||" + String((int)heaterA_status) + "|||" + String((int)heaterB_status) + "|||" + String((int)cooler_status) + "|||" + String((int)Light_Status) + "|||" + String((int)LED_Status) + "|||" + String((int)MinTemp_Trigger) + "|||" + String((int)MaxTemp_Trigger) + "|||" + String((int)MinVent_Trigger) + "|||" + String((int)MaxVent_Trigger) + "|||" + String((int)ResetFlag) + "|||" + String((int)Set_ForcedHA) + "|||" + String((int)Set_ForcedHB) + "|||" + String((int)Set_ForcedF);
  String url = server + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + DATA;  
  //Serial.println(url);
  //Start to send data to IFTTT
  HTTPClient http;
  //Serial.print("[HTTP] begin...\n");
  http.begin(url); //HTTP

  //Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  // httpCode will be negative on error
  if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      //Serial.println(payload);
    }
  } else {
    //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

}
void eeprom_read(int address)
{
  int x = 0;
  //Serial.println("Starting Reading");
  for (int i = address; i < address + 40; i++, x++)
  {
    readeeprom[x] = EEPROM.read(i);
  }
  //Serial.print(" readeeprom  ");
  //Serial.println(readeeprom);
}
void eeprom_clear(int address)
{
  int x = 0;
  //Serial.println("Starting Clearing");
  for (int i = address; i < address + 40; i++, x++)
  {
    EEPROM.put(i, "\n");
  }
  //Serial.print("Commit : ");
  //Serial.println(EEPROM.commit());
}
void eeprom_Write(int address, char*ptr)
{
  //Serial.println("Starting Writing");
  for (int i = address, x = 0; i < (address + 40); i++, x++)
  {
    EEPROM.put(i, *(ptr + x));
  }
  //Serial.print("Commit : ");
  //Serial.println(EEPROM.commit());
}
void saveConfigCallback ()
{
  //Serial.println("Should save config");
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
  //Serial.print("gas: "); //Serial.println(gas);
  //Serial.print("conductivity: "); //Serial.println(conductivity);
  //Serial.print("quality: "); //Serial.println(quality);
}
void EEPROMWriteInt(int p_address, int p_value)
{
  byte lowByte = ((p_value >> 0) & 0xFF);
  byte highByte = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, lowByte);
  EEPROM.write(p_address + 1, highByte);
  //Serial.print("Commit : ");
  //Serial.println(EEPROM.commit());
}
unsigned int EEPROMReadInt(int p_address)
{
  byte lowByte = EEPROM.read(p_address);
  byte highByte = EEPROM.read(p_address + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}
void setup()
{
  /////////////////////////////////////////////////////////////////////

  //wifiManager.resetSettings();

  ////////////////////////////Reading from EEPROM//////////////////////
  Serial.begin(115200);
  dht1.begin();
  dht2.begin();
  dht3.begin();

  //Serial.println("\n\n\n\nVoid Setup\n\n\n\n");
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
  heaterA_status = 0;
  heaterB_status = 1;
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    //Serial.println("failed to initialise EEPROM"); delay(1000000);
    ESP.restart();
  }

  //Setting the custom parameters
  //Serial.println("//Setting the custom parameters");

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  WiFiManagerParameter custom_Host("Host", "Host", mqtt_server, 40);
  WiFiManagerParameter custom_Token("Token", "Token", output, 40);
  wifiManager.addParameter(&custom_Host);
  wifiManager.addParameter(&custom_Token);

  ///////////////connecting to WIFI/////////////////////////////

  //Serial.println("Conecting....");

  wifiManager.setConfigPortalTimeout(30);
  wifiManager.autoConnect("Chickens control", "123456789");
  delay(500);
  if (WiFi.status() == WL_CONNECTED)
  {
    //Serial.print("Connected with IP: ");
    //Serial.println(WiFi.localIP());
    //Serial.println();
  }
  else
  {
    //Serial.print("Not Connected!!");
  }


  /////////////////////////////////////////////////////////////
  if (shouldSaveConfig == true)
  {
    Host  = custom_Host.getValue();
    Token = custom_Token.getValue();
    for (int i = 0; i < (Host.length()); i++)
    {
      Host_charArray[i] = Host[i];
    }
    for (int i = 0; i < (Token.length()); i++)
    {
      Token_charArray[i] = Token[i];
    }
    //Serial.println("Host_charArray & Token_charArray");
    //Serial.println(Host_charArray);
    //Serial.println(Token_charArray);
    eeprom_clear(0);
    eeprom_clear(100);
    eeprom_Write(0, Host_charArray);
    eeprom_Write(100, Token_charArray);
  }
  else
  {
    eeprom_read(0);
    for (int i = 0; i < 45; i++)
    {
      Host_charArray[i] = readeeprom[i];
    }
    eeprom_read(100);
    for (int i = 0; i < 45; i++)
    {
      Token_charArray[i] = readeeprom[i];
    }
  }

  ///////////////////////////////////////////////////////////////////////////////////////////
  //Serial.println("Host_charArray & Token_charArray");
  //Serial.println(Host_charArray);
  //Serial.println(Token_charArray);

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

  timeClient.begin();
  timeClient.setTimeOffset(7200);

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
////////////////////////////////////////////////////////////////////////
void loop()
{
  //Serial.println("inside void loop");
  //delay(TimeDelay * 1000);

  Error = firebaseData.httpCode();
  //Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); //Serial.println(Error);
  //Firebase.pathExist(firebaseData, "/Chickens");
  //////////////////////////////////////////////////TempFn///////////////////////////////////////////////////////
  //DHT data
  Temperature1 = dht1.readTemperature(); // Gets the values of the temperature
  Humidity1 = dht1.readHumidity(); // Gets the values of the humidity
  //Serial.print("temp1: "); //Serial.println(Temperature1);
  Temperature2 = dht2.readTemperature(); // Gets the values of the temperature
  Humidity2 = dht2.readHumidity(); // Gets the values of the humidity
  //Serial.print("temp2: "); //Serial.println(Temperature2);

  Temperature3 = dht3.readTemperature(); // Gets the values of the temperature
  Humidity3 = dht3.readHumidity(); // Gets the values of the humidity
  //Serial.print("temp3: "); //Serial.println(Temperature3);


  /////////////////////////////////protection/////////////////////////////////////
  if (isnan(Temperature1))
    Temperature1 = 00.0;
  if (isnan(Temperature2))
    Temperature2 = 00.0;
  if (isnan(Temperature3))
    Temperature3 = 00.0;
  if (isnan(Humidity1))
    Humidity1 = 00.0;
  if (isnan(Humidity2))
    Humidity2 = 00.0;
  if (isnan(Humidity3))
    Humidity3 = 00.0;

  Temperature = ( Temperature1 + Temperature2 + Temperature3) / 3;
  Humidity    = (Humidity1 + Humidity2 + Humidity3) / 3;
  detectGas();

  val = (int)(Temperature * 10 + 0.5);
  Temperature = (float) val / 10;
  if (isnan(Temperature))
    Temperature = 00.0;
  val = (int)(Humidity * 10 + 0.5);
  Humidity = (float) val / 10;
  if (isnan(Humidity))
    Humidity = 00.0;
  if (isnan(conductivity))
    conductivity = 00.0;


  //variables needed to be uploaded to firebase     : Temperature1,Temperature2,Temperature3,Humidity1,Humidity2,Humidity3,heaterA_status,heaterB_status,AirQuality,conductivity
  //                                                  Temperature,Humidity,MinVent_Trigger,MaxVent_Trigger
  //variables needed to be downloaded from firebase : MinTemp_Trigger, MaxTemp_Trigger,delayTime,ResetFlag
  if (ResetFlag == 1)
  {
    Firebase.setInt(firebaseData, "/Chickens/ResetFlag", 0);
    ESP.restart();
  }
  if ((gas > MaxVent_Trigger) && (Temperature > MinTemp_Trigger))
  {
    digitalWrite(Cooler, 1);
    cooler_status = 1;
  }
  else if (Temperature > MaxTemp_Trigger + 5)
  {
    digitalWrite(Cooler, 1);
    cooler_status = 1;
  }
  else if (Set_ForcedF == 1)
  {
    digitalWrite(Cooler, 1);
    cooler_status = 1;
  }
  else if ((Temperature < MinTemp_Trigger) || (gas < MinVent_Trigger))
  {
    digitalWrite(Cooler, 0);
    cooler_status = 0;
  }
  if (Set_ForcedHA == 1)
  {
    //HeaterA
    digitalWrite(Heater1, 1);
    digitalWrite(Heater2, 1);
    heaterA_status = 1;
  }
  else if (Set_ForcedHB == 1)
  {
    //HeaterB
    digitalWrite(Heater3, 1);
    digitalWrite(Heater4, 1);
    heaterB_status = 1;
  }
  else if (Temperature <= MinTemp_Trigger)
  {
    //HeaterA
    digitalWrite(Heater1, 1);
    digitalWrite(Heater2, 1);
    //HeaterB
    digitalWrite(Heater3, 1);
    digitalWrite(Heater4, 1);
    heaterA_status = 1;
    heaterB_status = 1;
  }
  else if ((Temperature > MinTemp_Trigger) && (Temperature < MaxTemp_Trigger))
  {
    if (HeaterFlag == 0)
    {
      if (WhichHeater == "A")
      {
        //HeaterA
        digitalWrite(Heater1, 0);
        digitalWrite(Heater2, 0);
        heaterA_status = 0;
        //HeaterB
        digitalWrite(Heater3, 1);
        digitalWrite(Heater4, 1);
        heaterB_status = 1;
        WhichHeater = "B";
        timeClient.update();
        heaterB_Stime = timeClient.getHours();
      }
      else
      {
        //HeaterA
        digitalWrite(Heater1, 1);
        digitalWrite(Heater2, 1);
        heaterA_status = 1;
        WhichHeater = "A";
        //HeaterB
        digitalWrite(Heater3, 0);
        digitalWrite(Heater4, 0);
        heaterB_status = 0;
        timeClient.update();
        heaterA_Stime = timeClient.getHours();
      }
      HeaterFlag = 1;
    }
    else
    {
      timeClient.update();
      if (((heaterA_status == 1) && (timeClient.getHours() != (heaterA_Stime))) || ((heaterB_status == 1) && (timeClient.getHours() != (heaterB_Stime))))
      {
        HeaterFlag = 0;
      }
    }
  }
  else if (Temperature > MaxTemp_Trigger)
  {
    //HeaterA
    digitalWrite(Heater1, 0);
    digitalWrite(Heater2, 0);
    //HeaterB
    digitalWrite(Heater3, 0);
    digitalWrite(Heater4, 0);
    heaterA_status = 0;
    heaterB_status = 0;
  }
  int xe = 0;
  //  //////////////////////////////////////////////////firebase///////////////////////////////////////////////////////
    //Serial.println("will start firebase");
  if((timeClient.getMinutes()>=(TimeDelay+Time_prev))||(timeClient.getMinutes()<Time_prev))
  {
    Time_prev=timeClient.getMinutes();
    Error = firebaseData.httpCode();
  //Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); //Serial.println(Error);
  if (WhichHeater_prev != WhichHeater)
  {
    //Serial.print("WhichHeater: "); //Serial.println(WhichHeater);
    WhichHeater_prev = WhichHeater;
    Firebase.setString(firebaseData, "/Chickens/WhichHeater", WhichHeater);
  }
  if (temp_prev != Temperature)
  {
    //Serial.print("Temperature: "); //Serial.println(Temperature);
    temp_prev = Temperature;
    Firebase.setFloat(firebaseData, "/Chickens/Temperature", Temperature);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  if (temp_prev1 != Temperature1)
  {
    //Serial.print("temp1: "); //Serial.println(Temperature1);
    temp_prev1 = Temperature1;
    Firebase.setFloat(firebaseData, "/Chickens/Temp1", Temperature1);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Error = firebaseData.httpCode();
  if ((Error != Error_prev)&&(Error!=200)) ESP.restart();
  //Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); //Serial.println(Error);
  if (temp_prev2 != Temperature2)
  {
    temp_prev2 = Temperature2;
    //Serial.print("temp2: "); //Serial.println(Temperature2);
    Firebase.setFloat(firebaseData, "/Chickens/Temp2", Temperature2);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  if (temp_prev3 != Temperature3)
  {
    temp_prev3 = Temperature3;
    //Serial.print("temp3: "); //Serial.println(Temperature3);
    Firebase.setFloat(firebaseData, "/Chickens/Temp3", Temperature3);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Error = firebaseData.httpCode();
  if ((Error != Error_prev)&&(Error!=200)) ESP.restart();
  //Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); //Serial.println(Error);
  if (hum_prev != Humidity)
  {
    hum_prev = Humidity;
    //Serial.print("Humidity: "); //Serial.println(Humidity);
    Firebase.setFloat(firebaseData, "/Chickens/Humidity", Humidity);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  if (hum_prev1 != Humidity1)
  {
    hum_prev1 = Humidity1;
    //Serial.print("Humidity1: "); //Serial.println(Humidity1);
    Firebase.setFloat(firebaseData, "/Chickens/Hum1", Humidity1);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  if (hum_prev2 != Humidity2)
  {
    hum_prev2 = Humidity2;
    //Serial.print("Humidity2: "); //Serial.println(Humidity2);
    Firebase.setFloat(firebaseData, "/Chickens/Hum2", Humidity2);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  if (hum_prev3 != Humidity3)
  {
    hum_prev3 = Humidity3;
    //Serial.print("Humidity3: "); //Serial.println(Humidity3);
    Firebase.setFloat(firebaseData, "/Chickens/Hum3", Humidity3);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Error = firebaseData.httpCode();
  if ((Error != Error_prev)&&(Error!=200)) ESP.restart();
  //Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); //Serial.println(Error);
  if (conductivity_prev != gas)
  { // the String will be produced in the android program
    conductivity_prev = conductivity;
    //Serial.print("gas: "); //Serial.println(gas);
    Firebase.setFloat(firebaseData, "/Chickens/AirQuality", gas);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  if (heaterA_status_prev != heaterA_status)
  {
    heaterA_status_prev = heaterA_status;
    //Serial.print("heaterA_status: "); //Serial.println(heaterA_status);
    Firebase.setInt(firebaseData, "/Chickens/HeaterA_status", heaterA_status);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Error = firebaseData.httpCode();
  //Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); //Serial.println(Error);
  if (heaterB_status_prev != heaterB_status)
  {
    heaterB_status_prev = heaterB_status;
    //Serial.print("heaterB_status: "); //Serial.println(heaterB_status);
    Firebase.setInt(firebaseData, "/Chickens/HeaterB_status", heaterB_status);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  if (cooler_status_prev != cooler_status)
  {
    cooler_status_prev = cooler_status;
    //Serial.print("cooler_status: "); //Serial.println(cooler_status);
    Firebase.setInt(firebaseData, "/Chickens/Cooler_status", cooler_status);
  }
  /////////////////////////////////////////////////////////
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Error = firebaseData.httpCode();
  if ((Error != Error_prev)&&(Error!=200)) ESP.restart();
  //Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); //Serial.println(Error);
  Firebase.get(firebaseData, "/Chickens/Set_ForcedHA");
  string = " ";
  string = firebaseData.stringData();
  if (Set_ForcedHA_prev != string)
  {
    //Serial.println("ForcedHA Changed!");
    Set_ForcedHA_prev = string;
    Set_ForcedHA = Set_ForcedHA_prev.toInt();
    if (!((Set_ForcedHA == 1) || (Set_ForcedHA == 0)))
      Set_ForcedHA = 0;
    //Serial.print("Set_ForcedHA: "); //Serial.println(Set_ForcedHA);
    Firebase.setInt(firebaseData, "/Chickens/Get_ForcedHA", Set_ForcedHA);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Error = firebaseData.httpCode();
  if ((Error != Error_prev)&&(Error!=200)) ESP.restart();
  //Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); //Serial.println(Error);
  Firebase.get(firebaseData, "/Chickens/Set_ForcedHB");
  string = " ";
  string = firebaseData.stringData();
  if (Set_ForcedHB_prev != string)
  {
    //Serial.println("ForcedHB Changed!");
    Set_ForcedHB_prev = string;
    Set_ForcedHB = Set_ForcedHB_prev.toInt();
    if (!((Set_ForcedHB == 1) || (Set_ForcedHB == 0)))
      Set_ForcedHB = 0;
    //Serial.print("Set_ForcedHB: "); //Serial.println(Set_ForcedHB);
    Firebase.setInt(firebaseData, "/Chickens/Get_ForcedHB", Set_ForcedHB);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Firebase.get(firebaseData, "/Chickens/Set_ForcedF");
  string = " ";
  string = firebaseData.stringData();
  if (Set_ForcedF_prev != string)
  {
    //Serial.println("ForcedF Changed!");
    Set_ForcedF_prev = string;
    Set_ForcedF = Set_ForcedF_prev.toInt();
    if (!((Set_ForcedF == 1) || (Set_ForcedF == 0)))
      Set_ForcedF = 0;
    //Serial.print("Set_ForcedF: "); //Serial.println(Set_ForcedF);
    Firebase.setInt(firebaseData, "/Chickens/Get_ForcedF", Set_ForcedF);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Error = firebaseData.httpCode();
  if ((Error != Error_prev)&&(Error!=200)) ESP.restart();
  Firebase.get(firebaseData, "/Chickens/ResetFlag");
  string = " ";
  string = firebaseData.stringData();
  if (ResetFlag_prev != string)
  {
    //Serial.println("ResetFlag Changed!");
    ResetFlag_prev = string;
    ResetFlag = ResetFlag_prev.toInt();
    if (!((ResetFlag == 1) || (ResetFlag == 0)))
      ResetFlag = 0;
  }  
  Firebase.get(firebaseData, "/Chickens/UpdateCode");
  string = " ";
  string = firebaseData.stringData();
  if (UpdateCode_prev != string)
  {
    UpdateCode_prev = string;
    UpdateCode = UpdateCode_prev.toInt();
    if(UpdateCode==1)
    {
    Firebase.setInt(firebaseData, "/Chickens/UpdateCode", 0);
    UpdateCode=0;
    update();
    }
    if (!((UpdateCode == 1) || (UpdateCode == 0)))
      UpdateCode = 0;
  }
  Firebase.get(firebaseData, "/Chickens/Set_Led1");
  string = " ";
  string = firebaseData.stringData();
  if (c != string)
  {
    //Serial.println("BlueLed Changed!");
    c = string;
    LED_Status = c.toInt();
    if (!((LED_Status == 1) || (LED_Status == 0)))
      LED_Status = 0;
    Firebase.setInt(firebaseData, "/Chickens/Get_Led1", LED_Status);
    digitalWrite(BlueLed, LED_Status);
  }
  ///////////////////////////////////////////////////////////////////////////////////////
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Error = firebaseData.httpCode();
  if ((Error != Error_prev)&&(Error!=200)) ESP.restart();
  //Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); //Serial.println(Error);
  Firebase.get(firebaseData, "/Chickens/Set_Light");
  string = " ";
  string = firebaseData.stringData();
  if (Light_prev != string)
  {
    //Serial.println("Light Changed!");
    Light_prev = string;
    Light_Status = string.toInt();
    //Serial.print("Light_Status: "); //Serial.println(Light_Status);
    if ((Light_Status != 0))
      Light_Status = 1;
    //Serial.print("Light_Status: "); //Serial.println(Light_Status);

    Firebase.setInt(firebaseData, "/Chickens/Get_Light", Light_Status);
    digitalWrite(Light, !(Light_Status));
  }
  //////////////////////////////////////////////////////////////////////////////////////////////
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Firebase.get(firebaseData, "/Chickens/MinVent");
  string = " ";
  string = firebaseData.stringData();
  if (MinVent_prev != string)
  {
    MinVent_prev = string;
    MinVent_Trigger = MinVent_prev.toInt();
    if (((MinVent_Trigger >= 50) && (MinVent_Trigger <= 800)))
      EEPROMWriteInt(200, MinVent_Trigger);
    if (!((MinVent_Trigger >= 50) && (MinVent_Trigger <= 800)))
      MinVent_Trigger = EEPROMReadInt(200);
    //Serial.print("MinVent_Trigger :"); //Serial.println(MinVent_Trigger);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Error = firebaseData.httpCode();
  if ((Error != Error_prev)&&(Error!=200)) ESP.restart();
  //Serial.print("HTTPC_ERROR_NOT_CONNECTED : "); //Serial.println(Error);
  Firebase.get(firebaseData, "/Chickens/MaxVent");
  string = " ";
  string = firebaseData.stringData();
  if (MaxVent_prev != string)
  {
    MaxVent_prev = string;
    MaxVent_Trigger = MaxVent_prev.toInt();
    //Serial.print("MaxVent_Trigger before :"); //Serial.println(MaxVent_Trigger);
    if (((MaxVent_Trigger >= 50) && (MaxVent_Trigger <= 800)))
      EEPROMWriteInt(210, MaxVent_Trigger);
    if (!((MaxVent_Trigger >= 50) && (MaxVent_Trigger <= 800)))
      MaxVent_Trigger = EEPROMReadInt(210);
    //Serial.print("MaxVent_Trigger :"); //Serial.println(MaxVent_Trigger);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Firebase.get(firebaseData, "/Chickens/MaxTemp");
  string = " ";
  string = firebaseData.stringData();
  if (MaxTemp_prev != string)
  {
    MaxTemp_prev = string;
    MaxTemp_Trigger = MaxTemp_prev.toInt();
      //Serial.print("MaxTemp_Trigger before :"); //Serial.println(MaxTemp_Trigger);
    if (((MaxTemp_Trigger >= 5) && (MaxTemp_Trigger <= 50)))
      EEPROMWriteInt(220, MaxTemp_Trigger);
    if (!((MaxTemp_Trigger >= 5) && (MaxTemp_Trigger <= 50)))
      MaxTemp_Trigger = EEPROMReadInt(220);
    //Serial.print("MaxTemp_Trigger :"); //Serial.println(MaxTemp_Trigger);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Firebase.get(firebaseData, "/Chickens/MinTemp");
  string = " ";
  string = firebaseData.stringData();
  if (MinTemp_prev != string)
  {
    MinTemp_prev = string;
    MinTemp_Trigger = MinTemp_prev.toInt();
    if (((MinTemp_Trigger >= 5) && (MinTemp_Trigger <= 50)))
    EEPROMWriteInt(230, MinTemp_Trigger);
    if (!((MinTemp_Trigger >= 5) && (MinTemp_Trigger <= 50)))
      MinTemp_Trigger = EEPROMReadInt(230);
    //Serial.print("MinTemp_Trigger :"); //Serial.println(MinTemp_Trigger);
  }
  //Serial.print("After: "); //Serial.println(xe); xe++;
  Firebase.get(firebaseData, "/Chickens/Delay");
  string = " ";
  string = firebaseData.stringData();
  if (TimeDelay_prev != string)
  {
    TimeDelay_prev = string;
    TimeDelay = TimeDelay_prev.toInt();
    if (!((TimeDelay >= 0) && (TimeDelay <= 5000)))
      TimeDelay = 1;
    //Serial.print("TimeDelay :"); //Serial.println(TimeDelay);
  }
  timeClient.update();
  Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
  Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
  Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  //////////////////////////////////////checking the connection////////////////////////////////////
  }
    //Serial.println("will start googleSheets");
      /////////////////////////////////////////////////////GoogleSheets////////////////////////////////////////////////////
  //if(timeClient.getHours()!=Hour_prev)
  {
    Hour_prev=timeClient.getHours();
    sendDataToSheet();
  }
  timeClient.getMinutes();
  if ((Error <= 0))
  { //waits spesific time before trying to reconnect
    delay(1000);
    LED_Status = ! LED_Status;
    digitalWrite(BlueLed, LED_Status);
    digitalWrite(Light, 0);
    Firebase.reconnectWiFi(true);
    //Serial.println(" Firebase.reconnectWiFi(true)");
    Light_Status = 1;
    digitalWrite(Light, !(Light_Status));
  }
  else
  {
    delay(1000);
    Firebase.reconnectWiFi(false);
    //Serial.println(" Firebase.reconnectWiFi(false)");
  }

  if ((Error != Error_prev)&&(Error!=200))
  {
    ResetFlag = 1;
    //////Serial.println("\n\n\n\nError!=Error_prev will reset \n\n\n\n");
    Error_prev = Error;
    temp_prev = 50;
    temp_prev1 = 50;
    temp_prev2 = 50;
    temp_prev3 = 9;
    hum_prev1 = 9;
    hum_prev = 9;
    hum_prev2 = 9;
    hum_prev3 = 9;
    conductivity_prev = 9;
    heaterA_status_prev = 9;
    heaterB_status_prev = 9;
    cooler_status_prev = 9;
    string = " ";
    Set_ForcedHA_prev = " ";
    Set_ForcedHB_prev = " ";
    Set_ForcedF_prev = " ";
    c = " ";
    WhichHeater_prev = "A";
    Light_prev = " ";
    MinTemp_prev = " ";
    MaxTemp_prev = " ";
    MaxVent_prev = " ";
    MinVent_prev = " ";
    ResetFlag_prev = " ";
    TimeDelay_prev = " ";
  }

  //Serial.print("cooler_status: "); //Serial.println(cooler_status);
  //Serial.print("heaterA_status: "); //Serial.println(heaterA_status);
  //Serial.print("heaterB_status: "); //Serial.println(heaterB_status);
  //Serial.print("WhichHeater: "); //Serial.println(WhichHeater);
  //Serial.print("Temperature1: "); //Serial.println(Temperature1);
  //Serial.print("Temperature2: "); //Serial.println(Temperature2);
  //Serial.print("Temperature3: "); //Serial.println(Temperature3);
  //Serial.print("Temperature: "); //Serial.println(Temperature);
  //Serial.print("Humidity: "); //Serial.println(Humidity);
  //Serial.print("Humidity1: "); //Serial.println(Humidity1);
  //Serial.print("Humidity2: "); //Serial.println(Humidity2);
  //Serial.print("Humidity3: "); //Serial.println(Humidity3);
  //Serial.print("gas: "); //Serial.println(gas);
  //Serial.print("Set_ForcedHA: "); //Serial.println(Set_ForcedHA);
  //Serial.print("Set_ForcedHB: "); //Serial.println(Set_ForcedHB);
  //Serial.print("Set_ForcedF: "); //Serial.println(Set_ForcedF);
  //Serial.print("ResetFlag: "); //Serial.println(ResetFlag);
  //Serial.print("LED_Status: "); //Serial.println(LED_Status);
  //Serial.print("Light_Status: "); //Serial.println(Light_Status);
  //Serial.print("MinVent_Trigger: "); //Serial.println(MinVent_Trigger);
  //Serial.print("MaxVent_Trigger: "); //Serial.println(MaxVent_Trigger);
  //Serial.print("MinTemp_Trigger: "); //Serial.println(MinTemp_Trigger);
  //Serial.print("MaxTemp_Trigger: "); //Serial.println(MaxTemp_Trigger);
  //Serial.print("TimeDelay: "); //Serial.println(TimeDelay);
  //Serial.print("timeClient.getSeconds(): "); //Serial.println(timeClient.getSeconds());
  //Serial.print("timeClient.getMinutes(): "); //Serial.println(timeClient.getMinutes());
  //Serial.print("timeClient.getHours(): "); //Serial.println(timeClient.getHours());








}
