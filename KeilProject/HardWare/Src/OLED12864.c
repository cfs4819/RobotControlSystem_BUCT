#include "OLED12864.h"
#include "I2C_SIM.h"
#include "string.h"
#include "oledfont.h"
#include "oledicons.h"
uint8_t OLED_Arg[7];
uint8_t OLED_Rev[10];
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
uint8_t OLED_GRAM[8][128];		

const unsigned char  OLED_init_cmd[25]=			//SH1106
{/*0xae,0X00,0X10,0x40,0X81,0XCF,0xff,0xa1,0xa4,
  0xA6,0xc8,0xa8,0x3F,0xd5,0x80,0xd3,0x00,0XDA,0X12,
  0x8d,0x14,0xdb,0x40,0X20,0X02,0xd9,0xf1,0xAF*/
       0xAE,//关闭显示
       0xD5,//设置时钟分频因子,震荡频率
       0x80,  //[3:0],分频因子;[7:4],震荡频率
       0xA8,//设置驱动路数
       0X3F,//默认(1/64)
       0xD3,//设置显示偏移
       0X00,//默认为0
       0x40,//设置显示开始行 [5:0],行数.
       0x8D,//电荷泵设置
       0x14,//bit2，开启/关闭
       0x20,//设置内存地址模式
       0x02,//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
       0xA1,//段重定义设置,bit0:0,0->0;1,0->127;  A1
       0xC8,//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 (C0 翻转显示) C8
       0xDA,//设置COM硬件引脚配置
       0x12,//[5:4]配置  
       0x81,//对比度设置
       0x66,//1~255;默认0X7F (亮度设置,越大越亮)
       0xD9,//设置预充电周期
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
       0xDB,//设置VCOMH 电压倍率
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
       0xA4,//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	   0xA6,//设置显示方式;bit0:1,反相显示;0,正常显示 
       0xAF,//开启显示        
};


/*****************************
 * 函数名：uint8_t OLED_Send
 * 作用：OLED发送指令以及指令参数
 * 参数：Command-指令，Argument-参数，Arglen-参数个数
 * 返回：1-成功，0-无响应
 *****************************/
uint8_t OLED_Send(I2C_SIM* OLED_i2c,uint8_t Command1,uint8_t *Argument,unsigned int Arglen)
{
	I2C_Start(OLED_i2c);
	I2C_SendByte(OLED_i2c,OLED_ADDR_W);//发送地址
	I2C_SendByte(OLED_i2c,Command1);
	for(unsigned int i=0;i<Arglen;i++)
	{
		I2C_SendByte(OLED_i2c,*Argument);
		Argument++;
	}

	I2C_Stop(OLED_i2c);
	return 1;
}

/*****************************
 * 函数名：uint8_t OLED_Res
 * 作用：OLED接受数据
 * 参数：Command-指令，Argument-参数，Arglen-参数个数
 * 返回：0-无响应，other-state，数据存在OLED_Rev数组中
 *****************************/
uint8_t OLED_Res(I2C_SIM* OLED_i2c,uint8_t Revlen)
{
	uint8_t state;
	I2C_Start(OLED_i2c);
	I2C_SendByte(OLED_i2c,OLED_ADDR_R);
	if(Revlen==0)
		state=I2C_ReadByte(OLED_i2c,NACK); //发送NACK
	else{
		state=I2C_ReadByte(OLED_i2c,ACK);
		for(int i=0;i<Revlen;i++){
			if (i<Revlen-1)
				OLED_Rev[i]=I2C_ReadByte(OLED_i2c,ACK);
			else
				OLED_Rev[i]=I2C_ReadByte(OLED_i2c,NACK);//最后一个发送NACK
		}
	}
	I2C_Stop(OLED_i2c);
	return state;
}

void OLED_send_cmd(I2C_SIM* OLED_i2c,unsigned char o_command)
{
	OLED_Arg[0]=o_command;
    OLED_Send(OLED_i2c,OLED_CMD,OLED_Arg,1);
}

