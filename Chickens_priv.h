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
unsigned long long int currentmillis =0;

//WiFiCheck 
unsigned long long int WiFiCheck_previousMillis = 0;
//WiFi_Reconnect
unsigned long long int WiFi_Reconnect_previousMillis = 0;
//CreditionalsConfig
unsigned long long int CreditionalsConfig_previousMillis = 0;
// temperature
unsigned long long int temp_previousMillis = 0;
// gas
unsigned long long int gas_previousMillis = 0;
// control
unsigned long long int control_previousMillis = 0;
//firebase
unsigned long long int firebase_previousMillis = 0;
// ResetCheck
unsigned long long int resetCheck_previousMillis = 0;
// GoogleSheet
unsigned long long int googleSheet_previousMillis = 0;
//timeUpgrade
unsigned long long int timeUpgrade_previousMillis =0;
// serial
unsigned long long int serial_previousMillis = 0;
// RFID_Read
unsigned long long int RFID_Read_previousMillis = 0;
// LCD
unsigned long long int LCD_previousMillis = 0;
unsigned long long int ErrorDetect_previousMillis = 0;

unsigned int heaterA_Stime = 0;
unsigned int heaterB_Stime = 0;
unsigned int heaterA_Stime_previousMillis = 0;
unsigned int heaterB_Stime_previousMillis = 0;

///////////////////////////////////////////////// LCD /////////////////////////////////////////////////////
String line1, line2;


//////////////////////////////////////////////////////////////////////////////////////////////////////////
String ifttt_server = "http://maker.ifttt.com";
String eventName = "ESP32_Mod2";
String IFTTT_Key = "dRKdPFw2Xk3MFxA-JSDKKi7W_Aq_hoU4CADOtnMWXTh";
String IFTTTUrl="https://maker.ifttt.com/trigger/ESP32/with/key/dRKdPFw2Xk3MFxA-JSDKKi7W_Aq_hoU4CADOtnMWXTh";

/////////////////////////Variables//////////////////////////////////
boolean RFID_Success = true;
int httpResponseCode = 0 ; // being here for handling error and send againg http request  if error occured using while loop


String ResetFlag = "OFF";
String UpdateCode ="OFF";
/************Auto Variables*******************/
String FanAuto="ON";
String heaterAauto="ON";
String heaterBAuto="ON";
String FanAuto_prev="H";
String heaterAauto_prev="H";
String heaterBAuto_prev="H";

/*********************************************/
float val;

String heaterA_status = "OFF";
String heaterB_status = "OFF";
String heaterA_status_prev = "H";
String heaterB_status_prev = "H";



String HeaterFlag = "OFF";
String WhichHeater = "A";
String cooler_status = "OFF";
String cooler_status_prev = "H";

///////////////////////////////////Creditionals////////////////////////
int buttonState = 1;
boolean ConfigResetFlag = false;
boolean shouldSaveConfig = false;


String ssid{};
String password{};
String username{};
// lIkJLB0WSwUwpAhJIPBgXroy9ce2_1



String header{};
int count = 0;
boolean exit_but;
///////////////////////////////////////////////////////////////////////////
int LED_Status=0;
String LED_Status_Str = "OFF";
String Light_Status = "ON";
String Set_ManualHA = "OFF";
String Set_ManualHB = "OFF";
String Set_ManualF = "OFF";

String string;
String Set_ManualHA_prev = "H";
String Set_ManualHB_prev = "H";
String Set_ManualF_prev = "H";
String LED_prev = "H";
String WhichHeater_prev = "H";
String Light_prev = "H";
String VentRange_prev = "H";
String TempRange_prev = "H";
String ResetFlag_prev = "H";
String firebase_interval_prev;
String UpdateCode_prev;
int Hour_prev;
/////////////////mq135/////////////////////
String quality = "FF";
long int gas_prev;
long int gas;
int GAS[ADC_READING_NUMBER];
int Error;
int Error_prev;
int gasCounter=0;

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

#endif CHICKENS_PRIV_H_
