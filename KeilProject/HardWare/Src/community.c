#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "community.h"
#include "KMP.h"
#include "ESP8266.h"
#include "usart.h"
#include "servo.h"
#include "cJSON.h"
#include "w25qxx.h"

/***************信号解析***************
 * 信号格式1："*#10240#*\r\n{\"S1\":{\"a\":1500,\"t\":1000},\"S2\":{\"a\":1500,\"t\":1000},\"S3\":{\"a\":1500,\"t\":1000},\"S4\":{\"a\":1500,\"t\":1000},\"S5\":{\"a\":1500,\"t\":1000},\"S6\":{\"a\":1500,\"t\":1000}}"
 * 		即: "*#逐个控制模式#*\r\n{"舵机编号":{"a":角度大小,"t":运行时间}}"
 * 信号格式2："*#10241#*\r\n G0:S1:1500 t1000 ,S2:2500 t1000 ,S3:2500 t1000 ,S4:500 t1000 ,S5:1700 t1000 "
 * 		即: "*#动作组存入模式#*\r\n{"动作编号":"五个舵机的角度与时间"}"
 * 信号格式3："*#10242#*\r\n{\"Command\":3,\"G0\":1000,\"G1\":1000}"
 * 		即: "*#动作组模式#*\r\n*动作编号#运转时间"	//多个动作组则按顺序执行 
 * 													//	command:01B=仅保存，10B=仅执行，11B=保存后执行，
 * 													//			100B=使能演示，1000B=从Flash演示 。用十进制输入
 * 信号格式4："*#10248#*\r\n{\"SSID\":\"707\",\"PWD\":\"741852963.0\",\"Enable\":1}"
 * 		即: "*#WIFI模式#*\r\n{"SSID":账号,"PWD":密码}"
***************************************/

