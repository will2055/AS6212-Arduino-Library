#include "AS6212.h"
#include <Wire.h>

AS6212 sensor;

void setup(){

  Wire.begin();
  Serial.begin(115200);

  Serial.println("Starting Sensor, here goes nothing...");
  delay(500);

  if(sensor.begin() == true){
    Serial.println("Beginning");
  }

  else{
    Serial.println("Well here we are, feeling foolish are we?");
    while(1);
  }
}

void loop(){

  float tempC = sensor.readTempC();
  float tempF = sensor.readTempF();

  Serial.println();
  Serial.print("Temperature (°C): ");
  Serial.println(tempC, 6);
  Serial.print("Temperature(°F): ");
  Serial.println(tempF, 6);
  
  delay(1000);
}
