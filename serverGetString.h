struct GET_STRING_STATION
{
	String wl_status_tToString(wl_status_t type) {
		switch (type)
		{
			case WL_IDLE_STATUS: {
					return "{\"String\":\"WL_IDLE_STATUS\",\"code\":\"0\"}";
				}
			case WL_NO_SSID_AVAIL: {
					return "{\"String\":\"WL_NO_SSID_AVAIL\",\"code\":\"1\"}";
				}
			case WL_CONNECTED: {
					return "{\"String\":\"WL_CONNECTED\",\"code\":\"3\"}";
				}
			case WL_CONNECT_FAILED: {
					return "{\"String\":\"WL_CONNECT_FAILED\",\"code\":\"4\"}";
				}
			case WL_DISCONNECTED: {
					return "{\"String\":\"WL_DISCONNECTED\",\"code\":\"6\"}";
				}
			default: {
					return "";
				}
		}
	}

	String status() {
		return wl_status_tToString(WiFi.status());
	}

	String begin(String ssid, String pass, String hostName) {
		return wl_status_tToString(wifiAuto.connect(ssid.c_str(), pass.c_str()));
	}

	String disconnect() { //C:\Users\Admin\Documents\ArduinoData\packages\esp8266\hardware\esp8266\2.4.0\libraries\ESP8266WiFi\src\ESP8266WiFiSTA.cpp :: 289
		WiFi.disconnect();          // if true  thì tắt chế độ STA, ko thì vẫn bật STA, cả 2 đều set ssid = 0 và pass = 0;
		return "[\"1\"]";
	}

	String listNetworks() {
		uint8_t total = WiFi.scanNetworks();
		String s = "[";
		for (uint8_t i = 0; i < total; i++) {
			s += "{\"ssid\":\""          +  WiFi.SSID(i)                    + "\",";
			s += "\"encryptionType\":\"" +  String(WiFi.encryptionType(i))  + "\",";
			s += "\"RSSI\":\""           +  String(WiFi.RSSI(i))            + "\",";
			s += "\"BSSIDstr\":\""       +  WiFi.BSSIDstr(i)                + "\",";
			s += "\"channel\":\""        +  String(WiFi.channel(i))         + "\",";
			s += "\"isHidden\":\""       +  String(WiFi.isHidden(i))        + "\"},";
		}
		s.remove(s.length() - 1);
		return s + "]";
	}

	String ssid() {
		return "[\"" + DB.exportData(14) + "\"]";
	}

	String pass() {
		return "[\"" + DB.exportData(15) + "\"]";
	}

	String isConnected() {
		return "[\"" + String(WiFi.isConnected()) + "\"]";
	}

	String localIP() {
		return "[\"" + WiFi.localIP().toString() + "\"]";
	}

	String macAddress() {
		return "[\"" + WiFi.macAddress() + "\"]";
	}

	String subnet() {
		return "[\"" + WiFi.subnetMask().toString() + "\"]";
	}

	String gateway() {
		return "[\"" + WiFi.gatewayIP().toString() + "\"]";
	}

	String dns() {
		return "[\"" + WiFi.dnsIP().toString() + "\"]";
	}

	String hostName() {
		return "[\"" + WiFi.hostname() + "\"]";
	}

	String bssid() {
		return "[\"" + WiFi.BSSIDstr() + "\"]";
	}

	String rssi() {
		return "[\"" + String(WiFi.RSSI()) + "\"]";
	}
} getStringSTA;


struct GET_STRING_ACCESS_POINT
{
	String ssid() {
		return "[\"" + DB.exportData(DATA_APSSID) + "\"]";
	}

	String pass() {
		return "[\"" + DB.exportData(DATA_APPASS) + "\"]";
	}

	String staConnecteds() {
		return "[\"" + String(WiFi.softAPgetStationNum()) + "\"]";
	}

	String ip() {
		String ret = (String)DB.exportDataToAddress(DATA_LOCAL_IP1) + ".";
			ret += (String)DB.exportDataToAddress(DATA_LOCAL_IP2) + ".";
			ret += (String)DB.exportDataToAddress(DATA_LOCAL_IP3) + ".";
			ret += (String)DB.exportDataToAddress(DATA_LOCAL_IP4);
		return "[\"" + ret + "\"]";
	}

	String mac() {
		return "[\"" + WiFi.softAPmacAddress() + "\"]";
	}

	String gw(){
		String ret = (String)DB.exportDataToAddress(DATA_GETWAY1) + ".";
			ret += (String)DB.exportDataToAddress(DATA_GETWAY2) + ".";
			ret += (String)DB.exportDataToAddress(DATA_GETWAY3) + ".";
			ret += (String)DB.exportDataToAddress(DATA_GETWAY4);
		return "[\"" + ret + "\"]";
	}

