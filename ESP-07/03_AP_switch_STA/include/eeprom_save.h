#pragma once
#include "main.h"

// Struktura konfiguracji WiFi
struct WiFiConfig
{
    char ssid[32];
    char password[64];
    bool configured;
};

void saveConfig(); // Zapisz konfigurację do EEPROM
void loadConfig(); // Wczytaj konfigurację z EEPROM
