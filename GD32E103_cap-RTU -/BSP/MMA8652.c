#include "MMA8652.h"
#include "bsp.h"

void Delay2p5us(void)
{
	volatile unsigned long i = 30;
	while(--i)
	{
		;
	}
}

signed int  xvalue=0;
signed int  yvalue=0;
signed int  zvalue=0;

float xg,yg,zg;//���ٶȷ���
float angle1,angle2,angle3;//�Ƕ�

uchar BUF[8]; //�������ݻ�����  

#define CTRL_REG1  0x2A
#define CTRL_REG2  0x2B
#define CTRL_REG3  0x2C
#define CTRL_REG4  0x2D
#define CTRL_REG5  0x2E
#define INT_SOURCE_REG 0x0c

void MMA8652_GPIO_Config(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);

//	//SCL==>PB13
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	//SDA==>PB14
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13|GPIO_PIN_14);
//    GPIO_BC(GPIOB) = GPIO_PIN_13|GPIO_PIN_14;
	
	
}

void MMA8652_Setting(void)
{
	// Go to the Standby Mode
	Single_Write_MMA8652(CTRL_REG1, 0x00 | (0x7 << 3));
	// Configure the INT pins for Open Drain and Active Low
    Single_Write_MMA8652(CTRL_REG3, 0x01);
	// Enable the Data Ready Interrupt and route it to INT2
	Single_Write_MMA8652(CTRL_REG4, 0x01);
	Single_Write_MMA8652(CTRL_REG5, 0X00);
	
	// Go back to Active Mode
	Single_Write_MMA8652(CTRL_REG1, 0x01 | (0x7 << 3));
}

/**************************************
��ʼ�ź�
**************************************/
void MMA8652_Start(void)
{
    SDA_1;        	//����������
    SCL_1;        	//����ʱ����
    Delay_Us(4); 	//��ʱ
    SDA_0;        	//�����½���
    Delay_Us(4); 	//��ʱ
    SCL_0;        	//����ʱ����
    Delay_Us(4);	//��ʱ
}

