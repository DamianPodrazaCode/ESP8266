#include "../include/mian.h"
#include <Arduino.h>
#include "ap_serw.h"

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // Wyłącz LED na starcie

  apConfig();
}

void loop()
{
  mainLoop();
}