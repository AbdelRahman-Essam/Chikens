#ifndef CONFIG_H_
#define CONFIG_H_

//////////////////////Config Settings//////////////////////
String CodeVersion ="1.2.0.0";

const char* ssid_ap     = "ChickenFarm";
const char* password_ap = "88888888";
String Host="bird-system-default-rtdb.firebaseio.com/";
String Token= "Wzsly4ViUy5D579xhksXRkIPGnTbYMjfp2xxWvzx";

// Timing intervals in seconds
unsigned long int WiFiCheck_interval = 1000;//            uses internet periodically update the keepalive
unsigned long int WiFi_Reconnect_interval = 30000;
unsigned long int CreditionalsConfig_interval = 100;
unsigned long int temp_interval=1000;
unsigned long int gas_interval=1000*10;
unsigned long int control_interval = 100;
unsigned long int firebase_interval = 0; // in minutes     uses internet periodically send & recieve the data with firebase
unsigned long int resetCheck_interval = 1000;
unsigned long int googleSheet_interval = 60; // in minutes uses internet periodically send & recieve the data with googlesheet
unsigned long int timeUpgrade_interval = 20000;//          uses internet periodically update the timing
unsigned long int serial_interval = 2000;
unsigned long int RFID_Read_interval = 100;
unsigned long int LCD_interval = 10000;
unsigned long int HeaterSwitch_interval = 1000*60*60;//1Hour
unsigned long int CoolerSwitch_interval = 1000*60*60;//1Hour   
unsigned long int FanDelayBTWN_Fans_interval = 1000*20;//20 second   
unsigned long int button_interval = 50; //millis

unsigned long int ErrorDetect_interval = 1000*60;

unsigned long int Fan_min_interval = 1000*60;

unsigned int Cooler_on_time = 60*1000;
unsigned int Cooler_off_time =240*1000+Cooler_on_time;


//Group1
//
  #define light_ON    0
  #define light_OFF   1
  #define control_ON  1
  #define control_OFF 0

  #define Buzzer  15
  #define buttonPin 0 
  #define BlueLed  2

//  #define RFID_TX 16   
//  #define RFID_RX 17  
  #define SW_N 16 //BTNs Switch negative
  #define SW_P 17 //Switch Positive
  #define SW_S 33 //Switch Set
  
  #define LCD_SDA  21
  #define LCD_SCL  22
  
  #define HeaterA 4
  #define HeaterB 23
  #define FanA    5
  #define FanB    18  
  #define Cooler  19
  #define Light   13
// others  

  #define WLS1    34
  #define WLS2    35

  #define IO25    25
  #define IO12    12

  #define GASA1   36
  #define GASA2   39
  uint8_t DHT1Pin = 26;
  uint8_t DHT2Pin = 27;
  uint8_t DHT3Pin = 14;
  uint8_t DHT4Pin = 32;
//  uint8_t DHT5Pin = 33;



////Group2
//
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
//  #define Fan 15
//  #define Cooler 3
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

#define ADC_READING_NUMBER  100
#define EEPROM_SIZE 512
int Temp_variance_Cool = 2; // this is the jetter after the max or less than the min to start cooling
int Temp_variance_FanB = 1; // this is the jetter after the max or less than the min to start cooling
int Temp_variance_FanA = 0; // this is the jetter after the max or less than the min to start cooling

float MinTemp_Trigger = 25;
float MaxTemp_Trigger = 35;
float MinVent_Trigger = 350;
float MaxVent_Trigger = 400;
float Hum_Trigger = 50;
int stringLength = 60;  // length of the host and token

#endif CONFIG_H_
