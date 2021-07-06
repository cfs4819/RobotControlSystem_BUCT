#include "I2C_SIM.h"


//改成SMBUS频率需要在 100khz以下 即10us以上
#define I2C_Delay() __ASM ("nop") 

void delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 16000000 * us);
    while (delay--)
	{
		;
	}
}


/*
* 函数名:I2C_StructInit
* 输入参数:待初始化的i2c结构体，clk_port,clk_pin,sda_port,sda_pin
* 输出参数:无
* 返回值:无
* 函数作用:初始化i2c结构体，定义引脚，独立运行
*/
void I2C_StructInit(I2C_SIM* i2c,
					GPIO_TypeDef* clk_GPIO_Port,uint16_t clk_Pin,
					GPIO_TypeDef* sda_GPIO_Port,uint16_t sda_Pin)
{
	i2c->CLK_GPIO_Port=clk_GPIO_Port;
	i2c->CLK_Pin=clk_Pin;
	i2c->SDA_GPIO_Port=sda_GPIO_Port;
	i2c->SDA_Pin=sda_Pin;

}

/*
* 函数名:static void I2C_SDA_OUT(void)
* 输入参数:
* 输出参数:无
* 返回值:无
* 函数作用:配置 SDA 引脚为输出
*/
static void I2C_SDA_OUT(I2C_SIM* i2c)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = i2c->SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(i2c->SDA_GPIO_Port, &GPIO_InitStruct);
}

/*
* 函数名:static void I2C_SDA_IN(void)
* 输入参数:
* 输出参数:无
* 返回值:无
* 函数作用:配置 SDA 引脚为输入
*/
static void I2C_SDA_IN(I2C_SIM* i2c)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = i2c->SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(i2c->SDA_GPIO_Port, &GPIO_InitStruct);
}

/*
* 函数名:void I2C_Init(void)
* 输入参数:
官网:www.100ask.net
淘宝:100ask.taobao.com
- 139 -
电话:0755-86200561
论坛:bbs.100ask.net100ASK_STM32MP157
M4 User Manual
* 输出参数:无
* 返回值:无
* 函数作用: SCL/SDA 都初始为高电平
*/
void I2C_Init(I2C_SIM* i2c)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*Configure GPIO pins : PBPin PBPin */
	GPIO_InitStruct.Pin = i2c->CLK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(i2c->CLK_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin=i2c->SDA_Pin;
	HAL_GPIO_Init(i2c->SDA_GPIO_Port, &GPIO_InitStruct);

	//SCL_H();
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_SET);
	//SDA_H();
	HAL_GPIO_WritePin(i2c->SDA_GPIO_Port,i2c->SDA_Pin,GPIO_PIN_SET);
}



/*///////////////////////
 * SMBus与I2C
 * 开始与停止信号相同
 * 无需改变
 *///////////////////////
/*
* 函数名:void I2C_Start(void)
* 输入参数:
* 输出参数:无
* 返回值:无
* 函数作用:I2C 开始信号
*/
void I2C_Start(I2C_SIM* i2c)
{
	I2C_SDA_OUT(i2c);
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_SET);//SCL_H();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->SDA_GPIO_Port,i2c->SDA_Pin,GPIO_PIN_SET);//SDA_H();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->SDA_GPIO_Port,i2c->SDA_Pin,GPIO_PIN_RESET);//SDA_L();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
	I2C_Delay();
}

/*
* 函数名:void I2C_Stop(void)
* 输入参数:
* 输出参数:无
* 返回值:无
* 函数作用:I2C 停止信号
*/
void I2C_Stop(I2C_SIM* i2c)
{
	I2C_SDA_OUT(i2c);
	HAL_GPIO_WritePin(i2c->SDA_GPIO_Port,i2c->SDA_Pin,GPIO_PIN_RESET);//SDA_L();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_SET);//SCL_H();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->SDA_GPIO_Port,i2c->SDA_Pin,GPIO_PIN_SET);//SDA_H();
	I2C_Delay();
}


