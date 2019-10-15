#include <Arduino.h>

const int floatSwtichPin = 5; // should have 10k pull-up resistor
const int moistureSwitchPin = 13; // should have 10k pull-down resistor
const int waterPumpPin = 14; // should have current limiting resistor
int delaySec = 10;
int delayCount = 0;
int forceStopSec = 120;

void setup()
{
  Serial.begin(115200);
  pinMode(floatSwtichPin, INPUT);
  pinMode(moistureSwitchPin, OUTPUT);
  pinMode(waterPumpPin, OUTPUT);
}

void loop()
{
  int moistureValue = 0;
  bool needWatering = false;

  // Read the water level switch to see if water is avaiable
  bool floatSwitch = digitalRead(floatSwtichPin);

  // float switch == low means has water
  if(floatSwitch == LOW)
  {

    // Power on moisture sensor
    digitalWrite(moistureSwitchPin, HIGH);
    
    // make sure the sensor will have time to turn on
    delay(10);

    // read soil moisture value
    moistureValue = analogRead(A0);

    // Power off moisture sensor
    digitalWrite(moistureSwitchPin, LOW);

    if(moistureValue > 800)
    {
      needWatering = true;
    }
  }

  // force stop if watering time is more than X seconds
  // in case the moisture sensor out of control.
  if(delaySec*delayCount > forceStopSec)
  {
    needWatering = false;
  }

  if(needWatering == true)
  {
    // water pump on
    digitalWrite(waterPumpPin, HIGH);

    // delay milli seconds
    delay(delaySec*1000);

    delayCount++;
  }
  else
  {
    // close water pump whether it's on or not
    digitalWrite(waterPumpPin, LOW);

    // into deep sleep mode
    ESP.deepSleep(30e6);
  }

}