
WiFiAuto::WiFiAuto() {
	WiFi.persistent(false); // không lưu giữ ssid và pass lại
	WiFi.mode(WIFI_AP_STA);
	blnprint("[WIFI_AUTO] Begin WiFi Auto done. mode AP and STA.");
}

WiFiAuto::~WiFiAuto() {
	blnprint("[WIFI_AUTO] ---- Close WiFi Auto -----");
}

void WiFiAuto::saveInfoWiFiSuccess() {
	blnprintf("[WIFI_AUTO] Connect success : SSID: %s, PASS: %s", WiFi.SSID().c_str(), WiFi.psk().c_str());
	blnprintf("[WIFI_AUTO] IP : %s", WiFi.localIP().toString().c_str());
	DB.importData(DATA_SSIDNOW, WiFi.SSID());
	DB.importData(DATA_PASSNOW, WiFi.psk());
	for(uint8_t i = 1; i < 6; i++){
		if(DB.exportData(i) == WiFi.SSID()){
			if(DB.exportData(i+5) != WiFi.psk()){
				DB.importData(i+5,WiFi.psk());
			}
			return;
		}
	}
	if((int)EEPROM.read(DATA_ADDRESS_SAVE_WIFI) < 5){
        EEPROM.write(DATA_ADDRESS_SAVE_WIFI, (uint8_t)((int)EEPROM.read(DATA_ADDRESS_SAVE_WIFI) + 1));
    }else{
        EEPROM.write(DATA_ADDRESS_SAVE_WIFI,(uint8_t)1);
    }
    EEPROM.commit();
    DB.importData((int)EEPROM.read(DATA_ADDRESS_SAVE_WIFI)    ,WiFi.SSID());
    DB.importData((int)EEPROM.read(DATA_ADDRESS_SAVE_WIFI) + 5, WiFi.psk());
    blnprintf("[WIFI_AUTO] New SSID : %s, PASS : %s, Location save : %d",WiFi.SSID().c_str(), WiFi.psk().c_str(),(int)EEPROM.read(DATA_ADDRESS_SAVE_WIFI));
}

wl_status_t WiFiAuto::connect(String ssid, String pass) {
	_stateAutoConnect = false;
	WiFi.mode(WIFI_AP_STA);
	WiFi.hostname(BASIC_STA_HOSTNAME);
	if (pass.length() < 8) {
		return WiFi.begin(ssid.c_str());
	} else {
		return WiFi.begin(ssid.c_str(), pass.c_str());
	}
}

bool WiFiAuto::getStateAutoConnect() {
	return _stateAutoConnect;
}

statusAutoConnect WiFiAuto::getStatusAutoConnect() {
	return _status;
}

void WiFiAuto::startAutoConnect() {
	_status = SCAN;
	delete _infoNetworks;
	_infoNetworks = NULL;
	_stateAutoConnect = true;
	WiFi.scanDelete();
}

void WiFiAuto::begin(String ssid, String pass) {
	if (WiFi.getMode() != WIFI_AP_STA)
		WiFi.mode(WIFI_AP_STA);
	WiFi.hostname(BASIC_STA_HOSTNAME);

	if((pass.length() < 8) ? (WiFi.begin(ssid.c_str()) == WL_CONNECT_FAILED) : (WiFi.begin(ssid.c_str(), pass.c_str()) == WL_CONNECT_FAILED)) {
		_status = CHECK_SSID;
		blnprint("[WIFI_AUTO] Connect failse !!!");
	} else {
		_status = WAIT;
		_timeOutConnect = millis();
		bprint(",Connecting ...");
	}
}

void WiFiAuto::autoConnect() {
	switch (_status) {
		case PAUSE : {
				if (millis() - _timeOutReConnect > 60000) {
					_status = SCAN;
					blnprint("[WIFI_AUTO] Pause --> Scan");
				}
				break;
			}
		case WAIT : {
				if (WiFi.status() == WL_CONNECTED) {
					for (uint8_t i = 0; i < 4; i++) {
						if (Ping.ping("www.google.com")) {
							_status = DONE;
							saveInfoWiFiSuccess();
							delete _infoNetworks;
							_infoNetworks = NULL;
							_stateAutoConnect = false;
							return;
						}
					}
				}
				if (millis() - _timeOutConnect > 10000) { // waiting 10s, change thread
					_status = CHECK_SSID;
					blnprint("[WIFI_AUTO] Connect failse !!!");
				}
				break;
			}
		case SCAN : {
				if (WiFi.scanComplete() == WIFI_SCAN_RUNNING) {
					break;
				}
				if (WiFi.scanComplete() == WIFI_SCAN_FAILED) {
					WiFi.scanNetworks(true); // scan đồng bộ
					break;
				}
				_totalNetworks = WiFi.scanComplete();
				_infoNetworks = new infoNetworks[_totalNetworks];
				for (uint8_t i = 0; i < _totalNetworks; i++) {	// gán info
					_infoNetworks[i].ssid = WiFi.SSID(i);
					_infoNetworks[i].rssi = WiFi.RSSI(i);
				}
				WiFi.scanDelete();
				for (uint8_t i = 0; i < _totalNetworks; i++) {	// sort info theo rssi, [0] max -> [_total...] min
					for (uint8_t j = 0; j < _totalNetworks; j++) {
						if (_infoNetworks[i].rssi > _infoNetworks[j].rssi) {
							infoNetworks buff = _infoNetworks[j];
							_infoNetworks[j] = _infoNetworks[i];
							_infoNetworks[i] = buff;
						}
					}
				}
				_status = CHECK_SSID_NOW;
				_remain = _totalNetworks;
				blnprint("[WIFI_AUTO] Scan --> Check ssid current");
				break;
			}
		case CHECK_SSID_NOW : {
				for (uint8_t i = 0; i < _totalNetworks; i++) {
					if (_infoNetworks[i].ssid == DB.exportData(DATA_SSIDNOW)) {
						blnprint("[WIFI_AUTO] Reconnect SSID : "); bprint(DB.exportData(DATA_SSIDNOW));
						begin(DB.exportData(DATA_SSIDNOW), DB.exportData(DATA_PASSNOW));
						return;
					}
				}
				_status = CHECK_SSID;
				blnprint("[WIFI_AUTO] Check ssid current --> Check ssid");
				break;
			}
		case CHECK_SSID : {
				for (uint8_t i = _totalNetworks - _remain; i < _totalNetworks; i++) {
					_remain--;
					blnprint("[WIFI_AUTO] Check SSID : " + _infoNetworks[i].ssid);
					bprint(", dBm : " + (String)_infoNetworks[i].rssi);
					for (uint8_t j = 1; j < 6; j++) {
						if (_infoNetworks[i].ssid == DB.exportData(j)) {
							blnprint("[WIFI_AUTO] Connect SSID : "); bprint(_infoNetworks[i].ssid);
							bprint(", PASS : "); bprint(DB.exportData(j + 5));
							begin(DB.exportData(j), DB.exportData(j + 5));
							return;
						}
					}
				}
				_status = PAUSE;
				_timeOutReConnect = millis();
				blnprint("[WIFI_AUTO] Check ssid --> Pause... , 60s");
				delete _infoNetworks;
				_infoNetworks = NULL;
				break;
			}
		case DONE : {
				break;
			}
	}
}
