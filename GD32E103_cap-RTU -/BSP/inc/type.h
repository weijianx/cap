#ifndef __TYPE_H
#define __TYPE_H

#include "gd32e10x.h"
#include "Sys_cfg.h"

#define ECHO_ENABLE 	        0                                               //����
#define ECHO_DISABLE 	        1                                               //�ⶳ

#define FLASH_WR_ENABLE         1                                               //Flashдʹ��
#define FLASH_WR_DISABLE        0                                               //Flashд��ֹ


typedef enum
{ 
  Bit_RESET = 0,
  Bit_SET
}BitAction;

typedef struct 
{
	uint8_t SlaveAddr;
	uint8_t BaudRate;
	uint8_t WordLength;
	uint8_t StopBits;
	uint8_t Parity;
	uint8_t OutputMode;
	uint8_t Echo;
}UartCommTypeDef;

typedef struct
{
  uint16_t DACode[8];                                                                //DA��
  uint16_t Scale[8];                                                                 //�ٷֶ�
}FloaterCalibTypeDef;

typedef struct
{
	uint32_t CapMin;
	uint32_t CapMax;
	float DAMin;
	float DAMax;
	float Def_Range;
	float Def_CapMin;
	float Def_CapMax;
	float Temp_K;
	float Temp_B;
	float Temp2_K;
	float Temp2_B;
    float LiquitHeightforDAMax;
	uint8_t AutoUpLoadTime;
	uint8_t OutputUnit;
	uint8_t bFlashWEn;
	FloaterCalibTypeDef Floater;
}ProductParaTypeDef;

typedef enum 
{
  Def_State = 0x00,                                                             //�����궨״̬
  CalibrateLow = 0x01,                                                          //�궨���
  CalibrateHeight = 0x02,                                                       //�궨����
  Calibrated = 0x03,                                                            //�궨���
}CalState_TypeDef;



typedef struct
{
  uint8_t FilterLevel;                                                               //�˲��ȼ�
  uint8_t FilterBufMax;                                                              //�˲��������ֵ
  uint8_t FilterN;                                                                   //Һλ�߶�����λ�����ڸ�ֵ��ΪС���������ݿɸ��£�
  uint8_t FilterM;                                                                   //Һλ�߶�����λ�����ڸ�ֵ��Ϊ�󲨶������ݿɸ��£�
  uint16_t FilterCycle;                                                              //�˲�����
  uint32_t HFil[10];                                                                 //�߽��˲�����
  uint32_t HFilBak[10];                                                              //�߽��˲����鱸��
  uint32_t LFil[96];                                                                 //�ͽ��˲�����
  uint32_t LFilBak[96];                                                              //�ͽ��˲����鱸��        
  uint32_t Pressure_Filter;                                                              //�˲���ĵ���ֵ
  float UserFlowK;                                                             //�û�ϵ��K
  float UserFlowB;                                                             //�û�ϵ��B
  
}P_UserParamTypeDef;


/* PressureDataConvert������ת����Ҫ�Ĳ����ṹ */
typedef struct {
    uint8_t CompenEn;                   //����ʹ��
    uint16_t HeightRange;               //�߶�����
    uint16_t PressureADMin;                  //ѹ��ADֵ���
    uint16_t PressureADLow;                  //ѹ��ADֵ�¿̶�
    uint16_t PressureADHigh;                 //ѹ��ADֵ�Ͽ̶�
    uint16_t PressureADMax;                  //ѹ��ADֵ������
    uint16_t PressureDAMin;                  //ѹ��DAֵ���
    uint16_t PressureDALow;                  //ѹ��DAֵ�¿̶�
    uint16_t PressureDAHigh;                 //ѹ��DAֵ�Ͽ̶�
    uint16_t PressureDAMax;                  //ѹ��DAֵ������
    uint32_t PressureMin;                    //ѹ�����
    uint32_t PressureMax;                    //ѹ��������
    uint32_t PressureMinBak;                 //ѹ�����
    uint32_t PressureMaxBak;                 //ѹ��������
    float Correct_K;                    //ѹ������ϵ��K
    float Correct_B;                    //ѹ������ϵ��B
}PressureDataConvert_Param;

