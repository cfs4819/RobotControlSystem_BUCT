#ifndef __OLED12864_H
#define __OLED12864_H

#include "I2C_SIM.h"
#include "stdint.h"


#define OLED_ADDR 0x3C
#define OLED_ADDR_W (OLED_ADDR<<1)+0
#define OLED_ADDR_R (OLED_ADDR<<1)+1

#define OLED_CMD 0x00 
#define OLED_SENDDATA 0x40
#define OLED_COLUMN_NUMBER 128
#define OLED_LINE_NUMBER 64
#define OLED_COLUMN_OFFSET 2
#define OLED_PAGE_NUMBER OLED_LINE_NUMBER/8




void OLED_Init(I2C_SIM* OLED_i2c);
void OLED_Clear(I2C_SIM* OLED_i2c);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Refresh_Gram(I2C_SIM* OLED_i2c);
void OLED_Fill(I2C_SIM* OLED_i2c,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(I2C_SIM* OLED_i2c,uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);
void OLED_Showicons(uint8_t x,uint8_t y,uint8_t* icon,uint8_t sizex,uint8_t sizey,uint8_t mode);
#endif
