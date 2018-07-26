#ifndef HTTPSERVER_h
#define HTTPSERVER_h 

#include "BasicDeviceInfo.h"
#include "ProcessManager.h"
#include "debug/log.h"
#include "memory/database.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>
#include <ESP8266Ping.h>



ESP8266WebServer _port80(80);
ESP8266WebServer _port8080(8080);

struct
{
	HTTPClient client;
	String url; //https://vietjack.com/cplusplus/truyen_tham_so_boi_tham_chieu_trong_cplusplus.jsp
	int code;
	int remain;
	String getUrlTail(){
		uint16_t len = url.length();
		String ret = String();
		char buf[len];
		url.toCharArray(buf,len + 1);
		for(uint16_t i=len; i>0; --i){
			if(buf[i] == '/'){
				for(uint16_t j = i ; i < len; i++){
					ret += (String)buf[j];
				}
				return ret;
			}
		}
		return String();
	}
}http;

class MemoryFile
{
public:
    MemoryFile(){
        SPIFFS.begin();
        //SPIFFS.format();
    }
};
MemoryFile memoryFile;

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
