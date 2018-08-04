#if 0
enum serverCmd
{
    SET_PINMODE         = 1,
    GET_PINMODE         = 2,
    SET_DIGITAL         = 3,
    GET_DIGITAL         = 4,
    SET_ANALOG          = 5,
    GET_ANALOG          = 6,
    
    SET_DATA            = 20,
    GET_DATA            = 21,
    SET_DATA_TO_ADDRESS = 22,
    GET_DATA_TO_ADDRESS = 23,
    
    READ_FILE               = 30,
    CREAD_FILE              = 31,
    REMOVE_FILE             = 32,
    EXISTS_FILE             = 33,
    RENAME_FILE             = 34,
    GET_LIST_FILE           = 35,
    GET_TOTAL_BYTES_FILE    = 36,
    GET_USED_BYTES_FILE     = 37,
    GET_FULL_INFO_FILE      = 38,
    UPDATE_FROM_FILE        = 45,
    STATUS_UPDATE_FILE      = 46,
    SUB_FILE                = 47,

    GET_STA_STATUS         = 50,
    SET_STA_CONNECT        = 51,
    SET_STA_DISCONNECT     = 52,
    GET_STA_LIST_NETWORKS  = 53,
    GET_STA_SSID           = 54,
    GET_STA_PASS           = 55,
    GET_STA_ISCONNECTED    = 56,
    GET_STA_LOCAL_IP       = 57,
    GET_STA_MAC_ADDRESS    = 58,
    GET_STA_SUBNET         = 59,
    GET_STA_GATEWAY        = 60,
    GET_STA_DNS            = 61,
    GET_STA_HOST_NAME      = 62,
    GET_STA_BSSID          = 63,
    GET_STA_RSSI           = 64,

    GET_AP_SSID             = 80,
    GET_AP_PASS             = 81,
    GET_AP_STA_CONECTEDS    = 82,
    GET_AP_IP               = 83,
    GET_AP_MAC              = 84,
    GET_AP_GATEWAY          = 85,
    GET_AP_SUBNET           = 86,
    SET_AP_SSID             = 87,
    SET_AP_PASS             = 88,
    SET_AP_IP               = 89,
    SET_AP_GATEWAY          = 90,
    SET_AP_SUBNET           = 91,

    RETURN_ARGUMENTS_TO_JSON = 100,

    SET_ESP_RESTART             = 110, // khởi động lại
    GET_ESP_RESET_REASON        = 111, // lý do restart cuối cùng
    GET_ESP_RESET_INFO          = 112,  // thông tin restart
    GET_ESP_FREE_HEAP           = 113,  // ram trống
    GET_ESP_CHIP_ID             = 114,  // ID chip esp8266
    GET_ESP_CORE_VERSION        = 115,  // phiên bản core
    GET_ESP_CPU_FREQ_MHZ        = 116,  // tần số CPU (MHz)
    GET_ESP_SKETCH_SIZE         = 117,  // dung lượng chương trình đang chạy
    GET_ESP_FREE_SKETCH_SPACE   = 118,  // dung lượng còn trống, có thể thêm chương trình
    GET_ESP_SKETCH_MD5          = 119,  // trả về chuỗi chữ thường chứa MD5 của bản phác thảo hiện tại
    GET_ESP_FLASH_CHIP_ID       = 120,  // IP Chip FLASH
    GET_ESP_FLASH_CHIP_SIZE     = 121,  // size Chip theo SDK (chương trình compiler thấy)
    GET_ESP_FLASH_CHIP_READ_SIZE = 122, // size chip thực tế
    GET_ESP_FLASH_CHIP_SPEED    = 123,  // tần số chíp (Hz)
    GET_ESP_CYCLE_COUNT         = 124,  // số chu kỳ cpu chạy được từ khi bắt đầu
    GET_ESP_VCC                 = 125,  // đọc điện áp nguồn cấp cho esp nhưng phải bỏ chân A0 đi -> bỏ API chức năng này

