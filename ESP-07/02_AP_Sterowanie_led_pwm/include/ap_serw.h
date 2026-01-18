#pragma once

void apConfig();   // Konfiguracja Access Point
void handleRoot(); // Obsługa strony głównej
void handleOn();   // Obsługa włączenia LED
void handleOff();  // Obsługa wyłączenia LED
void handleRelayOn();   // Obsługa włączenia przekaźnika
void handleRelayOff();  // Obsługa wyłączenia przekaźnika
void handlePWM();  // Obsługa ustawienia PWM
void apMainLoop();   // Główna pętla serwera
