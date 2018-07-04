#ifndef MEMORYFILE_H
#define MEMORYFILE_H
#include <Arduino.h>
#include <FS.h>
#include "log.h"

#define MAX_FILES 40


class memoryFile{
private:
    String fileNameArray[MAX_FILES];
    int8_t totalFileFromDir = 0;
    File f;
    Dir dir;
    FSInfo fs_info;
    void listErrorUpdate();
public:
    memoryFile();

    String readFile(String filePath);
    bool createFile(String filePath, String content);
    bool removeFile(String filePath);
    bool existsFile(String filePath);
    uint32_t sizeFile(String filePath);
    bool renameFile(String currentName, String newName);
      
    bool openDir(String dirPath);
    String fileNameFromDir();
    int8_t available();
    
    void updateFromFile(String filePath);

    size_t getTotalBytes();
    size_t getUsedBytes();

    
};

#include "memoryFile.impl.h";

memoryFile DiskD;

#endif
