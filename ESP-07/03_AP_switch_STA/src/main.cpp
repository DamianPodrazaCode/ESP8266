#include "main.h"

// ==== Konfiguracja LED ====
const int LED_PIN = 2;          // GPIO2 - wbudowana dioda
const int PWM_PIN = 5;          // GPIO5 - wyjście PWM
const int DS1_PIN = 14;         // GPIO14 - DS18B20 #1
const int DS2_PIN = 12;         // GPIO12 - DS18B20 #2
int pwmValue = 0;               // Aktualna wartość PWM (0-1023)
bool ledState = false;          // Stan LED (true = włączony, false = wyłączony)
float temp1 = 0.0, temp2 = 0.0; // Odczytane temperatury

// ==== DS18B20 setup ====
OneWire oneWire1(DS1_PIN);
OneWire oneWire2(DS2_PIN);
DallasTemperature sensor1(&oneWire1);
DallasTemperature sensor2(&oneWire2);

void readTemperatures()
{
  sensor1.requestTemperatures();
  sensor2.requestTemperatures();

  temp1 = sensor1.getTempCByIndex(0);
  temp2 = sensor2.getTempCByIndex(0);

  if (temp1 == DEVICE_DISCONNECTED_C)
    temp1 = -127;
  if (temp2 == DEVICE_DISCONNECTED_C)
    temp2 = -127;

  Serial.printf("Temp1: %.2f°C, Temp2: %.2f°C\n", temp1, temp2);
}

// ==== Główne funkcje ====
void setup()
{
  Serial.begin(115200);

  // Konfiguracja pinów
  pinMode(LED_PIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Wyłącz LED na starcie

  analogWriteRange(1023);         // Ustaw zakres PWM na 0-1023
  pwmValue = 0;                   // Domyślna wartość PWM
  analogWrite(PWM_PIN, pwmValue); // Ustaw PWM na 0

  // Inicjalizacja czujników DS18B20
  sensor1.begin();
  sensor2.begin();
  sensor1.setResolution(12);
  sensor2.setResolution(12);

  // Zainicjalizuj EEPROM i wczytaj zapisane dane
  loadConfig();

  // ------------------------------------------------------------------
  // Spróbuj połączyć się z zapisaną siecią
  if (savedConfig.configured)
  {
    Serial.print("Łączenie z: ");
    Serial.println(savedConfig.ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(savedConfig.ssid, savedConfig.password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
      delay(500);
      Serial.print(".");
      attempts++;
    }

    if (MDNS.begin("regulator")) // "regulator" to Twoja nazwa, np. http://regulator.local
    {
      Serial.println("mDNS responder started - http://regulator.local");
    }
    else
    {
      Serial.println("Error setting up MDNS responder!");
    }
  }

  // ------------------------------------------------------------------
  // Jeśli nie udało się połączyć, uruchom Captive Portal
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("\nUruchamiam tryb konfiguracji AP");
    startCaptivePortal();
  }
  else
  {
    // Połączono z siecią - uruchom serwer sterowania
    Serial.println("\nPołączono z siecią!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    startControlServer();
  }

  // ------------------------------------------------------------------
  readTemperatures();
}

unsigned long lastTempRead = 0;
void loop()
{
  if (WiFi.getMode() == WIFI_AP)
  {
    dnsServer.processNextRequest(); // Obsłuż DNS tylko w trybie AP
  }
  else
  {
    MDNS.update();
  }
  webServer.handleClient();

  // Odczytuj temperatury co 5 sekund
  if (millis() - lastTempRead > 2000)
  {
    readTemperatures();
    lastTempRead = millis();
  }
}