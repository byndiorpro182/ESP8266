#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "DuongBui";
const char* password = "19011980";
//String html = "<!DOCTYPE HTML> <html> <head> <script type = 'text/javascript'> function WebSocketTest() { if ('WebSocket' in window) { alert('WebSocket is supported by your Browser!'); // Let us open a web socket var ws = new WebSocket('ws://192.168.1.75:23'); ws.onopen = function() { // Web Socket is connected, send data using send() ws.send('Message to send'); alert('Message is sent...'); }; ws.onmessage = function (evt) { var received_msg = evt.data; alert('Message is received...'); }; ws.onclose = function() { // websocket is closed. alert('Connection is closed...'); }; } else { // The browser doesn't support WebSocket alert('WebSocket NOT supported by your Browser!'); } } </script> </head> <body> <div id = 'sse'> <a href = 'javascript:WebSocketTest()'>Run WebSocket</a> </div> </body> </html>";
ESP8266WebServer server(80);
ESP8266WebServer stream(8080);


//ESP8266WebServer stream(8080);
uint8_t sukien = 0;
uint8_t statechay = 0;
uint32_t timeout = 0;

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", "oabc");
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
void chay(){
  statechay = 1;
  timeout = millis();
  Serial.println("\n co truy van den");
}

void chaytrongloop(){
    if (statechay == 0)
    return;
  if(sukien > 0){
    stream.send(200,"text/plain", String(sukien));
    statechay = 0;
  }else{
    if(millis() - timeout > 1500){
      stream.send(200, "text/plain", "ok");
      Serial.println("\ngui du lieu");
      statechay = 0;
    }
  }
}


void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);
 stream.on("/chay", chay);

  server.begin();
  stream.begin();
  Serial.println("HTTP server started");
  Serial.println("HTTP stream started");
  Serial.println(sizeof(server));
  Serial.println(sizeof(stream));
}

void loop(void){
  server.handleClient();
  stream.handleClient();
  chaytrongloop();
}
