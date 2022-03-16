#ifndef CONFIG_H_
#define CONFIG_H_

//////////////////////Config Settings//////////////////////
String CodeVersion ="2.0.0.1";

const char* ssid_ap     = "ChickenFarm";
const char* password_ap = "88888888";
String Host="bird-system-default-rtdb.firebaseio.com/";
String Token= "Wzsly4ViUy5D579xhksXRkIPGnTbYMjfp2xxWvzx";

// Timing intervals in seconds
unsigned long int WiFiCheck_interval = 1000;//            uses internet periodically update the keepalive
unsigned long int WiFi_Reconnect_interval = 30000;
unsigned long int CreditionalsConfig_interval = 100;
unsigned long int temp_interval=1000;
unsigned long int gas_interval=1000;
unsigned long int control_interval = 100;
unsigned long int firebase_interval = 0; // in minutes     uses internet periodically send & recieve the data with firebase
unsigned long int resetCheck_interval = 1000;
unsigned long int googleSheet_interval = 60; // in minutes uses internet periodically send & recieve the data with googlesheet
unsigned long int timeUpgrade_interval = 20000;//          uses internet periodically update the timing
unsigned long int serial_interval = 2000;
unsigned long int RFID_Read_interval = 100;
unsigned long int LCD_interval = 10000;
unsigned long int HeaterSwitch_interval = 1000*60*60;//1Hour

//Group1

  #define light_ON    0
  #define light_OFF   1
  #define control_ON  1
  #define control_OFF 0
//// Relay 
  #define Heater1 5
  #define Heater2 19
  #define Heater3 18
  #define Heater4 21
  #define Cooler 22
  #define Light  4
// others  
  #define buttonPin 0 
  #define BlueLed  2


  #define GASA1   33
  //#define GASA2   12
  uint8_t DHT1Pin = 13;
  uint8_t DHT2Pin = 25;
  uint8_t DHT3Pin = 14;
  #define SDA0_Pin  17
  #define SCL0_Pin  16


//Group2

//  #define light_ON    1
//  #define light_OFF   0
//  #define control_ON  0
//  #define control_OFF 1
//// Relay 
//  #define unused1 1
//  #define Light  4
//  #define Heater1 5
//  #define Heater3 18
//  #define Heater2 19
//  #define Heater4 23
//  #define Cooler 15
//  #define unused2 3
//  
//  #define buttonPin 0 
//  #define BlueLed  2
//  #define GASA1   34
//  //#define GASA2   39
//  uint8_t DHT1Pin = 32;
//  uint8_t DHT2Pin = 33;
//  uint8_t DHT3Pin = 27;
//
//  #define SDA0_Pin  25
//  #define SCL0_Pin  26

/////////////////////////////////////
//DHT test
//success 33 32 25 26 27 14 12 13

//fail 34 35 36 39
////////////////////////////////////
//    Analog Test with WiFi ADC2 not working
//Success 36 39 34 35 32 33
//Fail    25 26 27 14 12 13 ADC2 not working with WiFi
////////////////////////////////////
//used pins 15 2 4 16 17 5 18 19 21 3 1 22 23 this side is complete 
//Dont use 32,34,35,12
// pin 27 ADC2 can't be used while using wifi


#define EEPROM_SIZE 512
int Temp_variance_Cool = 2; // this is the jetter after the max to start cooling
float MinTemp_Trigger = 25;
float MaxTemp_Trigger = 35;
float MinVent_Trigger = 350;
float MaxVent_Trigger = 400;
float Hum_Trigger = 50;
int stringLength = 60;  // length of the host and token

#endif CONFIG_H_
