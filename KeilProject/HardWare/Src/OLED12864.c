#include "OLED12864.h"
#include "I2C_SIM.h"
#include "string.h"
#include "oledfont.h"
#include "oledicons.h"
uint8_t OLED_Arg[7];
uint8_t OLED_Rev[10];
//OLED���Դ�
//��Ÿ�ʽ����.
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
       0xAE,//�ر���ʾ
       0xD5,//����ʱ�ӷ�Ƶ����,��Ƶ��
       0x80,  //[3:0],��Ƶ����;[7:4],��Ƶ��
       0xA8,//��������·��
       0X3F,//Ĭ��(1/64)
       0xD3,//������ʾƫ��
       0X00,//Ĭ��Ϊ0
       0x40,//������ʾ��ʼ�� [5:0],����.
       0x8D,//��ɱ�����
       0x14,//bit2������/�ر�
       0x20,//�����ڴ��ַģʽ
       0x02,//[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
       0xA1,//���ض�������,bit0:0,0->0;1,0->127;  A1
       0xC8,//����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·�� (C0 ��ת��ʾ) C8
       0xDA,//����COMӲ����������
       0x12,//[5:4]����  
       0x81,//�Աȶ�����
       0x66,//1~255;Ĭ��0X7F (��������,Խ��Խ��)
       0xD9,//����Ԥ�������
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
       0xDB,//����VCOMH ��ѹ����
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
       0xA4,//ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	   0xA6,//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ 
       0xAF,//������ʾ        
};


/*****************************
 * ��������uint8_t OLED_Send
 * ���ã�OLED����ָ���Լ�ָ�����
 * ������Command-ָ�Argument-������Arglen-��������
 * ���أ�1-�ɹ���0-����Ӧ
 *****************************/
uint8_t OLED_Send(I2C_SIM* OLED_i2c,uint8_t Command1,uint8_t *Argument,unsigned int Arglen)
{
	I2C_Start(OLED_i2c);
	I2C_SendByte(OLED_i2c,OLED_ADDR_W);//���͵�ַ
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
 * ��������uint8_t OLED_Res
 * ���ã�OLED��������
 * ������Command-ָ�Argument-������Arglen-��������
 * ���أ�0-����Ӧ��other-state�����ݴ���OLED_Rev������
 *****************************/
uint8_t OLED_Res(I2C_SIM* OLED_i2c,uint8_t Revlen)
{
	uint8_t state;
	I2C_Start(OLED_i2c);
	I2C_SendByte(OLED_i2c,OLED_ADDR_R);
	if(Revlen==0)
		state=I2C_ReadByte(OLED_i2c,NACK); //����NACK
	else{
		state=I2C_ReadByte(OLED_i2c,ACK);
		for(int i=0;i<Revlen;i++){
			if (i<Revlen-1)
				OLED_Rev[i]=I2C_ReadByte(OLED_i2c,ACK);
			else
				OLED_Rev[i]=I2C_ReadByte(OLED_i2c,NACK);//���һ������NACK
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
 * ��������OLED_Init
 * ���ã���ʼ���� ����OLED
 * ������
 * ���أ�
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
		OLED_send_cmd (OLED_i2c,0xb0+i);    //����ҳ��ַ��0~7��
		OLED_send_cmd (OLED_i2c,0x02);      //������ʾλ�á��е͵�ַ
		OLED_send_cmd (OLED_i2c,0x10);      //������ʾλ�á��иߵ�ַ   
		OLED_send_datas(OLED_i2c,OLED_GRAM[i],128); 
	}   
}

//����OLED��ʾ    
void OLED_Display_On(I2C_SIM* OLED_i2c)
{
	OLED_send_cmd(OLED_i2c,0X8D);  //SET DCDC����
	OLED_send_cmd(OLED_i2c,0x14);  //DCDC ON
	OLED_send_cmd(OLED_i2c,0xAF);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(I2C_SIM* OLED_i2c)
{
	OLED_send_cmd(OLED_i2c,0X8D);  //SET DCDC����
	OLED_send_cmd(OLED_i2c,0X10);  //DCDC OFF
	OLED_send_cmd(OLED_i2c,0XAE);  //DISPLAY OFF
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(I2C_SIM* OLED_i2c)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[i][n]=0X00;  
	OLED_Refresh_Gram(OLED_i2c);//������ʾ
}
//���� 
//x:0~127
//y:0~63
//sett:1 ��� 0,���				   
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t set)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(set)OLED_GRAM[pos][x]|=temp;
	else OLED_GRAM[pos][x]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(I2C_SIM* OLED_i2c,uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram(OLED_i2c);//������ʾ	

}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
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
//m^n����
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
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
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
//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED_ShowString(I2C_SIM* OLED_i2c,uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
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
	uint8_t csize=(sizey/8+((sizey%8)?1:0));		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
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
