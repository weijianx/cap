
#ifndef GD_FlashAPP_H
#define GD_FlashAPP_H
#include "stdint.h"
#include "gd32e10x.h"

#define	start_adder			0x08003000

//#define	start_adder			0x0801F800

#define FLASH_PAGE_SIZE                 0x00000400U    /*!< FLASH页面大小，1K字节 */
//#define IN_FLASH_BASE_ADDRESS           0x0801F800   

/**@brief       向内部Flash指定位置读取一字节数据
* @param[in]    RWAddr : 读取地址
* @return       函数执行结果 - 1个字节数据
* @note         
*/
uint8_t Flash_Read_OneByte(uint32_t RWAddr);

/**@brief       向内部Flash指定位置写入一字节数据
* @param[in]    RWAddr : 写入地址
* @param[in]    WrData : 写入数据
* @return       函数执行结果  1 成功  0 失败
* @note         
*/
uint8_t Flash_Write_OneByte(uint32_t RWAddr, uint8_t WrData);


/**@brief       向内部Flash指定位置读取二字节数据
* @param[in]    RWAddr : 读取地址
* @return       函数执行结果 - 2个字节数据
* @note         
*/
uint16_t Flash_Read_twoByte(uint32_t RWAddr);

/**@brief       向内部Flash指定位置写入二字节数据
* @param[in]    RWAddr : 写入地址
* @param[in]    WrData : 写入数据
* @return       函数执行结果  1 成功  0 失败
* @note         
*/
uint8_t  Flash_Write_twoByte(uint32_t RWAddr, uint16_t WrData);


/**@brief       向内部Flash指定位置读取四字节数据
* @param[in]    RWAddr : 读取地址
* @return       函数执行结果 - 4个字节数据
* @note         
*/
uint32_t Flash_Read_fourByte(uint32_t RWAddr);

/**@brief       向内部Flash指定位置写入四字节数据
* @param[in]    uint32_t RWAddr : 写入地址
* @param[in]    uint32_t WrData : 写入数据
* @return       函数执行结果  1 成功  0 失败
* @note         
*/
uint8_t Flash_Write_fourByte(uint32_t RWAddr, uint32_t WrData);


/**@brief       从内部Flash指定位置读多个字节
* @param[in]    RWAddr : 读起始地址
* @param[in]    pRdbuf : 数据缓存指针
* @param[in]    Rdlen : 读数据长度
* @return       函数执行结果
* @note        
*/
void Flash_Read_MultiBytes(uint32_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen);


/**@brief       向内部Flash指定位置写多个字节
* @param[in]    RWAddr : 写起始地址
* @param[in]    pWrbuf : 数据缓存指针
* @param[in]    Wrlen : 写数据长度
* @return       函数执行结果 1 成功  0 失败
* @note  		Wrlen < 2048,单次最大可以写2048个字节，可跨页写
*/
uint8_t Flash_Write_MultiBytes(uint32_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);
	
#endif
