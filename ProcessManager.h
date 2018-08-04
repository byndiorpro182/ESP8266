#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <Arduino.h>
#include "BasicDeviceInfo.h"
#include "debug/log.h"

#define MAX_OBJECT_FUNC 10

class ProcessManager
{
	public:
		ProcessManager() {
			for (uint8_t i = 0; i < MAX_OBJECT_FUNC; i++) {
				_ObjectFunc[i] = NULL;
			}
			blnprint("[PROCESS MANAGER] Start PROCESS MANAGER");
		}
		~ProcessManager() {
			blnprint("[PROCESS MANAGER] -----END PROCESS MANAGER-----");
		}

		bool add(void (*func)(void), String name) {
			for (uint8_t i = 0, remain = _totalProcess; i < MAX_OBJECT_FUNC && remain > 0; i++) {
				if (_ObjectFunc[i] != NULL){
					remain--;
					if (_ObjectFunc[i]->name == name)
						blnprint("[PROCESS MANAGER] ADD --> False!!!!!");
					return 0;
				}

			}
			for (uint8_t i = 0; i < MAX_OBJECT_FUNC; i++) {
				if (_ObjectFunc[i] == NULL) {
					_ObjectFunc[i] = new ObjectFunc;
					_ObjectFunc[i]->id = i;
					_ObjectFunc[i]->name = name;
					_ObjectFunc[i]->func = func;
					_totalProcess++;
					blnprintf("[PROCESS MANAGER] ADD --> id : %d, name : %s, total process : %d", i, name.c_str(), _totalProcess);
					return 1;
				}
			}
			blnprint("[PROCESS MANAGER] ADD --> False!!!");
			return 0;
		}

		bool remove(uint8_t id) {
			if (id > MAX_OBJECT_FUNC)
				return 0;
			_totalProcess--;
			blnprintf("[PROCESS MANAGER] REMOVE --> id : %d, name : %s, total process : %d", id, _ObjectFunc[id]->name.c_str(), _totalProcess);
			_ObjectFunc[id]->id = 0;
			_ObjectFunc[id]->func = NULL;
			_ObjectFunc[id]->name = String();
			delete _ObjectFunc[id];
			_ObjectFunc[id] = NULL;
			return 1;
		}

		bool remove(String name) {
			for (uint8_t i = 0; i < MAX_OBJECT_FUNC; i++)
				if (_ObjectFunc[i] != NULL)
					if (_ObjectFunc[i]->name == name)
						return remove(i);
			return 1;
		}

		bool remove(void (*func)(void)) {
			for (uint8_t i = 0; i < MAX_OBJECT_FUNC; i++)
				if (_ObjectFunc != NULL)
					if (_ObjectFunc[i]->func == func)
						return remove(i);
			return 1;
		}

		void run() {
			if (_totalProcess == 0)
				return;
			static uint8_t i, remain;
			i = 0;
			remain = _totalProcess;
			while (i < MAX_OBJECT_FUNC && remain > 0) {
				if (_ObjectFunc[i] != NULL) {
					_ObjectFunc[i]->func();
					remain--;
				}
				i++;
			}
		}

	private:
		uint8_t _totalProcess = 0;
		struct ObjectFunc {
			uint8_t id = NULL;
			String name = String();
			void (*func)(void) = NULL;
		} ;

		ObjectFunc* _ObjectFunc[MAX_OBJECT_FUNC];

}   Task;



#endif
