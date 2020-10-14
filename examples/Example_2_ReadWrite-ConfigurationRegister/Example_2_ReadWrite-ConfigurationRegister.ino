/************************************************************
  Brandon Williams
  Example 2 Read/Write Configuration Register
  Creation Date: 10/14/20
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
  
  uint16_t configReg;
  configReg = sensor.readConfig();			//Read configuration register and put the register value in configReg
  Serial.print("Configuration Register(BINARY): ");
  Serial.println(configReg, BIN);				//Print out Binary configuration register value
  
  Serial.println("Do you wish to write to the register (y/n) ?: ");
  while(1){
	  char incomingByte;
	  if(Serial.available>0){
		  
		  incomingByte = Serial.read();
		  
		  if(incomingByte = 'y'){
			  uint16_t newReg;
			  Serial.println("Please enter the new register value (BINARY): ")
			  while(1){
				  if(Serial.available>0){
					  newReg = Serial.read();
					  sensor.setConfig(newReg);
					  Serial.print("New configuration register: ");
					  configReg = sensor.readConfig();
					  Serial.println(configReg, BIN);
					  break;
				  }
			  }
			  break;
		  }
		  if(incomingByte = 'n'){
			  break;
		  }
	  }
  }
}
