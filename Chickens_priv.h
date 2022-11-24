#ifndef CHICKENS_PRIV_H_
#define CHICKENS_PRIV_H_
#include "Chikens_config.h"
/*V2.0.0.0
 * make timing for all the update functions done
 * make Group1 and Group2 Pinout done
 * edit the firebase function done
 * edit the 0,1 configurations done
 * edit the KeepAlive with period done
 * edit the googlesheet transmission
 * need to change the mechanism of ranges search until the ,
 * add the LCD
 * 
 * Errors
 * network fail will cause to get the latest data rcvd
 * 
 */
/*V1.0.2.7
 * Error when wifi DC status changes
 * 
 */
/*V1.0.2.3
 * temporary edits 
 * to solve status error in flutter
 * search for temporary and remove the commands 
 */
/*V1.0.2.2
 * Added the Manual/Auto Compilation
 * Upload to the farm directly
 */
/*
 * added RFID
 * need buzzer
 */
/* V1.0.1.X
 * need to separate the code to files and arrange it done 
 * need to find away to clear string  done
 * need to make my own wifi manager with html page to take the id & p.w too Done
 * need to make authentication wich create splitted part in firebase Done using Flutter
 * need to add charts for the last 50 sample in google sheet Done using flutter
 * */
/* V1.0.0.X
 * need to make Ven motion trigger changable done!
 * the circuit stuck when internet down found the problem done
 * Restart function done
 * Vent Min & max Values synchronized with cloud done
 * flag for changing the heaterA & B and synchorizing with the network done
 * uploading average Temp and humidity done
 * Manual State and control for heaters and fans done
 * vibrate blue led when no conection done
 * light need to be set to zero to work done
 * add a googlesheet to the code done
 * add FOTA to gthe code DONE
 * changes to be switable with the flutter APP
*/

/******************************* Timing ************************************/
uint32_t currentmillis =0;
/*
 * delay
 * 
 * 
 */
String firebase_interval_prev;
String UpdateCode_prev;
int Hour_prev;

uint32_t safe_mode_prev =0;
// ResetCheck
uint32_t resetCheck_previousMillis = 0;
// GoogleSheet
uint32_t googleSheet_previousMillis = 0;
//timeUpgrade
uint32_t timeUpgrade_previousMillis =0;
// serial
uint32_t serial_previousMillis = 0;
// RFID_Read
uint32_t RFID_Read_previousMillis = 0;
// LCD
uint32_t ErrorDetect_previousMillis = 0;

/******************************* Sensors ************************************/
float val;
String VentRange_prev = "H";
String TempRange_prev = "H";

String quality = "FF";
long int gas_prev;
long int gas;

String quality1 = "FF";
long int gas1_prev;
long int gas1;

String quality2 = "FF";
long int gas2_prev;
long int gas2;

int GAS[ADC_READING_NUMBER];

int gasCounter=0;

float Temperature = 0;
float Humidity = 0;
float temp_prev = 200; // save previous to check if there is a change
float hum_prev = 200;

float Temperature1 = 0;
float Humidity1 = 0;
float temp_prev1 = 200; // save previous to check if there is a change
float hum_prev1 = 200;
int   T1_S =1;

float Temperature2;
float Humidity2;
float temp_prev2 = 200; // save previous to check if there is a change
float hum_prev2 = 200;
int   T2_S =1;

float Temperature3;
float Humidity3;
float temp_prev3 = 200; // save previous to check if there is a change
float hum_prev3 = 200;
int   T3_S =1;

float Temperature4;
float Humidity4;
float temp_prev4 = 200; // save previous to check if there is a change
float hum_prev4 = 200;
int   T4_S =1;

float Temperature5;
float Humidity5;
float temp_prev5 = 200; // save previous to check if there is a change
float hum_prev5 = 200;
int   T5_S =1;

String Gas1_Sen_Cal_Prev = "NNN";
String Gas2_Sen_Cal_Prev = "NNN";

/******************************* Fans ************************************/
uint32_t FanA_Flag = 0;
uint32_t FanB_Flag = 0;
uint32_t FanAStartTime = 0;
uint32_t FanBStartTime = 0;

uint32_t FanA_STime_MinInterval = 0;
uint32_t FanB_STime_MinInterval = 0;

String FanA_Auto="ON";
String FanB_Auto="ON";

String FanA_Auto_prev="H";
String FanB_Auto_prev="H";

