#include <Arduino.h>
#include "main.h"
#include "ds18b20.h"
#include "tabData.h"

/////////////////////////////////////////////////////////////
const uint8_t ledIO = 2;
tabData glTablica; // główna tablica stanów i danych

/////////////////////////////////////////////////////////////
void softTimer1Update(uint32_t delayTime = 1000);
void softTimer2Update(uint32_t delayTime = 1000);
void softTimerSerialUpdate(uint32_t delayTime = 1000);
/////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(115200);
  pinMode(ledIO, OUTPUT);
  initTemperatureSensor();
}

/////////////////////////////////////////////////////////////
uint32_t debugTime = 0;
void loop()
{
  debugTime = millis();
  // ------------------------------------------------------------------------------------------------
  softTimer1Update(100); // led
  softTimer2Update(200); // temperatury

  softTimerSerialUpdate(1000); // Serial wysyłanie

  // ------------------------------------------------------------------------------------------------
  debugTime = millis() - debugTime;
  if (debugTime > glTablica.data.timeDelay)
    glTablica.data.timeDelay = debugTime; // maksymalny czas trwania pętli zadań
}

/////////////////////////////////////////////////////////////
void inline softTimer1Proc()
{
  digitalWrite(ledIO, !digitalRead(ledIO));
  // Serial.println("led blink");
}
void softTimer1Update(uint32_t delayTime)
{
  uint32_t nowTime = millis();
  static uint32_t oldTime = 0;
  if (nowTime >= (oldTime + delayTime))
  {
    softTimer1Proc();
    oldTime = nowTime;
  }
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
void inline softTimer2Proc()
{
  updateNextTemperatureSensor();
}
void softTimer2Update(uint32_t delayTime)
{
  uint32_t nowTime = millis();
  static uint32_t oldTime = 0;
  if (nowTime >= (oldTime + delayTime))
  {
    softTimer2Proc();
    oldTime = nowTime;
  }
}
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
void inline softTimerSerialProc()
{
  // Serial.println("softTimerSerialUpdate");
  // writeSerialData();
  Serial.print(glTablica.data.ds[0]);
  Serial.print(" ");
  Serial.println(glTablica.data.ds[1]);
}

void softTimerSerialUpdate(uint32_t delayTime)
{
  uint32_t nowTime = millis();
  static uint32_t oldTime = 0;
  if (nowTime >= (oldTime + delayTime))
  {
    softTimerSerialProc();
    oldTime = nowTime;
  }
}
/////////////////////////////////////////////////////////////