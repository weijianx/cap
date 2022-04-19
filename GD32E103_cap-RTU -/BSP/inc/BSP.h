#ifndef _BSP_H_
#define _BSP_H_

#include "gd32e10x.h"
#include "stdlib.h"
#include "math.h"

#include "systick.h"

//#include "delay.h"
#include "usart.h"
#include "led.h"
#include "Capacitive.h"
#include "SoftwareIIC.h"
#include "AT24Cxx.h"
#include "DA.h"
#include "Filter.h"

#include "modbus_asc.h"
#include "PCap.h"
//#include "SoftSpi.h"
#include "type.h"
#include "algorithm.h"
#include "sys_cfg.h"
#include "MMA8652.h"
#include "MEMS.h"
//#include "Modbus.h"
#include  <string.h>
//#include  <includes.h>
#include <rthw.h>
#include <rtthread.h>

#include "GD_FlashApp.h"

#include "ds18b20.h"

#include "task.h"

///* 看门狗开关 */
//#define IWDG_EN  0

///*ID长度选择，旧的设备ID是12位的，新的设备ID是11位的
//则使用11位ID。*/
//#define DEFAULT_DEV_ID_LEN		11

///*倾角传感器开关*/
//#define MMA8652 0

extern rt_timer_t LED_Short_Timer;
extern rt_timer_t LED_Double_Timer;

extern UartCommTypeDef UartComm;
extern RunVarTypeDef RunVar;
extern ProductParaTypeDef ProductPara;
extern UserParamTypeDef UserParam;
extern uint8_t Cur_Param[PRO_DEFAULT_LEN];

//void Dev_Init();

#endif
