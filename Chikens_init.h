#ifndef CHICKENS_H_
#define CHICKENS_H_

#include "Chickens_priv.h"
#include "Chikens_config.h"
#include <String.h>
#include <WiFi.h>
#include"FirebaseESP32.h"
#include <EEPROM.h>
#include "DHT.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
//#include <PN532_HSU.h>
//#include <PN532.h>
#include <LiquidCrystal_I2C.h>
#include <esp_task_wdt.h>


#define DHTTYPE DHT11
DHT dht1(DHT1Pin, DHTTYPE);
DHT dht2(DHT2Pin, DHTTYPE);
DHT dht3(DHT3Pin, DHTTYPE);
DHT dht4(DHT4Pin, DHTTYPE);
DHT dht5(DHT5Pin, DHTTYPE);

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


int firebaseErrorDetect(void);
void tempFn(void);
void detectGas();
void controlStatments(void);
void firebaseStatments(void);
void resetCheck(void);
void gooogleSheetStatments(void);
void timeUpgrade(void);
void serialPrints(void);

String getChipId();
void UPDATE();
void UpdateCheck(void);
void sendDataToSheet(void);
String eeprom_read(int address, int stringLength);
void eeprom_Write(int address, char*ptr, int stringLength);
void EEPROMWriteInt(int p_address, int p_value);
unsigned int EEPROMReadInt(int p_address);

void LCD_weather();
void LCD_setup();

void pinSetup(void);
void WifiSetup(void);
void DHTSetup(void);
void EEPROMSetup(void);
void firbaseSetup(void);
void parameterSetup(void);
void timeServerSetup(void);

//boolean RFID_Setup(void);
//void RFID_Read(boolean RFID_Success);
//void RFIDsendData(String params);

void set_access_point(void);
void client_handle(void);
void CreditionalsConfig(void);
void WiFiCheck(void);
#endif //CHICKENS_H_