void OLED_send_data(I2C_SIM* OLED_i2c,unsigned char o_data)
{ 
	OLED_Arg[0]=o_data;
    OLED_Send(OLED_i2c,OLED_SENDDATA,OLED_Arg,1);
}
 
void OLED_send_datas(I2C_SIM* OLED_i2c,uint8_t *Datas,unsigned int Datalen)
{ 
	unsigned char databack[Datalen];
	unsigned char *p;
	p=Datas+Datalen-1;
	for (int i=0;i<Datalen;i++)
		databack[i]=*p--;
    OLED_Send(OLED_i2c,OLED_SENDDATA,databack,Datalen);
}




/*****************************
 * 函数名：OLED_Init
 * 作用：初始化后 填满OLED
 * 参数：
 * 返回：
 *****************************/
void OLED_Init(I2C_SIM* OLED_i2c)
{
	unsigned char i;
    for(i=0;i<25;i++){
		OLED_send_cmd(OLED_i2c,OLED_init_cmd[i]);
	}
}

void OLED_Refresh_Gram(I2C_SIM* OLED_i2c)
{
	uint8_t i;	    
	for(i=0;i<8;i++)  
	{  
		OLED_send_cmd (OLED_i2c,0xb0+i);    //设置页地址（0~7）
		OLED_send_cmd (OLED_i2c,0x02);      //设置显示位置—列低地址
		OLED_send_cmd (OLED_i2c,0x10);      //设置显示位置—列高地址   
		OLED_send_datas(OLED_i2c,OLED_GRAM[i],128); 
	}   
}

//开启OLED显示    
void OLED_Display_On(I2C_SIM* OLED_i2c)
{
	OLED_send_cmd(OLED_i2c,0X8D);  //SET DCDC命令
	OLED_send_cmd(OLED_i2c,0x14);  //DCDC ON
	OLED_send_cmd(OLED_i2c,0xAF);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(I2C_SIM* OLED_i2c)
{
	OLED_send_cmd(OLED_i2c,0X8D);  //SET DCDC命令
	OLED_send_cmd(OLED_i2c,0X10);  //DCDC OFF
	OLED_send_cmd(OLED_i2c,0XAE);  //DISPLAY OFF
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(I2C_SIM* OLED_i2c)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[i][n]=0X00;  
	OLED_Refresh_Gram(OLED_i2c);//更新显示
}
//画点 
//x:0~127
//y:0~63
//sett:1 填充 0,清空				   
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t set)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(set)OLED_GRAM[pos][x]|=temp;
	else OLED_GRAM[pos][x]&=~temp;	    
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(I2C_SIM* OLED_i2c,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram(OLED_i2c);//更新显示	

}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)
				OLED_DrawPoint(x,y,mode);
			else 
				OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n函数
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}		

uint8_t byte_change(uint8_t data)
{
	uint8_t i = 0;
	uint8_t temp = 0;
	
	for (i = 0; i < 8; i ++)
	{
		temp = temp << 1;
		temp |= (data >> i) & 0x01;
	}
	
	return temp;
	
}
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void OLED_ShowString(I2C_SIM* OLED_i2c,uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear(OLED_i2c);}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	
}	

void OLED_Showicons(uint8_t x,uint8_t y,uint8_t* icon,uint8_t sizex,uint8_t sizey,uint8_t mode)
{      			    
	uint8_t temp,t,t1,t2;
	uint8_t y0=y,x0=x;	
	uint8_t csize=(sizey/8+((sizey%8)?1:0));		//得到字体一个字符对应点阵集所占的字节数	
    for(t=0;t<csize;t++)
    {   
		for (t1=0;t1<sizex;t1++){
			temp=icon[t1+t*sizex];							
			for(t2=0;t2<8;t2++)
			{
				if(temp&0x01)
					OLED_DrawPoint(x,y,mode);
				else 
					OLED_DrawPoint(x,y,!mode);
				temp>>=1;
				y++;
				if((y-y0)>=8)
				{
					y=y0;
					x++;
					break;
				}
			} 			
		}
		x=x0;
		y+=8;
		y0+=8;
    }          
}
