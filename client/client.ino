#define DEBUG_WRITE_SERIAL
#define DEBUG_WRITE_LOG

#include <ESP8266WiFi.h>;
#include "log.h"

 const char *ssid = "DuongBui";
 const char *password = "19011980";
 IPAddress server_ip(192, 168, 1, 75);
 #define PORT  23
   // port 23 la port cua esp8226 lam AP da khoi tao.
 WiFiClient client;
void setup() {
   uint8_t i = 0;
   bprint("\n\n\n -----------------\n\n\nstart");
   //Serial.begin(115200);
   WiFi.begin(ssid, password);
//Serial.print("\nConnecting to ");
   //Serial.println(ssid);
   // Kiem tra tình trang ket noi, neu chua ket noi duoc
   // se in chuoi "connecting..." tren man hinh serial terminal.
   while (WiFi.status() != WL_CONNECTED) { 
     //Serial.println("Connecting...");
     delay(500);
   }
   blnprintf("ssid : %s", WiFi.SSID().c_str());
 }
 unsigned long previousMillis = 0;
 void loop() {
   if (WiFi.status() == WL_CONNECTED) {
     // Kiem tra neu client(STA) chua duoc ket noi.
     // Kiem tra tiep tuc neu khong duoc ket noi den IP va PORT cua server(AP
     // thi in ra serial terminal chuoi "connection failed".
       if (!client.connect(server_ip, PORT)) {
         //Serial.println("connection failed");
          blnprint("khong ket noi duoc");
         delay(1000);
         return;
       }
       uint32_t timesend = millis();
       uint32_t dem = 0;
       while(client.connected()){
        
         if(Serial.available() > 0){
      String str = "";
      while(Serial.available() > 0){
      str += (char)Serial.read();  
      }
      
    if(str == "log"){
      bReadNote();
      return;
    }
    if(str == "deletelog"){
      bRemoveNote();
      return;
    }
    blnprint("client send : " + str);
    uint16_t len = str.length();
    char header[] = {'t','t'};
        client.write(170);      
        client.write(header);
        client.write(len/255);
        client.write(len%255);
        client.print(str);
       unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      blnprint("ko thay phan hoi");
      client.stop();
      return;
    }
  }
  blnprint("server send : ");
    while(client.available()){
      bprint((String)(char)client.read());
    }
       }else{
        if(millis() - timesend > 10000){
         client.write(170);
         client.write((uint8_t)0);
        client.write(200);
        String str = "hieu=123&hoang=abc&van=x12@";
        client.write((uint8_t)str.length()/255);
        client.write((uint8_t)str.length()%255);
        client.write(str.c_str());
        client.flush();
               unsigned long timeout = millis();
        while (client.available() == 0) {
          if (millis() - timeout > 3000) {
            blnprint("ko thay phan hoi");
            client.stop();
            return;
          }
        }
        delay(1000);
          blnprint("server send : ");
        while(client.available())
          bprint((String)(uint8_t)client.read() + ",");
      
         timesend = millis(); 
        }
       }
       yield();
     }
    blnprint("mat ket noi, chuan bi ket noi lai");
    delay(3000);
}

 }
