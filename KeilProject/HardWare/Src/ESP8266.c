#include "ESP8266.h"
#include "KMP.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "OLED12864.h"
#include "stdio.h"
#include "oledicons.h"
#include "w25qxx.h"

extern UART_String uart3str;
extern char string111[UART_REVBUFFSIZE];
extern uint16_t strLength;
/* Definitions for checkserverTask */
extern osThreadId_t checkserverTaskHandle ,task20TaskHandle;;
extern const osThreadAttr_t checkserverTask_attributes ;
extern I2C_SIM OLED_i2c;
extern uint8_t offline_count;
extern unsigned char init_finish_flag;
/* USER CODE BEGIN Header_checkserver */
/**
* @brief Function implementing the checkserverTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_checkserver */
void checkserver(void *argument)
{
  /* USER CODE BEGIN checkserver */
  /* Infinite loop */
 	while(1){
		osDelay(15000);
//		printf("current PID:\tpid_count %d\r\n",osThreadGetCount());
//		printf("the min free stack size is %d \r\n",(int32_t)uxTaskGetStackHighWaterMark(NULL));
//		printf("the free Heap size is %d \r\n",(int32_t)xPortGetFreeHeapSize());
//		printf("%d\t%s\t%d\t%d\t%d\r\n",(int)task20TaskHandle,
//								osThreadGetName(task20TaskHandle),
//								osThreadGetState(task20TaskHandle),
//								osThreadGetStackSize(task20TaskHandle),
//								osThreadGetStackSpace(task20TaskHandle));
		UART3_Print("%s","Hi");
		offline_count++;
		if(offline_count>=3){
			OLED_Fill(&OLED_i2c,69,0,128,12,0);
			printf("Server Offline...\r\n");
			printf("Request timed out...\r\n");
			printf("Reconnecting...\r\n");
			Reconnect();
		}
	}
  /* USER CODE END checkserver */
}

void Reconnect(void)
{
	uint8_t timeout;
	timeout=0;
	init_finish_flag=0;
	UART3_Print("%s",EXIT_DTU);
	memset(string111,0,1024);
	while((0>KMP((uint8_t *)string111,strLength,(uint8_t *)"CLOSED",6))&&
		(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"+++",3)&&(timeout<100))) {
		osDelay(50);
		UART3_Print("%s",EXIT_DTU);
		timeout++;
	}
	if(timeout<10){
		UART3_Print("%s=\"%s\",\"%s\",%s\r\n","AT+CIPSTART","TCP",ONENET_ADDR,ONENET_PORT);
		memset(string111,0,1024);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"CONNECT",7)) osDelay(10);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"OK",2)) osDelay(10);
		UART3_Print("%s\r\n","AT+CIPMODE=1");
		memset(string111,0,1024);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"OK",2)) osDelay(10);
		UART3_Print("%s\r\n","AT+CIPSEND");
		memset(string111,0,1024);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)">",1)) osDelay(10);
		UART3_Print("%s\r\n","*399316#buct2017030367#GraduaProj*");
		checkserverTaskHandle	 = osThreadNew(checkserver,NULL,&checkserverTask_attributes);
		printf("%d\t%s\t%d\r\n",(int)checkserverTaskHandle,osThreadGetName(checkserverTaskHandle),osThreadGetState(checkserverTaskHandle));
		OLED_Showicons(75,0,(uint8_t *)gImage_wifi2,16,9,1);
		OLED_ShowString(&OLED_i2c,91,0,(const uint8_t *)"Online",12);
		OLED_ShowString(&OLED_i2c,0,11,(const uint8_t *)"WiFi Connected",12);
		OLED_Refresh_Gram(&OLED_i2c);
		/*********初始化WiFi模块********/
	}
	else{
		printf("WiFi Is Not Detected!");
		OLED_ShowString(&OLED_i2c,0,11,(const uint8_t *)"WiFi Not Detected!",12);
	}
	init_finish_flag=1;
}

