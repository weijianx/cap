#include "BSP.h"
//#include <intrinsics.h>

const unsigned char SRAM_DATA[SRAM_DATA_NUM] = {
0x00,0x00,0x00,0x62,0x63,0x00,0x65,0xBE,0x01,0x20,0x26,0x42,0x5C,0x48,0xA0,0x03,
0x21,0xE4,0x20,0x31,0xA1,0x03,0x21,0xE4,0x20,0x31,0x84,0x01,0x23,0x63,0x01,0x00,
0x00,0x00,0x00,0x00,0x20,0x0B,0x43,0x58,0xC0,0xFE,0x43,0xC0,0x44,0x7A,0x7E,0x20,
0x0B,0xC0,0xC0,0xC0,0xC8,0xFF,0x43,0xED,0x44,0xC0,0xC0,0xC0,0xF6,0xFF,0x43,0xEC,
0x44,0xC0,0xC0,0xC0,0xC8,0xFF,0x41,0x23,0x94,0xD0,0x43,0xEE,0x44,0xD2,0x43,0xEF,
0x44,0x20,0x5A,0x70,0x60,0x71,0x61,0x78,0x68,0x02,0x7A,0xF3,0x43,0xC7,0xFE,0x41,
0xEB,0x45,0x5A,0x21,0xDF,0x46,0x46,0x46,0x46,0xEC,0x44,0xC0,0xC0,0xC0,0xC8,0xFF,
0x43,0x55,0xED,0x45,0xEC,0x51,0xF4,0x41,0x23,0x88,0xEA,0x45,0xF5,0x41,0x23,0x88,
0xE9,0x45,0x1D,0x41,0x43,0x58,0xEA,0x21,0x99,0xE9,0x50,0x46,0xEB,0x44,0xA9,0x02,
0xEB,0x59,0x43,0xCA,0xFE,0x41,0x5C,0xA8,0x03,0xC0,0x5A,0xEB,0x45,0xEB,0x41,0xF2,
0x45,0xF6,0x41,0x23,0x88,0xEA,0x45,0xF7,0x41,0x23,0x88,0xE9,0x45,0x1F,0x41,0x43,
0x58,0xEA,0x21,0x99,0xE9,0x50,0x46,0xEB,0x44,0xA9,0x02,0xEB,0x59,0x43,0xCA,0xFE,
0x41,0x5C,0xA8,0x03,0xC0,0x5A,0xEB,0x45,0xEB,0x41,0xF3,0x45,0x02,0xFF,0xFF,0xFF,
0x47,0x47,0x47,0x47,0x47,0x47,0x47,0x47,0x7C,0x7D,0x45,0x41,0x02,0x47,0x47,0x47,
0x47,0x47,0x47,0x47,0xF0,0x7C,0x6D,0x45,0x41,0x6C,0x7D,0x45,0x41,0x02,0x47,0x47,
0x47,0x47,0x47,0x47,0x47,0xF0,0x6C,0x7D,0x45,0x41,0x02,0x47,0x47,0x47,0x47,0x47,
0x47,0x47,0xF0,0x7C,0x6D,0x45,0x41,0x45,0x41,0x02,0x47,0x47,0x47,0x47,0x47,0x47,
0x47,0xF0,0x7C,0x6D,0x45,0x41,0x02,0x47,0x47,0x47,0x47,0x47,0x47,0x47,0x02,0x4F,
0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,
0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,
0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x4F,0x02,
0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,
0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,
0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,
0x02,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,
0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,
0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,
0x56,0x02,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,
0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,
0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,
0x5E,0x5E,0x5E,0x02,0x6A,0xFD,0x43,0x40,0x4F,0x4F,0x4F,0xEB,0x45,0x7A,0xF9,0x41,
0x43,0x58,0xEB,0x21,0x99,0xEA,0x44,0xC0,0xC0,0xC0,0xF1,0xFF,0x43,0xEC,0x44,0xC0,
0xC0,0xC0,0xC8,0xFF,0x41,0xED,0x45,0xC0,0x41,0xC0,0xC0,0xC0,0xF8,0xFF,0x43,0xE9,
0x44,0x6A,0x1D,0x43,0xAB,0x01,0xEA,0x58,0x8E,0x03,0xEC,0x53,0x1D,0x50,0x1F,0x44,
0xEC,0x53,0xED,0x53,0xE9,0x43,0xEC,0x58,0xAC,0xE6,0x8E,0x26,0xC0,0xC0,0xC0,0xF9,
0xFF,0x43,0xEC,0x44,0xC0,0xC0,0xC0,0xC8,0xFF,0x43,0xED,0x44,0xC0,0x41,0xC0,0xC0,
0xC0,0xFC,0xFF,0x43,0xE9,0x44,0x1D,0x43,0x1F,0x59,0xE9,0x43,0xED,0x53,0xEC,0x53,
0x58,0xAC,0xF2,0x7A,0xC0,0xC0,0xC0,0xC9,0xFF,0x43,0xEC,0x44,0xE7,0x44,0xE8,0x44,
0xC0,0xC0,0xC0,0xC8,0xFF,0x43,0xED,0x44,0x1F,0x43,0x4E,0x4E,0x4E,0x44,0xC0,0xC0,
0xC0,0xCF,0xFF,0x43,0xE9,0x44,0x8E,0x07,0xC0,0xC0,0xC0,0xCB,0xFF,0x43,0xE9,0x44,
0x40,0x5D,0x1D,0x43,0x1F,0x21,0xCA,0xE8,0x43,0xEC,0x44,0x1D,0x45,0xF8,0x43,0xAB,
0x0C,0xC0,0x41,0xED,0x53,0x53,0x1F,0x43,0x4E,0x4E,0x4E,0x44,0xE7,0x53,0xC0,0x41,
0xE8,0x53,0xE7,0x53,0x41,0xEC,0x45,0xE9,0x43,0x5C,0xAC,0xD3,0xC0,0xC0,0xC0,0xCF,
0xFF,0x43,0xE9,0x44,0xE8,0x41,0xE9,0x43,0x5C,0xA8,0x0C,0xC0,0x41,0xE8,0x43,0x53,
0xEC,0x44,0x1D,0x44,0x59,0x43,0xAB,0xEB,0xC8,0x43,0x46,0x46,0x46,0x44,0x7A,0x8A,
0x1B,0xC0,0x43,0x40,0x5D,0x5D,0x90,0x15,0xC8,0x45,0xC9,0x45,0xF8,0x43,0xAA,0x0B,
0xCA,0x45,0xCB,0x45,0xCC,0x45,0xCD,0x45,0xCE,0x45,0xCF,0x45,0x00,0x02,0xC0,0x43,
0x4E,0x4E,0xEA,0x44,0xE9,0x44,0x8E,0x06,0xC0,0x43,0x4E,0xEA,0x44,0xE9,0x44,0xF8,
0x43,0xAB,0x13,0xC0,0x43,0x4E,0xEA,0x44,0x4E,0x50,0xE9,0x44,0x8E,0x08,0xC0,0x43,
0xEA,0x44,0x4E,0x4E,0x50,0xE9,0x44,0xC0,0xC0,0xC0,0xC8,0xFF,0x43,0x4E,0x4E,0xE9,
0x51,0x91,0x01,0x23,0x5A,0xEA,0x43,0xE9,0x51,0x92,0x01,0x23,0x5A,0xEA,0x43,0xE9,
0x51,0x93,0x01,0x23,0x5A,0xEA,0x43,0xE9,0x51,0x94,0x01,0x23,0x5A,0xEA,0x43,0xE9,
0x51,0x95,0x01,0x23,0x5A,0xEA,0x43,0xE9,0x51,0x96,0x01,0x23,0x5A,0xEA,0x43,0xE9,
0x51,0x97,0x01,0x23,0x5A,0xEA,0x43,0xE9,0x51,0x02,0xE9,0x43,0x46,0x46,0xEC,0x44,
0x1D,0x45,0x02,0x7A,0xFA,0x41,0x4F,0x4F,0x4F,0xE7,0x45,0x5A,0xFB,0x43,0xE7,0x75,
0x21,0xCA,0x65,0xD0,0x45,0x5A,0xFC,0x43,0xE7,0x21,0xCA,0xD1,0x45,0x5A,0xFD,0x43,
0xE7,0x21,0xCA,0xD2,0x45,0x79,0x69,0x02,0xD7,0xFE,0x43,0xE7,0x45,0x5D,0xAD,0x01,
0x5D,0x45,0x41,0x02,0x1F,0x43,0x1D,0x44,0xC0,0x43,0xEC,0x51,0xED,0x51,0x5D,0xAA,
0xF2,0x02,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};


