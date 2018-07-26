
memoryFile::memoryFile(){
    SPIFFS.begin();
    //SPIFFS.format();
    blnprint("[MEMORY_FILE] Begin memory file done.");
}


String memoryFile::readFile(String filePath){
    f = SPIFFS.open(filePath, "r");
    String str = "";
    if(f)
        while(f.available() > 0)
            str += (char)f.read();
    else
        blnprintf("[MEMORY_FILE] Open file %s failed !!!",filePath.c_str());
    f.close();
    return str;
}

bool memoryFile::createFile(String filePath, String content){
    f = SPIFFS.open(filePath, "w");
    if(f && f.print(content)){
        f.close();
        return 1;
    }else
        blnprintf("[MEMORY_FILE] Create file %s failed !!!", filePath.c_str());
    return 0;
}

bool memoryFile::removeFile(String filePath){
    SPIFFS.remove(filePath);
    blnprintf("[MEMORY_FILE] Remove file %s failed", filePath.c_str());
    return 1;
}

bool memoryFile::renameFile(String currentName, String newName){
    blnprintf("[MEMORY_FILE] Rename file %s to %s", currentName.c_str(), newName.c_str());
    return SPIFFS.rename(currentName,newName);
}

bool memoryFile::existsFile(String filePath){
    return SPIFFS.exists(filePath);
}
  
uint32_t memoryFile::sizeFile(String filePath){
    f = SPIFFS.open(filePath, "r");
    uint32_t sizef = f.size();
    f.close();
    return sizef;
}

bool memoryFile::openDir(String dirPath){
    dir = SPIFFS.openDir(dirPath);
    while(dir.next()){
        fileNameArray[totalFileFromDir] = dir.fileName();
        totalFileFromDir++;
    }
    blnprintf("[MEMORY_FILE] Total Files : %d", totalFileFromDir);
    return 1;
}

String memoryFile::fileNameFromDir(){
    totalFileFromDir--;
    blnprintf("[MEMORY_FILE] %d : %s", totalFileFromDir + 1, fileNameArray[totalFileFromDir].c_str());
    return fileNameArray[totalFileFromDir];
}

int8_t memoryFile::available(){
    return totalFileFromDir;
}

size_t memoryFile::getTotalBytes(){
    SPIFFS.info (fs_info);
    return fs_info.totalBytes;
}

size_t memoryFile::getUsedBytes(){
    SPIFFS.info (fs_info);
    return fs_info.usedBytes;
}


void memoryFile::updateFromFile(String filePath){
    if(existsFile(filePath)){
        if(filePath.endsWith(".bin")){
            f = SPIFFS.open(filePath, "r");
            uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
            if(f){
                if(f.size() < maxSketchSpace){
                    if(Update.begin(maxSketchSpace)){
                        if(Update.write(f) == f.size()){
                            if(Update.end(true)){
                                blnprint("[MEMORY_FILE] Update Success: Restart after 5 seconds");
                                for(uint8_t i = 5; i > 0; i--){
                                    blnprintf("[MEMORY_FILE] %d", i);
                                    delay(1000);
                                }
                                bprint("\r\n.\n.\n...");
                                ESP.restart();
                            }else
                                blnprintf("[MEMORY_FILE] UPDATE ERROR : %d",Update.getError());
                        }else
                            blnprintf("[MEMORY_FILE] UPDATE ERROR : %d",Update.getError());
                    }else
                        blnprintf("[MEMORY_FILE] UPDATE ERROR : %d",Update.getError());
                }else
                    blnprintf("[MEMORY_FILE] Can not enough Sketch Space. File size = %d, Max Sketch Space = %d", f.size(), maxSketchSpace);
            }else
                blnprintf("[MEMORY_FILE] Can not open the File : %s", filePath.c_str());
        }else
            blnprintf("[MEMORY_FILE] Filename %s not format", filePath.c_str());
    }else
        blnprintf("[MEMORY_FILE] Filepath %s don't exist.", filePath.c_str());
    listErrorUpdate();
}

void memoryFile::listErrorUpdate(){
    blnprint("[MEMORY_FILE] UPDATE_ERROR_OK                 (0)");
    blnprint("[MEMORY_FILE] UPDATE_ERROR_WRITE              (1)");
    blnprint("[MEMORY_FILE] UPDATE_ERROR_ERASE              (2)");
    blnprint("[MEMORY_FILE] UPDATE_ERROR_READ               (3");
    blnprint("[MEMORY_FILE] UPDATE_ERROR_SPACE              (4)");
    blnprint("[MEMORY_FILE] UPDATE_ERROR_SIZE               (5)");
    blnprint("[MEMORY_FILE] UPDATE_ERROR_STREAM             (6)");
    blnprint("[MEMORY_FILE] UPDATE_ERROR_MD5                (7)");
    blnprint("[MEMORY_FILE] UPDATE_ERROR_FLASH_CONFIG       (8)");
    blnprint("[MEMORY_FILE] UPDATE_ERROR_NEW_FLASH_CONFIG   (9)");
    blnprint("[MEMORY_FILE] UPDATE_ERROR_MAGIC_BYTE         (10)");
    blnprint("[MEMORY_FILE] UPDATE_ERROR_BOOTSTRAP          (11)");
}
















  