/* PCapת����������ݵĽṹ */
typedef struct {
   
    uint32_t OilQuantity;                                                              //��������          L
    uint32_t LiquidHeight;                                                             //Һλ�߶�ֵ        0.1mm
    uint16_t LiquidHeightAD;                                                           //Һλ�߶�ADֵ     0-65535
    uint16_t LiquidPercent;                                                            //Һλ�ٷֱ�       0-1000��0-1����1000��
    
    
    uint16_t PressureDA_ResultValue;        //PressureDAԭʼ���ֵ
    uint16_t PressureDA_OutValue;           //PressureDAת�����ֵ
    uint32_t Pressure_ResultValue;          //Pressureԭʼֵ
}PressureDataConvert_Out_Param;

typedef struct
{
	uint32_t CapFromPCap;
	uint32_t RawCap;
	uint32_t RawCapBak;
	float HeightBak;
	float LiquidHeight;
	float LiquidHeightRate;
	float TempInSensor;
	float TempInAir;
	CalState_TypeDef CalState;
	uint32_t DAForFloater;
//    OS_TICK uartInitTime;
    BitAction bInitializing;
	int temp;
}RunVarTypeDef;

//typedef struct
//{
//  uint8_t OilInit;                                                                   //������ʼ����־
//  uint8_t AccStatus;                                                                 //ACC״̬
//  uint16_t CarSpeed;                                                                 //�����ٶ�
//  uint16_t FiltSpeed;                                                                //�˲��ٶ�
//  uint16_t LowSpeedCount;                                                            //�������ټ���
//  uint16_t DAForFloater;                                                             //�͸�DAֵ
//  uint32_t CarMileage;                                                               //���
//  uint32_t CapFromPCap;                                                              //����
//  uint32_t RawCap;                                                                   //ԭʼ����
//  uint32_t RawCapBak;                                                                //ԭʼ���ݱ���

//  uint32_t OilQuantity;                                                              //��������
//  uint32_t LiquidHeight;                                                             //Һλ�߶�ֵmm
//  uint16_t LiquidAD;                                                                 //Һλ�߶�AD
//  uint16_t LiquidPercent;                                                            //Һλ�ٷֱ�
//  float TempInSensor;                                                           //Һ���¶�
//  float TempInAir;                                                              //�����¶�
//  float HeightBak;
//  
//  CalState_TypeDef CalState;                                                    //�궨״̬�ṹ�����
////  OS_TICK uartInitTime;                                                         //Uart��ʼ��ʱ��
//  BitAction bInitializing;
//  
//  uint32_t Pvd_LiquidHeight;                                                      //�ϵ�˲���Һλ�߶�
//  uint32_t Pvd_OilQuantity;                                                       //�ϵ�˲�������
//  uint8_t  Pvd_Flag;       //PVD��־λ
//}RunVarTypeDef;

typedef struct
{
  uint8_t FilterLevel;                                                               //�˲��ȼ�
  uint8_t FilterBufMax;                                                              //�˲��������ֵ
  uint8_t FilterN;                                                                   //Һλ�߶�����λ�����ڸ�ֵ��ΪС���������ݿɸ��£�
  uint8_t FilterM;                                                                   //Һλ�߶�����λ�����ڸ�ֵ��Ϊ�󲨶������ݿɸ��£�
  uint16_t FilterCycle;                                                              //�˲�����
  uint32_t HFil[10];                                                                 //�߽��˲�����
  uint32_t HFilBak[10];                                                              //�߽��˲����鱸��
  uint32_t LFil[96];                                                                 //�ͽ��˲�����
  uint32_t LFilBak[96];                                                              //�ͽ��˲����鱸��        
  uint32_t PCap_Filter;                                                              //�˲���ĵ���ֵ
  float UserFlowK;                                                             //�û�ϵ��K
  float UserFlowB;                                                             //�û�ϵ��B
}UserParamTypeDef;



typedef struct
{
    BitAction FilterStart;                                                      //ÿ���˲���ʼ��־                                                           
    uint8_t Ms100_Cycle;                                                             //ÿ1s��õ��ݴ�������                        
    int32_t EverySecCap;                                                            //ÿ��ĵ���ֵ                
    uint32_t FilArray[10];                                                           //ÿ������˲�����
}EverySecFilTypeDef;

#endif

