// nazwa sieci i hasło dla trybu Access Point
const char *ssid = "ESP8266_Control";
const char *password = "config123"; // minimum 8 znaków

#include "mian.h"
#include "ap_serw.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
static String ipStr;

// Konfiguracja Access Point
void apConfig()
{
    WiFi.softAP(ssid, password); // Uruchomienie Access Point

    // Wyświetlenie informacji o Access Point
    Serial.println();
    Serial.print("Access Point uruchomiony. SSID: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP()); // Domyślnie

    //zamiana adresu IP na String
    IPAddress ip = WiFi.softAPIP();
    ipStr = String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
    Serial.print("Adres IP w formacie String: ");
    Serial.println(ipStr);
    
    /*
    Serwer ma adress 192.168.4.1 jak w przeglądarce wpiszemy 192.168.4.1/ to wywoła się handleRoot, jak wpiszemy 192.168.4.1/on to handleOn, a 192.168.4.1/off to handleOff
    */
    // Konfiguracja serwera WWW
    server.on("/", handleRoot);   // Strona główna
    server.on("/on", handleOn);   // Włącz LED
    server.on("/off", handleOff); // Wyłącz LED
    server.on("/relayon", handleRelayOn);   // Obsługa włączenia przekaźnika
    server.on("/relayoff", handleRelayOff); // Obsługa wyłączenia przekaźnika
    server.on("/pwm", handlePWM); // Obsługa PWM

    // Uruchomienie serwera
    server.begin();
    Serial.println("Serwer HTTP uruchomiony");
}

// Obsługa strony głównej
void handleRoot()
{
    // Strona główna z przyciskami do sterowania LED oraz obsługa pwm przy pomocy suwaka    
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
    // Wyświetlenie statusu LED
    html += "<p>Status LED: " + String(digitalRead(ledPin) ? "OFF" : "ON") + "</p>";  
    // Przyciski do sterowania LED
    html += "<p><a href=\"/on\"><button class=\"button\">WŁĄCZ LED</button></a></p>"; 
    html += "<p><a href=\"/off\"><button class=\"button button2\">WYŁĄCZ LED</button></a></p>";
    // sterowanie przekaźnikiem
    html += "<p><a href=\"/relayon\"><button class=\"button\">WŁĄCZ PRZEKAŹNIK</button></a></p>";
    html += "<p><a href=\"/relayoff\"><button class=\"button button2\">WYŁĄCZ PRZEKAŹNIK</button></a></p>"; 
    // Suwak do sterowania PWM
    html += "<h2>Sterowanie PWM</h2>";
    html += "<form action=\"/pwm\" method=\"get\">";
    html += "<input type=\"range\" name=\"value\" min=\"0\" max=\"1023\" value=\"0\" oninput=\"this.nextElementSibling.value = this.value\">";
    html += "<output>0</output>";       
    html += "<br><br><input type=\"submit\" value=\"Ustaw PWM\">";
    html += "</form>";

    html += "<p>Adres IP: " + ipStr + "</p>"; 
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

//  obsługa prtzekaźnika
void handleRelayOn()
{
    digitalWrite(relayPin, HIGH); // Włącz przekaźnik
    server.sendHeader("Location", "/");
    server.send(303);
}       

//  obsługa prtzekaźnika
void handleRelayOff()
{
    digitalWrite(relayPin, LOW); // Wyłącz przekaźnik
    server.sendHeader("Location", "/");
    server.send(303);
}

void handlePWM()    
{
    if (server.hasArg("value")) {
        int pwmValue = server.arg("value").toInt();
        pwmValue = constrain(pwmValue, 0, 1023); // Ograniczenie wartości do zakresu 0-1023
        analogWrite(pwmPin, pwmValue); // Ustawienie wartości PWM
    }
    server.sendHeader("Location", "/");
    server.send(303);
}

// Główna pętla serwera
void apMainLoop()
{
    server.handleClient();
}
