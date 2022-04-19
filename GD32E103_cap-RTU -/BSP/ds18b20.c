#include "ds18b20.h"



void delay_us(uint32_t tim)
{
	tim = tim*1;
	while(tim--);	
}


void delay_2us(void)
{
	uint32_t tim = 7;
	while(tim--);	
}


/**
 * ����         : delay_750us(void)
 * ��������     : 
 * ����         : 
 * ����         : 750΢����ʱ
 * �������     : ��
 * �������     : ��
 * ���ؽ��     : ��
 * ע���˵��   : ��
 * �޸�����     : �� 
 */
void delay_750us(void)
{
	uint32_t tim=4000;
	while(tim--);	
}

/**
 * ����         : delay_15us(void)
 * ��������     : 
 * ����         : 
 * ����         : 15΢����ʱ
 * �������     : ��
 * �������     : ��
 * ���ؽ��     : ��
 * ע���˵��   : ��
 * �޸�����     : �� 
 */
void delay_15us(void)
{
	uint32_t tim=78;
	while(tim--);	
}

/**
 * ����         : delay_12us(void)
 * ��������     : 
 * ����         : 
 * ����         : 12΢����ʱ
 * �������     : ��
 * �������     : ��
 * ���ؽ��     : ��
 * ע���˵��   : ��
 * �޸�����     : �� 
 */
void delay_12us(void)
{
	uint32_t tim=62;
	while(tim--);	
}


/**
 * ����         : delay_50us(void)
 * ��������     : 
 * ����         : 
 * ����         : 50΢����ʱ
 * �������     : ��
 * �������     : ��
 * ���ؽ��     : ��
 * ע���˵��   : ��
 * �޸�����     : �� 
 */
void delay_50us(void)
{
	uint32_t tim=277;
	while(tim--);	
}

/**
 * ����         : delay_60us(void)
 * ��������     : 
 * ����         : 
 * ����         : 60΢����ʱ
 * �������     : ��
 * �������     : ��
 * ���ؽ��     : ��
 * ע���˵��   : ��
 * �޸�����     : �� 
 */
void delay_60us(void)
{
	uint32_t tim=330;
	while(tim--);	
}


/**
* @brief        ds18b20�����������
* @retval       None
*/
void DS18B20_Output(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
		/* configure PA2(ADC channel2) as analog input */
	
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

}

/**
* @brief        ds18b20������������
* @retval       None
*/
void DS18B20_Input(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
		/* configure PA2(ADC channel2) as analog input */

	gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
}

/**
* @brief       ��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ��� 
* @retval      None
*/
void DS18B20_Init(void)
{
	DS18B20_Output();
	
	DS18B20_Rst();
	DS18B20_Check();
}

//��λDS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_Output();   //����Ϊ���
	DS18B20_DQ_OUT_L;  	//����DQ

	delay_750us();
	
	DS18B20_DQ_OUT_H;  	//DQ=1
	delay_15us();

}

//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_Input();    //����Ϊ���� 
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(5);
//		delay_us(1);

	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(5);
//		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}


/**
* @brief		��DS18B20��ȡһ��λ
* @para			NULL
* @retval		����ֵ��1/0      
* - None		
* @note			��ȡ����ʱҪ��ʱ��һ�㣬���򣬶�ȡ����ʱ���ݲ�׼ȷ
*/
uint8_t DS18B20_Read_Bit(void) 
{
	uint8_t data;
	DS18B20_Output();   //����Ϊ���
	DS18B20_DQ_OUT_L; 
//	delay_2us();
	delay_us(5);

	DS18B20_DQ_OUT_H; 
	DS18B20_Input();    //����Ϊ����
//	delay_12us();
	delay_us(15);

	if(DS18B20_DQ_IN)data=1;
	else data=0;
//	delay_50us();
	delay_us(54);
     
	return data;
}

//��DS18B20��ȡһ���ֽ�
//����ֵ������������
uint8_t DS18B20_Read_Byte(void)   
{        
	uint8_t i,j,dat;
	dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
	return dat;
}

//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(uint8_t dat)     
 {             
    uint8_t j;
    uint8_t testb;
    DS18B20_Output();     //����Ϊ���
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if(testb)       // д1
        {
            DS18B20_DQ_OUT_L;

//			delay_2us();
			delay_us(5);
            DS18B20_DQ_OUT_H;

			delay_60us();            
        }
        else            //д0
        {
            DS18B20_DQ_OUT_L;

			delay_60us();
               
            DS18B20_DQ_OUT_H;
			delay_us(5);
//			delay_2us();
                          
        }
    }
}
 
/**
* @brief		��ʼ�¶�ת��
* @retval		None
*/
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}

/**
* @brief		��ds18b20�õ��¶�ֵ
* @retval		�����ͣ��¶�ֵ ��-55~125�� 
*/

uint16_t temp;
uint32_t f_tem;

int DS18B20_Get_Temp(void)
{
//    uint8_t temp;
    uint8_t TL,TH;
    int16_t tem;
	
	int temperature;
	
    DS18B20_Start ();           //��ʼת��
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);   // skip rom
    DS18B20_Write_Byte(0xbe);   // convert	    
    TL=DS18B20_Read_Byte();     // LSB   
    TH=DS18B20_Read_Byte();     // MSB   
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��	  	  
    tem=TH; //��ø߰�λ
    tem<<=8;    
    tem+=TL;//��õװ�λ
    temperature=(int)tem*6250;//ת��     
	if(temp)return temperature; //�����¶�ֵ
	else return -temperature;    

//	tem = TH<<8;
//	tem = tem | TL;
//	if(tem < 0 )
//		f_tem = (~tem+1) * 0.625;
////		f_tem = (~tem+1) * 0.0625;
//	else
//		f_tem = tem * 0.625;
////		f_tem = tem * 0.0625;
//	
//	return f_tem;
}


