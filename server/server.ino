#define DEBUG_WRITE_SERIAL
#define DEBUG_WRITE_LOG

#include <ESP8266WiFi.h>;
#include "log.h"
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>

// Hardcode WiFi parameters as this isn't going to be moving around.
const char* ssid = "DuongBui";
const char* password = "19011980";

// Start a TCP Server on port 5045
WiFiServer server(5045);

void setup() {
  //Serial.begin(115200);
  bprint("\r\n\n\n--------------Start---------");

  WiFi.begin(ssid,password);
  //Serial.println("");
  //Wait for connection
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
    blnprint(".");
  }
  //Serial.print("Connected to "); Serial.println(ssid);
  //Serial.print("IP Address: "); Serial.println(WiFi.localIP());
  blnprint((String)WiFi.localIP());
  // Start the TCP server
  server.begin();
}

void loop() {
  TCPServer();
      if (Serial.available() > 0){
    String str = "";
    while(Serial.available() > 0){
      str += (char)Serial.read();  
    }
    if(str == "log"){
      bReadNote();
    }
    if(str == "deletelog"){
      bRemoveNote();
    }
  }
}

void TCPServer () {
    WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      //Serial.println("Connected to client");
      blnprint("co client ket noi");
    }
    
    String str = "";
           unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      blnprint("ko thay client gui tin hieu");
      client.stop();
      return;
    }
  }
    while (client.available() > 0) {
      // Read incoming message
      str += (char)client.read();
      // Send back something to the clinet
      
      // Echo input on Serial monitor
      //Serial.write(inChar);
      
    }
    client.write(str.c_str());
    blnprint("client send : " + str);
  }
}
   