    GET_BLYNK_CONNECTED         = 140,
    SET_BLYNK_CONNECT           = 141,


    GET_PING_HOST               = 150,
    GET_PING_IP                 = 151,

    HTTP_SET_URL            = 160,
    HTTP_DOWNLOAD           = 161,
    HTTP_DOWNLOAD_LIST      = 162,

    NOTIFY_PING             = 200,

};
#endif


#include "serverGetString.h"


void serverRun() {
	_port80.handleClient();
	Task.run();
	yield();
}



void handleApi() { // /api?get:post application/json

	handleCmd();
}

/*
	Arduino.h
	#define INPUT 0x0
	#define OUTPUT 0x1
	#define INPUT_PULLUP 0x2
	https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/include/wl_definitions.h
	typedef enum {
		WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
		WL_IDLE_STATUS      = 0,
		WL_NO_SSID_AVAIL    = 1,
		WL_SCAN_COMPLETED   = 2,
		WL_CONNECTED        = 3,
		WL_CONNECT_FAILED   = 4,
		WL_CONNECTION_LOST  = 5,
		WL_DISCONNECTED     = 6
	} wl_status_t;
	API ESP
	https://arduino-esp8266.readthedocs.io/en/latest/libraries.html
*/

void handleCmd() {
	switch ((serverCmd)_port80.arg("cmd").toInt())
	{
		case SET_PINMODE:  {
				pinMode(_port80.arg("pin").toInt(), _port80.arg("mode").toInt());
				_port80.send(200);  break;
			}
		case SET_DIGITAL: {
				digitalWrite(_port80.arg("pin").toInt(), _port80.arg("value").toInt());
				_port80.send(200); break;
			}
		case GET_DIGITAL: {
				_port80.send(200, "application/json", "[\"" + String(digitalRead(_port80.arg("pin").toInt())) + "\"]");
				break;
			}
		case SET_ANALOG: {
				analogWrite(_port80.arg("pin").toInt(), _port80.arg("value").toInt());
				_port80.send(200); break;
			}
		case GET_ANALOG: {
				_port80.send(200, "application/json", "[\"" + String(analogRead(_port80.arg("pin").toInt())) + "\"]");
				break;
			}
		case SET_DATA: {
				_port80.send(200, "application/json", "[\"" + String(DB.importData(_port80.arg("page").toInt(), _port80.arg("value"))) + "\"]");
				break;
			}
		case GET_DATA: {
			uint8_t page = _port80.arg("page").toInt();
			if(page == 16){
				_port80.send(200, "application/json", getStringAP.ip());
			}else if(page == 17){
				_port80.send(200, "application/json", getStringAP.gw());
			}else if(page == 18){
				_port80.send(200, "application/json", getStringAP.sn());
			}else{
				_port80.send(200, "application/json", "[\"" + DB.exportData(page) + "\"]");
			}
			break;
		}
		case SET_DATA_TO_ADDRESS: {
				_port80.send(200, "application/json", "[\"" + String(DB.importDataToAddress(_port80.arg("address").toInt(), _port80.arg("value").toInt())) + "\"]");
				break;
			}
		case GET_DATA_TO_ADDRESS: {
				_port80.send(200, "application/json", "[\"" + String(DB.exportDataToAddress(_port80.arg("address").toInt())) + "\"]");
				break;
			}
		case READ_FILE: {
				if (!getStringFile.read())
					_port80.send(200, "text/plain", "Failed!!!");
				break;
			}
		case CREAD_FILE: {
				_port80.send(200, "application/json", getStringFile.cread(_port80.arg("path"), _port80.arg("content")));
				break;
			}
		case REMOVE_FILE: {
				_port80.send(200, "application/json", getStringFile.remove(_port80.arg("path")));
				break;
			}
		case EXISTS_FILE: {
				_port80.send(200, "application/json", getStringFile.exists(_port80.arg("path")));
				break;
			}
		case RENAME_FILE: {
				_port80.send(200, "application/json",  getStringFile.rename(_port80.arg("currentName"), _port80.arg("newName")));
				break;
			}
		case GET_LIST_FILE: {
				_port80.send(200, "application/json", getStringFile.list());
				break;
			}
		case UPDATE_FROM_FILE: {
				if (!DB.importData(DATA_UPDATE, _port80.arg("filePath")))
					_port80.send(200, "application/json", "[\"0\"]") ;
				_port80.send(200, "application/json", "[\"1\"]");
				getStringFile.updateFromFile();
				break;
			}
		case STATUS_UPDATE_FILE: {
				_port80.send(200, "application/json", getStringFile.statusUpdate);
				break;
			}
		case GET_TOTAL_BYTES_FILE: {
				_port80.send(200, "application/json", getStringFile.totalBytes());
				break;
			}
		case GET_USED_BYTES_FILE: {
				_port80.send(200, "application/json", getStringFile.usedBytes());
				break;
			}
		case GET_FULL_INFO_FILE: {
				_port80.send(200, "application/json", getStringFile.info());
				break;
			}
		case SUB_FILE : {
			_port80.send(200, "application", getStringFile.subFile(_port80.arg("path1"), _port80.arg("path2")));
			break;
		}
		case GET_STA_STATUS: {
				_port80.send(200, "application/json", getStringSTA.status());
				break;
			}
		case SET_STA_CONNECT: {
				_port80.send(200, "application/json", getStringSTA.begin(_port80.arg("ssid"), _port80.arg("pass"), _port80.arg("hostName")));
				break;
			}
		case SET_STA_DISCONNECT: {
				_port80.send(200, "text/plain", getStringSTA.disconnect());
				break;
			}
		case GET_STA_LIST_NETWORKS: {
				_port80.send(200, "application/json", getStringSTA.listNetworks());
				break;
			}
		case GET_STA_SSID: {
				_port80.send(200, "application/json", getStringSTA.ssid());
				break;
			}
		case GET_STA_PASS: {
				_port80.send(200, "application/json", getStringSTA.pass());
				break;
			}
		case GET_STA_ISCONNECTED: {
				_port80.send(200, "application/json", getStringSTA.isConnected());
				break;
			}
		case GET_STA_LOCAL_IP: {
				_port80.send(200, "application/json", getStringSTA.localIP());
				break;
			}
		case GET_STA_MAC_ADDRESS: {
				_port80.send(200, "application/json", getStringSTA.macAddress());
				break;
			}
		case GET_STA_SUBNET: {
				_port80.send(200, "application/json", getStringSTA.subnet());
				break;
			}
		case GET_STA_GATEWAY: {
				_port80.send(200, "application/json", getStringSTA.gateway());
				break;
			}
		case GET_STA_DNS: {
				_port80.send(200, "application/json", getStringSTA.dns());
				break;
			}
		case GET_STA_HOST_NAME: {
				_port80.send(200, "application/json", getStringSTA.hostName());
				break;
			}
		case GET_STA_BSSID: {
				_port80.send(200, "application/json", getStringSTA.bssid());
				break;
			}
		case GET_STA_RSSI: {
				_port80.send(200, "application/json", getStringSTA.rssi());
				break;
			}
		case GET_AP_SSID: {
				_port80.send(200, "application/json", getStringAP.ssid());
				break;
			}
		case GET_AP_PASS: {
				_port80.send(200, "application/json", getStringAP.pass());
				break;
			}
		case GET_AP_STA_CONECTEDS: {
				_port80.send(200, "application/json", getStringAP.staConnecteds());
				break;
			}
		case GET_AP_IP: {
				_port80.send(200, "application/json", getStringAP.ip());
				break;
			}
		case GET_AP_MAC: {
				_port80.send(200, "application/json", getStringAP.mac());
				break;
			}
		case GET_AP_GATEWAY: {
			_port80.send(200, "application/json", getStringAP.gw());
			break;
		}
		case GET_AP_SUBNET:{
			_port80.send(200, "application/json", getStringAP.sn());
			break;
		}
		case SET_AP_SSID:{
			_port80.send(200, "application/json", getStringAP.setSsid(_port80.arg("ssid")));
			break;
		}
		case SET_AP_PASS:{
			_port80.send(200, "application/json", getStringAP.setPass(_port80.arg("pass")));
			break;
		}
		case SET_AP_IP:{
			_port80.send(200, "application/json", getStringAP.setIp(_port80.arg("ip1"),_port80.arg("ip2"),_port80.arg("ip3"),_port80.arg("ip4")));
			break;
		}
		case SET_AP_GATEWAY:{
			_port80.send(200, "application/json", getStringAP.setGw(_port80.arg("ip1"),_port80.arg("ip2"),_port80.arg("ip3"),_port80.arg("ip4")));
			break;
		}
		case SET_AP_SUBNET : {
			_port80.send(200, "application/json", getStringAP.setSn(_port80.arg("ip1"),_port80.arg("ip2"),_port80.arg("ip3"),_port80.arg("ip4")));
			break;
		}
		case RETURN_ARGUMENTS_TO_JSON: {
				String c = String();
				for (uint8_t i = 0; i < _port80.args(); i++)
					c += "\"" + _port80.argName(i) + "\":\"" + _port80.arg(i) + "\",";
				c.remove(c.length() - 1);
				_port80.send(200, "application/json", "{" + c + "}"); break;
			}
		case SET_ESP_RESTART: {
				_port80.send(200, "application/json", getStringESP.restart());
				break;
			}
		case GET_ESP_RESET_REASON: {
				_port80.send(200, "text/plain", getStringESP.resetReason());
				break;
			}
		case GET_ESP_RESET_INFO: {
				_port80.send(200, "text/plain", getStringESP.resetInfo());
				break;
			}
		case GET_ESP_FREE_HEAP: {
				_port80.send(200, "application/json", getStringESP.freeHeap());
				break;
			}
		case GET_ESP_CHIP_ID: {
				_port80.send(200, "application/json", getStringESP.chipId());
				break;
			}
		case GET_ESP_CORE_VERSION: {
				_port80.send(200, "application/json", getStringESP.coreVersion());
				break;
			}
		case GET_ESP_CPU_FREQ_MHZ: {
				_port80.send(200, "application/json", getStringESP.cpuFreq());
				break;
			}
		case GET_ESP_SKETCH_SIZE: {
				_port80.send(200, "application/json", getStringESP.sketchSize());
				break;
			}
		case GET_ESP_FREE_SKETCH_SPACE: {
				_port80.send(200, "application/json", getStringESP.freeSketch());
				break;
			}
		case GET_ESP_SKETCH_MD5: {
				_port80.send(200, "application/json", getStringESP.sketchMD5());
				break;
			}
		case GET_ESP_FLASH_CHIP_ID: {
				_port80.send(200, "application/json", getStringESP.flashChipId());
				break;
			}
		case GET_ESP_FLASH_CHIP_SIZE: {
				_port80.send(200, "application/json", getStringESP.flashChipSize());
				break;
			}
		case GET_ESP_FLASH_CHIP_READ_SIZE: {
				_port80.send(200, "application/json", getStringESP.flashChipReadSize());
				break;
			}
		case GET_ESP_FLASH_CHIP_SPEED: {
				_port80.send(200, "application/json", getStringESP.flashChipSpeed());
				break;
			}
		case GET_ESP_CYCLE_COUNT: {
				_port80.send(200, "application/json", getStringESP.cycleCount());
				break;
			}
		case GET_ESP_VCC: {
				_port80.send(200, "text/plain", "Not Available!");
				break;
			}
		case GET_BLYNK_CONNECTED: {
				_port80.send(200, "application/json", "[\"" + (String)Blynk.connected() + "\"]");
			break;
		}
		case SET_BLYNK_CONNECT: {
				_port80.send(200, "application/json", "[\"1\"]");
				onBlynk();
				break;
		}
		case GET_PING_IP: {
				_port80.send(200, "application/json", getStringPingHost(_port80.arg("ip0").toInt(), _port80.arg("ip1").toInt(), _port80.arg("ip2").toInt(), (uint8_t)_port80.arg("ip3").toInt()));
				break;
			}
		case GET_PING_HOST:{
			_port80.send(200,"application/json", getStringPingHost(_port80.arg("host")));
			break;
		}
		case NOTIFY_PING: {
				_port80.send(200);
				break;
			}
		case HTTP_SET_URL: {
				_port80.send(200, "application/json", "[\"" + (String)http.setUrl(_port80.arg("url")) + "\"]");
				break;
		}
		case HTTP_DOWNLOAD: {
			_port80.send(200, "application/json", "[\"1\"]");
			Task.add([](){
				if(http.getStatus() == DOWNLOAD_FAILSE || http.getStatus() == DOWNLOAD_END){
					Task.remove("httpDownload");
				}
				http.download();
			}, "httpDownload");
			break;
		}
		case HTTP_DOWNLOAD_LIST : {
			_port80.send(200, "application/json", "[\"1\"]");
			Task.add([](){
				if(http.getStatus() == DOWNLOAD_LIST_END || http.getStatus() == DOWNLOAD_LIST_FAILSE){
					Task.remove("httpDownloadList");
				}
				http.downloadList();
			}, "httpDownloadList");
			break;
		}
		default: {
				_port80.send(200, "text/plain", "HELLO");
				break;
			}
	}
}



