#ifndef __AT24CXX_H__
#define __AT24CXX_H__
#include "gd32e10x.h"
//---------------------------------------------------
#include "type.h"

//---------------------------------------------------
#define RUN_ADDR_BASE				0x00								//参数初始化标志位，避免每次上电都写EEPROMP
#define SLAVE_ADDR					(RUN_ADDR_BASE		+ 0x01)
#define BAUDRATE					(SLAVE_ADDR			+ 0x01)
#define PARITY						(BAUDRATE			+ 0x01)
#define FILTER_LEVEL				(PARITY				+ 0x01)
#define AUTO_UPLOAD_TIME			(FILTER_LEVEL		+ 0x01)
#define USER_FLOW_K					(AUTO_UPLOAD_TIME	+ 0x01)
#define USER_FLOW_B					(USER_FLOW_K		+ 0x04)
#define OUTPUT_UNIT					(USER_FLOW_B		+ 0x04)
#define OUTPUT_MODE					(OUTPUT_UNIT		+ 0x01)
#define ECHOEN						(OUTPUT_MODE		+ 0x01)

#define USER_DEFAULT_LEN			(ECHOEN - RUN_ADDR_BASE	+ 0x01)		//16byte

//---------------------------------------------------
#define CAPMIN						(ECHOEN				+ 0x01)
#define CAPMAX						(CAPMIN				+ 0x04)
#define DAMIN						(CAPMAX				+ 0x04)
#define DAMAX						(DAMIN				+ 0x04)
#define DEF_RANGE					(DAMAX				+ 0x04)
#define DEF_CAPMIN					(DEF_RANGE			+ 0x04)
#define DEF_CAPMAX					(DEF_CAPMIN			+ 0x04)
#define TEMP_K						(DEF_CAPMAX			+ 0x04)
#define TEMP_B						(TEMP_K				+ 0x04)
#define TEMP2_K						(TEMP_B				+ 0x04)
#define TEMP2_B						(TEMP2_K			+ 0x04)
#define HEIGHTDAMAX					(TEMP2_B			+ 0x04)


#define PRO_DEFAULT_LEN				(HEIGHTDAMAX			+ 0x04)			//64byte

//---------------------------------------------------
#define CALIB_FLOATER_SCALE_1				(RUN_ADDR_BASE		+ 0x80)
#define CALIB_FLOATER_SCALE_2				(CALIB_FLOATER_SCALE_1	+ 0x02)
#define CALIB_FLOATER_SCALE_3				(CALIB_FLOATER_SCALE_2	+ 0x02)
#define CALIB_FLOATER_SCALE_4				(CALIB_FLOATER_SCALE_3	+ 0x02)
#define CALIB_FLOATER_SCALE_5				(CALIB_FLOATER_SCALE_4	+ 0x02)
#define CALIB_FLOATER_SCALE_6				(CALIB_FLOATER_SCALE_5	+ 0x02)
#define CALIB_FLOATER_SCALE_7				(CALIB_FLOATER_SCALE_6	+ 0x02)
#define CALIB_FLOATER_SCALE_8				(CALIB_FLOATER_SCALE_7	+ 0x02)

#define CALIB_FLOATER_DA_1 					(CALIB_FLOATER_SCALE_8	+ 0x02)
#define CALIB_FLOATER_DA_2					(CALIB_FLOATER_DA_1	+ 0x02)
#define CALIB_FLOATER_DA_3					(CALIB_FLOATER_DA_2	+ 0x02)
#define CALIB_FLOATER_DA_4					(CALIB_FLOATER_DA_3	+ 0x02)
#define CALIB_FLOATER_DA_5					(CALIB_FLOATER_DA_4	+ 0x02)
#define CALIB_FLOATER_DA_6					(CALIB_FLOATER_DA_5	+ 0x02)
#define CALIB_FLOATER_DA_7					(CALIB_FLOATER_DA_6	+ 0x02)
#define CALIB_FLOATER_DA_8					(CALIB_FLOATER_DA_7	+ 0x02)

#define CALIB_DEFAULT_LEN			(CALIB_FLOATER_DA_8 - CALIB_FLOATER_SCALE_1	+ 0x02)

//---------------------------------------------------
#define USER_DEFAULT_PARA_BAK1		(CALIB_FLOATER_SCALE_1 			+ 0x80)
#define CALIB_DEFAULT_PARA_BAK1		(USER_DEFAULT_PARA_BAK1 		+ 0x80)
#define USER_DEFAULT_PARA_BAK2		(CALIB_DEFAULT_PARA_BAK1 		+ 0x80)
#define CALIB_DEFAULT_PARA_BAK2		(USER_DEFAULT_PARA_BAK2 		+ 0x80)

//---------------------------------------------------
#define COMPANY						(CALIB_DEFAULT_PARA_BAK2		+ 0x80)
#define DEV_ENCODING				(COMPANY						+ 0x80)
#define SENSOR_VER					(DEV_ENCODING					+ 0x80)
#define DEV_ID						(SENSOR_VER						+ 0x80)


#define AT24C01			127
#define AT24C02			255
#define AT24C04			511
#define AT24C08			1023
#define AT24C16			2047
#define AT24C32			4095
#define AT24C64	    	8191
#define AT24C128		16383
#define AT24C256		32767
#define EE_TYPE			AT24C16

#define EEPROM_ADDRESS       0xA0


extern ProductParaTypeDef ProductPara;
extern RunVarTypeDef RunVar;
extern UartCommTypeDef UartComm;
extern UserParamTypeDef UserParam;

extern uint8_t Cur_Param[PRO_DEFAULT_LEN];
extern uint8_t CALIB_Data[2];
extern uint8_t HighFilterbufNum;
extern BitAction FilterStartFlag;

uint8_t I2C1_ReadByte(uint16_t DriverAddr, uint16_t ReadAddr);							//指定地址读取一个字节
void I2C1_WriteByte(uint16_t DriverAddr, uint16_t WriteAddr, uint8_t DataToWrite);		//指定地址写入一个字节
void I2C1_WriteNBytes(uint16_t DriverAddr, uint16_t WriteAddr, uint16_t NumToWrite, const uint8_t *pBuffer);   	//从指定地址开始读出指定长度的数据
void I2C1_WNBytesMul3T(uint16_t DriverAddr, uint16_t WriteAddr, uint16_t NumToWrite, const uint8_t *pBuffer);   	//从指定地址开始读出指定长度的数据
void I2C1_ReadNBytes(uint16_t DriverAddr, uint16_t ReadAddr, uint16_t NumToRead, uint8_t *pBuffer);					//从指定地址开始写入指定长度的数据

void AT24CXX_Init(void);   //初始化IIC
void Switch_Fiter(uint8_t value);
void EEProm_Init(void);
void Rd_Dev_Param(void);

#endif


