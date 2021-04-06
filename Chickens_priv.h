#ifndef CHICKENS_PRIV_H_
#define CHICKENS_PRIV_H_

/*need to make Ven motion trigger changable done!
   the circuit stuck when internet down found the problem done
  Restart function done
  Vent Min & max Values synchronized with cloud done
  flag for changing the heaterA & B and synchorizing with the network done
  uploading average Temp and humidity done
  Forced State and control for heaters and fans done
  vibrate blue led when no conection
  light need to be set to zero to work
*/
/////////////////////////Variables//////////////////////////////////

int WifiReconnectingTime_Counter = 0;
int ResetFlag = 0;

float val;

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

int LED_Status = 0;
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
int Time_prev;
bool shouldSaveConfig = false;
/////////////////mq135/////////////////////
int conductivity = 0;
int conductivity_prev = 0;
String quality = "FF";
int gas;
int Error;
int Error_prev;


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



#endif CHICKENS_PRIV_H_
