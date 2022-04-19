/*****************************************************************************************
*                                        modbus.h
*
*
******************************************************************************************/

#ifndef __MODBUS_H_
#define __MODBUS_H_

//#include "./config.h"
//#include "./PCap.h"
#include "bsp.h"

//modbus ���ܺ�
#define MDB_READ_HOLD_REG       		0x03 //0000 0011
#define MDB_READ_INPUT_REG         		0x04 //0000 0100
#define MDB_WRITE_SINGLE_REG     		0x06 //0000 0110
#define MDB_WRITE_MULTIPLE_REG     		0x10 //0001 0000

#define MDB_SAVE_CAL_PARAM              0x65 //ʮ����101
#define MDB_WRITE_NC_TABLE              0x66 //��������Բ�����
#define MDB_READ_NC_TABLE               0x67 //��ȡ�����Բ�����
#define MDB_RESTORE_PARAM               0x68 //�ָ���������

//modbus error���ش�����
#define MDB_NO_ERROR       				 0x0//�޴���
#define MDB_FNO_INVALID                  0x1//��֧�ֵĹ���
#define MDB_REG_ADDR_INVALID             0x2//�Ĵ�����ַ����ȷ
#define MDB_REG_VALUE_OVERRIDE           0x3//�Ĵ���ֵ������Χ
#define MDB_OP_FAIL                      0x4//����ʧ��
#define MDB_WRITE_M_ADDR_OVERRIDE        0x5//д�Ĵ�����ַ������Χ
#define MDB_FUNCNO_INVALID               0x6//���ܺ���Ч

#define MDB_SEND_LEVEL               	 0x21 //��ʾҺλ
#define MDB_RESET_ADDR         		     0x22 //�����ַ
#define MDB_RESET_ZERO     		         0x23 //�������
#define MDB_CALIB_PARAM     		     0x24 //�궨����
#define MDB_RESTORE                      0x25 //�ָ���������
#define MDB_SEND_COM_PARAM               0x26 //������ͨ����
#define MDB_SEND_CALIB_PARAM             0x27 //���ͱ궨����


/* ----------------------- Defines ------------------------------------------*/
#define MB_ADDRESS_BROADCAST    ( 0 )   /*! Modbus broadcast address. */
#define MB_ADDRESS_MIN          ( 1 )   /*! Smallest possible slave address. */
#define MB_ADDRESS_MAX          ( 247 ) /*! Biggest possible slave address. */
#define MB_FUNC_NONE                          (  0 )
#define MB_FUNC_READ_COILS                    (  1 )
#define MB_FUNC_READ_DISCRETE_INPUTS          (  2 )
#define MB_FUNC_WRITE_SINGLE_COIL             (  5 )
#define MB_FUNC_WRITE_MULTIPLE_COILS          ( 15 )
#define MB_FUNC_READ_HOLDING_REGISTER         (  3 )
#define MB_FUNC_READ_INPUT_REGISTER           (  4 )
#define MB_FUNC_WRITE_REGISTER                (  6 )
#define MB_FUNC_WRITE_MULTIPLE_REGISTERS      ( 16 )
#define MB_FUNC_READWRITE_MULTIPLE_REGISTERS  ( 23 )
#define MB_FUNC_DIAG_READ_EXCEPTION           (  7 )
#define MB_FUNC_DIAG_DIAGNOSTIC               (  8 )
#define MB_FUNC_DIAG_GET_COM_EVENT_CNT        ( 11 )
#define MB_FUNC_DIAG_GET_COM_EVENT_LOG        ( 12 )
#define MB_FUNC_OTHER_REPORT_SLAVEID          ( 17 )
#define MB_FUNC_ERROR                         ( 128 )
/* ----------------------- Err Type definitions -----------------------------*/

#define MB_EX_NONE 					(0x00)
#define MB_EX_ILLEGAL_FUNCTION 		(0x01)                                  //�Ƿ�������
#define MB_EX_ILLEGAL_DATA_ADDRESS	(0x02)                                  //�Ƿ����ݵ�ַ
#define MB_EX_ILLEGAL_DATA_VALUE	(0x03)                                  //�Ƿ�����ֵ
#define MB_EX_SLAVE_DEVICE_FAILURE	(0x04)                                  //Flashδʹ��
//#define MB_EX_ACKNOWLEDGE			(0x05)
//#define MB_EX_SLAVE_BUSY			(0x06)
#define MB_EX_MEMORY_PARITY_ERROR	(0x05)                                  //У�����
#define MB_EX_GATEWAY_PATH_FAILED	(0x0A)
#define MB_EX_GATEWAY_TGT_FAILED	(0x0B)


#define HOLDING_REG_REGION1_BGEIN		0x0030
#define HOLDING_REG_REGION1_END			0x0041
#define HOLDING_REG_REGION2_BGEIN		0X004A
#define HOLDING_REG_REGION2_END			0X004A

#define INPUT_REG_REGION1_BGEIN        	0x0000
#define INPUT_REG_REGION1_END          	0x000C
#define INPUT_REG_REGION2_BGEIN        	0x8020
#define INPUT_REG_REGION2_END          	0x802A

#define SINGLE_COIL_ADDR_BGEIN      0x0050
#define SINGLE_COIL_ADDR_END        0x0054

#define SPECICAL_COIL_ADDR_BGEIN    0x00B0
#define SPECICAL_COIL_ADDR_END      0x00B0

#define MUL_REG_REGION1_BGEIN          HOLDING_REG_REGION1_BGEIN
#define MUL_REG_REGION1_END            HOLDING_REG_REGION1_END
#define MUL_REG_REGION2_BGEIN          HOLDING_REG_REGION2_BGEIN
#define MUL_REG_REGION2_END            HOLDING_REG_REGION2_END

#define DATAPACKET_LEN  		200			 //���ݰ�����


//��������
#define UART_WAIT_TIME	100

void  APP_MB_Task(void *paremeter);
extern u8 const User_Default_Param[PRO_DEFAULT_LEN];

#endif
