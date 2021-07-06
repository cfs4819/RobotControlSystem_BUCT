#include "delay.h"


__IO uint16_t delay_end = 0; // 延时完成标志
/*
* 函数名:void us_timer_delay(uint32_t t)
* 输入参数:t-延时时间 us(范围:0~21,474,836us)
* 输出参数:无
* 返回值:无
* 函数作用:定时器实现的延时函数,延时时间为 t us,为了缩短时间,函数体使用寄存器操作,用户可对照手册查看每个寄存器每位的含义
*/
void us_timer_delay(uint32_t t)
{
	uint32_t counter = 0;
	uint32_t autoreload = 200*t-1;
	delay_end = 0;
	HAL_TIM_Base_Stop_IT(&htim5); // 先停止 TIM5 计数
	__HAL_TIM_SET_AUTORELOAD(&htim5, autoreload); // 设置计数自动加载值,每 200 个为 1us
	__HAL_TIM_SET_COUNTER(&htim5, counter); // 设置计数初始值为 0,启动定时器后 counter 将增加到 autoreload
	HAL_TIM_Base_Start_IT(&htim5); // 启动 TIM5 计数
	while(!delay_end);
}


/*
* 函数名:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
* 输入参数:htim->定时器句柄
* 输出参数:无
* 函数作用:每当发生一次周期溢出时间会到此中断回调函数,在此函数内停止计数并修改标志位
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM5)
	{
		HAL_TIM_Base_Stop_IT(&htim5); // 停止 TIM5 计数
		delay_end = 1;
	}
}
