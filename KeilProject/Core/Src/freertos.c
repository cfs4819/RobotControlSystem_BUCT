/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "servo.h"
#include "stdio.h"
#include "OLED12864.h"
#include "ESP8266.h"
#include "oledicons.h"
#include "KMP.h"
#include "string.h"
#include "community.h"
#include "cJSON.h"
#include "w25qxx.h"
//#include "I2C_SIM.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define WIFIENABLE
#define ACTION_DEMO
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern UART_String uart3str,uart4str,uart1str;
extern I2C_SIM OLED_i2c;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */


unsigned char init_finish_flag=0;
myservo Base_Servo,Servo1,Servo2,Servo3,Servo4,Grap_Servo;
char string111[UART_REVBUFFSIZE];
uint16_t strLength=0;
uint8_t dispalymode=0;
uint8_t offline_count;
extern uint8_t actGroupRW_temp[5][2];
uint8_t actGroupToRun[10][2];
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityLow,
  .stack_size = 128 * 4
};
/* Definitions for Init_all_task */
osThreadId_t Init_all_taskHandle;
const osThreadAttr_t Init_all_task_attributes = {
  .name = "Init_all_task",
  .priority = (osPriority_t) osPriorityHigh7,
  .stack_size = 128 * 4
};
/* Definitions for ToggleLEDTask */
osThreadId_t ToggleLEDTaskHandle;
const osThreadAttr_t ToggleLEDTask_attributes = {
  .name = "ToggleLEDTask",
  .priority = (osPriority_t) osPriorityAboveNormal7,
  .stack_size = 128 * 4
};
/* Definitions for task20Task */
osThreadId_t task20TaskHandle;
const osThreadAttr_t task20Task_attributes = {
  .name = "task20Task",
  .priority = (osPriority_t) osPriorityAboveNormal1,
  .stack_size = 1024 * 4
};
/* Definitions for servo0Task */
osThreadId_t servo0TaskHandle;
const osThreadAttr_t servo0Task_attributes = {
  .name = "servo0Task",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for servo1Task */
osThreadId_t servo1TaskHandle;
const osThreadAttr_t servo1Task_attributes = {
  .name = "servo1Task",
  .priority = (osPriority_t) osPriorityNormal1,
  .stack_size = 128 * 4
};
/* Definitions for servo2Task */
osThreadId_t servo2TaskHandle;
const osThreadAttr_t servo2Task_attributes = {
  .name = "servo2Task",
  .priority = (osPriority_t) osPriorityNormal2,
  .stack_size = 128 * 4
};
/* Definitions for servo3Task */
osThreadId_t servo3TaskHandle;
const osThreadAttr_t servo3Task_attributes = {
  .name = "servo3Task",
  .priority = (osPriority_t) osPriorityNormal3,
  .stack_size = 128 * 4
};
/* Definitions for servo4Task */
osThreadId_t servo4TaskHandle;
const osThreadAttr_t servo4Task_attributes = {
  .name = "servo4Task",
  .priority = (osPriority_t) osPriorityNormal4,
  .stack_size = 128 * 4
};
/* Definitions for servo5Task */
osThreadId_t servo5TaskHandle;
const osThreadAttr_t servo5Task_attributes = {
  .name = "servo5Task",
  .priority = (osPriority_t) osPriorityNormal5,
  .stack_size = 128 * 4
};
/* Definitions for checkserverTask */
osThreadId_t checkserverTaskHandle;
const osThreadAttr_t checkserverTask_attributes = {
  .name = "checkserverTask",
  .priority = (osPriority_t) osPriorityNormal6,
  .stack_size = 128 * 4
};
/* Definitions for actGroupTask */
osThreadId_t actGroupTaskHandle;
const osThreadAttr_t actGroupTask_attributes = {
  .name = "actGroupTask",
  .priority = (osPriority_t) osPriorityBelowNormal7,
  .stack_size = 128 * 4
};
/* Definitions for myMutex_ServoInput */
osMutexId_t myMutex_ServoInputHandle;
const osMutexAttr_t myMutex_ServoInput_attributes = {
  .name = "myMutex_ServoInput"
};
/* Definitions for serialSemaphore1 */
osSemaphoreId_t serialSemaphore1Handle;
const osSemaphoreAttr_t serialSemaphore1_attributes = {
  .name = "serialSemaphore1"
};
/* Definitions for newSignal */
osEventFlagsId_t newSignalHandle;
const osEventFlagsAttr_t newSignal_attributes = {
  .name = "newSignal"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void ToggleLED1(void *argument);
void task20(void *argument);
void setServo0(void *argument);
void setServo1(void *argument);
void setServo2(void *argument);
void setServo3(void *argument);
void setServo4(void *argument);
void setServo5(void *argument);
void actGroupCtl(void *argument);
void checkserver(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationTickHook(void);

/* USER CODE BEGIN 3 */
void vApplicationTickHook( void )
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
}
/* USER CODE END 3 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of myMutex_ServoInput */
  myMutex_ServoInputHandle = osMutexNew(&myMutex_ServoInput_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of serialSemaphore1 */
  serialSemaphore1Handle = osSemaphoreNew(1, 1, &serialSemaphore1_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of Init_all_task */
  Init_all_taskHandle = osThreadNew(StartTask02, NULL, &Init_all_task_attributes);

  /* creation of ToggleLEDTask */
  ToggleLEDTaskHandle = osThreadNew(ToggleLED1, NULL, &ToggleLEDTask_attributes);
  
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of newSignal */
  newSignalHandle = osEventFlagsNew(&newSignal_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	while(1)
	{
	    /* Initialize all configured peripherals */
		HAL_GPIO_WritePin(LED2_sys_GPIO_Port,LED2_sys_Pin,GPIO_PIN_RESET);
		osDelay(100);
		HAL_GPIO_WritePin(LED2_sys_GPIO_Port,LED2_sys_Pin,GPIO_PIN_SET);
		osDelay(100);
		HAL_GPIO_WritePin(LED2_sys_GPIO_Port,LED2_sys_Pin,GPIO_PIN_RESET);
		osDelay(100);
		HAL_GPIO_WritePin(LED2_sys_GPIO_Port,LED2_sys_Pin,GPIO_PIN_SET);
		osDelay(2700);
		
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the Init_all_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
	
  /* Infinite loop */
	while(1){
		/* USER CODE BEGIN 2 */
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
		
		cJSON_Hooks hooks;					//函数挂钩
		hooks.malloc_fn = pvPortMalloc;		//FreeRTOS内存申请
		hooks.free_fn = vPortFree;			//FreeRTOS内存释放
		cJSON_InitHooks(&hooks);			//保存设置

		/* USER CODE END 2 */
		
		/* USER TASK CREATION BEGIN */

		
		  /* creation of task20Task */
		task20TaskHandle = osThreadNew(task20, NULL, &task20Task_attributes);
		
		//printf("%d\r\n",(int)task20Handle);
		Base_Servo.angle_new=1500;
		Base_Servo.runtime=100;
		Servo1.angle_new=2500;
		Servo1.runtime=100;
		Servo2.angle_new=2500;
		Servo2.runtime=100;
		Servo3.angle_new=500;
		Servo3.runtime=100;
		Servo4.angle_new=1700;
		Servo4.runtime=100;
		Grap_Servo.angle_new=500;
		Grap_Servo.runtime=100;
		
		servo0TaskHandle = osThreadNew(setServo0, NULL, &servo0Task_attributes);
		servo1TaskHandle = osThreadNew(setServo1, NULL, &servo1Task_attributes);
		servo2TaskHandle = osThreadNew(setServo2, NULL, &servo2Task_attributes);
		servo3TaskHandle = osThreadNew(setServo3, NULL, &servo3Task_attributes);
		servo4TaskHandle = osThreadNew(setServo4, NULL, &servo4Task_attributes);
		servo5TaskHandle = osThreadNew(setServo5, NULL, &servo5Task_attributes);
		
		actGroupTaskHandle = osThreadNew(actGroupCtl,NULL,&actGroupTask_attributes);
		
		osThreadSuspend(task20TaskHandle);
		OLED_Fill(&OLED_i2c,0,0,127,63,1);
		OLED_Clear(&OLED_i2c);
		OLED_Showicons(0,0,(uint8_t *)gImage_BUCT,128,32,1);
		OLED_Showicons(16,40,(uint8_t *)gImage_jidian,96,24,1);
		OLED_Refresh_Gram(&OLED_i2c);
		osDelay(1000);
		osDelay(500);
		OLED_Clear(&OLED_i2c);
		OLED_ShowString(&OLED_i2c,0,0,(const uint8_t *)"Developed With: ",16);
		OLED_Showicons(0,32,(uint8_t *)gImage_onenet,128,32,1);
		OLED_Refresh_Gram(&OLED_i2c);
		osDelay(1000);
		osDelay(500);
		OLED_Clear(&OLED_i2c);
		
		W25qxx_Init();
		
		
		for(int i =0;i<128;i++){
			OLED_DrawPoint(i,11,1);
		}
		osThreadResume(task20TaskHandle);
		
#ifdef	ACTION_DEMO
		dispalymode=1;
		demo_group();
		dispalymode=0;
		OLED_Fill(&OLED_i2c,0,12,127,63,0);
#endif
		
#ifdef WIFIENABLE
		uint8_t *ptemp;
		ptemp=pvPortMalloc(10);
		W25qxx_ReadSector((uint8_t *)ptemp,WIFI_SECTOR,ENABLE_OFFSET,10);
		if(atoi((char *)ptemp)==1)
			Esp8266_Init();
		vPortFree(ptemp);
#endif
		for(int i =0;i<128;i++){
			OLED_DrawPoint(i,11,1);
		}
		
		osEventFlagsClear(newSignalHandle,0xFFFFFFFF);
		
		printf("current PID:\tpid_count %d\r\n",osThreadGetCount());
		printf("%d\t%s\t%d\r\n",(int)defaultTaskHandle,osThreadGetName(defaultTaskHandle),osThreadGetState(defaultTaskHandle));
		printf("%d\t%s\t%d\r\n",(int)ToggleLEDTaskHandle,osThreadGetName(ToggleLEDTaskHandle),osThreadGetState(ToggleLEDTaskHandle));
		printf("%d\t%s\t%d\r\n",(int)servo0TaskHandle,osThreadGetName(servo0TaskHandle),osThreadGetState(servo0TaskHandle));
		printf("%d\t%s\t%d\r\n",(int)servo1TaskHandle,osThreadGetName(servo1TaskHandle),osThreadGetState(servo1TaskHandle));
		printf("%d\t%s\t%d\r\n",(int)servo2TaskHandle,osThreadGetName(servo2TaskHandle),osThreadGetState(servo2TaskHandle));
		printf("%d\t%s\t%d\r\n",(int)servo3TaskHandle,osThreadGetName(servo3TaskHandle),osThreadGetState(servo3TaskHandle));
		printf("%d\t%s\t%d\r\n",(int)servo4TaskHandle,osThreadGetName(servo4TaskHandle),osThreadGetState(servo4TaskHandle));
		printf("%d\t%s\t%d\r\n",(int)servo5TaskHandle,osThreadGetName(servo5TaskHandle),osThreadGetState(servo5TaskHandle));
		printf("%d\t%s\t%d\r\n",(int)task20TaskHandle,osThreadGetName(task20TaskHandle),osThreadGetState(task20TaskHandle));	
		printf("the min free stack size is %d \r\n",(int32_t)uxTaskGetStackHighWaterMark(NULL));
		//printf("%d",osEventFlagsGet(newSignalHandle));
		init_finish_flag=1;
		OLED_Fill(&OLED_i2c,0,12,127,63,0);
		osDelay(10);
		
		/* USER TASK CREATION END */
		//osThreadDetach(Init_all_taskHandle);
		
		dispalymode=1;
		vTaskDelete(Init_all_taskHandle);
//		osThreadExit();
//		osThreadSuspend(Init_all_taskHandle);
		//osDelay(500);
	}
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_ToggleLED1 */
/**
* @brief Function implementing the ToggleLED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ToggleLED1 */
void ToggleLED1(void *argument)
{
  /* USER CODE BEGIN ToggleLED1 */
  /* Infinite loop */
	while(1)
	{
		char strtemp[10];
		memset(strtemp,0,10);
		if(dispalymode==1){
//			OLED_Fill(&OLED_i2c,0,12,127,63,0);
			memset(strtemp,0,10);
			sprintf(strtemp,"%d    ",Base_Servo.angle_old);
			OLED_ShowString(&OLED_i2c,0,12,(const uint8_t *)strtemp,12);
			memset(strtemp,0,10);
			sprintf(strtemp,"%d   ",Servo1.angle_old);
			OLED_ShowString(&OLED_i2c,43,12,(const uint8_t *)strtemp,12);
			memset(strtemp,0,10);
			sprintf(strtemp,"%d   ",Servo2.angle_old);
			OLED_ShowString(&OLED_i2c,86,12,(const uint8_t *)strtemp,12);
			memset(strtemp,0,10);
			sprintf(strtemp,"%d   ",Servo3.angle_old);
			OLED_ShowString(&OLED_i2c,0,25,(const uint8_t *)strtemp,12);
			memset(strtemp,0,10);
			sprintf(strtemp,"%d   ",Servo4.angle_old);
			OLED_ShowString(&OLED_i2c,43,25,(const uint8_t *)strtemp,12);
			memset(strtemp,0,10);
			sprintf(strtemp,"%d   ",Grap_Servo.angle_old);
			OLED_ShowString(&OLED_i2c,86,25,(const uint8_t *)strtemp,12);
		}
		osDelay(200);
		OLED_Refresh_Gram(&OLED_i2c);
	}
  /* USER CODE END ToggleLED1 */
}

/* USER CODE BEGIN Header_task20 */
/**
* @brief Function implementing the task20Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_task20 */
void task20(void *argument)
{
  /* USER CODE BEGIN task20 */
  /* Infinite loop */
	while(1)
	{
		osSemaphoreAcquire(serialSemaphore1Handle,osWaitForever);
		osDelay(20);
		printf("new msg,length=%d\r\n",uart3str.Uart_RecvLens);
		strLength = Get_Uart_Data(USART3,(uint8_t *)&string111,uart3str.Uart_RecvLens);
		if(strLength > 0){
			if(0<=KMP((uint8_t *)string111,strLength,(uint8_t *)"Hey",3)){
				offline_count--;
				HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
				continue;
			}	
			printf("USART3 Received:%s",string111);
			if(init_finish_flag==0){
				OLED_Fill(&OLED_i2c,0,12,127,63,0);
				OLED_ShowString(&OLED_i2c,0,12,(const uint8_t *)string111,12);
			}
			else{
				Get_Info();
			}
			
			//Uart3_DMA_Sent((uint8_t *)string111,strLength);
		}
		else{
			memset(string111,0,UART_REVBUFFSIZE);
			strLength = Get_Uart_Data(USART1,(uint8_t *)&string111,uart1str.Uart_RecvLens);
			printf("USART1 Received:%s",string111);
			Get_Info();
		}
//		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
		
	}	
  /* USER CODE END task20 */
}

/* USER CODE BEGIN Header_setServo0 */
/**
* @brief Function implementing the servo0Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_setServo0 */
void setServo0(void *argument)
{
  /* USER CODE BEGIN setServo0 */
	int i=0;
  /* Infinite loop */
	while(1)
	{
		i=0;
		printf("servo0 alive\r\n");
		while((i < (Base_Servo.runtime/50))&&(Base_Servo.angle_old!=Base_Servo.angle_new)){
			TIM3 -> CCR1 = Base_Servo.angle_old+((Base_Servo.angle_new-Base_Servo.angle_old)/
												(Base_Servo.runtime/50))*i;
			i++;
			//printf("running1\r\n");
			osDelay(50);
		}
		TIM3 -> CCR1 = Base_Servo.angle_new;
		printf("OK\r\n");
		Base_Servo.angle_old = Base_Servo.angle_new;
		osEventFlagsSet(newSignalHandle,0x1000);		//1<<12
		osDelay(50);
		osEventFlagsWait(newSignalHandle,0x01,osFlagsWaitAll,osWaitForever);
	}
  /* USER CODE END setServo0 */
}

/* USER CODE BEGIN Header_setServo1 */
/**
* @brief Function implementing the servo1Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_setServo1 */
void setServo1(void *argument)
{
  /* USER CODE BEGIN setServo1 */
	unsigned int i;
  /* Infinite loop */
	while(1)
	{
		i=0;
		printf("servo1 alive\r\n");
		while((i < (Servo1.runtime/50))&&(Servo1.angle_old!=Servo1.angle_new)){
			TIM3 -> CCR2 = Servo1.angle_old+((Servo1.angle_new-Servo1.angle_old)/
												(Servo1.runtime/50))*i;
			i++;
			osDelay(50);
		}
		TIM3 -> CCR2 = Servo1.angle_new;
		Servo1.angle_old = Servo1.angle_new;
		osEventFlagsSet(newSignalHandle,0x2000);	//1<<13
		osDelay(50);
		osEventFlagsWait(newSignalHandle,0x02,osFlagsWaitAll,osWaitForever);
	}	
  /* USER CODE END setServo1 */
}

/* USER CODE BEGIN Header_setServo2 */
/**
* @brief Function implementing the servo2Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_setServo2 */
void setServo2(void *argument)
{
  /* USER CODE BEGIN setServo2 */
	unsigned int i;
  /* Infinite loop */
	while(1)
	{
		i=0;
		printf("servo2 alive\r\n");
		while((i < (Servo2.runtime/50))&&(Servo2.angle_old!=Servo2.angle_new)){
			TIM3 -> CCR3 = Servo2.angle_old+((Servo2.angle_new-Servo2.angle_old)/
												(Servo2.runtime/50))*i;
			i++;
			osDelay(50);
		}
		TIM3 -> CCR3 = Servo2.angle_new;
		Servo2.angle_old = Servo2.angle_new;
		osEventFlagsSet(newSignalHandle,0x4000);	//1<<14
		osDelay(50);
		osEventFlagsWait(newSignalHandle,0x04,osFlagsWaitAll,osWaitForever);
	}
  /* USER CODE END setServo2 */
}

/* USER CODE BEGIN Header_setServo3 */
/**
* @brief Function implementing the servo3Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_setServo3 */
void setServo3(void *argument)
{
  /* USER CODE BEGIN setServo3 */
	unsigned int i;
  /* Infinite loop */
	while(1)
	{
		i=0;
		printf("servo3 alive\r\n");
		while((i < (Servo3.runtime/50))&&(Servo3.angle_old!=Servo3.angle_new)){
			TIM3 -> CCR4 = Servo3.angle_old+((Servo3.angle_new-Servo3.angle_old)/
												(Servo3.runtime/50))*i;
			i++;
			osDelay(50);
		}
		TIM3 -> CCR4 = Servo3.angle_new;
		Servo3.angle_old = Servo3.angle_new;
		osEventFlagsSet(newSignalHandle,0x8000);	//1<<15
		osDelay(50);
		osEventFlagsWait(newSignalHandle,0x08,osFlagsWaitAll,osWaitForever);
	}
  /* USER CODE END setServo3 */
}

/* USER CODE BEGIN Header_setServo4 */
/**
* @brief Function implementing the servo4Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_setServo4 */
void setServo4(void *argument)
{
  /* USER CODE BEGIN setServo4 */
	unsigned int i;
  /* Infinite loop */
	while(1)
	{
		i=0;
		printf("servo4 alive\r\n");
		while((i < (Servo4.runtime/50))&&(Servo4.angle_old!=Servo4.angle_new)){
			TIM4 -> CCR3 = Servo4.angle_old+((Servo4.angle_new-Servo4.angle_old)/
												(Servo4.runtime/50))*i;
			i++;
			osDelay(50);
		}
		TIM4 -> CCR3 = Servo4.angle_new;
		Servo4.angle_old = Servo4.angle_new;
		osEventFlagsSet(newSignalHandle,0x10000);	//1<<16
		osDelay(50);
		osEventFlagsWait(newSignalHandle,0x10,osFlagsWaitAll,osWaitForever);
	}	
  /* USER CODE END setServo4 */
}

/* USER CODE BEGIN Header_setServo5 */
/**
* @brief Function implementing the servo5Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_setServo5 */
void setServo5(void *argument)
{
  /* USER CODE BEGIN setServo5 */
	unsigned int i;
  /* Infinite loop */
	while(1)
	{
		i=0;
		printf("servo5 alive\r\n");
		while((i < (Grap_Servo.runtime/50))&&(Grap_Servo.angle_old!=Grap_Servo.angle_new)){
			TIM4 -> CCR4 = Grap_Servo.angle_old+((Grap_Servo.angle_new-Grap_Servo.angle_old)/
												(Grap_Servo.runtime/50))*i;
			i++;
			osDelay(50);
		}
		TIM4 -> CCR4 = Grap_Servo.angle_new;
		Grap_Servo.angle_old = Grap_Servo.angle_new;
		osEventFlagsSet(newSignalHandle,0x20000);	//1<<17
		osDelay(50);
		osEventFlagsWait(newSignalHandle,0x20,osFlagsWaitAll,osWaitForever);
		
	}
  /* USER CODE END setServo5 */
}



/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void actGroupCtl(void *argument)
{
	char *pTemp;
	
	while(1){
		uint8_t count=0;
		pTemp=pvPortMalloc(20);
		memset(pTemp,0,20);
		osDelay(20);
		osEventFlagsWait(newSignalHandle,0x100000,osFlagsWaitAll,osWaitForever);//1<<20
		while(count<10){
			//执行
			memset(actGroupRW_temp,0,20);
			memset(pTemp,0,20);
			printf("ActGroup to Run is:");
			for (int i=0;i<10;i++){
				printf("G%d:%d",actGroupToRun[i][0],actGroupToRun[i][1]);
			}
			printf("\r\n");
			printf("Reading Group%d in Flash\r\n",actGroupToRun[count][0]);
			W25qxx_ReadSector((uint8_t *)pTemp,ACTGROUP_SECTOR,ACTGROUP_OFFSET+actGroupToRun[count][0]*ACTGROUP_SIZE,ACTGROUP_SIZE);
			//memcpy(actGroupRW_temp,pTemp,ACTGROUP_SIZE);	//读到的存入数组
//			printf("offset=%d\r\n",ACTGROUP_OFFSET+actGroupToRun[count][0]*ACTGROUP_SIZE);
//			printf("\"%s\"\r\n",pTemp);
//			printf("\"%s\"\r\n",(char *)actGroupRW_temp);
//			for(int i=0;i<20;i++){
//				printf("%d\t",pTemp[i]);
//			}
//			printf("\r\nRead G%d,S1:a=%d,t=%d\r\n",actGroupToRun[count][0],pTemp[0]+pTemp[1]*256,pTemp[2]+pTemp[3]*256);
			
			Base_Servo.angle_new 	= pTemp[0]+pTemp[1]*256;
			Base_Servo.runtime 		= pTemp[2]+pTemp[3]*256;
			Servo1.angle_new 		= pTemp[4]+pTemp[5]*256;
			Servo1.runtime 			= pTemp[6]+pTemp[7]*256;
			Servo2.angle_new	 	= pTemp[8]+pTemp[9]*256;
			Servo2.runtime 			= pTemp[10]+pTemp[11]*256;
			Servo3.angle_new 		= pTemp[12]+pTemp[13]*256;
			Servo3.runtime 			= pTemp[14]+pTemp[15]*256;
			Servo4.angle_new 		= pTemp[16]+pTemp[17]*256;
			Servo4.runtime 			= pTemp[18]+pTemp[19]*256;
			
//			Base_Servo.angle_new 	= actGroupRW_temp[0][0];
//			Base_Servo.runtime 		= actGroupRW_temp[0][1];
//			Servo1.angle_new 		= actGroupRW_temp[1][0];
//			Servo1.runtime 			= actGroupRW_temp[1][1];
//			Servo2.angle_new	 	= actGroupRW_temp[2][0];
//			Servo2.runtime 			= actGroupRW_temp[2][1];
//			Servo3.angle_new 		= actGroupRW_temp[3][0];
//			Servo3.runtime 			= actGroupRW_temp[3][1];
//			Servo4.angle_new 		= actGroupRW_temp[4][0];
//			Servo4.runtime 			= actGroupRW_temp[4][1];
			
			osEventFlagsSet(newSignalHandle,0x1F);									//释放所有舵机
			osEventFlagsWait(newSignalHandle,0x1F000,osFlagsWaitAll,osWaitForever);//等待所有舵机完成动作	1<<12~1<<16
			if(actGroupToRun[count][1]>=0xFE)
				break;
			osDelay(actGroupToRun[count][1]*1000);
			count++;
		}
		vPortFree(pTemp);
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
