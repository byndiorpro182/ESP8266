#ifndef LOG_H
#define LOG_H
#include <Arduino.h>
#include <FS.h>

#ifdef DEBUG_WRITE_LOG
    #define URL_FILE_LOG    "/log.txt"
#endif

#ifdef DEBUG_WRITE_SERIAL
    #define BSERIAL_BAUD    115200
    #define BSERIAL     Serial
#endif

class NoteBook{
    public:
        NoteBook(){
            //SPIFFS.format();
            SPIFFS.begin();
            //SPIFFS.remove(URL_FILE_LOG);
            #ifdef DEBUG_WRITE_LOG
                File f = SPIFFS.open(URL_FILE_LOG, "a");
                if(f)
                    f.print("\n.\n.\n1\n2\n3\n[LOG] SPIFFS and LOG begin finish.");
                f.close();
            #endif
            #ifdef DEBUG_WRITE_SERIAL
                BSERIAL.begin(BSERIAL_BAUD);
                while(!BSERIAL)
                    delay(100);
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

