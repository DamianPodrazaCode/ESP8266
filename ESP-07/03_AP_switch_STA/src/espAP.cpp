#include "espAP.h"

ESP8266WebServer webServer(80); // Serwer WWW współny dla STA i AP
DNSServer dnsServer;

// AP (Captive Portal)
const char *AP_SSID = "ESP8266_Setup";
const char *AP_PASSWORD = "config123";
const byte DNS_PORT = 53;

const char *CONFIG_PAGE = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <style>
    body { font-family: Arial; text-align: center; margin: 50px; }
    input, button { padding: 10px; margin: 10px; width: 250px; }
    .status { color: #4CAF50; font-weight: bold; }
  </style>
</head>
<body>
  <h1>Konfiguracja WiFi</h1>
  <form action='/save' method='POST'>
    <input type='text' name='ssid' placeholder='Nazwa sieci (SSID)' required><br>
    <input type='password' name='password' placeholder='Hasło do sieci' required><br>
    <button type='submit'>Zapisz i Połącz</button>
  </form>
  <p>Połączony z: %AP_SSID%</p>
</body>
</html>
)rawliteral";

// ==== Obsługa Captive Portalu ====
// Strona konfiguracji
void handleConfig()
{
    String page = CONFIG_PAGE;
    page.replace("%AP_SSID%", AP_SSID);
    webServer.send(200, "text/html; charset=UTF-8", page);
}

// ==== Zapisz konfigurację WiFi ====
void handleSave()
{
    if (webServer.method() == HTTP_POST)
    {
        String ssid = webServer.arg("ssid");
        String password = webServer.arg("password");

        // Zapisz do EEPROM
        ssid.toCharArray(savedConfig.ssid, 32);
        password.toCharArray(savedConfig.password, 64);
        savedConfig.configured = true;
        saveConfig();

        webServer.send(200, "text/html",
                       "<h1>Zapisano!</h1><p>Łączę z siecią...<br>Restartuję za 3 sekundy.</p>"
                       "<script>setTimeout(() => location.href='http://'+location.hostname, 3000)</script>");

        delay(1000);
        ESP.restart();
    }
}

// Obsługa nieznanych żądań
void handleNotFound()
{
    String host = webServer.hostHeader();
    if (host != WiFi.softAPIP().toString() && host != WiFi.localIP().toString())
    {
        webServer.sendHeader("Location", "http://" + WiFi.softAPIP().toString());
        webServer.send(302, "text/plain", "");
    }
    else
    {
        webServer.send(404, "text/plain", "404: Not Found");
    }
}

// Ustawienie odpowiednich ścieżek, aby urządzenia mobilne rozpoznały Captive Portal
void setupCaptivePortal()
{
    webServer.on("/generate_204", []()
                 { webServer.send(204, "text/plain", ""); }); // Android
    webServer.on("/hotspot-detect.html", []()
                 { webServer.send(200, "text/html", ""); }); // iOS
    webServer.on("/connecttest.txt", []()
                 { webServer.send(200, "text/plain", "success"); }); // Windows
    webServer.on("/ncsi.txt", []()
                 { webServer.send(200, "text/plain", "success"); }); // Windows
    webServer.on("/success.txt", []()
                 { webServer.send(200, "text/plain", "success"); }); // Inne
}

// Uruchomienie Captive Portalu
void startCaptivePortal()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.print("AP SSID: ");
    Serial.println(AP_SSID);
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());

    // Przekieruj wszystkie domeny na IP AP
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

    // Konfiguracja serwera
    webServer.on("/", handleConfig);      // Strona główna
    webServer.on("/save", handleSave);    // Zapisz konfigurację
    setupCaptivePortal();                 // Ustaw obsługę Captive Portalu
    webServer.onNotFound(handleNotFound); // Przechwyć nieznane żądania
    webServer.begin();                    // Uruchom serwer
    Serial.println("Serwer Captive Portal uruchomiony");
}

void setupAP(){
    
}