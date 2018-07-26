#ifndef LOG_H
#define LOG_H
#include <Arduino.h>

#ifdef DEBUG_WRITE_LOG
    #include <FS.h>
    #define URL_FILE_LOG    "/log.txt"
#endif

#ifdef DEBUG_WRITE_SERIAL
    #define BSERIAL_BAUD    115200
    #define BSERIAL     Serial
#endif

class NoteBook{
    public:
        NoteBook(){
            #ifdef DEBUG_WRITE_SERIAL
                BSERIAL.begin(BSERIAL_BAUD);
                while(!BSERIAL)
                    delay(100);
            #endif
            #ifdef DEBUG_WRITE_LOG
                SPIFFS.begin();
                //SPIFFS.remove(URL_FILE_LOG);
            #endif
        }
};

NoteBook notebook;




String getStringf(const char *fmt, ...);
String getTimeFromTheStart();

void bReadNote();
void bRemoveNote();

void bprint(void);
void bprint(char c[]);
void bprint(char c);
void bprint(bool b);
void bprint(String str);

void blnprint(void);
void blnprint(char c[]);
void blnprint(char c);
void blnprint(bool b);
void blnprint(String str);

void bprintf(const char *fmt, ...);
void blnprintf(const char *fmt, ...);

#include "log.impl.h"

#endif