/*
* 函数名:void I2C_ACK(void)
* 输入参数:
* 输出参数:无
* 返回值:无
* 函数作用:I2C 发出应答信号
*/
void I2C_ACK(I2C_SIM* i2c)
{
	I2C_SDA_OUT(i2c);
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->SDA_GPIO_Port,i2c->SDA_Pin,GPIO_PIN_RESET);//SDA_L();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_SET);//SCL_H();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
	I2C_Delay();
}
/*
* 函数名:void I2C_NACK(void)
* 输入参数:
* 输出参数:无
* 返回值:无
* 函数作用:I2C 发出非应答信号
*/
void I2C_NACK(I2C_SIM* i2c)
{
	I2C_SDA_OUT(i2c);
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->SDA_GPIO_Port,i2c->SDA_Pin,GPIO_PIN_SET);//SDA_H();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_SET);//SCL_H();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
	I2C_Delay();
}
/*
* 函数名:uint8_t I2C_GetACK(void)
* 输入参数:
* 输出参数:无
* 返回值:1-等待超时，0-响应成功
* 函数作用:I2C 等待从机的应答信号
*/
uint8_t I2C_GetACK(I2C_SIM* i2c)
{
	uint8_t time = 0;
	I2C_SDA_IN(i2c);
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
	I2C_Delay();
	HAL_GPIO_WritePin(i2c->SDA_GPIO_Port,i2c->SDA_Pin,GPIO_PIN_SET);//SDA_H();
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_SET);//SCL_H();
	I2C_Delay();
	while(GPIO_PIN_SET==HAL_GPIO_ReadPin(i2c->SDA_GPIO_Port,i2c->SDA_Pin))//SDA_INPUT()
	{
		time++;
		if(time>250)
		{
			HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
			return 1;
		}
	}
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
	return 0;
}
/*
M4 User Manual
* 函数名:void I2C_SendByte(uint8_t data)
* 输入参数:data->发送的数据
* 输出参数:无
* 返回值:无
* 函数作用:I2C 发送一个字节
*/
void I2C_SendByte(I2C_SIM* i2c,uint8_t data)
{
	uint8_t cnt = 0;
	I2C_SDA_OUT(i2c);
	for(cnt=0; cnt<8; cnt++)
	{
		HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
		I2C_Delay();
		if(data & 0x80)
			HAL_GPIO_WritePin(i2c->SDA_GPIO_Port,i2c->SDA_Pin,GPIO_PIN_SET);//SDA_H();
		else
			HAL_GPIO_WritePin(i2c->SDA_GPIO_Port,i2c->SDA_Pin,GPIO_PIN_RESET);//SDA_L();
		data = data<<1;
		HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_SET);//SCL_H();
		I2C_Delay();
	}
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
	I2C_GetACK(i2c);
}
/*
* 函数名:uint8_t I2C_ReadByte(uint8_t ack)
* 输入参数:ack->发送的应答标志,1 应答,0 非应答
* 输出参数:无
* 返回值:返回读到的字节
* 函数作用:I2C 读出一个字节
*/
uint8_t I2C_ReadByte(I2C_SIM* i2c,uint8_t ack)
{
	uint8_t cnt;
	uint8_t data = 0xFF;
	I2C_SDA_IN(i2c);
	HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
	I2C_Delay();
	for(cnt=0; cnt<8; cnt++)
	{
		HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_SET);//SCL_H();
		//SCL 高(读取数据)
		I2C_Delay();
		data <<= 1;
		if(HAL_GPIO_ReadPin(i2c->SDA_GPIO_Port,i2c->SDA_Pin))//SDA_INPUT()
			data |= 0x01;
		//SDA 高(数据有效)
		HAL_GPIO_WritePin(i2c->CLK_GPIO_Port,i2c->CLK_Pin,GPIO_PIN_RESET);//SCL_L();
		I2C_Delay();
	}
	//发送应答信号,为低代表应答,高代表非应答
	if(ack == NACK)
		I2C_NACK(i2c);
	else
		I2C_ACK(i2c);
	return data;
	//返回数据
}

