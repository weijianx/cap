#ifndef _TASK_H
#define _TASK_H


#include "rtthread.h"


void APP_MB_Task(void *parameter);
void App_Filter_Task(void *parameter);
void App_MEMSAndTemp_Task(void *parameter);


typedef struct 
{
	//动态创建线程时使用的线程参数结构体
	char *name;
    void (*entry)(void *parameter);
    void       *parameter;
    rt_uint32_t stack_size;
	rt_uint8_t  priority;
	rt_uint32_t tick;
}TaskStruct;

void task_Init(void);

#endif