//���üĴ���
//��һ�ӵ�ģʽ
//�ο����ݽ�PC0����������PC1���ڲ��ŵ����30K
//�ο�����660pF
//�¶Ȳ�����ʽ���ر�
//RDC��PWM/PDM ����Ҫ
//���ݲ�������ģʽ����������
const unsigned long REG[REG_CONF_NUM]={
0xC04200FF,//addr0 0xC04200FF
             //0X4; 
             //0Bxx(MECOMP):; 
             //0B10; 
             //0Xxx(ECC_MODE):; 
             //0Xx(AUTOBOOT_DIS):; 
             //0Xx(MEM_LOCK_DIS):; 
0xC1201022,//addr1 0xC1201022 �̶�
0xC203160B,//0xC203160B,//addr2 0xC203160B
             //0Xxx(CMEAS_PORT_EN):CDCʹ��,����PC7-PC0;           0X03,����PC0��PC1
             //0Bxxxxx(CMEAS_BITS):���������� 00010-�ӵص��ݵ�һ���� 01000-Ư�Ƶ�һ���� 10000-Ư�Ʋ�ֵ���; ?   0B00010 �ӵص��ݵ�һ����
             //0Bxxx(RDCHG_INT_SEL):�ڲ�����ѡ�� 100-180K 101-90K 110-30K 111-10K; 111 ����ԽСƵ��ԽС����Խ��   0B101 90K
             //0X0B; 
0xC30D2200,//addr3 0xC3002001,0xC3002064
             //0Bxx(CY_CLK_SEL):CDCѭ����ʱ�ӻ�׼ 0-20us 2-1us(���ⲿ4M����ʱ) 3-0.25us(���ⲿ4M����ʱ); 0B00
             //0B0;
             //0Bxxxxx(SEQ_TIME):��ʱ��-����ģʽ���ô���������(TMEAS_TRIG_SEL=2) 0-�ر�;                 0B00000
             //0B0;
             //0Bxx(CMEAS_FAKE):�趨CDCÿ�β����ĺ�ļٲ����������ɴ�0��4ѡ��;                           0B01 2��
             //0Bxxx...(C_AVRG):CDC�������ڲ�ƽ���Ĵ���������Ϊ0��Ч��Ĭ��Ϊ1;                           0B0000000000001  100�� 064
0xC4000210,//addr4 0xC4200500,,0xC4002F00
             //0B00;
             //0Bxx(CMEAS_STARTPIN):ѡ����Ӧ��������ݲ��������崥���ܽ� 0-PG0 1-PG1 2-PG2 3-PG3;        0B10-PG2
             //0Bxx(CMEAS_TRIG_SEL):ѡ����ݲ����Ĵ���Դ 0-���������� 1-����ģʽ 2-ʱ�Ӵ���ģʽ 3-���崥��ģʽ;      0B00 -����������
             //0Bxxxxxxxxxx(CMEAS_CYTIME):�������ڵ��ݲ�����CDC����ʱ�� (CMEAS_CYTIME+1)*Clock_PeriodClock_period=20us/1us/0.25us����CYCLKSEL=0/2/3;  0X05   //2�μٲ�����100��ƽ��
             //0B000;
             //0Bx(TMEAS_CYTIME):���������¶Ȳ���������ʱ�� 0-140us(�Ƽ�) 1-280us;                           0-140us
             //0Bxx(TMEAS_STARTPIN):ѡ�������¶Ȳ��������崥���ܽ� 0-PG0 1-PG1 2-PG2 3-PG3;                  0-PG0
             //0Bxx(TMEAS_TRIG_SEL):ѡ���¶Ȳ����Ĵ���Դ.ѡ��2��3�벻ҪӦ��; 0-�ر�/�����봥�� 1-CMEAS-����(ͨ��C-����,�Ƽ�) 2-ʱ�Ӵ���ģʽ 3-���崥��ģʽ
0xC5800000,//addr5 0xC5000000
             //0Bxx(T_AVRG):0-1��ƽ�� 1-4��ƽ�� 2-8��ƽ�� 3-16��ƽ��; 
             //0Bxxx...(TMEAS_TRIG_PREDIV):RDC�����CDC�Ĳ������ֵĴ��� 0Ϊ1��; 
0xC600C940,//addr6 0xC6004340
             //0X00; 
             //0Bx(TMEAS_FAKE):0-2�μٲ��� 1-8�μٲ���;               0
             //0Bxxxxxxx(TMEAS_7BITS):ѡ���¶Ȳ��������ӷ�ʽ;       
             //0X40; �̶�
0xC71F0000,//addr7 0xC71F0000 �̶�
0xC8800310,//addr8 0xC8800010 DSP����
			 //0Bx(DSP_SRAM_SEL):Ϊ��Ƭ��ѡ�����洢�ռ� 0-OTP,1-SRAM                               0B1
			 //0Bx(DSP_START) :��Ƭ���Ŀ�ʼ����; ��Ƭ����ʱ�ӿ�ʼ����,��������ת����ַ0Ȼ��������ʼ����.���̼���ɺ�,DSP����ֹͣ��������ʱ��.
			 //0Bx(DSPSTARTONOVL):0
			 //0Bx(DSP_STARTONTEMP)0
			 //0Bxxxx(DSP_STARTPIN):�������津���ĸ����ܽ�,16 = PG0,17 = PG1,18 = PG2,19 = PG               0B0000
			 //0Bxxxx(DSP_FF_IN):�������津���ĸ����ܽ�,12 = PG0,13 = PG1,14 = PG2,15 = PG3                 0B0000
			 //0Bxx(DSP_WATCHDOG_LENGTH):0
			 //0Bxx(DSP_MOFLO_EN):0
			 //0B00
			 //0Bxx(DSP_SPEED):���� DSP �ٶ�  1 = ��׼ (����),3 = �͹���(����)                              0B01
			 //0B0
			 //0Bx(INT2PG2)��QFN24��װ��������,��Ϊ���ʱ��û��INTN�жϹܽ�.�������ж��ź�ͨ��PG2�˿����. 0
			 //0B00
0xC9BF208F,//addr9 0xC9DF2003 PWM/PDM����
			 //0Bxxxx(PG_DIR_IN):������л�Ϊ����(PG3/λ23 �� PG0/λ20).0-�����1-����                      0B1101��PG2���
			 //0Bxxxx(PG_PULL_UP):�� PG0 �� PG3 ���Ͽ�����������;                                           0B1111
			 //0Bxxxx(PI_EN):���������ܶȻ���������ȵ����������0Bxx10 = PDM0 ����                         0B0010  ����PDM0 
			 //0Bxxxx(PI1_CLK_SEL)��������ӿڵĻ���Ƶ��,�����ڲ���Ƶ��(OLF)�����ⲿ��Ƶ���� (OHF)        0X0
			 //0Bxxxx(PI0_CLK_SEL)                                                                          0X0
			 //0Bxx(PI1_RES):��������ľ���0 = 7 bit1 = 8, bit2 = 9, bit,3 = 10 bit,                        0
			 //0Bxx(PI0_RES):��������ľ���                                                                 0X03 10 bit
0xCA180047,//addr10 0xCA180087
             //0X1800
             //0Xxx(V_CORE_CTL):����1.8 V ���ĵ�ѹ������ 0x47-��׼ 0x87-�͹���                              ��׼0x47
0xCB000000,//addr11
0xCC000000,//addr12
0xCD000000,//addr13
0xCE000000,//addr14
0xCF000000,//addr15
0xD0000000,//addr16
0xD1000000,//addr17
0xD2000000,//addr18
0xD3200000,//addr19
0xD4000001,//addr20
};

