/*need to make Ven motion trigger changable done!
   the circuit stuck when internet down
  Restart function
  Vent Min & max Values synchronized with cloud
  flag for changing the heaterA & B and synchorizing with the network
  uploading average Temp and humidity
  Forced State and control for heaters and fans
  vibrate blue led when no conection
*/
#include <WiFi.h>
#include"FirebaseESP32.h"
#include <WiFiManager.h>
#include <EEPROM.h>
#include "DHT.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
FirebaseData firebaseData;
WiFiManager  wifiManager;

//////////////////////Config Settings//////////////////////
#define BlueLed  2
#define Light  4
#define Heater1 5
#define Heater2 18
#define Heater3 19
#define Heater4 21
#define Cooler 22
#define GASA1   33
#define GASA2   12
uint8_t DHT1Pin = 13;
uint8_t DHT2Pin = 25;
uint8_t DHT3Pin = 14;
//Dont use 32,34,35,12
// pin 27 ADC2 can't be used while using wifi
#define EEPROM_SIZE 512
float MinTemp_Trigger = 25;
float MaxTemp_Trigger = 35;
float MinVent_Trigger = 350;
float MaxVent_Trigger = 400;
float Hum_Trigger = 50;
int WifiReconnectingTime = 60; //in seconds
int TimeDelay = 500;
/////////////////////////Variables//////////////////////////////////

int WifiReconnectingTime_Counter = 0;
int ResetFlag = 0;

int heaterA_status = 0;
int heaterB_status = 0;
int heaterA_status_prev = 5;
int heaterB_status_prev = 5;

int heaterA_Stime = 0;
int heaterB_Stime = 0;
int heaterA_Stime_prev = 0;
int heaterB_Stime_prev = 0;

int HeaterFlag = 0;
String WhichHeater = "A";
int cooler_status = 0;
int cooler_status_prev = 5;


char output[40];
char mqtt_server[40];
String Host;
String Token;
char Host_charArray[45];
char Token_charArray[45];
char readeeprom[45];

int LED_Status;
int Light_Status = 1;
int Set_ForcedHA = 0;
int Set_ForcedHB = 0;
int Set_ForcedF = 0;

String string;
String Set_ForcedHA_prev;
String Set_ForcedHB_prev;
String Set_ForcedF_prev;
String c;
String WhichHeater_prev = "A";
String Light_prev;
String MinTemp_prev;
String MaxTemp_prev;
String MaxVent_prev;
String MinVent_prev;
String ResetFlag_prev;
String TimeDelay_prev;
bool shouldSaveConfig = false;
/////////////////mq135/////////////////////
int conductivity = 0;
int conductivity_prev = 0;
String quality = "FF";
int gas;


/////////////////DHT/////////////////
#define DHTTYPE DHT11
DHT dht1(DHT1Pin, DHTTYPE);
DHT dht2(DHT2Pin, DHTTYPE);
DHT dht3(DHT3Pin, DHTTYPE);

float Temperature;
float Humidity;
float temp_prev; // save previous to check if there is a change
float hum_prev;

float Temperature1 = 0;
float Humidity1 = 0;
float temp_prev1; // save previous to check if there is a change
float hum_prev1;

float Temperature2;
float Humidity2;
float temp_prev2; // save previous to check if there is a change
float hum_prev2;

float Temperature3;
float Humidity3;
float temp_prev3; // save previous to check if there is a change
float hum_prev3;

