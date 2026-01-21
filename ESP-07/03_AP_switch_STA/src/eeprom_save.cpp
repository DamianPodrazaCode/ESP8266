#include "eeprom_save.h"

WiFiConfig savedConfig; // Globalna zmienna przechowująca konfigurację WiFi

//  Zapisz konfigurację do EEPROM
void saveConfig()
{
    EEPROM.begin(sizeof(WiFiConfig));
    EEPROM.put(0, savedConfig);
    EEPROM.commit();
    EEPROM.end();
}

// Wczytaj konfigurację z EEPROM
void loadConfig()
{
    EEPROM.begin(sizeof(WiFiConfig));
    EEPROM.get(0, savedConfig);
    EEPROM.end();

    // Sprawdź poprawność danych
    if (savedConfig.configured && savedConfig.ssid[0] != 0)
    {
        Serial.println("Znaleziono zapisaną konfigurację WiFi");
    }
    else
    {
        savedConfig.configured = false; // Ustaw jako nie skonfigurowane
        Serial.println("Brak zapisanej konfiguracji");
    }
}
