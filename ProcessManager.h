#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <Arduino.h>
#include "BasicDeviceInfo.h"
#include "debug/log.h"

#define MAX_OJECT_FUNC 10

class ProcessManager
{
public:
	ProcessManager(){
        blnprint("[PROCESS MANAGER] Start PROCESS MANAGER");
    }
	~ProcessManager(){
        blnprint("[PROCESS MANAGER] -----END PROCESS MANAGER-----");
    }
	
	bool add(bool (*condition)(void), void (*func1)(void), void(*func0)(void)){
        for(uint8_t i = 0; i < MAX_OJECT_FUNC; i++){
            if(_OjectFunc[i].condition == NULL){
                _OjectFunc[i].condition = condition;
                _OjectFunc[i].func1 = func1;
                _OjectFunc[i].func0 = func0;
                _totalProcess++;
                blnprint("[PROCESS MANAGER] ADD --> Total Process : " + String(_totalProcess) + " , ID : " + String(i));
                return 1;
            }
        }
        return 0;
	}


    void remove(void (*func)(void)){
        for(uint8_t i = 0; i < MAX_OJECT_FUNC; i++){
            if(_OjectFunc[i].func1 == func){
                _OjectFunc[i].condition = NULL;
                _OjectFunc[i].func1 = NULL;
                _OjectFunc[i].func0 = NULL;
                _totalProcess--;
                blnprint("[PROCESS MANAGER] REMOVE --> Total Process : " + String(_totalProcess));
                return;
            }
        }
    }

    void run(){
        if(_totalProcess == 0)
            return;
        uint8_t i=0, remain = _totalProcess;
        while(i<MAX_OJECT_FUNC && remain > 0){
            if(_OjectFunc[i].condition != NULL){
                if(_OjectFunc[i].condition()){
                    if(_OjectFunc[i].func1 != NULL)
                        _OjectFunc[i].func1();
                }
                else{
                    if(_OjectFunc[i].func0 != NULL)
                        _OjectFunc[i].func0();
                }
                remain--;
            }
            i++;
        }
    }

private:
	uint8_t _totalProcess = 0;
	struct ObjectFunc{
		bool (*condition)(void) = NULL;
		void (*func1)(void)	= NULL;
		void (*func0)(void) = NULL;
	}  _OjectFunc[MAX_OJECT_FUNC];

}   Task;




#endif