void Esp8266_Init(void)
{
	uint8_t timeout;
	timeout=0;
	init_finish_flag=0;
	UART3_Print("%s",EXIT_DTU);
	memset(string111,0,UART_REVBUFFSIZE);
	//printf("0\r\n");
	while((0>KMP((uint8_t *)string111,strLength,(uint8_t *)"CLOSED",6))&&
		(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"+++",3)&&(timeout<100))) {
		osDelay(50);
		UART3_Print("%s",EXIT_DTU);
		if(strcmp(string111,"+++")!=NULL)
			break;
		timeout++;
	}
		//printf("1\r\n");
	if(timeout<100){
		/*********初始化WiFi模块********/
		//Uart3_DMA_Sent((uint8_t *)"Usart3 is on\r\n",14);
		UART3_Print("%s",EXIT_DTU);
		memset(string111,0,UART_REVBUFFSIZE);
		while((0>KMP((uint8_t *)string111,strLength,(uint8_t *)"CLOSED",6))
				&&(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"+++",3))) {
			osDelay(50);
			UART3_Print("%s",EXIT_DTU);
			if(strcmp(string111,"+++")!=NULL)
				break;
		}
		//printf("2\r\n");
		UART3_Print("%s\r\n","AT+CWMODE=1");
		memset(string111,0,UART_REVBUFFSIZE);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"OK",2)) {
			osDelay(100);
			UART3_Print("%s\r\n","AT+CWMODE=1");
		}
		//printf("3\r\n");
		UART3_Print("%s\r\n","AT+RST");
		osDelay(500);
		memset(string111,0,UART_REVBUFFSIZE);
//			while((0>KMP((uint8_t *)string111,strLength,(uint8_t *)"ready",5))
//					&&(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"WIFI GOT IP",11))) osDelay(10);
		while((0>KMP((uint8_t *)string111,strLength,(uint8_t *)"ready",5))&&(timeout<60)) {
			osDelay(50);
			timeout++;
		}
		memset(string111,0,UART_REVBUFFSIZE);
		timeout=0;
		while((0>KMP((uint8_t *)string111,strLength,(uint8_t *)"WIFI GOT IP",11))&&(timeout<50)) {
			osDelay(100);
			timeout++;
		}
		printf("3\r\n");
		UART3_Print("%s",EXIT_DTU);
		memset(string111,0,UART_REVBUFFSIZE);
		while((0>KMP((uint8_t *)string111,strLength,(uint8_t *)"CLOSED",6))
				&&(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"+++",3))) {
			osDelay(10);
			if(strcmp(string111,"+++")!=NULL)
				break;
		}
#ifdef IMFOR_IN_FLASH
		uint8_t *ssidAndPwd;
		ssidAndPwd=(uint8_t *)pvPortMalloc(40);
		W25qxx_ReadSector((uint8_t *)ssidAndPwd,WIFI_SECTOR,0,40);
		printf("flash read:%s\r\n",ssidAndPwd);
		vPortFree(ssidAndPwd);
		UART3_Print("%s=\"%s\",\"%s\"\r\n","AT+CWJAP",ssidAndPwd,ssidAndPwd+20);
#else			
		UART3_Print("%s=\"%s\",\"%s\"\r\n","AT+CWJAP",MYWIFI_SSID,MYWIFI_PWD);
#endif
		memset(string111,0,UART_REVBUFFSIZE);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"WIFI DISCONNECT",15)) osDelay(10);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"WIFI CONNECTED",14)) osDelay(10);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"WIFI GOT IP",11)) osDelay(10);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"OK",2))osDelay(10);
		UART3_Print("%s=\"%s\",\"%s\",%s\r\n","AT+CIPSTART","TCP",ONENET_ADDR,ONENET_PORT);
		memset(string111,0,UART_REVBUFFSIZE);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"CONNECT",7)) osDelay(10);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"OK",2)) osDelay(10);
		UART3_Print("%s\r\n","AT+CIPMODE=1");
		memset(string111,0,UART_REVBUFFSIZE);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)"OK",2)) osDelay(10);
		UART3_Print("%s\r\n","AT+CIPSEND");
		memset(string111,0,UART_REVBUFFSIZE);
		while(0>KMP((uint8_t *)string111,strLength,(uint8_t *)">",1)) osDelay(10);
		UART3_Print("%s\r\n","*399316#buct2017030367#GraduaProj*");
		checkserverTaskHandle	 = osThreadNew(checkserver,NULL,&checkserverTask_attributes);
		printf("%d\t%s\t%d\r\n",(int)checkserverTaskHandle,osThreadGetName(checkserverTaskHandle),osThreadGetState(checkserverTaskHandle));
		OLED_Showicons(75,0,(uint8_t *)gImage_wifi2,16,9,1);
		OLED_ShowString(&OLED_i2c,91,0,(const uint8_t *)"Online",12);
		OLED_ShowString(&OLED_i2c,0,11,(const uint8_t *)"WiFi Connected",12);
		OLED_Refresh_Gram(&OLED_i2c);
		/*********初始化WiFi模块********/
	}
	else{
		printf("WiFi Is Not Detected!");
		OLED_ShowString(&OLED_i2c,0,11,(const uint8_t *)"WiFi Not Detected!",12);
	}
	init_finish_flag=1;
}