unsigned long PCap_Status = 0;
unsigned long ulWritePCAPXramErr = 0;
BitAction PCap_Ready = Bit_RESET;

#define SET_SCK             gpio_bit_set(GPIOB, GPIO_PIN_3)
#define CLR_SCK             gpio_bit_reset(GPIOB, GPIO_PIN_3)

#define SET_MOSI            gpio_bit_set(GPIOB, GPIO_PIN_5)
#define CLR_MOSI            gpio_bit_reset(GPIOB, GPIO_PIN_5)

#define SET_SSN             gpio_bit_set(GPIOA, GPIO_PIN_15)
#define CLR_SSN             gpio_bit_reset(GPIOA, GPIO_PIN_15)


#define READ_MISO			gpio_input_bit_get(GPIOB, GPIO_PIN_4)

//SSN������,��ʼ��������
void SPI_Enable(void)
{
    gpio_bit_reset(GPIOA, GPIO_PIN_15);

//    for(unsigned long i = 0; i < 7; i++)    
//    __NOP();
//    __NOP();
//    __NOP();
//    __NOP();
//    __NOP();
//    __NOP();
//    __NOP();
//    __NOP();
//    __NOP();
}

//SSN���ø�,������������

void SPI_Disable(void)
{	
    gpio_bit_set(GPIOA, GPIO_PIN_15);
//    __NOP();
//    __NOP();
//    __NOP();
}


