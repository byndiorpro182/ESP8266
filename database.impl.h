


dataBase::dataBase(){
    EEPROM.begin(DATABASE_SIZE);
    blnprint("[DATABASE] Begin database done.");
}

bool dataBase::checkPage(uint8_t page){
    if(page > 0)
        if(page < TOTAL_PAGES)
            return 1;
    blnprintf("[DATABASE] Wrong page number : %d", page);
    return 0;
}

bool dataBase::importData(uint8_t page, String value){
    if(checkPage(page) && value.length() < PAGE_SIZE){
        blnprintf("[DATABASE] Import done! Page : %d. Value : %s", page, value.c_str());
        if(value == exportData(page))
            return 1;
        char toChar[PAGE_SIZE];
        value.toCharArray(toChar, value.length() + 1);
        uint8_t toCharLength = strlen(toChar);
        for(uint8_t i = 0; i < PAGE_SIZE; i++){
            if(i < toCharLength)
                EEPROM.write(i + page*PAGE_SIZE + 1,(uint8_t)toChar[i]);
            else{
                EEPROM.write(i + page*PAGE_SIZE + 1, 0);
                break;
            }
        }
        EEPROM.commit();
        return 1;
    }
    blnprintf("[DATABASE] Import failed! Page : %d. Value : %s", page, value.c_str());
    return 0;
}

String dataBase::exportData(uint8_t page){
    String result = "";
    if(checkPage(page)){
        for(uint8_t i = 0; i < PAGE_SIZE; i++){
            if(EEPROM.read(i + page*PAGE_SIZE + 1) == 0 || EEPROM.read( i + page*PAGE_SIZE + 1) == 255)
                break;
            result += (char)EEPROM.read( i + page*PAGE_SIZE + 1);
        }
    }
    return result;
}

bool dataBase::importDataToAddress(uint16_t address, uint8_t value){
    EEPROM.write(address,value);
    return 1;
}

uint8_t dataBase::exportDataToAddress(uint16_t address){
    return EEPROM.read(address);
}


uint16_t dataBase::databaseSize(){
    return DATABASE_SIZE;
}