void eeprom_read(int address)
{
  int x = 0;
  Serial.println("Starting Reading");
  for (int i = address; i < address + 40; i++, x++)
  {
    readeeprom[x] = EEPROM.read(i);
  }
  Serial.print(" readeeprom  ");
  Serial.println(readeeprom);
}
void eeprom_clear(int address)
{
  int x = 0;
  Serial.println("Starting Clearing");
  for (int i = address; i < address + 40; i++, x++)
  {
    EEPROM.put(i, "\n");
  }
  Serial.print("Commit : ");
  Serial.println(EEPROM.commit());
}
void eeprom_Write(int address, char*ptr)
{
  Serial.println("Starting Writing");
  for (int i = address, x = 0; i < (address + 40); i++, x++)
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
void setup()
{
  /////////////////////////////////////////////////////////////////////

  //wifiManager.resetSettings();

  ////////////////////////////Reading from EEPROM//////////////////////
  Serial.begin(115200);
  dht1.begin();
  dht2.begin();
  dht3.begin();

  Serial.println("\n\n\n\nVoid Setup\n\n\n\n");
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
    Serial.println("failed to initialise EEPROM"); delay(1000000);
    ESP.restart();
  }

  //Setting the custom parameters
  Serial.println("//Setting the custom parameters");

  wifiManager.setSaveConfigCallback(saveConfigCallback);
  WiFiManagerParameter custom_Host("Host", "Host", mqtt_server, 40);
  WiFiManagerParameter custom_Token("Token", "Token", output, 40);
  wifiManager.addParameter(&custom_Host);
  wifiManager.addParameter(&custom_Token);

  ///////////////connecting to WIFI/////////////////////////////

  Serial.println("Conecting....");

  wifiManager.setConfigPortalTimeout(120);
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
    Serial.println("Host_charArray & Token_charArray");
    Serial.println(Host_charArray);
    Serial.println(Token_charArray);
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
  Serial.println("Host_charArray & Token_charArray");
  Serial.println(Host_charArray);
  Serial.println(Token_charArray);

  Firebase.begin(Host_charArray, Token_charArray);
  //?????????????????????????????????????????????????????????????????????????????????????????????????????????
  //it tells the project to reconnect to wifi when it's able to, but it keep spaming
  Firebase.reconnectWiFi(false);
  Firebase.get(firebaseData, "/Chickens/WhichHeater");
  WhichHeater = firebaseData.stringData();
  timeClient.begin();
  timeClient.setTimeOffset(7200);

}
////////////////////////////////////////////////////////////////////////
void loop()
{
  Serial.println("inside void loop");
  delay(TimeDelay);
  //////////////////////////////////////////////////TempFn///////////////////////////////////////////////////////
  //DHT data
  Temperature1 = dht1.readTemperature(); // Gets the values of the temperature
  Humidity1 = dht1.readHumidity(); // Gets the values of the humidity
  Serial.print("temp1: "); Serial.println(Temperature1);
  Temperature2 = dht2.readTemperature(); // Gets the values of the temperature
  Humidity2 = dht2.readHumidity(); // Gets the values of the humidity
  Serial.print("temp2: "); Serial.println(Temperature2);

  Temperature3 = dht3.readTemperature(); // Gets the values of the temperature
  Humidity3 = dht3.readHumidity(); // Gets the values of the humidity
  Serial.print("temp3: "); Serial.println(Temperature3);

  Temperature = ( Temperature1 + Temperature2 + Temperature3) / 3;
  Humidity    = (Humidity1 + Humidity2 + Humidity3) / 3;
  detectGas();

  //variables needed to be uploaded to firebase     : Temperature1,Temperature2,Temperature3,Humidity1,Humidity2,Humidity3,heaterA_status,heaterB_status,AirQuality,conductivity
  //                                                  Temperature,Humidity,MinVent_Trigger,MaxVent_Trigger
  //variables needed to be downloaded from firebase : MinTemp_Trigger, MaxTemp_Trigger,delayTime,ResetFlag
  if ((gas > MaxVent_Trigger) && (Temperature > MinTemp_Trigger))
  {
    digitalWrite(Cooler, 1);
    cooler_status = 1;
  }
  else if(Temperature > MaxTemp_Trigger + 5)
  {
    digitalWrite(Cooler, 1);
    cooler_status = 1;
  }
  else if(Set_ForcedF == 1)
  {
    digitalWrite(Cooler, 1);
    cooler_status = 1;
  }
  else if(Temperature < MinTemp_Trigger)
  {
    digitalWrite(Cooler, 0);
    cooler_status = 0;
  }
  else if(gas < MinVent_Trigger)
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

  Serial.println("will start firebase");
  //////////////////////////////////////////////////firebase///////////////////////////////////////////////////////
  if (WiFi.status() != WL_CONNECTED)
    goto H;
  if (WhichHeater_prev != WhichHeater)
  {
    Serial.print("WhichHeater: "); Serial.println(WhichHeater);
    WhichHeater_prev = WhichHeater;
    Firebase.setString(firebaseData, "/Chickens/WhichHeater", WhichHeater);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (temp_prev != Temperature)
  {
    Serial.print("Temperature: "); Serial.println(Temperature);
    temp_prev = Temperature;
    Firebase.setFloat(firebaseData, "/Chickens/Temperature", Temperature);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (temp_prev1 != Temperature1)
  {
    Serial.print("temp1: "); Serial.println(Temperature1);
    temp_prev1 = Temperature1;
    Firebase.setFloat(firebaseData, "/Chickens/Temp1", Temperature1);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (temp_prev2 != Temperature2)
  {
    temp_prev2 = Temperature2;
    Serial.print("temp2: "); Serial.println(Temperature2);
    Firebase.setFloat(firebaseData, "/Chickens/Temp2", Temperature2);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (temp_prev3 != Temperature3)
  {
    temp_prev3 = Temperature3;
    Serial.print("temp3: "); Serial.println(Temperature3);
    Firebase.setFloat(firebaseData, "/Chickens/Temp3", Temperature3);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (hum_prev != Humidity)
  {
    hum_prev = Humidity;
    Serial.print("Humidity: "); Serial.println(Humidity);
    Firebase.setFloat(firebaseData, "/Chickens/Humidity", Humidity);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (hum_prev1 != Humidity1)
  {
    hum_prev1 = Humidity1;
    Serial.print("Humidity1: "); Serial.println(Humidity1);
    Firebase.setFloat(firebaseData, "/Chickens/Hum1", Humidity1);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (hum_prev2 != Humidity2)
  {
    hum_prev2 = Humidity2;
    Serial.print("Humidity2: "); Serial.println(Humidity2);
    Firebase.setFloat(firebaseData, "/Chickens/Hum2", Humidity2);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (hum_prev3 != Humidity3)
  {
    hum_prev3 = Humidity3;
    Serial.print("Humidity3: "); Serial.println(Humidity3);
    Firebase.setFloat(firebaseData, "/Chickens/Hum3", Humidity3);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (conductivity_prev != gas)
  { // the String will be produced in the android program
    conductivity_prev = conductivity;
    Serial.print("gas: "); Serial.println(gas);
    Firebase.setFloat(firebaseData, "/Chickens/AirQuality", gas);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (heaterA_status_prev != heaterA_status)
  {
    heaterA_status_prev = heaterA_status;
    Serial.print("heaterA_status: "); Serial.println(heaterA_status);
    Firebase.setInt(firebaseData, "/Chickens/HeaterA_status", heaterA_status);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (heaterB_status_prev != heaterB_status)
  {
    heaterB_status_prev = heaterB_status;
    Serial.print("heaterB_status: "); Serial.println(heaterB_status);
    Firebase.setInt(firebaseData, "/Chickens/HeaterB_status", heaterB_status);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  if (cooler_status_prev != cooler_status)
  {
    cooler_status_prev = cooler_status;
    Serial.print("cooler_status: "); Serial.println(cooler_status);
    Firebase.setInt(firebaseData, "/Chickens/Cooler_status", cooler_status);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  /////////////////////////////////////////////////////////

  Firebase.get(firebaseData, "/Chickens/Set_ForcedHA");
  string = firebaseData.stringData();
  if (Set_ForcedHA_prev != string)
  {
    Serial.println("ForcedHA Changed!");
    Set_ForcedHA_prev = string;
    Set_ForcedHA = Set_ForcedHA_prev.toInt();
    Firebase.setInt(firebaseData, "/Chickens/Get_ForcedHA", Set_ForcedHA);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  Firebase.get(firebaseData, "/Chickens/Set_ForcedHB");
  string = firebaseData.stringData();
  if (Set_ForcedHB_prev != string)
  {
    Serial.println("ForcedHB Changed!");
    Set_ForcedHB_prev = string;
    Set_ForcedHB = Set_ForcedHB_prev.toInt();
    Firebase.setInt(firebaseData, "/Chickens/Get_ForcedHB", Set_ForcedHB);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  Firebase.get(firebaseData, "/Chickens/Set_ForcedF");
  string = firebaseData.stringData();
  if (Set_ForcedF_prev != string)
  {
    Serial.println("ForcedF Changed!");
    Set_ForcedF_prev = string;
    Set_ForcedF = Set_ForcedF_prev.toInt();
    Firebase.setInt(firebaseData, "/Chickens/Get_ForcedF", Set_ForcedF);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  Firebase.get(firebaseData, "/Chickens/ResetFlag");
  if (ResetFlag_prev != firebaseData.stringData())
  {
    Serial.println("ResetFlag Changed!");
    ResetFlag_prev = firebaseData.stringData();
    ResetFlag = ResetFlag_prev.toInt();
    Firebase.setInt(firebaseData, "/Chickens/ResetFlag", 0);
    ESP.restart();
  }
  Firebase.get(firebaseData, "/Chickens/Set_Led1");
  if (c != firebaseData.stringData())
  {
    Serial.println("BlueLed Changed!");
    c = firebaseData.stringData();
    LED_Status = c.toInt();
    Firebase.setInt(firebaseData, "/Chickens/Get_Led1", LED_Status);
    digitalWrite(BlueLed, LED_Status);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  ///////////////////////////////////////////////////////////////////////////////////////
  Firebase.get(firebaseData, "/Chickens/Set_Light");
  if (Light_prev != firebaseData.stringData())
  {
    Serial.println("Light Changed!");
    Light_prev = firebaseData.stringData();
    Light_Status = Light_prev.toInt();
    Firebase.setInt(firebaseData, "/Chickens/Get_Light", Light_Status);
    digitalWrite(Light, !(Light_Status));
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  //////////////////////////////////////////////////////////////////////////////////////////////
  Firebase.get(firebaseData, "/Chickens/MinVent");
  if (MinVent_prev != firebaseData.stringData())
  {
    MinVent_prev = firebaseData.stringData();
    MinVent_Trigger = MinTemp_prev.toInt();
    Serial.print("MinVent_Trigger :"); Serial.println(MinVent_Trigger);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  Firebase.get(firebaseData, "/Chickens/MaxVent");
  if (MaxVent_prev != firebaseData.stringData())
  {
    MaxVent_prev = firebaseData.stringData();
    MaxVent_Trigger = MaxVent_prev.toInt();
    Serial.print("MaxVent_Trigger :"); Serial.println(MaxVent_Trigger);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  Firebase.get(firebaseData, "/Chickens/MaxTemp");
  if (MaxTemp_prev != firebaseData.stringData())
  {
    MaxTemp_prev = firebaseData.stringData();
    MaxTemp_Trigger = MaxTemp_prev.toInt();
    Serial.print("MaxTemp_Trigger :"); Serial.println(MaxTemp_Trigger);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  Firebase.get(firebaseData, "/Chickens/MinTemp");
  if (MinTemp_prev != firebaseData.stringData())
  {
    MinTemp_prev = firebaseData.stringData();
    MinTemp_Trigger = MinTemp_prev.toInt();
    Serial.print("MinTemp_Trigger :"); Serial.println(MinTemp_Trigger);
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  Firebase.get(firebaseData, "/Chickens/Delay");
  if (TimeDelay_prev != firebaseData.stringData())
  {
    TimeDelay_prev = firebaseData.stringData();
    TimeDelay = TimeDelay_prev.toInt();
    timeClient.update();
    Firebase.setInt(firebaseData, "/Chickens/Hour", timeClient.getHours());
    Firebase.setInt(firebaseData, "/Chickens/Minute", timeClient.getMinutes());
    Firebase.setInt(firebaseData, "/Chickens/Seconds", timeClient.getSeconds());
  }
  //////////////////////////////////////checking the connection////////////////////////////////////
H:
  Serial.println(WifiReconnectingTime_Counter);
  if (WiFi.status() != WL_CONNECTED)
  { //waits spesific time before trying to reconnect
    delay(500);
    int value = digitalRead(BlueLed);
    digitalWrite(BlueLed, ~value);
//need to toggle
    WifiReconnectingTime_Counter++;
    //if (WifiReconnectingTime_Counter >= WifiReconnectingTime)
    Firebase.reconnectWiFi(true);
    Serial.println(" Firebase.reconnectWiFi(true)");
    //ESP.restart();

  }
  else
  {
    Firebase.reconnectWiFi(false);
    Serial.println(" Firebase.reconnectWiFi(false)");
  }
}
