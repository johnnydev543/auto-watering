#include <Arduino.h>

const int floatSwtichPin = 5; // should have 10k pull-up resistor
const int moistureReadPin = 4; // should have 10k pull-up resistor
const int moistureOnPin = 13; // should have 10k pull-down resistor
const int waterPumpPin = 14; // should have current limiting resistor, pull-down resistor
int delaySec = 10; // seconds
int delayCount = 0;
int forceStopSec = 120; // seconds
int sleepTimeSec = 30; // seconds

void setup()
{
  Serial.begin(115200);
  pinMode(floatSwtichPin, INPUT);
  pinMode(moistureReadPin, INPUT);
  pinMode(moistureOnPin, OUTPUT);
  pinMode(waterPumpPin, OUTPUT);
}

void loop()
{
  bool needWatering = false;

  // Read the water level switch to see if water is avaiable
  bool floatSwitch = digitalRead(floatSwtichPin);

  Serial.println();

  // float switch == low means has water
  if(floatSwitch == LOW)
  {

    Serial.println("Water is avaiable.");

    // Power on moisture sensor
    digitalWrite(moistureOnPin, HIGH);
    
    // milli seconds, make sure the sensor will have time to turn on
    delay(20);

    // read soil moisture value
    int moistureRead = digitalRead(moistureReadPin);

    // Power off moisture sensor
    digitalWrite(moistureOnPin, LOW);

    Serial.println("Starting moisture measurement.");
    Serial.print("Moisture logic level:");
    Serial.print(moistureRead);
    Serial.println();

    if(moistureRead == HIGH)
    {
      needWatering = true;
    }
  }
  else
  {
    Serial.println("Water is not avaiable.");
  }

  // force stop if watering time is more than X seconds
  // in case the moisture sensor or something else out of control.
  if(delaySec*delayCount > forceStopSec)
  {
    Serial.println("Force stop watering");
    needWatering = false;
  }

  if(needWatering == true)
  {

    // water pump on
    digitalWrite(waterPumpPin, HIGH);

    Serial.print("Pumping for ");
    // Serial.print("Delay ");
    Serial.print(delaySec);
    Serial.print(" seconds");
    Serial.println();

    // delay milli seconds
    delay(delaySec*1000);

    delayCount++;
  }
  else
  {

    Serial.println("No pumping");

    // close water pump whether it's on or not
    digitalWrite(waterPumpPin, LOW);

    Serial.print("Deep sleep for ");
    Serial.print(sleepTimeSec);
    Serial.print(" seconds");
    Serial.println();

    // into deep sleep mode
    ESP.deepSleep(sleepTimeSec*1e6);
  }

}