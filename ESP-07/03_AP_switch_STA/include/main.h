#pragma once

#include <Arduino.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>

#include "eeprom_save.h"
#include "espAP.h"
#include "espSTA.h"

extern const int LED_PIN;  // GPIO2 - wbudowana dioda
extern const int PWM_PIN;  // GPIO5 - wyjście PWM
extern const int DS1_PIN;  // GPIO14 - DS18B20 #1
extern const int DS2_PIN;  // GPIO12 - DS18B20 #2
extern int pwmValue;       // Aktualna wartość PWM (0-1023)
extern bool ledState;      // Stan LED (true = włączony, false = wyłączony)
extern float temp1, temp2; // Odczytane temperatury

extern WiFiConfig savedConfig;         // Globalna zmienna przechowująca konfigurację WiFi
extern ESP8266WebServer webServer(80); // Serwer WWW współny dla STA i AP