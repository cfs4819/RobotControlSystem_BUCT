#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "usart.h"

#define EXIT_DTU "+++"
#define AUTH_INFO "*399316#buct2017030367#GraduaProj*"	//OneNet��Ȩ��Ϣ
#define ONENET_ADDR "dtu.heclouds.com"						//OneNet��ַ
#define ONENET_PORT "1811"									//OneNet�˿�
//#define MYWIFI_SSID "OnePlus 7 Pro(4G)"							//·����SSID����
//#define MYWIFI_PWD "zzcc112244"								//·��������
#define MYWIFI_SSID "706"							//·����SSID����
#define MYWIFI_PWD "741852963.0"								//·��������
#define ONLINE_SIGNAL "Hi"

#define IMFOR_IN_FLASH
#define WIFI_SECTOR 255
#define	SSID_OFFSET 0
#define PWD_OFFSET 20
#define ENABLE_OFFSET 40

void checkserver(void *argument);
void Esp8266_Init(void);
void Reconnect(void);
void changeSsidAndPwd(void);
#endif 
