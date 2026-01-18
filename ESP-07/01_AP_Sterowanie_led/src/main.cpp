#include "mian.h"
#include <Arduino.h>
#include "ap_serw.h"

// satałe do czujników i wyjść oraz ledy
const int ledPin = 2; // wbudowana dioda LED na module (GPIO2)

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // Wyłącz LED na starcie

  apConfig(); // Konfiguracja Access Point
}

void loop()
{
  apMainLoop(); // Główna pętla serwera
}