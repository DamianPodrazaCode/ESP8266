#pragma once
#include "main.h"

void handlePWM();        // Ustawienie wartości PWM
void handleGetPWM();     // Pobranie aktualnej wartości PWM
void handleGetLED();     // Pobranie stanu LED
void handleRoot();       // Strona główna sterowania
void handleTemps();      // Pobranie temperatur w formacie JSON 
void handleLED(String state); // Włącz/Wyłącz LED
void handleReset();      // Reset konfiguracji WiFi
void startControlServer(); // Uruchom serwer sterowania

