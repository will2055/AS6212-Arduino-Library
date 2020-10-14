/************************************************************
  Brandon Williams
  Example 1 Simple Temperature Reading
  Creation Date: 07/15/20
  https://github.com/will2055/AS6212-Arduino-Library/examples
  
  This file defines AS6212 registers and includes some configuration
  presets.
  
  Development environment specifics:
    IDE: Arduino 1.8.12
    Hardware Platform: Arduino Uno
    
  Distributed as-is; no warranty is given.
************************************************************/

#include "AS6212.h"
#include <Wire.h>

AS6212 sensor;

void setup(){

  Wire.begin();
  Serial.begin(115200);

  Serial.println("Starting Sensor, here goes nothing...");
  delay(500);

//Begin communication to the sensor. Returns false if device doesn't respond

  if(sensor.begin() == true){
    Serial.println("Beginning");
  }

  else{
    Serial.println("Well here we are, feeling foolish are we?");
    while(1);
  }
}

void loop(){

//Initialize temperature variables
  float tempC = sensor.readTempC();
  float tempF = sensor.readTempF();

  Serial.println();
  Serial.print("Temperature (°C): ");
  Serial.println(tempC, 6);							//Reads out 6 characters of the temperature float
  Serial.print("Temperature(°F): ");
  Serial.println(tempF, 6);							//Reads out 6 characters of the temperature float
  
  delay(1000);
}