	String sn(){
		String ret = (String)DB.exportDataToAddress(DATA_SUBNET1) + ".";
		ret += (String)DB.exportDataToAddress(DATA_SUBNET2) + ".";
		ret += (String)DB.exportDataToAddress(DATA_SUBNET3) + ".";
		ret += (String)DB.exportDataToAddress(DATA_SUBNET4);
		return "[\"" + ret + "\"]";
	}

	String setSsid(String ssid){
		if(ssid.length() < 2)
			return "[\"0\"]";
		return "[\"" + (String)DB.importData(DATA_APSSID,ssid) + "\"]";
	}

	String setPass(String pass){
		if(pass.length() < 8)
			return "[\"0\"]";
		return "[\"" + (String)DB.importData(DATA_APPASS,pass) + "\"]";
	}

	String setIp(String ip1, String ip2, String ip3, String ip4){
		DB.importDataToAddress(DATA_LOCAL_IP1, ip1.toInt());
		DB.importDataToAddress(DATA_LOCAL_IP2, ip2.toInt());
		DB.importDataToAddress(DATA_LOCAL_IP3, ip3.toInt());
		DB.importDataToAddress(DATA_LOCAL_IP4, ip4.toInt());
		return "[\"1\"]";
	}

	String setGw(String ip1, String ip2, String ip3, String ip4){
		DB.importDataToAddress(DATA_GETWAY1, ip1.toInt());
		DB.importDataToAddress(DATA_GETWAY2, ip2.toInt());
		DB.importDataToAddress(DATA_GETWAY3, ip3.toInt());
		DB.importDataToAddress(DATA_GETWAY4, ip4.toInt());
		return "[\"1\"]";
	}

	String setSn(String ip1, String ip2, String ip3, String ip4){
		DB.importDataToAddress(DATA_SUBNET1, ip1.toInt());
		DB.importDataToAddress(DATA_SUBNET2, ip2.toInt());
		DB.importDataToAddress(DATA_SUBNET3, ip3.toInt());
		DB.importDataToAddress(DATA_SUBNET4, ip4.toInt());
		return "[\"1\"]";
	}
} getStringAP;
/*

		void getFile(){
				static WiFiClient * stream = NULL;
				static std::unique_ptr<HTTPUpload> _currentUpload;
				if(stream == NULL){
						stream = http.client.getStreamPtr();
						_currentUpload->status = UPLOAD_FILE_START;
						_currentUpload->filename = http.getUrlTail();
						_currentUpload->totalSize = http.client.getSize();
						_currentUpload->currentSize = 0;
						http.remain = _currentUpload->totalSize;
						handleUpFile(*_currentUpload);

				}else{
						if(http.client.connected() && http.remain > 0){
								size_t size = stream->available();
								if(size){
										_currentUpload->status = UPLOAD_FILE_WRITE;
										_currentUpload->currentSize = stream->readBytes(_currentUpload->buf,((size > sizeof(_currentUpload->buf)) ? sizeof(_currentUpload->buf) : size));
										http.remain -= _currentUpload->currentSize;
										handleUpFile(*_currentUpload);
								}
						}else{
								_currentUpload->status = UPLOAD_FILE_END;
								handleUpFile(*_currentUpload);
								_currentUpload->status = UPLOAD_FILE_ABORTED;
								_currentUpload->filename = String();
								_currentUpload->totalSize = 0;
								_currentUpload->currentSize = 0;
								if(http.remain == 0){
										blnprint("update thanh cong");
								}else{
										blnprint("tai file bi loi~");
								}
								Task.remove(getFile);
						}
				}
		}

		String HttpClientBegin(String url, bool saveFile){
				http.url = url;
				http.client.begin(url);
				http.code = http.client.GET();
				if(http.code > 0){
						if(!saveFile){
								String str = String();
								for(uint8_t i = 0; i < http.client.headers(); i++){
										str += "\"" + http.client.headerName(i) + "\":\"" + http.client.header(i) + "\",";
								}
								str += "\"code\":\"" + String(http.code) + "\",";
								str += "\"size\":\"" + String(http.client.getSize()) + "\",";
								str += "\"content\":\"" + http.client.getString() + "\"";
								return "{" + str + "}";
						}else{
								if(http.code == 200){
										Task.add([](){return true;},getFile,[](){});
										return String();
								}
								return String();
						}
				}
		}
*/


