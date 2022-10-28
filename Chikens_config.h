#ifndef CONFIG_H_
#define CONFIG_H_

//////////////////////Config Settings//////////////////////
String CodeVersion ="3.1.0.10";

const char* ssid_ap     = "ChickenFarm";
const char* password_ap = "88888888";
String Host="bird-system-default-rtdb.firebaseio.com/";
String Token= "Wzsly4ViUy5D579xhksXRkIPGnTbYMjfp2xxWvzx";

// Timing intervals in seconds

#define WDT_TIMEOUT 60 //30 seconds WDT

uint32_t WiFiCheck_interval = 1000;//            uses internet periodically update the keepalive
uint32_t WiFi_Reconnect_interval = 30000;
uint32_t CreditionalsConfig_interval = 100;
uint32_t temp_interval=1000;
uint32_t gas_interval=1000*10;
uint32_t control_interval = 100;
uint32_t resetCheck_interval = 1000;
uint32_t googleSheet_interval = 60; // in minutes uses internet periodically send & recieve the data with googlesheet
uint32_t timeUpgrade_interval = 20000;//          uses internet periodically update the timing
uint32_t serial_interval = 2000;
uint32_t RFID_Read_interval = 100;
uint32_t LCD_interval = 10000;
uint32_t LCD_reinit_interval = 100000;
uint32_t Fast_LCD_interval = 5;
uint32_t HeaterSwitch_interval = 1000*60*60;//1Hour
uint32_t CoolerSwitch_interval = 1000*60*60;//1Hour   
uint32_t FanDelayBTWN_Fans_interval = 1000*20;//20 second   
uint32_t button_interval = 50; //millis
uint32_t Setting_interval = 10000; //millis
uint32_t ErrorDetect_interval = 1000*60;


// configurations that might be changed with user
uint8_t firebase_interval = 0; // in minutes     uses internet periodically send & recieve the data with firebase 0--255

uint16_t Fan_min_interval  = 60; //in seconds
uint16_t Cool_min_interval = 60; //in seconds
uint16_t Heat_min_interval = 60; //in seconds

uint16_t Fan_1_on_time  = 240;
uint16_t Fan_1_off_time = 0 + Fan_1_on_time;

uint16_t Fan_on_time  = 240;
uint16_t Fan_off_time = 60 + Fan_on_time;

uint16_t Fan_on_time_temperory  = 240;
uint16_t Fan_off_time_temperory = 0 + Fan_on_time_temperory;

uint16_t Heater_on_time  = 240;
uint16_t Heater_off_time = 60 + Heater_on_time;

uint16_t Cooler_on_time  = 60;
uint16_t Cooler_off_time = 240 + Cooler_on_time;

uint8_t Temp_variance_Cool = 2; // this is the jetter after the max or less than the min to start the action (after max to start cooler)
uint8_t Temp_variance_FanB = 1; // this is the jetter after the max or less than the min to start the action (after max to start fan)
uint8_t Temp_variance_FanA = 0; // this is the jetter after the max or less than the min to start the action (after max to start fan)

float MinTemp_Trigger = 25;
float MaxTemp_Trigger = 35;
float MinVent_Trigger = 350;
float MaxVent_Trigger = 400;

int Gas1_Sen_Cal = 0;
int Gas2_Sen_Cal = 0;

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
  #define SW_S 12 //Switch Set
  
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
//  #define IO12    12

  #define GASA1   36
  #define GASA2   39
  uint8_t DHT1Pin = 26;
  uint8_t DHT2Pin = 27;
  uint8_t DHT3Pin = 14;
  uint8_t DHT4Pin = 32;
  uint8_t DHT5Pin = 33;




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

#define ADC_READING_NUMBER  10
#define EEPROM_SIZE 512

int stringLength = 60;  // length of the host and token
int Number_of_Pages = 4;
int Number_of_Choices1 = 4;
int Number_of_Choices2 = 8;

#endif //CONFIG_H_
