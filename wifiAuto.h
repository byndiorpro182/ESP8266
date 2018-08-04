#ifndef WIFIAUTO_H
#define WIFIAUTO_H

#include "BasicDeviceInfo.h"
#include "debug/log.h"
#include "ProcessManager.h"
#include "memory/database.h"
#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <FS.h>
#include <BlynkSimpleEsp8266.h>

enum statusAutoConnect {
	PAUSE = 0, // dừng nếu chưa kết nối đc cái nào
	WAIT = 1,
	SCAN = 2,
	CHECK_SSID_NOW = 3,
	CHECK_SSID = 4,
	DONE = 5, // kết nối thành công với 1 mạng wifi
};


class WiFiAuto {
	public:
		WiFiAuto();
		~WiFiAuto();
		wl_status_t connect(String ssid, String pass);
		void startAutoConnect();
		void autoConnect(); // hàm đc gọi khi kết nối wifi bị mất
		bool getStateAutoConnect();
		statusAutoConnect getStatusAutoConnect();
		void saveInfoWiFiSuccess();
	private:
		struct infoNetworks
		{
			String ssid;
			int8_t rssi;
		};
		void begin(String ssid, String pass);

		bool _stateAutoConnect = false; // khởi tạo là bật auto connect luôn// phân biệt với connect vs connectAuto
		statusAutoConnect _status = SCAN;
		uint8_t _totalNetworks = 0;
		uint32_t _timeOutConnect = 0;
		uint32_t _timeOutReConnect = 0;

		infoNetworks *_infoNetworks = NULL;
		uint8_t _remain;

} wifiAuto;


bool onBlynk(void){
	if(WiFi.status() == WL_CONNECTED){
		for(uint8_t i=0; i < 5; i++)
			if(i=4)
				return 0;
			else
				if(Ping.ping("www.google.com"))
					break;
		if(DB.exportData(DATA_AUTH).length() == 32){
     		Blynk.config(DB.exportData(DATA_AUTH).c_str());
       		if(Blynk.connect())
       		   if(Blynk.connected()){
            Task.remove("blynk");
            Task.add([](){
              Blynk.run();
            }, "blynk");
            return 1;
          } 
		}
		return 0;
	}
}

/*
WiFiEventHandler onStationModeConnected = WiFi.onStationModeConnected([](const WiFiEventStationModeConnected& evt) {
	if (wifiAuto.getStateAutoConnect())
		return;
	wifiAuto.saveInfoWiFiSuccess();
});
*/

WiFiEventHandler onStationModeGotIP = WiFi.onStationModeGotIP([](const WiFiEventStationModeGotIP& evt) {
	//onBlynk();
	if (wifiAuto.getStateAutoConnect())
		return;
	wifiAuto.saveInfoWiFiSuccess();
});

WiFiEventHandler onStationModeDisconnected = WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected& evt) {
	blnprint("[WIFI_AUTO] Disconnected! SSID : " + evt.ssid + ", Reason : " + (String)evt.reason);
	//Task.remove("blynk");
	if (WiFi.getMode() != WIFI_AP_STA || wifiAuto.getStateAutoConnect())
		return;
	wifiAuto.startAutoConnect();
	Task.add([]() {
		wifiAuto.autoConnect();
		if (wifiAuto.getStatusAutoConnect() == DONE || !wifiAuto.getStateAutoConnect()) {
			Task.remove("autoConnect");
		}
	}, "autoConnect");

});

#include "wifiAuto.impl.h"


#endif
