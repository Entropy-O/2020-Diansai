#include "lcd_init.h"
#include "delay.h"

void LCD_GPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��PORTA~E,PORTGʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOD,GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_15);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	GPIO_SetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_10);
}


/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	
	u8 i;
	LCD_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
  LCD_CS_Set();	
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//д����
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
}

void LCD_Init(void)
{
	LCD_GPIO_Init();//��ʼ��GPIO
	
	LCD_RES_Clr();//��λ
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);
	
	LCD_BLK_Set();//�򿪱���
  delay_ms(100);
	
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //Sleep out 
	delay_ms(120);              //Delay 120ms 
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0xCF);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xC1);
	LCD_WR_DATA8(0X30);
	LCD_WR_REG(0xED);
	LCD_WR_DATA8(0x64);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0X12);
	LCD_WR_DATA8(0X81);
	LCD_WR_REG(0xE8);
	LCD_WR_DATA8(0x85);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x79);
	LCD_WR_REG(0xCB);
	LCD_WR_DATA8(0x39);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x34);
	LCD_WR_DATA8(0x02);
	LCD_WR_REG(0xF7);
	LCD_WR_DATA8(0x20);
	LCD_WR_REG(0xEA);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC0); //Power control
	LCD_WR_DATA8(0x1D); //VRH[5:0]
	LCD_WR_REG(0xC1); //Power control
	LCD_WR_DATA8(0x12); //SAP[2:0];BT[3:0]
	LCD_WR_REG(0xC5); //VCM control
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x3F);
	LCD_WR_REG(0xC7); //VCM control
	LCD_WR_DATA8(0x92);
	LCD_WR_REG(0x3A); // Memory Access Control
	LCD_WR_DATA8(0x55);
	LCD_WR_REG(0x36); // Memory Access Control
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
	else LCD_WR_DATA8(0xA8);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x12);
	LCD_WR_REG(0xB6); // Display Function Control
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0xA2);

	LCD_WR_REG(0x44);
	LCD_WR_DATA8(0x02);

	LCD_WR_REG(0xF2); // 3Gamma Function Disable
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0x26); //Gamma curve selected
	LCD_WR_DATA8(0x01);
	LCD_WR_REG(0xE0); //Set Gamma
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x1C);
	LCD_WR_DATA8(0x1B);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x48);
	LCD_WR_DATA8(0xB8);
	LCD_WR_DATA8(0x34);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0XE1); //Set Gamma
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x24);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x47);
	LCD_WR_DATA8(0x4B);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x0F);
	LCD_WR_REG(0x29); //Display on
} 