struct GET_STRING_FILE
{
	bool read() {
		if (SPIFFS.exists(_port80.arg("path"))) {
			File f = SPIFFS.open(_port80.arg("path"), "r");
			if (f) {
				_port80.streamFile(f, "text/plain");
				f.close();
				return 1;
			}
		}
		return 0;
	}

	String cread(String path, String content) {
		File f = SPIFFS.open(path.startsWith("/") ? path : "/" + path, "w");
		if (f) {
			f.print(content);
			f.close();
			return "[\"1\"]";
		}
		return "[\"0\"]";
	}

	String remove(String path) {
		return "[\"" + String(SPIFFS.remove(path)) + "\"]";
	}

	String exists(String path) {
		return "[\"" + String(SPIFFS.exists(path)) + "\"]";
	}

	String rename(String currentName, String newName) {
		return "[\"" + String(SPIFFS.rename(currentName, newName)) + "\"]";
	}

	String list() {
		Dir dir = SPIFFS.openDir("/");
		String content = "[";
		while (dir.next()) {
			content += "{\"name\":\"" + dir.fileName() + "\",\"size\":\"" + dir.fileSize() + "\"},";
		}
		content.remove(content.length() - 1);
		content += "]";
		return content;
	}

	String totalBytes() {
		FSInfo info; SPIFFS.info(info);
		return "[\"" + String(info.totalBytes) + "\"]";
	}

	String usedBytes() {
		FSInfo info; SPIFFS.info(info);
		return "[\"" + String(info.usedBytes) + "\"]";
	}

	String info() {
		FSInfo info; String content = "{"; SPIFFS.info(info);
		content += "\"totalBytes\":" + String(info.totalBytes) + ",";
		content += "\"usedBytes\":" + String(info.usedBytes) + ",";
		content += "\"blockSize\":" + String(info.blockSize) + ",";
		content += "\"pageSize\":" + String(info.pageSize) + ",";
		content += "\"maxOpenFiles\":" + String(info.maxOpenFiles) + ",";
		content += "\"maxPathLength\":" + String(info.maxPathLength) + "}";
		return content;
	}
	void listErrorUpdate() {
		blnprint("[MEMORY_FILE] UPDATE_ERROR_OK                 (0)");
		blnprint("[MEMORY_FILE] UPDATE_ERROR_WRITE              (1)");
		blnprint("[MEMORY_FILE] UPDATE_ERROR_ERASE              (2)");
		blnprint("[MEMORY_FILE] UPDATE_ERROR_READ               (3");
		blnprint("[MEMORY_FILE] UPDATE_ERROR_SPACE              (4)");
		blnprint("[MEMORY_FILE] UPDATE_ERROR_SIZE               (5)");
		blnprint("[MEMORY_FILE] UPDATE_ERROR_STREAM             (6)");
		blnprint("[MEMORY_FILE] UPDATE_ERROR_MD5                (7)");
		blnprint("[MEMORY_FILE] UPDATE_ERROR_FLASH_CONFIG       (8)");
		blnprint("[MEMORY_FILE] UPDATE_ERROR_NEW_FLASH_CONFIG   (9)");
		blnprint("[MEMORY_FILE] UPDATE_ERROR_MAGIC_BYTE         (10)");
		blnprint("[MEMORY_FILE] UPDATE_ERROR_BOOTSTRAP          (11)");
	}
	String statusUpdate = "[\"0\"]";
	void updateFromFile() {
		static bool state = false;
		if (state == true)
			return;
		String filePath = DB.exportData(DATA_UPDATE);
		if (SPIFFS.exists(filePath)) {
			if (filePath.endsWith(".bin")) {
				File f = SPIFFS.open(filePath, "r");
				uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
				if (f) {
					if (f.size() < maxSketchSpace) {
						if (Update.begin(maxSketchSpace)) {
							if (Update.write(f) == f.size()) {
								if (Update.end(true)) {
									blnprint("[MEMORY_FILE] Update Success.");
									state = true;
									statusUpdate = "[\"1\"]";
									return;
									/*
										for(uint8_t i = 5; i > 0; i--){
											blnprintf("[MEMORY_FILE] %d", i);
											delay(1000);
										}
										bprint("\r\n.\n.\n...");
										ESP.restart();
									*/
								} else
									blnprintf("[MEMORY_FILE] UPDATE ERROR : %d End", Update.getError());
							} else
								blnprintf("[MEMORY_FILE] UPDATE ERROR : %d Write", Update.getError());
						} else
							blnprintf("[MEMORY_FILE] UPDATE ERROR : %d Begin", Update.getError());
					} else
						blnprintf("[MEMORY_FILE] Can not enough Sketch Space. File size = %d, Max Sketch Space = %d", f.size(), maxSketchSpace);
				} else
					blnprintf("[MEMORY_FILE] Can not open the File : %s", filePath.c_str());
			} else
				blnprintf("[MEMORY_FILE] Filename %s not format", filePath.c_str());
		} else
			blnprintf("[MEMORY_FILE] Filepath %s don't exist.", filePath.c_str());
		listErrorUpdate();
		state = false;
		return;
	}

