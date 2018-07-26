#define DEBUG_WRITE_SERIAL
#define DEBUG_WRITE_LOG

#include "httpServer.h"
/*
void ham1(){
  Serial.println("ham1");
}
void ham2(){
  Task.remove(ham1);
}
void ham3(){
  static uint32_t time = millis();
  if(millis() - time > 1000){
      Serial.println("ham3");
      time = millis();
  }
}
bool hamx(){
  static uint32_t time = millis();
  if(millis() - time > 3000){
    time = millis();
    return 1;
  }else{
    return 0;
  }
}


bool hamy(){
  static uint8_t i = 0;
  i++;
  if(i > 10){
  return 0;
  }else{
    return 1;
  }
}
*/
void setup()
{
  serverBegin();
}

void loop()
{
	serverRun();
  //Task.run();
}
