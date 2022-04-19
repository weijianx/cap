//#include "BSP.h"
#include "SoftwareIIC.h"
#include "systick.h"
#include <rtthread.h>



void Delay_Ms(uint32_t cnt)
{
    cnt = cnt * 25000;

    while (cnt--);
//	rt_thread_delay(cnt);
}

void Delay_Us(uint32_t cnt)
{
    cnt = cnt * 18;

    while (cnt--);
}

//???IIC
void IIC_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6|GPIO_PIN_7);
    GPIO_BC(GPIOB) = GPIO_PIN_6|GPIO_PIN_7;
    Set_IIC_SCL;
    Set_IIC_SDA;
}

//??IIC????
void IIC_Start(void)
{
    SDA_OUT();     //sda???
    Set_IIC_SDA;
    Set_IIC_SCL;
//    GPIO_SetBits(GPIOC, GPIO_Pin_9);//del frank cl
    Delay_Us(4);
//    GPIO_ResetBits(GPIOC, GPIO_Pin_9);
    Reset_IIC_SDA;//START:when CLK is high,DATA change form high to low
    Delay_Us(4);
    Reset_IIC_SCL;//??I2C??,?????????
}

//??IIC????
void IIC_Stop(void)
{
    SDA_OUT();//sda???
    Reset_IIC_SCL;
    Reset_IIC_SDA;//STOP:when CLK is high DATA change form low to high
    Delay_Us(4);
    Set_IIC_SCL;
//    Set_IIC_SDA;//??I2C?????? change frank cl
    Delay_Us(4);
		Set_IIC_SDA;//??I2C??????
}

//????????
//???:1,??????
//        0,??????
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SDA_IN();      //SDA?????
    Set_IIC_SDA;
    Delay_Us(1);
    Set_IIC_SCL;
    Delay_Us(1);

    while (READ_SDA)
    {
        ucErrTime++;

        if (ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }

    Reset_IIC_SCL;//????0

    return 0;
}

//??ACK??
void IIC_Ack(void)
{
    Reset_IIC_SCL;
    SDA_OUT();
    Reset_IIC_SDA;
    Delay_Us(2);
    Set_IIC_SCL;
    Delay_Us(2);
    Reset_IIC_SCL;
}

//???ACK??
void IIC_NAck(void)
{
    Reset_IIC_SCL;
    SDA_OUT();
    Set_IIC_SDA;
    Delay_Us(2);
    Set_IIC_SCL;
    Delay_Us(2);
    Reset_IIC_SCL;
}

//IIC??????
//????????
// 1,???
// 0,???
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    Reset_IIC_SCL;//??????????

    for (t = 0;t < 8;t++)
    {
        Write_SDA((txd&0x80) >> 7);
        txd <<= 1;
        Delay_Us(2);   //?TEA5767??????????
        Set_IIC_SCL;
        Delay_Us(2);
        Reset_IIC_SCL;
        Delay_Us(2);
    }
}

//?1???,ack=1?,??ACK,ack=0,??nACK
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN();//SDA?????

    for (i = 0;i < 8;i++)
    {
        Reset_IIC_SCL;
        Delay_Us(2);
        Set_IIC_SCL;
        receive <<= 1;

        if (READ_SDA)receive++;

        Delay_Us(1);
    }

    if (!ack)
        IIC_NAck();//??nACK
    else
        IIC_Ack(); //??ACK

    return receive;
}