//spi send 0
void spi_send_0(void)
{
	SET_SCK;
	CLR_MOSI;
	CLR_SCK;
//    for(unsigned long i = 0; i < 4; i++)    
//        __NOP();
//    __NOP();
}

//spi send 1
void spi_send_1(void)
{
	SET_SCK;
	SET_MOSI;
	CLR_SCK;
//    for(unsigned long i = 0; i < 4; i++)    
//        __NOP();
//	__NOP();
}

void spi_write8(unsigned char wbuf8)
{
	unsigned long i;

	for(i = 0; i < 8;i++)
	{
		if(wbuf8 & 0x80) //�����λΪ1
			spi_send_1();
		else 
			spi_send_0();

		wbuf8 = wbuf8 << 1; //����һλ���ڶ���λ������λ����ʼ��һ��д������
	}
}

void spi_write16(unsigned int wbuf16)
{
	unsigned long i;

	for(i = 0; i < 16;i++)
	{
		if(wbuf16 & 0x8000) //�����λΪ1
			spi_send_1();
		else 
			spi_send_0();

		wbuf16 = wbuf16 << 1;//����һλ���ڶ���λ������λ����ʼ��һ��д������
	}
}

void spi_write32(unsigned long wbuf32)
{
	unsigned long i;
	
	for(i = 0; i < 32;i++)
	{
		if(wbuf32 & 0x80000000) //�����λΪ1
			spi_send_1();
		else 
			spi_send_0();

		wbuf32 = wbuf32 << 1;//����һλ���ڶ���λ������λ����ʼ��һ��д������
	}
}

