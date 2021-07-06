#include "servo.h"
#include "cmsis_os.h"
#include "w25qxx.h"
#include "string.h"
#include "stdio.h"

int actGroupRW_temp[5][2];
extern  uint8_t actGroupToRun[10][2];
extern myservo Base_Servo,Servo1,Servo2,Servo3,Servo4,Grap_Servo;
extern osEventFlagsId_t newSignalHandle;

void demo_group(void)
{
	char * pTemp;
	pTemp = pvPortMalloc(30); 
	memset(pTemp,0,30);
	W25qxx_ReadSector((uint8_t *)pTemp,ACTGROUPTORUN_SECTOR,ACTGROUPTORUN_OFFSET,30);
	if (*(pTemp+ACTGROUP_DEMO_ENABLE_OFFSET)==0){	//不用演示
		vPortFree(pTemp);
		return ;
	}
	else if (*(pTemp+ACTGROUP_DEMO_INFLASH_OFFSET)==1){	//在flash中
		memset(actGroupToRun,0,20);
		memcpy(actGroupToRun,pTemp,20);		
		osEventFlagsSet(newSignalHandle,0x100000);		//唤醒动作组控制进程
	}
	else if(*(pTemp+ACTGROUP_DEMO_INFLASH_OFFSET)==0){	//不在flash中
		///////爪子////////
		Grap_Servo.angle_new=1000;
		Grap_Servo.runtime=1000;
		osEventFlagsSet(newSignalHandle,0x20);
		osDelay(1000);
		osDelay(200);
		Grap_Servo.angle_new=500;
		Grap_Servo.runtime=1000;
		osEventFlagsSet(newSignalHandle,0x20);
		osDelay(1000);
		osDelay(1000);
		Grap_Servo.angle_new=1000;
		Grap_Servo.runtime=1000;
		osEventFlagsSet(newSignalHandle,0x20);
		osDelay(1000);
		osDelay(200);
		Grap_Servo.angle_new=500;
		Grap_Servo.runtime=1000;
		osEventFlagsSet(newSignalHandle,0x20);
		osDelay(1000);
		osDelay(1000);
		
		/////////旋转//////////
		Servo4.angle_new=500;
		Servo4.runtime=1000;
		osEventFlagsSet(newSignalHandle,0x10);
		osDelay(1300);
		Servo4.angle_new=2500;
		Servo4.runtime=2500;
		osEventFlagsSet(newSignalHandle,0x10);
		osDelay(3000);
		Servo4.angle_new=1700;
		Servo4.runtime=1700;
		osEventFlagsSet(newSignalHandle,0x10);
		osDelay(3000);
		/////////小臂//////////
		Servo3.angle_new=2500;
		Servo3.runtime=2000;
		osEventFlagsSet(newSignalHandle,0x08);
		osDelay(2300);
		Servo3.angle_new=500;
		Servo3.runtime=2000;
		osEventFlagsSet(newSignalHandle,0x08);
		osDelay(2300);
		Servo3.angle_new=2500;
		Servo3.runtime=2000;
		osEventFlagsSet(newSignalHandle,0x08);
		osDelay(2300);
		Servo3.angle_new=500;
		Servo3.runtime=2000;
		osEventFlagsSet(newSignalHandle,0x08);
		osDelay(2300);
		/////////总动//////////
		Servo3.angle_new=2500;
		Servo3.runtime=2000;
		Servo2.angle_new=500;
		Servo2.runtime=2000;
		Servo1.angle_new=500;
		Servo1.runtime=2000;
		osEventFlagsSet(newSignalHandle,0x02);
		osEventFlagsSet(newSignalHandle,0x04);
		osEventFlagsSet(newSignalHandle,0x08);
		osDelay(3000);
		Servo3.angle_new=500;
		Servo3.runtime=2000;
		Servo2.angle_new=2500;
		Servo2.runtime=2000;
		Servo1.angle_new=2500;
		Servo1.runtime=2000;
		osEventFlagsSet(newSignalHandle,0x02);
		osEventFlagsSet(newSignalHandle,0x04);
		osEventFlagsSet(newSignalHandle,0x08);
		osDelay(3000);
		///////基座////////
		Base_Servo.angle_new=500;
		Base_Servo.runtime=2000;
		osEventFlagsSet(newSignalHandle,0x01);
		osDelay(2000);
		osDelay(1000);
		Base_Servo.angle_new=2500;
		Base_Servo.runtime=4000;
		osEventFlagsSet(newSignalHandle,0x01);
		osDelay(4000);
		osDelay(1000);
		Base_Servo.angle_new=1500;
		Base_Servo.runtime=2000;
		osEventFlagsSet(newSignalHandle,0x01);
		///////////////////
	}
	vPortFree(pTemp);
}
