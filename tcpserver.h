/*
 *  1byte ch峄� huy
 *  2byte ID tin nh岷痭  (cmd , ki峄僽 l峄噉h)  1*255+1 , h峄� c啤 s峄� 256  // 0-> 255
 *  2byte 膼峄� d脿i tin nh岷痭
 *  content  0123456
 *  [ID][MessageID][leng/status][body]
 *  [1b][2b][2b][end]
 * 
 * 
 */
#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <Arduino.h>
#include "debug/log.h"
#include <ESP8266WiFi.h>

#define MAX_SRV_STREAMS     3
#define SERVER_PORT         23
#define TIME_OUT_STREAM     1e5
#define BYTE_COMMAND_SERVER 85
#define BYTE_COMMAND_CLIENT 170

enum ESPcmd{
    SET_PINMODE         = 1,
    GET_PINMODE         = 2,
    SET_DIGITAL         = 3,
    GET_DIGITAL         = 4,
    SET_ANALOG          = 5,
    GET_ANALOG          = 6,
    
    SET_DATA            = 7,
    GET_DATA            = 8,
    SET_DATA_TO_ADDRESS = 9,
    GET_DATA_TO_ADDRESS = 10,
    
    READ_FILE               = 11,
    CREAD_FILE              = 12,
    REMOVE_FILE             = 13,
    EXISTS_FILE             = 14,
    RENAME_FILE             = 15,
    GET_LIST_FILE           = 16,
    UPDATE_FROM_FILE        = 17,
    GET_TOTAL_BYTES_FILE    = 18,
    GET_USED_BYTES_FILE     = 19,

    GET_WIFI_STATUS         = 20,
    SET_WIFI_CONNECT        = 21,
    SET_WIFI_DISCONNECT     = 22,
    GET_WIFI_LIST_NETWORKS  = 23,
    GET_WIFI_STA_SSID       = 24,
    GET_WIFI_STA_PASS       = 25,
    GET_WIFI_AP_SSID        = 26,
    GET_WIFI_AP_PASS        = 27,

    RETURN_CONTENT_READ     = 100,

    NOTIFY_SUCCSESSFUL      = 200,
    NOTIFY_PING             = 201,

};

#if 0
#define MODE_OUT    1
#define MODE_IN     2
#define MODE_PULL   3
#endif

class tcpServer
{
public:
	tcpServer(uint16_t port);
	void handleClient();
    void begin();
    void close();
    void stop();
    String getCurrentTotalStreams();
private: 
    struct connectionStream{
        WiFiClient client;

        uint8_t command[512];
        int16_t currentAddress = 0;

        uint32_t lastReceveMessageTime = 0;
        bool status = false;
        uint8_t id;
        String infoClient = "";
    };
  
    bool setClientToStream();
    void manageStream();
    void checkMessageFromStream();
    void getMessage(connectionStream *Stream);
    void cmd(connectionStream *Stream,uint16_t length);
    void sendMessageToClient(connectionStream *Stream, uint8_t messageId2); // khi không có body
    void sendMessageToClient(connectionStream *Stream, uint8_t messageId2, String *body);
    void sendMessageToClient(connectionStream *Stream, uint8_t messageId1, uint8_t messageId2, String *body);
    
    String decodeString(connectionStream *Stream, uint16_t length, String name);
    //void encodeString(String *str, String name, String value);
  
    WiFiServer _server;
    connectionStream _Stream[MAX_SRV_STREAMS]; 
    int8_t _currentTotalStreams;
    bool statusSync = false;
};

#include "tcpserver.impl.h"

#endif

