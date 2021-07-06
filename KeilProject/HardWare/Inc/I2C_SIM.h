#ifndef __I2C_SIM_H
#define __I2C_SIM_H

#include "stm32f1xx_hal.h"
#include "main.h"
//#define SCL_H() HAL_GPIO_WritePin(I2C_SIM_CLK_GPIO_Port,I2C_SIM_CLK_Pin,GPIO_PIN_SET)
//#define SCL_L() HAL_GPIO_WritePin(I2C_SIM_CLK_GPIO_Port,I2C_SIM_CLK_Pin,GPIO_PIN_RESET)
//#define SDA_H() HAL_GPIO_WritePin(I2C_SIM_SDA_GPIO_Port,I2C_SIM_SDA_Pin,GPIO_PIN_SET)
//#define SDA_L() HAL_GPIO_WritePin(I2C_SIM_SDA_GPIO_Port,I2C_SIM_SDA_Pin,GPIO_PIN_RESET)
//#define SDA_INPUT() HAL_GPIO_ReadPin(I2C_SIM_SDA_GPIO_Port,I2C_SIM_SDA_Pin)
#define ACK 1
#define NACK 0


void I2C_StructInit(I2C_SIM* i2c,
					GPIO_TypeDef* clk_GPIO_Port,uint16_t clk_Pin,
					GPIO_TypeDef* sda_GPIO_Port,uint16_t sda_Pin);

void I2C_Init(I2C_SIM* i2c);
void I2C_Start(I2C_SIM* i2c);
void I2C_Stop(I2C_SIM* i2c);
void I2C_ACK(I2C_SIM* i2c);
void I2C_NACK(I2C_SIM* i2c);
uint8_t I2C_GetACK(I2C_SIM* i2c);
void I2C_SendByte(I2C_SIM* i2c,uint8_t data);
uint8_t I2C_ReadByte(I2C_SIM* i2c,uint8_t ack);


#endif
