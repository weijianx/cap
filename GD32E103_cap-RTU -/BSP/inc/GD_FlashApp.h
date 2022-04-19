
#ifndef GD_FlashAPP_H
#define GD_FlashAPP_H
#include "stdint.h"
#include "gd32e10x.h"

#define	start_adder			0x08003000

//#define	start_adder			0x0801F800

#define FLASH_PAGE_SIZE                 0x00000400U    /*!< FLASHҳ���С��1K�ֽ� */
//#define IN_FLASH_BASE_ADDRESS           0x0801F800   

/**@brief       ���ڲ�Flashָ��λ�ö�ȡһ�ֽ�����
* @param[in]    RWAddr : ��ȡ��ַ
* @return       ����ִ�н�� - 1���ֽ�����
* @note         
*/
uint8_t Flash_Read_OneByte(uint32_t RWAddr);

/**@brief       ���ڲ�Flashָ��λ��д��һ�ֽ�����
* @param[in]    RWAddr : д���ַ
* @param[in]    WrData : д������
* @return       ����ִ�н��  1 �ɹ�  0 ʧ��
* @note         
*/
uint8_t Flash_Write_OneByte(uint32_t RWAddr, uint8_t WrData);


/**@brief       ���ڲ�Flashָ��λ�ö�ȡ���ֽ�����
* @param[in]    RWAddr : ��ȡ��ַ
* @return       ����ִ�н�� - 2���ֽ�����
* @note         
*/
uint16_t Flash_Read_twoByte(uint32_t RWAddr);

/**@brief       ���ڲ�Flashָ��λ��д����ֽ�����
* @param[in]    RWAddr : д���ַ
* @param[in]    WrData : д������
* @return       ����ִ�н��  1 �ɹ�  0 ʧ��
* @note         
*/
uint8_t  Flash_Write_twoByte(uint32_t RWAddr, uint16_t WrData);


/**@brief       ���ڲ�Flashָ��λ�ö�ȡ���ֽ�����
* @param[in]    RWAddr : ��ȡ��ַ
* @return       ����ִ�н�� - 4���ֽ�����
* @note         
*/
uint32_t Flash_Read_fourByte(uint32_t RWAddr);

/**@brief       ���ڲ�Flashָ��λ��д�����ֽ�����
* @param[in]    uint32_t RWAddr : д���ַ
* @param[in]    uint32_t WrData : д������
* @return       ����ִ�н��  1 �ɹ�  0 ʧ��
* @note         
*/
uint8_t Flash_Write_fourByte(uint32_t RWAddr, uint32_t WrData);


/**@brief       ���ڲ�Flashָ��λ�ö�����ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @param[in]    pRdbuf : ���ݻ���ָ��
* @param[in]    Rdlen : �����ݳ���
* @return       ����ִ�н��
* @note        
*/
void Flash_Read_MultiBytes(uint32_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen);


/**@brief       ���ڲ�Flashָ��λ��д����ֽ�
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    pWrbuf : ���ݻ���ָ��
* @param[in]    Wrlen : д���ݳ���
* @return       ����ִ�н�� 1 �ɹ�  0 ʧ��
* @note  		Wrlen < 2048,����������д2048���ֽڣ��ɿ�ҳд
*/
uint8_t Flash_Write_MultiBytes(uint32_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);
	
#endif
