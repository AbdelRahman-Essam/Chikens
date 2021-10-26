#ifndef CONFIG_H_
#define CONFIG_H_

//////////////////////Config Settings//////////////////////
String CodeVersion ="1.0.1.1";

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
int WifiReconnectingTime = 30; //in seconds
int ConfigPortalTimeout = 60;
int TimeDelay = 1;
int stringLength = 60;  // length of the host and token

#endif CONFIG_H_