String fanA_status = "OFF";
String fanA_status_prev = "H";

String fanB_status = "OFF";
String fanB_status_prev = "H";

String Set_ManualFA = "OFF";
String Set_ManualFB = "OFF";
String Set_ManualFA_prev = "NN";
String Set_ManualFB_prev = "NN";

String Fan_min_interval_prev  = "0"; //in seconds

String Fan_1_on_time_prev  = "0";
String Fan_1_off_time_prev = "0";

String Fan_on_time_prev  = "0";
String Fan_off_time_prev = "0";

String Temp_variance_FanB_prev = "0"; // this is the jetter after the max or less than the min to start the action (after max to start fan)
String Temp_variance_FanA_prev = "0";
/******************************* Cooler ************************************/
uint32_t CoolerStartTime = 0;
uint8_t  CoolerFlag = 0;

String Cooler_Auto="ON";
String Cooler_Auto_prev="H";

String cooler_status = "OFF";
String cooler_status_prev = "H";
String Set_ManualC = "OFF";
String Set_ManualC_prev = "NN";
String Cool_min_interval_prev = "0"; //in seconds

String Cooler_on_time_prev  = "0";
String Cooler_off_time_prev = "0";
String Temp_variance_Cool_prev = "0"; // this is the jetter after the max or less than the min to start the action (after max to start cooler)

/******************************* Heaters ************************************/
uint32_t HeaterA_Flag = 0;
uint32_t HeaterB_Flag = 0;
uint32_t HeaterAStartTime = 0;//times used with the reset 
uint32_t HeaterBStartTime = 0;

uint32_t HeaterA_STime_MinInterval = 0;
uint32_t HeaterB_STime_MinInterval = 0;

String heaterA_Auto="ON";
String heaterB_Auto="ON";

String heaterA_Auto_prev="H";
String heaterB_Auto_prev="H";

String heaterA_status = "OFF";
String heaterB_status = "OFF";
String heaterA_status_prev = "H";
String heaterB_status_prev = "H";

String Set_ManualHA = "OFF";
String Set_ManualHB = "OFF";

String Heat_min_interval_prev = "0"; //in seconds

String Heater_on_time_prev  = "0";
String Heater_off_time_prev = "0";

String Set_ManualHA_prev = "NN";
String Set_ManualHB_prev = "NN";
/******************************* LCD ************************************/
String line1, line2,line3,line4;
byte wifi_customChar[8] = {B00000, B00000, B00000, B00000, B00001, B00011, B00111, B01111};
byte no_wifi[8] = {B00000, B10100, B01000, B10100, B0001, B00011, B00111, B01111};
byte arrow[8] = {0x0, 0x4 ,0x6, 0x3f, 0x6, 0x4, 0x0};
int Page_number = 0;
int Choise_number = 0;
int App_mode =0;
uint32_t LCD_previousMillis = 0;
uint32_t Fast_LCD_previousMillis = 0;
uint32_t Setting_previousMillis = 0;
/******************************* FFT ************************************/
String ifttt_server = "http://maker.ifttt.com";
String eventName = "ESP32_Mod2";
String IFTTT_Key = "dRKdPFw2Xk3MFxA-JSDKKi7W_Aq_hoU4CADOtnMWXTh";
String IFTTTUrl="https://maker.ifttt.com/trigger/ESP32/with/key/dRKdPFw2Xk3MFxA-JSDKKi7W_Aq_hoU4CADOtnMWXTh";

/******************************* Variables ************************************/
String string;

boolean RFID_Success = true;
int httpResponseCode = 0 ; // being here for handling error and send againg http request  if error occured using while loop


String ResetFlag = "OFF";
String UpdateCode ="OFF";

String ResetFlag_prev = "H";

int Error;
int Error_prev;
/******************************* BTNs ************************************/
int setFlag_h=0;
int negFlag_h=0;
int posFlag_h=0;
int setFlag_l=0;
int negFlag_l=0;
int posFlag_l=0;
int setFlag=0;
int negFlag=0;
int posFlag=0;

// button
long long int button_previousMillis = 0;
int buzzer_Status=0;

/******************************* Credintials ************************************/
int buttonState = 1;
boolean ConfigResetFlag = false;
boolean shouldSaveConfig = false;


String ssid{};
String password{};
String username{};

