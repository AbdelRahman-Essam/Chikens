#ifndef CHICKENS_PRIV_H_
#define CHICKENS_PRIV_H_
/*
 * added RFID
 * need buzzer
 */
/* V1.0.1.X
 * need to separate the code to files and arrange it done 
 * need to find away to clear string  done
 * need to make my own wifi manager with html page to take the id & p.w too
 * need to make authentication wich create splitted part in firebase
 * need to add charts for the last 50 sample in google sheet
 * */
/* V1.0.0.X
 * need to make Ven motion trigger changable done!
 * the circuit stuck when internet down found the problem done
 * Restart function done
 * Vent Min & max Values synchronized with cloud done
 * flag for changing the heaterA & B and synchorizing with the network done
 * uploading average Temp and humidity done
 * Forced State and control for heaters and fans done
 * vibrate blue led when no conection done
 * light need to be set to zero to work done
 * add a googlesheet to the code done
 * add FOTA to gthe code DONE
*/


String server = "http://maker.ifttt.com";
String eventName = "ESP32";
String IFTTT_Key = "dRKdPFw2Xk3MFxA-JSDKKi7W_Aq_hoU4CADOtnMWXTh";
String IFTTTUrl="https://maker.ifttt.com/trigger/ESP32/with/key/dRKdPFw2Xk3MFxA-JSDKKi7W_Aq_hoU4CADOtnMWXTh";

/////////////////////////Variables//////////////////////////////////
boolean RFID_Success = true;
int httpResponseCode = 0 ; // being here for handling error and send againg http request  if error occured using while loop


int WifiReconnectingTime_Counter = 0;
int ResetFlag = 0;
int UpdateCode =0;

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


char output[60];
char mqtt_server[60];
String Host;
String Token;
char Host_charArray[60];
char Token_charArray[60];
char readeeprom[60];

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
String UpdateCode_prev;
int Hour_prev;
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