//SPI ��8λ
unsigned char spi_read8()
{
	unsigned long cnt;
	unsigned char rcvdata = 0x00;

	for(cnt = 8; cnt > 0;cnt--)
	{
		rcvdata <<= 1; 
		SET_SCK;
//        __NOP();
//        __NOP();
		if(READ_MISO)           //
			rcvdata |=1;
		CLR_SCK;
//		for(unsigned long i = 0; i < 9; i++)    
//            __NOP();
//	    __NOP();
 	}
	return rcvdata;
}

//SPI ��24λ
unsigned long spi_read24()
{
	unsigned long cnt;
	unsigned long rcvdata = 0x00000000;

	for(cnt = 24;cnt > 0;cnt--)
	{
		rcvdata <<= 1; 
		SET_SCK;
//        __NOP();
//        __NOP();
		if(READ_MISO)
			rcvdata |=1;
		CLR_SCK;
//		for(unsigned long i = 0; i < 9; i++)    
//            __NOP();
//	    __NOP();
	}
	return rcvdata;
}

//�ϵ縴λָ��
void PCap_Reset(void)
{
    SPI_Enable();
	spi_write8(0x88);
	SPI_Disable();
}

//���ָ�λָ��
void PCap_PTL_Reset(void)
{
    SPI_Enable();
	spi_write8(0x8A);
	SPI_Disable();
}

//���ݲ���ָ��
void PCap_MEASURE(void)
{
    SPI_Enable();
	spi_write8(0x8C);
	SPI_Disable();
}

//����PCap
void PCap_Enable(void)
{
    SPI_Enable();
	spi_write32(0xD4000001);  //addr20
	SPI_Disable();
}

unsigned long PCap_WR_SRAM(void)       //�ѹ̼�д��SRAM�У�128���ֽ�
{
    unsigned short i = 0;
  	unsigned char test_write = 0x00;
	unsigned char test_read  = 0x00;
	unsigned long ErrCnt = 0;

    for(i = 0; i < SRAM_DATA_NUM; i++)
	{
	    test_write = SRAM_DATA[i];
	    
		//SRAM д
		SPI_Enable();
	    spi_write16(i | 0x9000);
		spi_write8(test_write);
		SPI_Disable();

		Delay_Us(50);

		//SARM ��		
		SPI_Enable();
		spi_write16(i | 0x1000);         //����ַΪ0λ�õ�����
		test_read = spi_read8();
		SPI_Disable();

        if(test_read != test_write)
        {
            ErrCnt++;
    		if(ErrCnt > 10) return 1;    //ͨ�Ų���ʧ��     (ֻ��200��)
    		Delay_Us(50);
    		i -= 1;
        }
	}
	
	return 0;                    //ͨ�Ų��Գɹ�
}


