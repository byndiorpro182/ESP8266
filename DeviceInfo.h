#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <Arduino.h>
#include "BasicDeviceInfo.h"
#include "log.h"
#include "memoryFile.h"
#include "database.h"



enum PinModeStatus{MODE_OUT,MODE_IN,MODE_PULL}; // PULLUP

class DeviceInfo{
  public:
    DeviceInfo();
    //PinModeStatus getModeStatus(uint8_t pin);
    uint16_t getValueA0();
    bool getValuePin(uint8_t pin);
    bool setValuePin(uint8_t pin, uint8_t value);
    
  private:
  PinModeStatus _mode[17];
};



DeviceInfo::DeviceInfo(){
  for(uint8_t i = 0; i<=16; i++){
    if(i > 5 && i < 12) continue;
    #ifdef DEBUG_WRITE_SERIAL
    if(i == 1 || i == 3)  continue;
    #endif
    pinMode(i,OUTPUT);
    _mode[i]  = MODE_OUT;
  }
}
uint16_t DeviceInfo::getValueA0(){
  return analogRead(A0);
}

bool DeviceInfo::getValuePin(uint8_t pin){
  if(_mode[pin] != MODE_IN){
    _mode[pin]  = MODE_IN;
    pinMode(pin,INPUT);
  }
   return digitalRead(pin);
}
bool DeviceInfo::setValuePin(uint8_t pin, uint8_t value){
  if(_mode[pin] != MODE_OUT){
    _mode[pin] = MODE_OUT;
    pinMode(pin, OUTPUT);
  }
  if(value == 0 || value == 1 || value == 255)
    digitalWrite(pin,value);
   else
   analogWrite(pin,value);
   return 1;
}





/*
 * pinmode chan  1,3 thi dung Serial.end();
 * 
 * 
 */



#endif