void handleUpFile(HTTPUpload& upload) {
	if (upload.status == UPLOAD_FILE_START) {
		blnprint("[HTTP SERVER] Upload name : " + (upload.filename.startsWith("/") ? upload.filename : "/" + upload.filename));
		blnprint("[HTTP SERVER] Upload size : ");
		if (SPIFFS.exists(upload.filename.startsWith("/") ? upload.filename : "/" + upload.filename))
			SPIFFS.remove(upload.filename.startsWith("/") ? upload.filename : "/" + upload.filename);
	} else if (upload.status == UPLOAD_FILE_WRITE) {
		File f = SPIFFS.open(upload.filename.startsWith("/") ? upload.filename : "/" + upload.filename, "a");
		if (f)
			f.write(upload.buf, upload.currentSize);
		f.close();
	} else if (upload.status == UPLOAD_FILE_END) {
		bprint(String(upload.totalSize) + " ... 100\% Upload success ");
	}
}

void handleCutFile(HTTPUpload& upload) {
	if (upload.status == UPLOAD_FILE_START) {
		blnprint("[HTTP SERVER] Start cut file name : " + (upload.filename.startsWith("/") ? upload.filename : "/" + upload.filename));
		blnprint("[HTTP SERVER] Upload size : ");
		if (SPIFFS.exists(upload.filename.startsWith("/") ? upload.filename : "/" + upload.filename))
			SPIFFS.remove(upload.filename.startsWith("/") ? upload.filename : "/" + upload.filename);
	} else if (upload.status == UPLOAD_FILE_WRITE) {
		uint8_t locationFile;
		if(upload.totalSize < FILE_CUT_MAX_SIZE)
			locationFile = 1;
		else
			locationFile = (upload.totalSize/FILE_CUT_MAX_SIZE) + 1;
		File f = SPIFFS.open(upload.filename.startsWith("/") ? (upload.filename + "." + (String)locationFile) : ("/" + upload.filename + "." + (String)locationFile), "a");
		if (f)
			f.write(upload.buf, upload.currentSize);
		f.close();
	} else if (upload.status == UPLOAD_FILE_END) {
		uint8_t totalFile = (upload.totalSize/FILE_CUT_MAX_SIZE) + 1;
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();
		root["name"] = upload.filename;
		root["size"] = upload.totalSize;
		root["total"] = totalFile;
		JsonArray& path = root.createNestedArray("path");
		for(uint8_t i = 1; i < totalFile + 1; i++){
			upload.filename.startsWith("/") ? path.add(upload.filename + "." + (String)i) : path.add("/" + upload.filename + "." + (String)i);
		}
		File f = SPIFFS.open(upload.filename.startsWith("/") ? upload.filename + ".js" : "/" + upload.filename + ".js", "w");
		if(f)
			root.printTo(f);
		f.close();
		bprint(String(upload.totalSize) + " ... 100\% Upload success");
	}
}


