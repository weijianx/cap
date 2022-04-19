#ifndef __SOFTWAREIIC_H__
#define __SOFTWAREIIC_H__

#include "gd32e10x.h"


void Delay_Ms(uint32_t cnt);
void Delay_Us(uint32_t cnt);

//iic配置为SCL为PB6/SDA为PB7
#define SDA_IN()  gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7)		//配置PB7为输入模式
#define SDA_OUT() gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7)             //配置PB7为输出模式

#define Set_IIC_SCL			gpio_bit_set(GPIOB,GPIO_PIN_6)
#define Reset_IIC_SCL		gpio_bit_reset(GPIOB,GPIO_PIN_6)
#define Set_IIC_SDA			gpio_bit_set(GPIOB,GPIO_PIN_7)
#define Reset_IIC_SDA		gpio_bit_reset(GPIOB,GPIO_PIN_7)
#define READ_SDA			gpio_input_bit_get(GPIOB,GPIO_PIN_7)
#define Write_SDA(x)		gpio_bit_write(GPIOB, GPIO_PIN_7, x?SET:RESET)

//IIC所有操作函数
void Delay_Ms(uint32_t cnt);
void Delay_Us(uint32_t cnt);
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	

#endif