//PCap �Ĵ�������
void PCap_Reg_Config(void)
{
	unsigned long i ;
	for(i = 0;i < REG_CONF_NUM;i++)      //д20������ͻ�ƿ�
	{
	    SPI_Enable();
		spi_write32(REG[i]);
		SPI_Disable();
	}
}

uint32_t PCap_Res_Temp(void)
{
	unsigned long cap_res_temp = 0;

	//������Ĵ���1 
	SPI_Enable();
	spi_write8(0x4D);
	//delayMs(5);
	cap_res_temp = spi_read24();
	SPI_Disable();

	return cap_res_temp;
}

unsigned long PCap_Res_Value(void)
{
	unsigned long cap_res = 0;

	//������Ĵ���1 
	SPI_Enable();
	spi_write8(0x41);
	cap_res = spi_read24();
	SPI_Disable();
	
	return	cap_res; 
}



unsigned long PCap1_Res_Value(void)
{
	unsigned long cap_res = 0;

	//������Ĵ���1 
	SPI_Enable();
	spi_write8(0x42);
	cap_res = spi_read24();
	SPI_Disable();
	
	return	cap_res; 
}

unsigned long PCap_Res_Stau(void)
{
	//unsigned long cap_stau = 0;

	//������Ĵ���1 
	SPI_Enable();
	spi_write8(0x48);
	PCap_Status = spi_read24();
	SPI_Disable();
	return	PCap_Status; 
}

void PCap_SPI_Init(void)
{

  	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_AF);
    
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	
	
    gpio_init( GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
	
	gpio_init( GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	
	gpio_init( GPIOB, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
	
	gpio_init( GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
	
}


void PCap_Init(void)
{
    unsigned short wrSRAMCnt = 0;
    
    PCap_SPI_Init();
    rt_thread_delay(1);
    
	PCap_Reset();				//�ϵ縴λ
	rt_thread_delay(1);
    
    do{
        //before this, wdog is no initiate, so don't feed the wdog in here
        ulWritePCAPXramErr = PCap_WR_SRAM();
        if(0 == ulWritePCAPXramErr)
            break;
        wrSRAMCnt++;
        Delay_Us(10);
    }
    while(wrSRAMCnt < 10);
    Delay_Us(10);

    PCap_Reg_Config();			//���üĴ���
    Delay_Us(10);

    PCap_Enable();				//����PCapоƬ
    rt_thread_delay(1);

    PCap_PTL_Reset();			//�����ϵ縴λ��SRAM�����üĴ���ֵ���䣩
    rt_thread_delay(1);

    PCap_MEASURE(); 					//������ǰ��
    rt_thread_delay(1);

	//Capture_TimerInit();
}

void resetPcap(void)
{
    static unsigned char ucResetCnt = 0;
    unsigned short wrSRAMCnt = 0;

    if(++ucResetCnt >= 3)
    {
        ucResetCnt = 0;
        //reboot
        NVIC_SystemReset(); //define in ..\IAR Systems\Embedded Workbench 7.0\arm\CMSIS\Include\core_cm0.h
        while(1)
        {;}
    }
    else
    {
        //disable timer interrupt
//        TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
//        TIM_Cmd(TIM3, DISABLE);
//        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		timer_interrupt_disable(TIMER2, TIMER_INT_UP);
		timer_disable(TIMER2);
		timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
		
        //Disable pcap, if SPI is work normally, the action will quickly reset pcap,and shut down the convert
        PCap_Reset();
        
        //initiate pcap
    
        PCap_SPI_Init();
        rt_thread_delay(1);
        
    	PCap_Reset();				//�ϵ縴λ
    	rt_thread_delay(1);
        
        do{
#if IWDG_EN > 0
            fwdgt_counter_reload();
#endif
            if(0 == PCap_WR_SRAM())
                break;
            wrSRAMCnt++;
            Delay_Us(10);
        }
        while(wrSRAMCnt < 10);
        Delay_Us(10);
        
    	PCap_Reg_Config();			                                //���üĴ���
    	Delay_Us(10);

#if IWDG_EN > 0
        fwdgt_counter_reload();
#endif

    	PCap_Enable();				                                //����PCapоƬ
    	rt_thread_delay(1);

    	PCap_PTL_Reset();			                                //�����ϵ縴λ��SRAM�����üĴ���ֵ���䣩
        rt_thread_delay(1);

    	PCap_MEASURE(); 					                //������ǰ��
    }
}