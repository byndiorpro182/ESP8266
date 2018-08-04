#ifndef HTTPCLIENT_H
#define	HTTPCLIENT_H

#include "BasicDeviceInfo.h"
#include "ProcessManager.h"
#include "memory/database.h"
#include "wifiAuto.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>
#include <ESP8266Ping.h>
#include <ArduinoJson.h>

#define HTTP_TIMEOUT_DOWNLOAD 2e4

enum statusDownload
{
	DOWNLOAD_BEGIN 		= 1,
	DOWNLOAD_GET_FILE	= 2,
	DOWNLOAD_END		= 3,
	DOWNLOAD_FAILSE		= 4,
	DOWNLOAD_LIST_END	= 5,
	DOWNLOAD_LIST_FAILSE = 6
};

class httpClient
{
	public:
		httpClient();
		~httpClient();
		bool setUrl(String url);
		statusDownload getStatus();
		void download();
		void downloadList();
	private:
		statusDownload _status;
		int _code;
		int _size;
		int _remain;
		uint32_t _timeOut;
		String _url;
		String _name;
		HTTPClient _http;
		WiFiClient *_client;

		String _urlBuf;
		String _fileList;
		int8_t _currentNumberFile;
		bool _stateDownFileList;
} http;

httpClient::httpClient() {
	_status = DOWNLOAD_FAILSE;
	_code = 0;
	_size = 0;
	_remain = 0;
	_url = String();
	_name = String();
	blnprint("[HTTP_CLIENT] Begin http client done.");
}

httpClient::~httpClient() {
	blnprint("[HTTP_CLIENT] Close http client.");
}


bool httpClient::setUrl(String url) {
	if (!url.startsWith("http://"))
		return 0;
	_url = url;
	_urlBuf = url;
	_urlBuf.remove(_urlBuf.lastIndexOf("/"));
	_name = String();
	uint16_t len = _url.length();
	for (uint16_t i = _url.lastIndexOf("/"); i < len; i++) {
		_name += _url.charAt(i);
	}
	if (SPIFFS.exists(_name)) SPIFFS.remove(_name);
	_status = DOWNLOAD_BEGIN;
	_code = 0;
	_size = 0;
	_remain = 0;
	_stateDownFileList = false;
	_currentNumberFile = -1;
	blnprint("[HTTP_CLIENT] URL : " + _url);
	blnprint("[HTTP_CLIENT] File name : " + _name);
	return 1;
}

statusDownload httpClient::getStatus() {
	return _status;
}

void httpClient::downloadList() {
	switch (_status) {
		case DOWNLOAD_BEGIN: {
				if (_stateDownFileList) {
					download();
				} else {
					if (_name.endsWith(".js")) {
						download();
					} else {
						blnprint("[HTTP_CLIENT] Download list Failse !!! File name : " + _name);
						_status = DOWNLOAD_LIST_FAILSE;
					}
				}
				break;
			}
		case DOWNLOAD_GET_FILE: {
				download();
				break;
			}
		case DOWNLOAD_END : {
				if (!_stateDownFileList) {
					_stateDownFileList = true;
					_fileList = _name;
				}
				_currentNumberFile++;
				File f = SPIFFS.open(_fileList, "r");
				if (f) {
					char buff[f.size()];
					f.readBytes(buff, f.size());
					DynamicJsonBuffer jsonBuffer;
					JsonObject& root = jsonBuffer.parseObject(buff);
					if (_currentNumberFile == root["total"]) {
						_status = DOWNLOAD_LIST_END;
					} else {
						_name = root["path"][_currentNumberFile].asString();
						_url = _urlBuf + _name;
						_code = 0;
						_size = 0;
						_remain = 0;
						_status = DOWNLOAD_BEGIN;
						blnprint("[HTTP_CLIENT] URL : " + _url);
						blnprint("[HTTP_CLIENT] File name : " + _name);
					}
				} else {
					_stateDownFileList = DOWNLOAD_LIST_FAILSE;
					blnprint("[HTTP_CLIENT] Download list Failse !!! Open file Failse.");
				}
				break;
			}
		case DOWNLOAD_LIST_END : {
				break;
			}
		case DOWNLOAD_FAILSE : {
				_status = DOWNLOAD_LIST_FAILSE;
				break;
			}
		case DOWNLOAD_LIST_FAILSE : {
				break;
			}
		default : {
				break;
			}
	}
}

void httpClient::download() {
	switch (_status) {
		case DOWNLOAD_BEGIN : {
				_http.begin(_url);
				_code = _http.GET();
				if (_code == HTTP_CODE_OK) {
					if (_http.getSize() > 0) {
						_status = DOWNLOAD_GET_FILE;
						_size = _http.getSize();
						_remain = _size;
						_client = _http.getStreamPtr();
						_timeOut = millis();
						blnprint("[HTTP_CLIENT] Size : " + (String)_size + " --> Start download ...");
						break;
					} else {
						blnprint("[HTTP_CLIENT] Download Failse !!! Size = " + String(_http.getSize())); // nếu size = -1 thì là do server không gửi size file.
					}
				} else {
					blnprint("[HTTP_CLIENT] Download Failse !!! Code = " + (String)_code);
				}
				_status = DOWNLOAD_FAILSE;
				break;
			}
		case DOWNLOAD_GET_FILE: {
				if (millis() - _timeOut < HTTP_TIMEOUT_DOWNLOAD) {
					if (_http.connected()) {
						if (_remain > 0) {
							size_t size = _client->available();
							if (size) {
								uint8_t buff[1024] = {0};
								int16_t c = _client->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
								_remain -= c;
								File f = SPIFFS.open(_name, "a");
								if (f) {
									f.write(buff, c);
								} else {
									_status = DOWNLOAD_FAILSE;
									_http.end();
									blnprint("[HTTP_CLIENT] Open file Failse !!!");
								}
								f.close();
								if (_remain == 0) {
									_status = DOWNLOAD_END;
									_http.end();
									blnprint("[HTTP_CLIENT] Download success");
								}
							}
							break;
						} else {
							blnprint("[HTTP_CLIENT] Download Failse !!! Remain = " + (String)_remain);
						}
					} else {
						blnprint("[HTTP_CLIENT] Download Failse !!! HTTP Disconnected.");
					}
				} else {
					blnprint("[HTTP_CLIENT] Download Failse !!! Time out");
				}
				_status = DOWNLOAD_FAILSE;
				_http.end();
				break;
			}
		case DOWNLOAD_END: {
				break;
			}
		case DOWNLOAD_FAILSE: {
				break;
			}
		default : {
				break;
			}
	}
}




#endif
