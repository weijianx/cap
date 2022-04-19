/****************************************************************************************************************************************************************
** ��Ȩ:          2017-2027,��������Ϊ�Ƽ���չ���޹�˾
** �ļ���:        SoftSpi.h
** ����:          ׯ��Ⱥ
** �汾:          V1.0.0
** ����:          2017-05-09
** ����:          ���ģ��SPIͨ��
** ����:          SPI����
*****************************************************************************************************************************************************************
** �޸���:        No
** �汾:          No
** �޸�����:      No
** ����:          No
****************************************************************************************************************************************************************/

#ifndef __SOFTSPI_H
#define __SOFTSPI_H

//#include "stm32l0xx_hal_rcc.h"
//#include "stm32l0xx_hal_rcc_ex.h"
//#include "stm32l0xx_hal_gpio.h"
//#include "stm32l0xx_hal_gpio_ex.h"
//#include "stm32l0xx_hal_dma.h"

#include "gd32e10x.h"

//SPIƬѡ
#define SPI_NSS_PIN                                      GPIO_PIN_15
//SPIƬѡ�˿�
#define SPI_NSS_PORT                                     GPIOA

//SPI MOSI����
#define SPI_MOSI_PIN                                     GPIO_PIN_5
//SPI MOSI�˿�
#define SPI_MOSI_PORT                                    GPIOB

//SPI SCK����
#define SPI_SCK_PIN                                      GPIO_PIN_3
//SPI SCK�˿�
#define SPI_SCK_PORT                                     GPIOB

//SPI MISO����
#define SPI_MISO_PIN                                     GPIO_PIN_4
//SPI MISO�˿�
#define SPI_MISO_PORT                                    GPIOB

////SPIʹ��
//#define SPI_ENABLE                                       gpio_bit_write(SPI_NSS_PORT, SPI_NSS_PIN, GPIO_PIN_RESET)
////SPI��ֹ
//#define SPI_DISABLE                                      gpio_bit_write(SPI_NSS_PORT, SPI_NSS_PIN, GPIO_PIN_SET)

////SPI MOSI�ø�
//#define SPI_MOSI_SET                                     gpio_bit_write(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_PIN_SET)
////SPI MOSI�õ�
//#define SPI_MOSI_CLR                                     gpio_bit_write(SPI_MOSI_PORT, SPI_MOSI_PIN, GPIO_PIN_RESET)

////SPI SCK�ø�
//#define SPI_SCK_SET                                      gpio_bit_write(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_SET)
////SPI SCK�õ�
//#define SPI_SCK_CLR                                      gpio_bit_write(SPI_SCK_PORT, SPI_SCK_PIN, GPIO_PIN_RESET)
//SPI MISO��ȡ��ƽ
#define SPI_MISO_STA                                     gpio_input_bit_get(SPI_MISO_PORT, SPI_MISO_PIN)

//SPIʹ��
#define SPI_ENABLE                                       gpio_bit_reset(SPI_NSS_PORT, SPI_NSS_PIN)
//SPI��ֹ
#define SPI_DISABLE                                      gpio_bit_set(SPI_NSS_PORT, SPI_NSS_PIN)

//SPI MOSI�ø�
#define SPI_MOSI_SET                                     gpio_bit_set(SPI_MOSI_PORT, SPI_MOSI_PIN)
//SPI MOSI�õ�
#define SPI_MOSI_CLR                                     gpio_bit_reset(SPI_MOSI_PORT, SPI_MOSI_PIN)

//SPI SCK�ø�
#define SPI_SCK_SET                                      gpio_bit_set(SPI_SCK_PORT, SPI_SCK_PIN)
//SPI SCK�õ�
#define SPI_SCK_CLR                                      gpio_bit_reset(SPI_SCK_PORT, SPI_SCK_PIN)

void SPI_Init(void);
void SPI_Bit_Set(void);
void SPI_Bit_Clr(void);
void Delay_us(uint32_t us);
uint8_t SPI_Receive_8Bit(void);
uint32_t SPI_Receive_24Bit(void);
void SPI_Send_8Bit(uint8_t sBit8);
void SPI_Send_16Bit(uint16_t sBit16);
void SPI_Send_32Bit(uint32_t sBit32);

#endif
