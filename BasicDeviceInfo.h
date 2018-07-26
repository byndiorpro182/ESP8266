#ifndef BASICDEVICEINFO_H
#define BASICDEVICEINFO_H
#include <Arduino.h>
#include "memory/database.h"
#include "debug/log.h"

#define BASIC_AP_SSID 		"B182"
#define	BASIC_AP_PASS 		"12345678"
#define BASIC_STA_HOSTNAME	"Binary182"
#define BASIC_FIRST_RUN		"1"

#define BASIC_LOCAL_IP1	182
#define BASIC_LOCAL_IP2	182
#define BASIC_LOCAL_IP3	182
#define BASIC_LOCAL_IP4	182
#define	BASIC_GATEWAY1	182
#define	BASIC_GATEWAY2	182
#define	BASIC_GATEWAY3	182
#define	BASIC_GATEWAY4	182
#define	BASIC_SUBNET1	255
#define	BASIC_SUBNET2	255
#define	BASIC_SUBNET3	255
#define	BASIC_SUBNET4	0




#define DATA_SSID1 		1
#define DATA_SSID2 		2
#define DATA_SSID3 		3
#define DATA_SSID4 		4
#define DATA_SSID5 		5
#define DATA_PASS1 		6
#define DATA_PASS2 		7
#define DATA_PASS3 		8
#define DATA_PASS4 		9
#define DATA_PASS5 		10
#define DATA_AUTH		11
#define DATA_APSSID		12
#define DATA_APPASS		13
#define DATA_SSIDNOW	14
#define DATA_PASSNOW	15
#define DATA_LOCALIP	16
#define DATA_GETWAY		17
#define DATA_SUBNET		18
#define DATA_UPDATE     19
#define DATA_FIRST_RUN	25

#define DATA_LOCAL_IP1	641
#define DATA_LOCAL_IP2	642
#define DATA_LOCAL_IP3	643
#define DATA_LOCAL_IP4	644
#define DATA_GETWAY1	681
#define DATA_GETWAY2	682
#define DATA_GETWAY3	683
#define DATA_GETWAY4	684
#define DATA_SUBNET1	721
#define DATA_SUBNET2	722
#define DATA_SUBNET3	723
#define DATA_SUBNET4	724

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
    UPDATE_STATUS           = 46,

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

    GET_PING_HOST               = 150,

    NOTIFY_PING             = 200,

};


void runFirstTurn(){
	blnprint("[runFirstTurn] ...");
	DB.importData(DATA_APSSID,BASIC_AP_SSID);
	DB.importData(DATA_APPASS,BASIC_AP_PASS);
	
	DB.importDataToAddress(DATA_LOCAL_IP1,BASIC_LOCAL_IP1);
	DB.importDataToAddress(DATA_LOCAL_IP2,BASIC_LOCAL_IP2);
	DB.importDataToAddress(DATA_LOCAL_IP3,BASIC_LOCAL_IP3);
	DB.importDataToAddress(DATA_LOCAL_IP4,BASIC_LOCAL_IP4);

	DB.importDataToAddress(DATA_GETWAY1,BASIC_GATEWAY1);
	DB.importDataToAddress(DATA_GETWAY2,BASIC_GATEWAY2);
	DB.importDataToAddress(DATA_GETWAY3,BASIC_GATEWAY3);
	DB.importDataToAddress(DATA_GETWAY4,BASIC_GATEWAY4);

	DB.importDataToAddress(DATA_SUBNET1,BASIC_SUBNET1);
	DB.importDataToAddress(DATA_SUBNET2,BASIC_SUBNET2);
	DB.importDataToAddress(DATA_SUBNET3,BASIC_SUBNET3);
	DB.importDataToAddress(DATA_SUBNET4,BASIC_SUBNET4);

	DB.importData(DATA_FIRST_RUN, BASIC_FIRST_RUN);
}


#endif

