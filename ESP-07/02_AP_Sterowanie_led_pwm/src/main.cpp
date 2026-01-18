#include "mian.h"
#include <Arduino.h>
#include "ap_serw.h"

// satałe do czujników i wyjść oraz ledy
const int ledPin = 2; // wbudowana dioda LED na module (GPIO2)
const int pwmPin = 5; // Pin do sygnału PWM (GPIO5) 
const int relayPin = 4; // Pin przekaźnika (GPIO4)

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // Ustawienie pinu diody LED jako wyjście
  digitalWrite(ledPin, HIGH); // Wyłącz LED na starcie
  pinMode(relayPin, OUTPUT); // Ustawienie pinu przekaźnika jako wyjście
  digitalWrite(relayPin, LOW); // Wyłącz przekaźnik na starcie  

  analogWriteRange(1023); // zmiana rozdzielczości PWM na 10 bitów (0-1023)
  pinMode(pwmPin, OUTPUT); // Ustawienie pinu PWM jako wyjście
  analogWrite(pwmPin, 0); // Ustawienie początkowej wartości PWM na 0

  apConfig(); // Konfiguracja Access Point
}

void loop()
{
  apMainLoop(); // Główna pętla serwera
}