String getContentType(String path) {
	if (_port80.hasArg("download")) 	return "application/octet-stream";
	//else if (path.endsWith("log.txt"))  return "application/octet-stream";
	else if (path.endsWith(".htm")) 	return "text/html";
	else if (path.endsWith(".html")) 	return "text/html";
	else if (path.endsWith(".css")) 	return "text/css";
	else if (path.endsWith(".js")) 		return "application/javascript";
	else if (path.endsWith(".png")) 	return "image/png";
	else if (path.endsWith(".gif")) 	return "image/gif";
	else if (path.endsWith(".jpg")) 	return "image/jpeg";
	else if (path.endsWith(".ico")) 	return "image/x-icon";
	else if (path.endsWith(".xml")) 	return "text/xml";
	else if (path.endsWith(".pdf")) 	return "application/x-pdf";
	else if (path.endsWith(".zip")) 	return "application/x-zip";
	else if (path.endsWith(".gz")) 		return "application/x-gzip";
	else if (path.endsWith(".txt"))  	return "text/plain";
	else if (path.endsWith(".h"))    	return "text/plain";
	else if (path.endsWith(".cpp"))  	return "text/plain";
	else if (path.endsWith(".inp"))  	return "text/plain";
	else if (path.endsWith(".ino"))  	return "text/plain";
	else if (path.endsWith(".c"))    	return "text/plain";
	else if (path.endsWith(".properties"))return "text/plain";
	else if (path.endsWith(".py"))   	return "text/plain";
	else if (path.endsWith(".md"))   	return "text/plain";
	else if (path.endsWith(".sh"))   	return "text/plain";
	else if (path.endsWith(".bat"))  	return "text/plain";
	else if (path.endsWith(".json")) 	return "application/json";
	else if (path.endsWith(".png"))  	return "image/png";
	else if (path.endsWith(".svg"))  	return "image/svg+xml";
	else if (path.endsWith(".ttf"))  	return "application/x-font-ttf";
	else if (path.endsWith(".woff")) 	return "application/font-woff";
	else if (path.endsWith(".woff2"))	return "application/font-woff2";
	else if (path.endsWith(".eot"))  	return "application/vnd.ms-fontobject";
	else if (path.endsWith(".sfnt")) 	return "application/font-sfnt";
	else if (path.endsWith(".appcache"))return "text/cache-manifest";
	else if (path.endsWith(""))      	return "application/octet-stream";
	return "text/plain";
}

