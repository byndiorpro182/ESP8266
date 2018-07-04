

String getStringf(const char *fmt, ...){
    char buf[255];
    String str = "";
    va_list ap;
    va_start (ap, fmt);
    uint8_t len = vsnprintf(buf, 255, fmt, ap);
    va_end (ap);
    for (uint8_t i = 0; i < len; i++)
        str += buf[i];
    return str;
}

String getTimeFromTheStart(){
    static uint32_t h,m,s,ms;
    s = millis()/1000;
    m = s/60;
    h = m/60;
    return getStringf("[%02d:%02d:%02d:%03d] ", h, m%60, s%60, millis()%1000);
}

void bReadNote(){
    #ifdef DEBUG_WRITE_SERIAL
        #ifdef DEBUG_WRITE_LOG
            File f = SPIFFS.open(URL_FILE_LOG, "r");
            if(f)
                while(f.available() > 0)
                    Serial.print((char)f.read());
            f.close();
        #endif
    #endif
}

void bRemoveNote(){
    #ifdef DEBUG_WRITE_LOG
        SPIFFS.remove(URL_FILE_LOG);
    #endif
}

void b_print(String str){
    #ifdef DEBUG_WRITE_LOG
        File f = SPIFFS.open(URL_FILE_LOG, "a");
        f.print(str);
        f.close();
    #endif
}

void bprint(String str){
  #ifdef DEBUG_WRITE_SERIAL
    BSERIAL.print(str);
  #endif
  #ifdef DEBUG_WRITE_LOG
    b_print(str);
  #endif
}

void bprint(void){

}

void bprint(char c[]){
    uint8_t len = strlen(c);
    String str = "";
    for(uint8_t i = 0; i<len; i++){
        str += c[i];
    }
    bprint(str);
}

void bprint(bool b){
    bprint((String)b);
}

void bprint(char c){
    bprint((String)c);
}

void bprintf(const char *fmt, ...){
    char buf[255];
    String str = "";
    va_list ap;
    va_start (ap, fmt );
    uint8_t len = vsnprintf(buf, 255, fmt, ap);
    va_end (ap);
    for (uint8_t i = 0; i < len; i++)
        str += buf[i];
    bprint(str);
}

void blnprint(void){
    bprint("\r\n" + getTimeFromTheStart());
}

void blnprint(char c[]){
    uint8_t len = strlen(c);
    String str = "";
    for(uint8_t i = 0; i<len; i++){
        str += c[i];
    }
    blnprint(str);
}

void blnprint(bool b){
    blnprint((String)b);
}

void blnprint(char c){
    blnprint((String)c);
}

void blnprint(String str){
  bprint("\r\n" + getTimeFromTheStart() + str);
}

void blnprintf(const char *fmt, ...){
    char buf[255];
    String str = "";
    va_list ap;
    va_start (ap, fmt );
    uint8_t len = vsnprintf(buf, 255, fmt, ap);
    va_end (ap);
    for (uint8_t i = 0; i < len; i++)
        str += buf[i];
    bprint("\r\n" + getTimeFromTheStart() + str);
}