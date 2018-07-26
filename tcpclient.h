#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <Arduino.h>
#include "debug/log.h"
#include <ESP8266WiFi.h>
#define BYTE_COMMAND_SERVER 85
#define BYTE_COMMAND_CLIENT 170

class tcpclient
{
public:
	tcpclient();
	tcpclient(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet, uint16_t port);
	tcpclient(uint32_t address, uint16_t port);
	tcpclient(const uint8_t *address,uint16_t port);

	void close();
	void stop();

	bool connect();
	bool connect(IPAddress serverIP, uint16_t port);
	bool connected();
	void waitForServer();

	void sendMessageToServer(uint8_t messageID2);
	void sendMessageToServer(uint8_t messageID2, String body);
	void sendMessageToServer(uint8_t messageID1, uint8_t messageID2, String body);

	void setPort(uint16_t port);
	void setServerIP(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
	void setServerIP(uint32_t address);
	void setServerIP(const uint8_t *address);
	
	uint16_t getPort();
	uint32_t getServerIP();
private:
	struct IPAddess{
		uint8_t bytes[4];
		uint32_t dword;
	} _serverIP;
	uint16_t _port;
	
	void IpToDec();
	void DecToIp();
	void checkMessageFromServer();
	void getMessage();
	WiFiClient _client;
	
	uint8_t _command[512];
    int16_t _currentAddress = 0;
    uint32_t theStartTime;
	bool _stateWaitForServer = true;
};


bool tcpclient::connected(){
	return _client.connected();
}
void tcpclient::waitForServer(){
	if(!_stateWaitForServer)
		return;
	checkMessageFromServer();
}

void tcpclient::checkMessageFromServer(){
	if(_client.available() > 0){
		getMessage();
	}
}

void tcpclient::getMessage(){
	while(_client.available() > 0){
		_command[_currentAddress] = _client.read();
		_currentAddress++;
	}
	if(_currentAddress > 4){
		if(_command[0] != BYTE_COMMAND_SERVER){
			blnprintf("[TCP_CLIENT] >> Command %d => Failse struct", _command[0]);
			_currentAddress = 0;
			return;
		}
		uint16_t len = _command[3]*255 + _command[4];
		if(_currentAddress - (len + 5) >= 0){
			blnprintf("[TCP_CLIENT] >> [%d]:[%d%d]:[%05d]:[", _command[0],_command[1],_command[2],len);
			for(uint16_t i=5; i<len+5; i++)
				bprint((String)(char)_command[i]);
			bprint("]");
			_currentAddress = 0;
		}
	}
}

void tcpclient::sendMessageToServer(uint8_t messageID2){
	sendMessageToServer(0,messageID2,"");
}

void tcpclient::sendMessageToServer(uint8_t messageID2, String body){
	sendMessageToServer(0,messageID2, body);
}

void tcpclient::sendMessageToServer(uint8_t messageID1, uint8_t messageID2, String body){
	uint16_t length = body.length();
	_client.write(BYTE_COMMAND_CLIENT); // 10101010
	_client.write(messageID1);
	_client.write(messageID2);
	if(length == 0){
		_client.write((uint8_t)0);
		_client.write((uint8_t)0);
	}else if(length < 256){
		_client.write((uint8_t)0);
		_client.write((uint8_t)length);
		_client.write(body.c_str());
	}else{
		_client.write((uint8_t)length/255);
		_client.write((uint8_t)length%255);
		_client.write(body.c_str());
	}
	blnprintf("[TCP_CLIENT] << [%d]:[%d%d]:[%05d]:[",BYTE_COMMAND_CLIENT,messageID1,messageID2,length);
	bprint(body);
	bprint("]");
}

tcpclient::tcpclient(){
	_serverIP.dword = 0;
	_port = 0;
}

tcpclient::tcpclient(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet, uint16_t port){
	setServerIP(first_octet, second_octet, third_octet, fourth_octet);
    setPort(port);
}

tcpclient::tcpclient(uint32_t address, uint16_t port){
	setServerIP(address);
	setPort(port);
}
tcpclient::tcpclient(const uint8_t *address,uint16_t port){
	setServerIP(address);
	setPort(port);
}

void tcpclient::stop(){
	_client.stop();
}

void tcpclient::close(){
	stop();
}

bool tcpclient::connect(){
	IPAddress IP(_serverIP.bytes[0],_serverIP.bytes[1],_serverIP.bytes[2],_serverIP.bytes[3]);
	_client.connect(IP, _port);
}

bool tcpclient::connect(IPAddress IP, uint16_t port){
	//char charIP[IP.toString().length()];
	//IP.toString().toCharArray(charIP, IP.toString().length() + 1);
	_port = port;
	_client.connect(IP, _port);
}

void tcpclient::setPort(uint16_t port){
	_port = port;
}

uint16_t tcpclient::getPort(){
	return _port;
}

void tcpclient::setServerIP(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet){
	_serverIP.bytes[0] = first_octet;
    _serverIP.bytes[1] = second_octet;
    _serverIP.bytes[2] = third_octet;
    _serverIP.bytes[3] = fourth_octet;
    IpToDec();
}

void tcpclient::setServerIP(uint32_t address){
	_serverIP.dword = address;
	DecToIp();
}
void tcpclient::setServerIP(const uint8_t *address){
	memcpy(_serverIP.bytes, address, sizeof(_serverIP.bytes));
	IpToDec();
}

uint32_t tcpclient::getServerIP(){
	return _serverIP.dword;
}



void tcpclient::IpToDec(){
	uint32_t buf[4];
	buf[0] = _serverIP.bytes[0]*256*256*256;
	buf[1] = _serverIP.bytes[0]*256*256;
	buf[2] = _serverIP.bytes[0]*256;
	buf[3] = _serverIP.bytes[0];
	_serverIP.dword = buf[0] + buf[1] + buf[2] + buf[3];
}

void tcpclient::DecToIp(){
	uint32_t buf[3];	
	buf[2] = _serverIP.dword/256;
	buf[1] = buf[2]/256;
	buf[0] = buf[1]/256;
	_serverIP.bytes[3] = _serverIP.dword%256;
	_serverIP.bytes[2] = buf[2]%256;
	_serverIP.bytes[1] = buf[1]%256;
	_serverIP.bytes[0] = buf[0];
}
#include "tcpclient.impl.h"

#endif
