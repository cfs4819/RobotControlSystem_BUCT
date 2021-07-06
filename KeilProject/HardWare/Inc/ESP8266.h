#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "usart.h"

#define EXIT_DTU "+++"
#define AUTH_INFO "*399316#buct2017030367#GraduaProj*"	//OneNet鉴权信息
#define ONENET_ADDR "dtu.heclouds.com"						//OneNet地址
#define ONENET_PORT "1811"									//OneNet端口
//#define MYWIFI_SSID "OnePlus 7 Pro(4G)"							//路由器SSID名称
//#define MYWIFI_PWD "zzcc112244"								//路由器密码
#define MYWIFI_SSID "706"							//路由器SSID名称
#define MYWIFI_PWD "741852963.0"								//路由器密码
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
