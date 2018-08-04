#ifndef HTTPSERVER_h
#define HTTPSERVER_h 

#include "BasicDeviceInfo.h"
#include "ProcessManager.h"
#include "memory/database.h"
#include "wifiAuto.h"
#include "httpClient.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>
#include <ESP8266Ping.h>
#include <BlynkSimpleEsp8266.h>
#include <ArduinoJson.h>



ESP8266WebServer _port80(80);
ESP8266WebServer _port8080(8080);


void handleApi(); // /Api
void handleCmd();
void handleUpFile(HTTPUpload& upload);  // /UpFile
void handleNotFound();

// mở các url khác
void handFileRead(String path);
String getContentType(String path);


void serverBegin();
void serverRun();

#include "httpServer.impl.h"



#endif
