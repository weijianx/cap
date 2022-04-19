/**
* @file         task.c
* @brief        rt-thread线程应用
* @details      创建线程、执行任务
* @author       weijianx
* @date         2020-07-02
* @version      V1.0.0
* @copyright    2020-2030,深圳市信为科技发展有限公司
**********************************************************************************
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/07/02  <td>1.0.0    <td>weijianx  <td>创建初始版本
* </table>
*
**********************************************************************************
*/
#include "task.h"
#include "bsp.h"
#include "string.h"
#include "stdio.h"


/*
******************************************************************
*                               变量
******************************************************************
*/
/* 定义线软件定时器制块 */
rt_timer_t LED_Short_Timer = RT_NULL;
rt_timer_t LED_Double_Timer = RT_NULL;


static rt_thread_t dynamic_thread = RT_NULL;//动态线程控制块指针


/*************************************************************************
*                             线程结构体数组(适合线程动态创建)
*************************************************************************
*/
TaskStruct TaskThreads[] = {

//			{"AppThread",  APP_MB_Task,   RT_NULL,  256,  0, 1},

//			{"Filter_Thread",  App_Filter_Task,   RT_NULL,  256,  2, 1},
	
			{"App_MEMSAndTemp_Thread", App_MEMSAndTemp_Task,  RT_NULL,  256,  2, 1},

	
			/*********************************************************/
			//改变栈的大小，可改变线程数量；也可通过改变 rtconfig.h 中的 RT_MAIN_THREAD_STACK_SIZE 或 FINSH_THREAD_STACK_SIZE 大小，来改变线程数量，
			//用户添加线程参数
			//例如：{线程名字,线程入口函数,线程入口函数参数,线程栈大小,线程的优先级,线程时间片},
			
			
			
			{"",RT_NULL, RT_NULL,RT_NULL,RT_NULL,RT_NULL}
	
};

void App_CreateObject(void);


/**
 *@brief      初始化任务
 *@para       None
 *@retval     None
 */
void task_Init(void)
{
	uint8_t TaskThreadIndex = 0;

	App_CreateObject();
	
	 while(1)
	 {
		 if(strcmp(TaskThreads[TaskThreadIndex].name,"") != 0)
		 {
			 
				dynamic_thread = rt_thread_create(TaskThreads[TaskThreadIndex].name,       // 线程名字 
                                 TaskThreads[TaskThreadIndex].entry,                       // 线程入口函数 
                                 TaskThreads[TaskThreadIndex].parameter,                   // 线程入口函数参数
                                 TaskThreads[TaskThreadIndex].stack_size,                  // 线程栈大小 
                                 TaskThreads[TaskThreadIndex].priority,                    // 线程的优先级 
                                 TaskThreads[TaskThreadIndex].tick                         // 线程时间片
                   	            );
				if(dynamic_thread != RT_NULL)
				{
					rt_thread_startup(dynamic_thread);
				}
			  TaskThreadIndex ++;
		 }
		 else
			 break;
		 
	 }
}

void App_CreateObject(void)
{
	/* 创建一个软件定时器 */
	LED_Short_Timer = rt_timer_create("LED_Short_Light_Timer", 						/* 软件定时器的名称 */
                        LED_Short_Light,										/* 软件定时器的回调函数 */
                        RT_NULL,												/* 定时器超时函数的入口参数 */
                        10,   													/* 软件定时器的超时时间(周期回调时间) */
                        RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);		/* 软件定时器模式 一次模式 */
																				
//  /* 启动定时器 */
//  if (LED_Short_Timer != RT_NULL) 
//      rt_timer_start(LED_Short_Timer);
  
     /* 创建一个软件定时器 */
	LED_Double_Timer = rt_timer_create("LED_long_Light_Timer", 						/* 软件定时器的名称 */
                        LED_Double_Light,										/* 软件定时器的回调函数 */
                        (void  *)ulLedStatue,									/* 定时器超时函数的入口参数 */
                        10,   													/* 软件定时器的超时时间(周期回调时间) */
                        RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);		/* 软件定时器模式 一次模式 */
																			
//  /* 启动定时器 */
//  if (LED_Double_Timer != RT_NULL) 
//      rt_timer_start(LED_Double_Timer);


}





