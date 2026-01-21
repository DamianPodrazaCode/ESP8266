#pragma once
#include "main.h"

void handleConfig();       // Strona konfiguracji
void handleSave();         // Zapisz konfigurację
void handleNotFound();     // Przechwyć nieznane żądania
void setupCaptivePortal(); // Obsługa Captive Portalu
void startCaptivePortal(); // Uruchom Captive Portal
void setupAP();