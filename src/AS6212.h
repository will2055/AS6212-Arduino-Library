/************************************************************
  Brandon Williams
  AS6212 Library Header File
  Creation Date: 07/16/20
  https://github.com/will2055/AS6212-Arduino-Library/src
  
  This file defines AS6212 core function definitions and
  initializers.
  
  Development environment specifics:
    IDE: Arduino 1.8.12
    Hardware Platform: Arduino Uno

  Special thanks to Madison Chodikov @ SparkFun Electronics
    for code examples from TMP117 Arduino Library
    (https://github.com/sparkfun/SparkFun_TMP117_Arduino_Library)
    
  Distributed as-is; no warranty is given.
************************************************************/

#ifndef __AS6212_H__
#define __AS6212_H__

#include <Wire.h>
#include <Arduino.h>
#include "AS6212_Registers.h"

class AS6212{
  public:
    AS6212();
    bool begin(uint8_t sensorAddress = 0x48, TwoWire &wirePort = Wire);
    uint8_t getAddress();
    float readTempC();
    float readTempF();
    float getTLow();
    bool setTLow(int16_t lowLimit);
    float getTHigh();
    bool setTHigh(int16_t highLimit);
    uint16_t readConfig();
    void setConfig(uint16_t targetState);
  private:
    TwoWire *_i2cPort = NULL;
    uint8_t _deviceAddress;
    uint16_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg, int16_t data);
};

#endif