String header{};
int count = 0;
boolean exit_but;
/******************************* Light ************************************/
int LED_Status=0;
String LED_Status_Str = "OFF";
String Light_Status = "ON";
uint8_t Light_Status_Char = '1';
String LED_prev = "H";
String Light_prev = "H";

/******************************* WDT ************************************/

//typedef enum {
//    ESP_RST_UNKNOWN,    //!< Reset reason can not be determined
//    ESP_RST_POWERON,    //!< Reset due to power-on event
//    ESP_RST_EXT,        //!< Reset by external pin (not applicable for ESP32)
//    ESP_RST_SW,         //!< Software reset via esp_restart
//    ESP_RST_PANIC,      //!< Software reset due to exception/panic
//    ESP_RST_INT_WDT,    //!< Reset (software or hardware) due to interrupt watchdog
//    ESP_RST_TASK_WDT,   //!< Reset due to task watchdog
//    ESP_RST_WDT,        //!< Reset due to other watchdogs
//    ESP_RST_DEEPSLEEP,  //!< Reset after exiting deep sleep mode
//    ESP_RST_BROWNOUT,   //!< Brownout reset (software or hardware)
//    ESP_RST_SDIO,       //!< Reset over SDIO
//} esp_reset_reason_t;

String Clear_Reset_Reason = "OFF";
String WDT_Action = "OFF";
String Clear_Reset_Reason_prev = "H";
String WDT_Action_prev = "H";

/*******************************       ************************************/

const char * root_ca = \
                       "-----BEGIN CERTIFICATE-----\n" \
                       "MIIDujCCAqKgAwIBAgILBAAAAAABD4Ym5g0wDQYJKoZIhvcNAQEFBQAwTDEgMB4G\n" \
                       "A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjIxEzARBgNVBAoTCkdsb2JhbFNp\n" \
                       "Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDYxMjE1MDgwMDAwWhcNMjExMjE1\n" \
                       "MDgwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEG\n" \
                       "A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n" \
                       "hvcNAQEBBQADggEPADCCAQoCggEBAKbPJA6+Lm8omUVCxKs+IVSbC9N/hHD6ErPL\n" \
                       "v4dfxn+G07IwXNb9rfF73OX4YJYJkhD10FPe+3t+c4isUoh7SqbKSaZeqKeMWhG8\n" \
                       "eoLrvozps6yWJQeXSpkqBy+0Hne/ig+1AnwblrjFuTosvNYSuetZfeLQBoZfXklq\n" \
                       "tTleiDTsvHgMCJiEbKjNS7SgfQx5TfC4LcshytVsW33hoCmEofnTlEnLJGKRILzd\n" \
                       "C9XZzPnqJworc5HGnRusyMvo4KD0L5CLTfuwNhv2GXqF4G3yYROIXJ/gkwpRl4pa\n" \
                       "zq+r1feqCapgvdzZX99yqWATXgAByUr6P6TqBwMhAo6CygPCm48CAwEAAaOBnDCB\n" \
                       "mTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUm+IH\n" \
                       "V2ccHsBqBt5ZtJot39wZhi4wNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2NybC5n\n" \
                       "bG9iYWxzaWduLm5ldC9yb290LXIyLmNybDAfBgNVHSMEGDAWgBSb4gdXZxwewGoG\n" \
                       "3lm0mi3f3BmGLjANBgkqhkiG9w0BAQUFAAOCAQEAmYFThxxol4aR7OBKuEQLq4Gs\n" \
                       "J0/WwbgcQ3izDJr86iw8bmEbTUsp9Z8FHSbBuOmDAGJFtqkIk7mpM0sYmsL4h4hO\n" \
                       "291xNBrBVNpGP+DTKqttVCL1OmLNIG+6KYnX3ZHu01yiPqFbQfXf5WRDLenVOavS\n" \
                       "ot+3i9DAgBkcRcAtjOj4LaR0VknFBbVPFd5uRHg5h6h+u/N5GJG79G+dwfCMNYxd\n" \
                       "AfvDbbnvRG15RjF+Cv6pgsH/76tuIMRQyV+dTZsXjAzlAcmgQWpzU/qlULRuJQ/7\n" \
                       "TBj0/VLZjmmx6BEP3ojY+x1J96relc8geMJgEtslQIxq/H5COEBkEveegeGTLg==\n" \
                       "-----END CERTIFICATE-----\n";

  
#endif //CHICKENS_PRIV_H_