void handleFileRead(String path) {
	blnprint("[HTTP SERVER] Send file : " + path + " ---> ");
	if (!SPIFFS.exists(path)) {
		bprint("Failed!");
		handleNotFound();
		return;
	}
	String contentType = getContentType(path);
	File f = SPIFFS.open(path, "r");
	if (f)
		_port80.streamFile(f, contentType);
	f.close();
	bprint("Ready");
}


void handleNotFound() {
	if (!SPIFFS.exists(_port80.uri())) {
		String message = "File Not Found\n\n";
		message += "URI: ";
		message += _port80.uri();
		message += "\nMethod: ";
		message += (_port80.method() == HTTP_GET) ? "GET" : "POST";
		message += "\nArguments: ";
		message += _port80.args();
		message += "\n";
		for (uint8_t i = 0; i < _port80.args(); i++) {
			message += " " + _port80.argName(i) + ": " + _port80.arg(i) + "\n";
		}
		_port80.send(404, "text/plain", message);
		blnprint("[HTTP SERVER] NotFound : " + _port80.uri());
		return;
	}
	blnprint("[HTTP SERVER] serverOn : " + _port80.uri());
	_port80.on(_port80.uri(), []() {
		handleFileRead(_port80.uri());
	});
}



void serverBegin() {
	blnprint("[HTTP SERVER] SERVER CHECK CONFIG");
	blnprint("[HTTP SERVER] Number of restart : " + DB.exportData(DATA_FIRST_RUN));
	blnprint("[HTTP SERVER] ESP reset reason : " + ESP.getResetReason());
	blnprint("[HTTP SERVER] ESP reset info : " + ESP.getResetInfo());
	if (DB.exportData(DATA_FIRST_RUN) != BASIC_FIRST_RUN)
		runFirstTurn();
	// Setup WiFi AP
	WiFi.mode(WIFI_AP_STA);
	IPAddress local_ip(DB.exportDataToAddress(DATA_LOCAL_IP1), DB.exportDataToAddress(DATA_LOCAL_IP2), DB.exportDataToAddress(DATA_LOCAL_IP3), DB.exportDataToAddress(DATA_LOCAL_IP4));
	IPAddress gateway(DB.exportDataToAddress(DATA_GETWAY1), DB.exportDataToAddress(DATA_GETWAY2), DB.exportDataToAddress(DATA_GETWAY3), DB.exportDataToAddress(DATA_GETWAY4));
	IPAddress subnet(DB.exportDataToAddress(DATA_SUBNET1), DB.exportDataToAddress(DATA_SUBNET2), DB.exportDataToAddress(DATA_SUBNET3), DB.exportDataToAddress(DATA_SUBNET4));

	blnprint("[HTTP SERVER] Setting softAP ... " + String(WiFi.softAP(DB.exportData(DATA_APSSID).c_str(), DB.exportData(DATA_APPASS).c_str()) ? "Ready" : "Failed!"));
	blnprint("[HTTP SERVER] Setting softAPConfig ... " + String(WiFi.softAPConfig(local_ip, gateway, subnet) ? "Ready" : "Failed!"));
	blnprintf("[HTTP SERVER] SSID  : %s", DB.exportData(DATA_APSSID).c_str());
	blnprintf("[HTTP SERVER] PASS  : %s", DB.exportData(DATA_APPASS).c_str());
	blnprintf("[HTTP SERVER] IP    : %s", WiFi.softAPIP().toString().c_str());
	blnprintf("[HTTP SERVER] MAC   : %s", WiFi.softAPmacAddress().c_str());
	//end setup ap

	_port80.on("/Api", handleApi);
	_port80.on("/UpFile", HTTP_GET, []() {
		_port80.send(200, "text/html", "<html><body><form method='POST' action='' enctype='multipart/form-data'><input type='file' name='update' multiple><input type='submit' value='Update'></form></body></html>");
	});
	_port80.on("/UpFile", HTTP_POST, []() {
		_port80.send(200, "text/plain", "ok");
	}, []() {
		handleUpFile(_port80.upload());
	});

	_port80.on("/cut", HTTP_GET, [](){
		_port80.send(200, "text/html", "<html><body><form method='POST' action='' enctype='multipart/form-data'><input type='file' name='update' multiple><input type='submit' value='Update'></form></body></html>");
	});

	_port80.on("/cut", HTTP_POST, [](){
		_port80.send(200,"text/plain", "ok");
	}, [](){
		handleCutFile(_port80.upload());
	});

	_port80.on("/inline", []() {
		_port80.send(200, "text/plain", "hello");
	});

	_port80.onNotFound(handleNotFound);

	_port80.begin();
	_port8080.close();
}