/**************************************
ֹͣ�ź�
**************************************/
void MMA8652_Stop(void)
{
    SDA_0;      	//����������
    SCL_1;      	//����ʱ����
    Delay_Us(4);	//��ʱ
    SDA_1;      	//����������
    Delay_Us(4);    //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void MMA8652_SendACK(char ack)
{
    gpio_bit_write(GPIOB, GPIO_PIN_14, (BitAction)(ack));//дӦ���ź�
    SCL_1;        	//����ʱ����
    Delay_Us(4);    //��ʱ
    SCL_0;        	//����ʱ����
    Delay_Us(4);    //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
char MMA8652_RecvACK(void)
{
  char ack=0;
  
  SCL_1; 
  Delay_Us(4);
  ack = (char)gpio_input_bit_get(GPIOB,GPIO_PIN_14);
  SCL_0; 
  Delay_Us(4);
  return ack;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void MMA8652_SendByte(char dat)
{
	char i,temp;

	for(i=0;i<8;i++)
	{
		temp = dat&0x80;
		if(temp == 0)
		{
			//GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)0);
			gpio_bit_reset(GPIOB,GPIO_PIN_14);
		}
		else
		{
			//GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)1);
			gpio_bit_set(GPIOB,GPIO_PIN_14);
		}
		SCL_1;
		Delay_Us(2);
		SCL_0;
		Delay_Us(2);
		dat <<= 1;
	}
	MMA8652_RecvACK();
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
char MMA8652_RecvByte(void)
{
	char i;
	char dat = 0;

	SDA_1; 
	for(i=0;i<8;i++)
	{
		dat <<= 1;
		SCL_1;   
		Delay_Us(2);
		dat |= gpio_input_bit_get(GPIOB,GPIO_PIN_14); 
		SCL_0; 
		Delay_Us(1); 
	}
	return dat;
}

//******���ֽ�д��*******************************************
void Single_Write_MMA8652(uchar REG_Address,uchar REG_data)
{
  MMA8652_Start(); //��ʼ�ź�
  MMA8652_SendByte(MMA845x_I2C_ADDRESS);//�����豸��ַ+д�ź�
  MMA8652_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ
  MMA8652_SendByte(REG_data);       //�ڲ��Ĵ�������
  MMA8652_Stop();                   //����ֹͣ�ź�
}

//********���ֽڶ�ȡ*****************************************
uchar Single_Read_MMA8652(uchar REG_Address)
{
  uchar REG_data;
  
  MMA8652_Start(); //��ʼ�ź�
  MMA8652_SendByte(MMA845x_I2C_ADDRESS);//�����豸��ַ+д�ź�
  MMA8652_SendByte(REG_Address); //���ʹ洢��Ԫ��ַ����0��ʼ	
  MMA8652_Start(); //��ʼ�ź�
  MMA8652_SendByte(MMA845x_I2C_ADDRESS+1);//�����豸��ַ+���ź�
  REG_data=MMA8652_RecvByte();     //�����Ĵ�������
  MMA8652_SendACK(1);
  MMA8652_Stop();  //ֹͣ�ź�
  return REG_data; 
}

//*********************************************************
//
//��������MMA8652�ڲ����ٶ����ݣ���ַ��Χ0x01~0x06
//
//*********************************************************
void Multiple_read_MMA8652(ACCEL_TypeDef *pAccel)
{
	char i;
	unsigned char BUF[6]; //�������ݻ�����  

	MMA8652_Start(); //��ʼ�ź�
	MMA8652_SendByte(MMA845x_I2C_ADDRESS);//�����豸��ַ+д�ź�
	MMA8652_SendByte(0x01);//���ʹ洢��Ԫ��ַ����0x01��ʼ	
	MMA8652_Start();//��ʼ�ź�
	MMA8652_SendByte(MMA845x_I2C_ADDRESS + 1);//�����豸��ַ+���ź�
	for(i=0; i<6; i++)//������ȡ6����ַ���ݣ��洢��BUF
	{
		BUF[i] = MMA8652_RecvByte(); //BUF[0]�洢0x32��ַ�е�����
		if(i == 5)
		{
		  MMA8652_SendACK(1); //���һ��������Ҫ��NOACK
		}
		else
		{
		  MMA8652_SendACK(0); //��ӦACK
		}
	}
	MMA8652_Stop();  //ֹͣ�ź�
	
	pAccel->X_Value = (BUF[0]<<8)|BUF[1];
	pAccel->Y_Value = (BUF[2]<<8)|BUF[3];
	pAccel->Z_Value = (BUF[4]<<8)|BUF[5];
	
	if(BUF[0]&0x80) pAccel->X_Value = ~pAccel->X_Value + 1;
	if(BUF[2]&0x80) pAccel->Y_Value = ~pAccel->Y_Value + 1;
	if(BUF[4]&0x80) pAccel->Z_Value = ~pAccel->Z_Value + 1;
	
	pAccel->X_Value >>= 4;
	pAccel->Y_Value >>= 4;
	pAccel->Z_Value >>= 4;
}

void getAngle(ACCEL_TypeDef *pAccel, ANGLE_TypeDef *pAngle)
{
	float total;
	float ftemp;
	BitAction bXNegetive = Bit_RESET;
	BitAction bZNegetive = Bit_RESET;

	if(pAccel->X_Value<0)
	{
		pAccel->X_Value = 4096 + pAccel->X_Value;
		pAccel->X_Value = -pAccel->X_Value;
		bXNegetive = Bit_SET;
	}
	if(pAccel->Y_Value<0)
	{
		pAccel->Y_Value = 4096 + pAccel->Y_Value;
		pAccel->Y_Value = -pAccel->Y_Value;   
	}
	if(pAccel->Z_Value<0)
	{
		pAccel->Z_Value = 4096 + pAccel->Z_Value;
		pAccel->Z_Value = -pAccel->Z_Value;    
		bZNegetive = Bit_SET;
	}
	
	total = pAccel->X_Value * pAccel->X_Value + 
		    pAccel->Y_Value * pAccel->Y_Value + 
		    pAccel->Z_Value * pAccel->Z_Value;
	
	total = sqrt(total);
	
	ftemp = pAccel->X_Value / total;
	pAngle->XtoG = acos(ftemp) * 180 / 3.1415926;
	
	ftemp = pAccel->Y_Value / total;
	pAngle->YtoG = acos(ftemp) * 180 / 3.1415926;
	
	ftemp = pAccel->Z_Value / total;
	pAngle->ZtoG = 180.00 - acos(ftemp) * 180 / 3.1415926;

	if(Bit_SET == bXNegetive)
	{
		pAngle->ZtoG = 0 - pAngle->ZtoG;
	}
	if(Bit_SET == bZNegetive)
	{
		pAngle->XtoG = 0.0 - pAngle->XtoG;
		pAngle->YtoG = 0.0 - pAngle->XtoG;
	}
}