extern myservo Base_Servo,Servo1,Servo2,Servo3,Servo4,Grap_Servo;
extern osThreadId_t servo0TaskHandle,servo1TaskHandle,servo2TaskHandle,servo3TaskHandle,servo4TaskHandle,servo5TaskHandle;
extern osEventFlagsId_t newSignalHandle;
extern UART_String uart3str,uart4str;
extern char string111[UART_REVBUFFSIZE];
extern uint16_t strLength;
extern uint16_t actGroupRW_temp[5][2];
extern uint8_t actGroupToRun[10][2];
void Get_Info(void)
{
	char *string_p;
	char *ppp;
	char *ppp2;
	unsigned int signal_num=0;
	unsigned int numtemp,angletemp,timetemp;
	int actGroupRW_temp_int[5][2];
	
	cJSON * root = NULL;
	
	
	sscanf((char *)&string111,"*#%u#*\r\n",&signal_num);
	string_p=string111+11;
	if(signal_num==10240){
			root = cJSON_Parse(string_p); 
			if (root==NULL){
				printf("Error before: [%s]\r\n",cJSON_GetErrorPtr());
			}
			else{
				printf("JSON OK\r\n");
//				printf("***********************\r\n");
//				printJson(root);
//				printf("***********************\r\n");	
				for(int i=0; i<cJSON_GetArraySize(root); i++)   //遍历最外层json键值对
				{
					cJSON * item;
					item = cJSON_GetArrayItem(root, i);
					//printf("item string:%s\r\n",item->string);
					numtemp=item->string[1]-48;
					if(cJSON_Object == item->type){
						for(int j=0; j<cJSON_GetArraySize(item); j++){
							cJSON * item_index = cJSON_GetArrayItem(item, j); 
							if(strstr(item_index->string,"t")!=NULL){
								ppp=cJSON_Print(item_index);
								timetemp=atoi(ppp);
								cJSON_free(ppp);		//与cJSON_Print()需成对出现
							}
							else if(strstr(item_index->string,"a")!=NULL){
								ppp=cJSON_Print(item_index);
								angletemp=atoi(ppp);
								cJSON_free(ppp);		//与cJSON_Print()需成对出现
							}
	//						cJSON_Delete(item_index);
						}
					}
					//printf("numtemp=%d\r\n",numtemp);
					switch (numtemp){
						case 1:{
							Base_Servo.angle_new=angletemp;
							Base_Servo.runtime=timetemp;
							osEventFlagsGet(newSignalHandle);
							osEventFlagsSet(newSignalHandle,0x01);
							printf("Changed servo1:%d\t%d\r\n",Base_Servo.angle_new,Base_Servo.runtime);
							vTaskResume(servo0TaskHandle);
							//osThreadResume(servo0TaskHandle);
						}break;
						case 2:{
							Servo1.angle_new=angletemp;
							Servo1.runtime=timetemp;
							osEventFlagsSet(newSignalHandle,0x02);
							printf("Changed servo2:%d\t%d\r\n",Servo1.angle_new,Servo1.runtime);
							//osThreadResume(servo1TaskHandle);
						}break;
						case 3:{
							Servo2.angle_new=angletemp;
							Servo2.runtime=timetemp;
							osEventFlagsSet(newSignalHandle,0x04);
							printf("Changed servo3:%d\t%d\r\n",Servo2.angle_new,Servo2.runtime);
							//osThreadResume(servo2TaskHandle);
						}break;
						case 4:{
							Servo3.angle_new=angletemp;
							Servo3.runtime=timetemp;
							osEventFlagsSet(newSignalHandle,0x08);
							printf("Changed servo4:%d\t%d\r\n",Servo3.angle_new,Servo3.runtime);
							//osThreadResume(servo3TaskHandle);
						}break;
						case 5:{
							Servo4.angle_new=angletemp;
							Servo4.runtime=timetemp;
							osEventFlagsSet(newSignalHandle,0x10);
							printf("Changed servo5:%d\t%d\r\n",Servo4.angle_new,Servo4.runtime);
							//osThreadResume(servo4TaskHandle);
						}break;
						case 6:{
							Grap_Servo.angle_new=angletemp;
							Grap_Servo.runtime=timetemp;
							osEventFlagsSet(newSignalHandle,0x20);
							printf("Changed servo6:%d\t%d\r\n",Grap_Servo.angle_new,Grap_Servo.runtime);
							//osThreadResume(servo5TaskHandle);
						}break;
						
					}
					
	//				cJSON_Delete(item);
				}
			}
			if(root)
				cJSON_Delete(root);
		}
	else if(signal_num==10241){
		ppp2=pvPortMalloc(256);
		for (int i=0;i<5;i++){
			for(int j=0;j<2;j++)
			actGroupRW_temp[i][j]=0;
		}
		int actGroupNum=0;
		sscanf(string_p,"G%d:S1:%d t%d ,S2:%d t%d ,S3:%d t%d ,S4:%d t%d ,S5:%d t%d ",
							&actGroupNum,
							&actGroupRW_temp_int[0][0],&actGroupRW_temp_int[0][1],
							&actGroupRW_temp_int[1][0],&actGroupRW_temp_int[1][1],
							&actGroupRW_temp_int[2][0],&actGroupRW_temp_int[2][1],
							&actGroupRW_temp_int[3][0],&actGroupRW_temp_int[3][1],
							&actGroupRW_temp_int[4][0],&actGroupRW_temp_int[4][1]);
		for (int i=0;i<5;i++){
			for(int j=0;j<2;j++)
			actGroupRW_temp[i][j]=(uint16_t)actGroupRW_temp_int[i][j];
		}
		W25qxx_ReadSector((uint8_t *)ppp2,ACTGROUP_SECTOR,ACTGROUP_OFFSET,256);	//读出整片区域
		memset(ppp2+actGroupNum*ACTGROUP_SIZE,0,20);							//把待的缓存擦除
		memcpy(ppp2+actGroupNum*ACTGROUP_SIZE,actGroupRW_temp,20);				//写入缓存
		W25qxx_EraseSector(ACTGROUP_SECTOR);									//擦除扇区
		W25qxx_WriteSector((uint8_t *)ppp2,ACTGROUP_SECTOR,ACTGROUP_OFFSET,256);//整片写入	
		memset(ppp2,0,256);
		memset(actGroupRW_temp,0,20);
		W25qxx_ReadSector((uint8_t *)ppp2,ACTGROUP_SECTOR,ACTGROUP_OFFSET+actGroupNum*ACTGROUP_SIZE,20);
		printf("saveoffset=%d\r\n",ACTGROUP_OFFSET+actGroupNum*ACTGROUP_SIZE);
		memcpy(actGroupRW_temp,ppp2,20);
		printf("\r\nChanged G%d,S1:a=%d,t=%d\r\n",actGroupNum,actGroupRW_temp[0][0],actGroupRW_temp[0][1]);
//		printf("%s\r\n",ppp2);
		cJSON_free(ppp2);
	}
	else if(signal_num==10242){
		uint8_t commandCount=0;
		uint8_t groupCount;
		ppp2=pvPortMalloc(30);
		memset(ppp2,0,30);
		root = cJSON_Parse(string_p); 
		if (root==NULL){
			printf("Error before: [%s]\r\n",cJSON_GetErrorPtr());
		}
		else{
			memset(actGroupToRun,0,20);
			groupCount=0;
			printf("JSON OK\r\n");
			printf("Free HeapSize is %d\r\n",xPortGetFreeHeapSize());
			for(int j=0; j<cJSON_GetArraySize(root); j++){
				cJSON * item_index = cJSON_GetArrayItem(root, j);
				if(0<=KMP((uint8_t *)"Command",7,(uint8_t *)item_index->string,7)){
					ppp=cJSON_Print(item_index);
					printf("%s\r\n",ppp);
					commandCount=atoi(ppp);
					printf("I see Command=%d\r\n",commandCount);
					cJSON_free(ppp);		//与cJSON_Print()需成对出现
				}
				else if(item_index->string[0]==71){
					ppp=cJSON_Print(item_index);
					numtemp = item_index->string[1]-48;
					timetemp = atoi(cJSON_Print(item_index));
					actGroupToRun[groupCount][0]=numtemp;
					actGroupToRun[groupCount][1]=timetemp;
					groupCount++;
					cJSON_free(ppp);		//与cJSON_Print()需成对出现
				}
				
			}
			if((commandCount>>1)&0x01){	//运行
				printf("run auto actgroup\r\n");
				osEventFlagsSet(newSignalHandle,0x100000);
			}
			else{
				W25qxx_ReadSector((uint8_t *)ppp2,ACTGROUPTORUN_SECTOR,ACTGROUPTORUN_OFFSET,256);	//读出整片区域
				if((commandCount&0x01)){	//保存	
					printf("Save actgroup\r\n");
					memcpy(ppp2,actGroupToRun,20);
				}
				
				memset(ppp2+ACTGROUP_DEMO_ENABLE_OFFSET,0,10);
				if((commandCount>>2)&0x01){	//使能演示
					printf("Enable demo on boot\r\n");
					*(ppp2+ACTGROUP_DEMO_ENABLE_OFFSET)=1;
				}
				if((commandCount>>3)&0x01){	//从flash读取演示动作
					printf("Demo of boot load from Flash\r\n");
					*(ppp2+ACTGROUP_DEMO_INFLASH_OFFSET)=1;
				}
				
				W25qxx_EraseSector(ACTGROUPTORUN_SECTOR);
				W25qxx_WriteSector((uint8_t *)ppp2,ACTGROUPTORUN_SECTOR,ACTGROUPTORUN_OFFSET,30);
			}
			
			
		}
		if(root)
			cJSON_Delete(root);
		vPortFree(ppp2);
		
	}
	else if(signal_num==10248){
		ppp2=pvPortMalloc(20);
		root = cJSON_Parse(string_p); 
		if (root==NULL){
			printf("Error before: [%s]\r\n",cJSON_GetErrorPtr());
		}
		else{
			printf("JSON OK\r\n");
			W25qxx_EraseSector(WIFI_SECTOR);
			osDelay(50);
			for(int j=0; j<cJSON_GetArraySize(root); j++){
				cJSON * item_index = cJSON_GetArrayItem(root, j); 
				if(0<=KMP((uint8_t *)"SSID",4,(uint8_t *)item_index->string,4)){
					memset(ppp,0,20);
					ppp=cJSON_Print(item_index);
					//printf("SSID:%s\r\n",ppp);
					memset(ppp2,0,20);
					memcpy(ppp2,ppp+1,strlen(ppp)-2);
					W25qxx_WriteSector((uint8_t *)ppp2,WIFI_SECTOR,SSID_OFFSET,20);
					cJSON_free(ppp);		//与cJSON_Print()需成对出现
				}
				else if(0<=KMP((uint8_t *)"PWD",3,(uint8_t *)item_index->string,3)){
					memset(ppp,0,20);
					ppp=cJSON_Print(item_index);
					//printf("SSID:%s\r\n",ppp);
					memset(ppp2,0,20);
					memcpy(ppp2,ppp+1,strlen(ppp)-2);
					W25qxx_WriteSector((uint8_t *)ppp2,WIFI_SECTOR,PWD_OFFSET,20);
					cJSON_free(ppp);		//与cJSON_Print()需成对出现
				}
				else if(0<=KMP((uint8_t *)"Enable",6,(uint8_t *)item_index->string,6)){
					memset(ppp,0,20);
					ppp=cJSON_Print(item_index);
					angletemp=atoi(ppp);
					sprintf((char *)ppp2,"%d",angletemp);
					W25qxx_WriteSector((uint8_t *)ppp2,WIFI_SECTOR,ENABLE_OFFSET,10);
					cJSON_free(ppp);		//与cJSON_Print()需成对出现
				}
			}
			W25qxx_ReadSector((uint8_t *)ppp,WIFI_SECTOR,SSID_OFFSET,20);
			printf("WIFI in Flash Changed:SSID:%s \t",ppp);
			W25qxx_ReadSector((uint8_t *)ppp,WIFI_SECTOR,PWD_OFFSET,20);
			printf("PWD:%s \t",ppp);
			W25qxx_ReadSector((uint8_t *)ppp,WIFI_SECTOR,ENABLE_OFFSET,10);
			printf("Enable:%s\r\n",ppp);
		}
		if(root)
			cJSON_Delete(root);
		vPortFree(ppp2);
		
	}

}


