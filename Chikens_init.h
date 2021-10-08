#ifndef CHICKENS_H_
#define CHICKENS_H_
#include "Chickens_priv.h"
#include "Chikens_config.h"
#include <WiFi.h>
#include"FirebaseESP32.h"
#include <WiFiManager.h>
#include <EEPROM.h>
#include "DHT.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>

#define DHTTYPE DHT11
DHT dht1(DHT1Pin, DHTTYPE);
DHT dht2(DHT2Pin, DHTTYPE);
DHT dht3(DHT3Pin, DHTTYPE);

void UPDATE();
void sendDataToSheet(void);
void eeprom_read(int address, int stringLength);
void eeprom_clear(int address, int stringLength);
void eeprom_Write(int address, char*ptr, int stringLength);
void saveConfigCallback ();
void detectGas();
void EEPROMWriteInt(int p_address, int p_value);
unsigned int EEPROMReadInt(int p_address);
void pinSetup(void);
void wifiSetup(void);
void DHTSetup(void);
void EEPROMSetup(void);
void firbaseSetup(void);
void parameterSetup(void);
void timeServerSetup(void);







#endif CHICKENS_H_
