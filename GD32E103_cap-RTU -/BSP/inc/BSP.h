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

///* ���Ź����� */
//#define IWDG_EN  0

///*ID����ѡ�񣬾ɵ��豸ID��12λ�ģ��µ��豸ID��11λ��
//��ʹ��11λID��*/
//#define DEFAULT_DEV_ID_LEN		11

///*��Ǵ���������*/
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
