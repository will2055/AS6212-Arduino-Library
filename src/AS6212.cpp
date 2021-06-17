/************************************************************
  Brandon Williams
  AS6212 Library Source File
  Creation Date: 07/16/20
  https://github.com/will2055/AS6212-Arduino-Library/src
  
  This file defines AS6212 core functions.
  
  Development environment specifics:
    IDE: Arduino 1.8.12
    Hardware Platform: Arduino Uno

  Special thanks to Madison Chodikov @ SparkFun Electronics
    for code examples from TMP117 Arduino Library
    (https://github.com/sparkfun/SparkFun_TMP117_Arduino_Library)
    
  Distributed as-is; no warranty is given.
************************************************************/

/*
  NOTE: Read for use for the most accurate readings from the sensor
  - Avoid heavy bypass traffic on the I2C bus for most accurate temperature readings
  - Use the highest available communication speeds
  - Use the minimal supply voltage acceptable for the system
*/

#include <Arduino.h>
#include <Wire.h>
#include "registers/AS6212_Registers.h"
#include "AS6212.h"

/* CONSTRUCTOR
    This function will use the main I2C port on the Arduino 
  by default, but this is configurable with the setBus function.
  This needs to be called when running the example sketches to
  initialize the sensor and be able to call to the library. 
*/

AS6212::AS6212(){}

/*
  Begin function. Sets the address for I2C communication.
  Returns True if checks pass.
 */

bool AS6212::begin(uint8_t sensorAddress, TwoWire &wirePort){
  _i2cPort = &wirePort;
  _deviceAddress = sensorAddress;

  _i2cPort->beginTransmission(_deviceAddress);
 
  if(_i2cPort->endTransmission() != 0){
    return false;
  }

  else{
    return true;
  }
}

uint8_t AS6212::getAddress(){

  return _deviceAddress;
  
}

uint16_t AS6212::readRegister(uint8_t reg, uint8_t size){

  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  _i2cPort->endTransmission();
  
  _i2cPort->requestFrom(_deviceAddress, size);
  
  uint8_t dataBuffer[size];
  
  int16_t datac = 0;

  if(_i2cPort->available() <= 2){
    for(size_t i = 0; i < size; i++) dataBuffer[i] = _i2cPort->read();
  }
  
  datac = ((dataBuffer[0] << 8) | dataBuffer[1]);

  return datac;
}

void AS6212::writeRegister(uint8_t reg, int16_t data){

  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  _i2cPort->write(highByte(data));
  _i2cPort->write(lowByte(data));
  _i2cPort->endTransmission();

}

float AS6212::readTempC(){

  int16_t digitalTempC = readRegister(TVAL,2);

  float finalTempC;

  if(digitalTempC < 32768){
    finalTempC = digitalTempC * 0.0078125;
  }

  if(digitalTempC >= 32768){
    finalTempC = ((digitalTempC - 1) * 0.0078125) * -1;
  }

  return finalTempC;
}

float AS6212::readTempF(){
  
  return readTempC() * 9.0 / 5.0 + 32.0;
  
}

float AS6212::getTLow(){
	int16_t lowTemp = readRegister(TLOW,2);
	
	float temp;

	if(lowTemp < 32768){
		temp = lowTemp * 0.0078125;
	}

	if(lowTemp >= 32768){
		temp = ((lowTemp - 1) * 0.0078125) * -1;
	}
	
	return temp;
}

/*
 * Sets TLow Threshold, if temp drops below the threshold then 
 * interrupt is triggered. Pointer to error flag to for custom
 * error reporting.
 */
bool AS6212::setTLow(int16_t lowLimit, uint8_t *tlow_err_flag){        

  if(lowLimit < getTHigh() && lowLimit != getTLow()){
    int16_t lowTemp = lowLimit / 0.0078125;
    writeRegister(TLOW, lowTemp);
    return true;
  }
  
  else if(lowLimit < getTHigh() && lowLimit == getTLow()){
	  //Serial.println("Value is the same as what is already set.");
	  *tlow_err_flag = 1;
	  return false;
  }
  
  else{
	  //Serial.println("Value is above the High Temperature Threshold.\n Please choose a different value.");
	  *tlow_err_flag = 2;
	  return false;
  }
}

float AS6212::getTHigh(){
	int16_t highTemp = readRegister(THIGH,2);
	
	float temp;

	if(highTemp < 32768){
		temp = highTemp * 0.0078125;
	}

	if(highTemp >= 32768){
		temp = ((highTemp - 1) * 0.0078125) * -1;
	}
	
	return temp;
}

bool AS6212::setTHigh(int16_t highLimit){
	
    if(highLimit > getTLow()){
		int16_t highTemp = highLimit / 0.0078125;
		writeRegister(THIGH, highTemp);
		return true;
  }
  
  else{
	  //Serial.println("Value is below the Low Temperature Threshold.\n Please choose a different value.");
	  return false;
  }
}

uint16_t AS6212::readConfig(){
	
		return readRegister(CONFIG,2);
	
}

void AS6212::setConfig(uint16_t targetState){
		
		writeRegister(CONFIG, targetState);
		
}

//Sleep Single-Shot mode (0xC1A0) returns odd register value (FFFFC1A0)
