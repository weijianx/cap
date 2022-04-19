#ifndef __TYPE_H
#define __TYPE_H

#include "gd32e10x.h"
#include "Sys_cfg.h"

#define ECHO_ENABLE 	        0                                               //冻结
#define ECHO_DISABLE 	        1                                               //解冻

#define FLASH_WR_ENABLE         1                                               //Flash写使能
#define FLASH_WR_DISABLE        0                                               //Flash写禁止


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
  uint16_t DACode[8];                                                                //DA码
  uint16_t Scale[8];                                                                 //百分度
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
  Def_State = 0x00,                                                             //出厂标定状态
  CalibrateLow = 0x01,                                                          //标定零点
  CalibrateHeight = 0x02,                                                       //标定满度
  Calibrated = 0x03,                                                            //标定完成
}CalState_TypeDef;



typedef struct
{
  uint8_t FilterLevel;                                                               //滤波等级
  uint8_t FilterBufMax;                                                              //滤波缓存最大值
  uint8_t FilterN;                                                                   //液位高度下限位（低于该值认为小波动，数据可更新）
  uint8_t FilterM;                                                                   //液位高度上限位（高于该值认为大波动，数据可更新）
  uint16_t FilterCycle;                                                              //滤波周期
  uint32_t HFil[10];                                                                 //高阶滤波数组
  uint32_t HFilBak[10];                                                              //高阶滤波数组备份
  uint32_t LFil[96];                                                                 //低阶滤波数组
  uint32_t LFilBak[96];                                                              //低阶滤波数组备份        
  uint32_t Pressure_Filter;                                                              //滤波后的电容值
  float UserFlowK;                                                             //用户系数K
  float UserFlowB;                                                             //用户系数B
  
}P_UserParamTypeDef;


/* PressureDataConvert做数据转换需要的参数结构 */
typedef struct {
    uint8_t CompenEn;                   //补偿使能
    uint16_t HeightRange;               //高度量程
    uint16_t PressureADMin;                  //压力AD值零点
    uint16_t PressureADLow;                  //压力AD值下刻度
    uint16_t PressureADHigh;                 //压力AD值上刻度
    uint16_t PressureADMax;                  //压力AD值满量程
    uint16_t PressureDAMin;                  //压力DA值零点
    uint16_t PressureDALow;                  //压力DA值下刻度
    uint16_t PressureDAHigh;                 //压力DA值上刻度
    uint16_t PressureDAMax;                  //压力DA值满量程
    uint32_t PressureMin;                    //压力零点
    uint32_t PressureMax;                    //压力满量程
    uint32_t PressureMinBak;                 //压力零点
    uint32_t PressureMaxBak;                 //压力满量程
    float Correct_K;                    //压力修正系数K
    float Correct_B;                    //压力修正系数B
}PressureDataConvert_Param;

/* PCap转换后输出数据的结构 */
typedef struct {
   
    uint32_t OilQuantity;                                                              //油箱油量          L
    uint32_t LiquidHeight;                                                             //液位高度值        0.1mm
    uint16_t LiquidHeightAD;                                                           //液位高度AD值     0-65535
    uint16_t LiquidPercent;                                                            //液位百分比       0-1000（0-1乘以1000）
    
    
    uint16_t PressureDA_ResultValue;        //PressureDA原始输出值
    uint16_t PressureDA_OutValue;           //PressureDA转换输出值
    uint32_t Pressure_ResultValue;          //Pressure原始值
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
//  uint8_t OilInit;                                                                   //油量初始化标志
//  uint8_t AccStatus;                                                                 //ACC状态
//  uint16_t CarSpeed;                                                                 //汽车速度
//  uint16_t FiltSpeed;                                                                //滤波速度
//  uint16_t LowSpeedCount;                                                            //持续低速计数
//  uint16_t DAForFloater;                                                             //油浮DA值
//  uint32_t CarMileage;                                                               //里程
//  uint32_t CapFromPCap;                                                              //电容
//  uint32_t RawCap;                                                                   //原始电容
//  uint32_t RawCapBak;                                                                //原始电容备份

//  uint32_t OilQuantity;                                                              //邮箱油量
//  uint32_t LiquidHeight;                                                             //液位高度值mm
//  uint16_t LiquidAD;                                                                 //液位高度AD
//  uint16_t LiquidPercent;                                                            //液位百分比
//  float TempInSensor;                                                           //液体温度
//  float TempInAir;                                                              //环境温度
//  float HeightBak;
//  
//  CalState_TypeDef CalState;                                                    //标定状态结构体参数
////  OS_TICK uartInitTime;                                                         //Uart初始化时间
//  BitAction bInitializing;
//  
//  uint32_t Pvd_LiquidHeight;                                                      //断电瞬间的液位高度
//  uint32_t Pvd_OilQuantity;                                                       //断电瞬间的油量
//  uint8_t  Pvd_Flag;       //PVD标志位
//}RunVarTypeDef;

typedef struct
{
  uint8_t FilterLevel;                                                               //滤波等级
  uint8_t FilterBufMax;                                                              //滤波缓存最大值
  uint8_t FilterN;                                                                   //液位高度下限位（低于该值认为小波动，数据可更新）
  uint8_t FilterM;                                                                   //液位高度上限位（高于该值认为大波动，数据可更新）
  uint16_t FilterCycle;                                                              //滤波周期
  uint32_t HFil[10];                                                                 //高阶滤波数组
  uint32_t HFilBak[10];                                                              //高阶滤波数组备份
  uint32_t LFil[96];                                                                 //低阶滤波数组
  uint32_t LFilBak[96];                                                              //低阶滤波数组备份        
  uint32_t PCap_Filter;                                                              //滤波后的电容值
  float UserFlowK;                                                             //用户系数K
  float UserFlowB;                                                             //用户系数B
}UserParamTypeDef;



typedef struct
{
    BitAction FilterStart;                                                      //每秒滤波开始标志                                                           
    uint8_t Ms100_Cycle;                                                             //每1s获得电容次数计数                        
    int32_t EverySecCap;                                                            //每秒的电容值                
    uint32_t FilArray[10];                                                           //每秒电容滤波数组
}EverySecFilTypeDef;

#endif

