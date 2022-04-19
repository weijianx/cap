/**
* @file         task.c
* @brief        rt-thread�߳�Ӧ��
* @details      �����̡߳�ִ������
* @author       weijianx
* @date         2020-07-02
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/07/02  <td>1.0.0    <td>weijianx  <td>������ʼ�汾
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
*                               ����
******************************************************************
*/
/* �����������ʱ���ƿ� */
rt_timer_t LED_Short_Timer = RT_NULL;
rt_timer_t LED_Double_Timer = RT_NULL;


static rt_thread_t dynamic_thread = RT_NULL;//��̬�߳̿��ƿ�ָ��


/*************************************************************************
*                             �߳̽ṹ������(�ʺ��̶߳�̬����)
*************************************************************************
*/
TaskStruct TaskThreads[] = {

//			{"AppThread",  APP_MB_Task,   RT_NULL,  256,  0, 1},

//			{"Filter_Thread",  App_Filter_Task,   RT_NULL,  256,  2, 1},
	
			{"App_MEMSAndTemp_Thread", App_MEMSAndTemp_Task,  RT_NULL,  256,  2, 1},

	
			/*********************************************************/
			//�ı�ջ�Ĵ�С���ɸı��߳�������Ҳ��ͨ���ı� rtconfig.h �е� RT_MAIN_THREAD_STACK_SIZE �� FINSH_THREAD_STACK_SIZE ��С�����ı��߳�������
			//�û�����̲߳���
			//���磺{�߳�����,�߳���ں���,�߳���ں�������,�߳�ջ��С,�̵߳����ȼ�,�߳�ʱ��Ƭ},
			
			
			
			{"",RT_NULL, RT_NULL,RT_NULL,RT_NULL,RT_NULL}
	
};

void App_CreateObject(void);


/**
 *@brief      ��ʼ������
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
			 
				dynamic_thread = rt_thread_create(TaskThreads[TaskThreadIndex].name,       // �߳����� 
                                 TaskThreads[TaskThreadIndex].entry,                       // �߳���ں��� 
                                 TaskThreads[TaskThreadIndex].parameter,                   // �߳���ں�������
                                 TaskThreads[TaskThreadIndex].stack_size,                  // �߳�ջ��С 
                                 TaskThreads[TaskThreadIndex].priority,                    // �̵߳����ȼ� 
                                 TaskThreads[TaskThreadIndex].tick                         // �߳�ʱ��Ƭ
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
	/* ����һ�������ʱ�� */
	LED_Short_Timer = rt_timer_create("LED_Short_Light_Timer", 						/* �����ʱ�������� */
                        LED_Short_Light,										/* �����ʱ���Ļص����� */
                        RT_NULL,												/* ��ʱ����ʱ��������ڲ��� */
                        10,   													/* �����ʱ���ĳ�ʱʱ��(���ڻص�ʱ��) */
                        RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);		/* �����ʱ��ģʽ һ��ģʽ */
																				
//  /* ������ʱ�� */
//  if (LED_Short_Timer != RT_NULL) 
//      rt_timer_start(LED_Short_Timer);
  
     /* ����һ�������ʱ�� */
	LED_Double_Timer = rt_timer_create("LED_long_Light_Timer", 						/* �����ʱ�������� */
                        LED_Double_Light,										/* �����ʱ���Ļص����� */
                        (void  *)ulLedStatue,									/* ��ʱ����ʱ��������ڲ��� */
                        10,   													/* �����ʱ���ĳ�ʱʱ��(���ڻص�ʱ��) */
                        RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);		/* �����ʱ��ģʽ һ��ģʽ */
																			
//  /* ������ʱ�� */
//  if (LED_Double_Timer != RT_NULL) 
//      rt_timer_start(LED_Double_Timer);


}





