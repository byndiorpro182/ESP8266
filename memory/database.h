#ifndef DATABASE_H
#define DATABASE_H

#include <EEPROM.h>
#include "../debug/log.h"

#define DATABASE_SIZE 	1024
#define PAGE_SIZE 		40
#define TOTAL_PAGES 	26 // 0->25

class dataBase{
  	private:
    	bool checkPage(uint8_t page);
  	public:
	    dataBase();
	    ~dataBase();
	    
	    bool importData(uint8_t page, String value);
	    bool importDataToAddress(uint16_t address, uint8_t value);
	    
	    String exportData(uint8_t page);
	    uint8_t exportDataToAddress(uint16_t address);

	    uint16_t databaseSize();
};

#include "database.impl.h"

dataBase DB;

#endif

