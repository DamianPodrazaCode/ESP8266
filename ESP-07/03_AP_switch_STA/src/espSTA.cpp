
#include "espSTA.h"

// Strona sterowania
const char *CONTROL_PAGE = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    body { font-family: Arial; margin: 20px; }
    .container { max-width: 800px; margin: 0 auto; }
    .section { background: #f5f5f5; padding: 20px; margin: 20px 0; border-radius: 10px; }
    .button { padding: 10px 20px; margin: 5px; border: none; border-radius: 5px; cursor: pointer; color: white; }
    .on { background: #4CAF50; }
    .off { background: #f44336; }
    .pwm-off { background: #666; }
    .temp-display { font-size: 24px; font-weight: bold; margin: 10px 0; }
    .temp-box { display: inline-block; padding: 15px; margin: 10px; background: white; border-radius: 5px; min-width: 150px; }
    .temp1 { border-left: 5px solid #2196F3; }
    .temp2 { border-left: 5px solid #FF9800; }
    .chart-container { position: relative; height: 300px; width: 100%; }
  </style>
  <script>
    let tempChart;
    let chartData = {
      labels: [],
      datasets: [{
        label: 'Czujnik 1 (GPIO14)',
        data: [],
        borderColor: '#2196F3',
        backgroundColor: 'rgba(33, 150, 243, 0.1)',
        fill: true,
        tension: 0.4
      }, {
        label: 'Czujnik 2 (GPIO12)',
        data: [],
        borderColor: '#FF9800',
        backgroundColor: 'rgba(255, 152, 0, 0.1)',
        fill: true,
        tension: 0.4
      }]
    };

    // Inicjalizacja wykresu
    window.onload = function() {
      const ctx = document.getElementById('tempChart').getContext('2d');
      tempChart = new Chart(ctx, {
        type: 'line',
        data: chartData,
        options: {
          responsive: true,
          maintainAspectRatio: false,
          scales: {
            y: {
              title: { display: true, text: 'Temperatura (°C)' },
              min: 0,
              max: 40
            },
            x: {
              title: { display: true, text: 'Czas' },
              ticks: { maxTicksLimit: 10 }
            }
          }
        }
      });
      
      // Wczytaj początkową wartość PWM
      fetch('/getpwm')
        .then(r => r.text())
        .then(v => {
          document.getElementById('pwmSlider').value = v;
          updatePWM(v);
        });
      
      // Rozpocznij odświeżanie temperatur
      updateTemperatures();
      setInterval(updateTemperatures, 5000);
    }

    function controlLED(action) {
      fetch('/' + action)
        .then(r => r.text())
        .then(d => console.log(d));
    }

    function updatePWM(value) {
      document.getElementById('pwmValue').textContent = value;
      document.getElementById('pwmPercent').textContent = Math.round((value/1023)*100);
      fetch('/pwm?value=' + value);
    }

    function turnOffPWM() {
      document.getElementById('pwmSlider').value = 0;
      updatePWM(0);
    }

    function updateTemperatures() {
      fetch('/temps')
        .then(r => r.json())
        .then(data => {
          // Aktualizuj wartości tekstowe
          document.getElementById('temp1').textContent = data.temp1.toFixed(1) + ' °C';
          document.getElementById('temp2').textContent = data.temp2.toFixed(1) + ' °C';
          
          // Dodaj do wykresu
          const now = new Date();
          const time = now.getHours() + ':' + now.getMinutes() + ':' + now.getSeconds();
          
          chartData.labels.push(time);
          chartData.datasets[0].data.push(data.temp1);
          chartData.datasets[1].data.push(data.temp2);
          
          // Ogranicz do 20 ostatnich punktów
          if (chartData.labels.length > 20) {
            chartData.labels.shift();
            chartData.datasets[0].data.shift();
            chartData.datasets[1].data.shift();
          }
          
          tempChart.update();
          
          // Kolorowanie w zależności od temperatury
          document.getElementById('temp1Box').style.borderLeftColor = 
            data.temp1 > 30 ? '#f44336' : data.temp1 > 25 ? '#FF9800' : '#2196F3';
          document.getElementById('temp2Box').style.borderLeftColor = 
            data.temp2 > 30 ? '#f44336' : data.temp2 > 25 ? '#FF9800' : '#FF9800';
        });
    }
  </script>
</head>
<body>
  <div class="container">
    <h1>ESP8266 - Sterowanie & Monitoring</h1>
    
    <!-- Sekcja temperatur -->
    <div class="section">
      <h2>Temperatury</h2>
      <div>
        <div class="temp-box temp1" id="temp1Box">
          <div>GPIO14 (DS18B20 #1)</div>
          <div class="temp-display" id="temp1">--.- °C</div>
        </div>
        <div class="temp-box temp2" id="temp2Box">
          <div>GPIO12 (DS18B20 #2)</div>
          <div class="temp-display" id="temp2">--.- °C</div>
        </div>
      </div>
      <div class="chart-container">
        <canvas id="tempChart"></canvas>
      </div>
    </div>
    
    <!-- Sekcja LED -->
    <div class="section">
      <h2>Sterowanie LED</h2>
      <button class="button on" onclick='controlLED("on")'>WŁĄCZ LED</button>
      <button class="button off" onclick='controlLED("off")'>WYŁĄCZ LED</button>
    </div>
    
    <!-- Sekcja PWM -->
    <div class="section">
      <h2>PWM (GPIO5)</h2>
      <input type="range" min="0" max="1023" value="0" 
             id="pwmSlider" oninput="updatePWM(this.value)">
      <p>Wartość: <span id="pwmValue">0</span> (<span id="pwmPercent">0</span>%)</p>
      <button class="button pwm-off" onclick="turnOffPWM()">Wyłącz PWM</button>
    </div>
    
    <!-- Info -->
    <div class="section">
      <p><strong>IP:</strong> %STA_IP% | <strong>WiFi:</strong> %STA_SSID%</p>
      <p><a href="/reset">Resetuj konfigurację WiFi</a></p>
    </div>
  </div>
</body>
</html>
)rawliteral";

// ==== NOWE FUNKCJE DO OBSŁUGI PWM ====
// Ustawienie wartości PWM
void handlePWM()
{
    if (webServer.hasArg("value"))
    {
        pwmValue = constrain(webServer.arg("value").toInt(), 0, 1023);
        analogWrite(PWM_PIN, pwmValue);
        webServer.send(200, "text/plain", String(pwmValue));
    }
}

// Pobranie aktualnej wartości PWM
void handleGetPWM()
{
    webServer.send(200, "text/plain", String(pwmValue));
}

// Pobranie stanu LED
void handleGetLED()
{
    String state = digitalRead(LED_PIN) == LOW ? "LED: WŁĄCZONA" : "LED: WYŁĄCZONA";
    webServer.send(200, "text/plain", state);
}

// Strona główna sterowania
void handleRoot()
{
  String page = CONTROL_PAGE;
  page.replace("%STA_IP%", WiFi.localIP().toString());
  page.replace("%STA_SSID%", WiFi.SSID());
  webServer.send(200, "text/html; charset=UTF-8", page);
}

// Pobranie temperatur w formacie JSON
void handleTemps()
{
  StaticJsonDocument<200> doc;
  doc["temp1"] = temp1;
  doc["temp2"] = temp2;

  String json;
  serializeJson(doc, json);
  webServer.send(200, "application/json", json);
}

// Włącz/Wyłącz LED
void handleLED(String state)
{
  ledState = (state == "on");
  digitalWrite(LED_PIN, ledState ? LOW : HIGH); // LOW = ON dla wbudowanej LED
  webServer.send(200, "text/plain", "LED: " + String(ledState ? "WŁĄCZONA" : "WYŁĄCZONA"));
}

// Reset konfiguracji WiFi
void handleReset()
{
  // Wyczyść konfigurację
  savedConfig.configured = false;
  saveConfig();

  webServer.send(200, "text/html",
                 "<h1>Konfiguracja zresetowana</h1><p>Restartuję w tryb konfiguracji...</p>"
                 "<script>setTimeout(() => location.href='http://'+location.hostname, 3000)</script>");

  delay(1000);
  ESP.restart();
}

// Uruchom serwer sterowania
void startControlServer()
{
  // Konfiguracja serwera sterowania
  webServer.on("/", handleRoot);
  webServer.on("/on", []()
               {
    digitalWrite(LED_PIN, LOW);
    webServer.send(200, "text/plain", "LED: WŁĄCZONA"); });
  webServer.on("/off", []()
               {
    digitalWrite(LED_PIN, HIGH);
    webServer.send(200, "text/plain", "LED: WYŁĄCZONA"); });

  // NOWE: Endpointy do obsługi PWM
  webServer.on("/pwm", handlePWM);
  webServer.on("/getpwm", handleGetPWM);
  webServer.on("/getled", handleGetLED);
  webServer.on("/temps", handleTemps); // Nowy endpoint
  webServer.on("/reset", handleReset);
  webServer.onNotFound(handleNotFound);
  webServer.begin();
  Serial.println("Serwer sterowania z PWM i DS -> uruchomiony");
}
