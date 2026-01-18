#include "../include/mian.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

// Konfiguracja Access Point
void apConfig()
{
    // Ustawienie trybu Access Point
    WiFi.softAP(ssid, password);

    Serial.println();
    Serial.print("Access Point uruchomiony. SSID: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP()); // Domyślnie

    // Konfiguracja serwera WWW
    server.on("/", handleRoot);
    server.on("/on", handleOn);
    server.on("/off", handleOff);

    // Uruchomienie serwera
    server.begin();
    Serial.println("Serwer HTTP uruchomiony");
}

// Obsługa strony głównej
void handleRoot()
{
    // Strona główna z przyciskami do sterowania LED
    String html = "<!DOCTYPE html><html>";
    html += "<meta charset='UTF-8'>";
    html += "<head><meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>";
    html += "body { font-family: Arial; text-align: center; margin: 50px; }";
    html += ".button { background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center;";
    html += "text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }";
    html += ".button2 { background-color: #f44336; }";
    html += "</style></head>";
    html += "<body>";
    html += "<h1>Sterowanie ESP8266</h1>";
    html += "<p>Status LED: " + String(digitalRead(ledPin) ? "OFF" : "ON") + "</p>";
    html += "<p><a href=\"/on\"><button class=\"button\">WŁĄCZ LED</button></a></p>";
    html += "<p><a href=\"/off\"><button class=\"button button2\">WYŁĄCZ LED</button></a></p>";
    html += "<p>Adres IP: 192.168.4.1</p>";
    html += "<p>Połącz się z WiFi: " + String(ssid) + "</p>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

// Obsługa włączenia LED
void handleOn()
{
    digitalWrite(ledPin, LOW); // Włącz LED (aktywny LOW dla wbudowanej diody)
    server.sendHeader("Location", "/");
    server.send(303);
}

// Obsługa wyłączenia LED
void handleOff()
{
    digitalWrite(ledPin, HIGH); // Wyłącz LED
    server.sendHeader("Location", "/");
    server.send(303);
}

// Główna pętla serwera
void mainLoop()
{
    server.handleClient();
}