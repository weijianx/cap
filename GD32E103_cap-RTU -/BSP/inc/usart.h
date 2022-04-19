#ifndef _USART_H
#define _USART_H
#include "stdio.h"
#include "gd32e10x.h"
#include <rtthread.h>
//extern uint8_t  *USART_RX_BUF; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符

extern uint8_t UART1_RXBuffLen;
#include "type.h"

#define TX_ON    gpio_bit_set(GPIOA,GPIO_PIN_8);
#define TX_OFF   gpio_bit_reset(GPIOA,GPIO_PIN_8);

#define MAX_USART1_DATA_LEN   200
extern uint8_t UART1_RXBuff[MAX_USART1_DATA_LEN];

#define MB_RTU_MODE          0x00
#define CAN_MODE             0x02
#define RS485_MODE          0x01 

extern BitAction UartRecvNewData;//
extern BitAction UartRecvFrameOK;//
extern rt_sem_t modbus_sem;

void UART1_Init(UartCommTypeDef *pUartStructure);
uint32_t UART1_SendData( uint8_t* UART1_SendBuff, uint32_t Len );
void UART1_IRQService( void ) ;
int32_t UART1_RecvData( uint8_t* UART1_RecvBuff, uint32_t Len );
void uprintf(const char *fmt,...)	;

#endif