	bool sub2File(String path1, String path2){ // nối 2 vào 1
		bool ret = 0;
		blnprintf("[HTTP_CLIENT] Sub File %s + %s --> %s", path1.c_str(),path2.c_str());
		if(path1 != path2){
			if(SPIFFS.exists(path1) && SPIFFS.exists(path2)){
				File f1 = SPIFFS.open(path1, "a");
				File f2 = SPIFFS.open(path2, "r");
				if(f1 && f2){
					uint32_t sizef = f1.size() + f2.size();
					while(f2.available()){
						uint8_t buf[4096] = {0};
						int c = f2.read(buf, (sizeof(buf) < f2.available()) ? sizeof(buf) : f2.available());
						f1.write(buf, c);
					}
					f1.close();
					f2.close();
					File f = SPIFFS.open(path1, "r");
					if(f){
						if(f.size() == sizef){
							ret = 1;
							blnprint("[FILE] Sub file Success.");
						}
					}else{
						blnprint("[FILE] Open file " + path1 + " Failse !!!");
					}
					f.close();
				}else{
					blnprintf("[FILE] Open file %s or %s failse !!!", path1.c_str(), path2.c_str());
				}
			}else{
				blnprint("[FILE] File does not exists !!!");
			}
		}else{
			blnprintf("[FILE] path1 : %s, path : %s => Failse !!!", path1.c_str(), path2.c_str());
		}
		return ret;
	}

	String subFile(String path1, String path2){
		return "[\"" + String(sub2File(path1,path2)) + "\"]";
	}

} getStringFile;


struct GET_STRING_ESP
{
	String restart() {
		Task.add([]() {
			static bool isFirstTurn = true;
			static uint32_t timeOut;
			if (isFirstTurn == true) {
				timeOut = millis();
				isFirstTurn = false;
			} else {
				if (millis() - timeOut > 5000) {
					isFirstTurn = true;
					ESP.restart();
				}
			}
		}, "restart");
		return  "{\"1\"}";
	}
	String resetReason() {  // text/plain
		return ESP.getResetReason();
	}
	String resetInfo() {    // text/plain
		return ESP.getResetInfo();
	}
	String freeHeap() {
		return "{\"" + String(ESP.getFreeHeap()) + "\"}";
	}
	String chipId() {
		return "{\"" + String(ESP.getChipId()) + "\"}";
	}
	String coreVersion() {
		return "{\"" + String(ESP.getCoreVersion()) + "\"}";
	}
	String cpuFreq() {
		return "{\"" + String(ESP.getCpuFreqMHz()) + "\"}";
	}
	String sketchSize() {
		return "{\"" + String(ESP.getSketchSize()) + "\"}";
	}
	String freeSketch() {
		return "{\"" + String(ESP.getFreeSketchSpace()) + "\"}";
	}
	String sketchMD5() {
		return "{\"" + String(ESP.getSketchMD5()) + "\"}";
	}
	String flashChipId() {
		return "{\"" + String(ESP.getFlashChipId()) + "\"}";
	}
	String flashChipSize() {
		return "{\"" + String(ESP.getFlashChipSize()) + "\"}";
	}
	String flashChipReadSize() {
		return "{\"" + String(ESP.getFlashChipRealSize()) + "\"}";
	}
	String flashChipSpeed() {
		return "{\"" + String(ESP.getFlashChipSpeed()) + "\"}";
	}
	String cycleCount() {
		return "{\"" + String(ESP.getCycleCount()) + "\"}";
	}

} getStringESP;


String getStringPingHost(uint8_t ip0, uint8_t ip1, uint8_t ip2, uint8_t ip3) {
	String str = String();
	IPAddress ip(ip0, ip1, ip2, ip3);
	str = "{\"state\":\"" + String(Ping.ping(ip)) + "\",";
	str += "\"delay\":\"" + String(Ping.averageTime()) + "\"}";
	return str;
}

String getStringPingHost(String host) {
	String str = String();
	str = "{\"state\":\"" + String(Ping.ping(host.c_str())) + "\",";
	str += "\"delay\":\"" + String(Ping.averageTime()) + "\"}";
	return str;
}
