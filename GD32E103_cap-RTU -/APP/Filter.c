#include "BSP.h"


#define BufLen 960
float DivideValue = 10;
#define DelLen (UserParam.FilterCycle/3)
uint8_t HighFilterbufNum = 0;
uint32_t HighFilterbuf[10] = {0};    //高滤波系数均值缓存区
uint32_t AdcBuf[BufLen] = {0};       //队列从大到小
uint16_t Index[BufLen] = {0};        //索引为0的就是要删除的元素
uint32_t SoomthBuf[10] = {0};        //滑动均值缓冲区
uint32_t MonotonyDataBuf[10] = {0};  //研究采样值单调性缓存区
float DalidValueZone = 0;
BitAction FilterStartFlag = Bit_RESET;

typedef enum
{
    NeitherInNorOut = 0x00,
    JustIn = 0x01,
    JustOut = 0x02,
    InAlsoOut = 0x03,
    FirstIn = 0x04,
    FirstOut = 0x08,
    Finish = 0x0C,
}ElementState_TypeDef;


/* 定义信号量控制块 */
rt_sem_t filter_sem = RT_NULL;

void TIMER2_IRQHandler(void)
{
//    OS_ERR Err;
//    OSIntEnter();   
    
    if (timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP))
    {
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);
       
          
        if(PCap_Res_Stau() & 0x100000)                                          //检查PCap的采集状态
        {
            RunVar.RawCap = PCap_Res_Value();                                   //读取采集PCap值
            PCap_MEASURE();                                                     //设置PCap模式        
            
//            OSTaskSemPost(&AppFilterTCB, OS_OPT_POST_NONE, &Err);               //发送消息到任务
			rt_sem_release(filter_sem);   //释放二值信号量 
        }
	}
//    OSIntExit();   
}

timer_parameter_struct timer_initpara;
void Timer2_Init(void)
{
	filter_sem = rt_sem_create("filter_sem",/* 信号量名字 */
				 1,     /* 信号量初始值，默认有一个信号量 */
				 RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
	
//	timer_oc_parameter_struct timer_ocintpara;

    rcu_periph_clock_enable(RCU_TIMER2);

    timer_deinit(TIMER2);

    /* TIMER configuration */
    timer_initpara.prescaler         = 7199;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);

    /* CH0 configuration in PWM mode1 */
//    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_LOW;
//    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
//    timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);

//    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 100);
//    timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM1);
//    timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

	nvic_irq_enable(TIMER2_IRQn, 0, 2);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
//    timer_primary_output_config(TIMER0, ENABLE);
	
	timer_interrupt_enable(TIMER2, TIMER_INT_UP);
	timer_enable(TIMER2);
}

void Pcap_INTN(void)
{

	    /* enable the clock of peripherals */
    rcu_periph_clock_enable(RCU_GPIOB);
//	rcu_periph_clock_enable(RCU_AF);
	/* once enabled the DAC, the corresponding GPIO pin is connected to the DAC converter automatically */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);	
	
	    /* enable the CFGCMP clock */
//    rcu_periph_clock_enable(RCU_CFGCMP);
    /* enable and set key EXTI interrupt to the specified priority */
    nvic_irq_enable(EXTI10_15_IRQn, 1U, 3);

    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_10);

    /* configure key EXTI line */
    exti_init(EXTI_10, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_flag_clear(EXTI_10);
}

void Capture_TimerInit(void)
{
    Timer2_Init();
}




uint16_t CalcuFloaterDACode(uint16_t usRate, FloaterCalibTypeDef *pFcal)
{
	uint32_t i = 0;
    uint32_t head, tail;
    uint32_t u32Temp;
    //find the first num that larger than usRate
    for(i = 0; i < 8; i++)
    {
        if(usRate <= pFcal->Scale[i])
            break;
    }

    if(i == 0)  // the first data is larger than usRate
    {
        return pFcal->DACode[0];
    }
    else if(i == 8) //no any data can find
    {
        head = 6;
        tail = 7;
    }
    else
    {
        head = i - 1;
        tail = i;
    }

    /*
       usRate - pFcal->Scale[head]      pFcal->Scale[tail]  - pFcal->Scale[head]
      ---------------------------- = -------------------------------------------
        x - pFcal->DACode[head]         pFcal->DACode[tail] - pFcal->DACode[head]
       */

    u32Temp = (usRate - pFcal->Scale[head]);
    u32Temp *= (pFcal->DACode[tail] - pFcal->DACode[head]);
    u32Temp /= (pFcal->Scale[tail]  - pFcal->Scale[head]);
    u32Temp += pFcal->DACode[head];

    return((uint16_t)u32Temp);
}

BitAction IsNeetResetPcap(void)
{
    //static unsigned char ucHeightEqualCnt = 0;
    static unsigned char ucRawCapEqualCnt = 0;
    BitAction bErrHappen = Bit_RESET;

    if(RunVar.RawCapBak == 0xffffffff)
    {
        RunVar.RawCapBak = RunVar.RawCap + 1;
    }

    if(RunVar.RawCapBak != RunVar.RawCap)
    {
        ucRawCapEqualCnt = 0;
    }
    else
    {
        ucRawCapEqualCnt += 1;
    }

    RunVar.RawCapBak = RunVar.RawCap;

    if(ucRawCapEqualCnt >= 10)
    {
        ucRawCapEqualCnt = 0;
        bErrHappen = Bit_SET;
    }

    return bErrHappen;
}


#if 0

#define ONESECONDSAMCNT  10
#define DAFILTERSECCNT  20

short sSamQuePerSec[ONESECONDSAMCNT];
short sDAQueInSec[DAFILTERSECCNT];

unsigned short DAOutPutFilter(short sDACIn)
{
    static BitAction bInitFilter = Bit_RESET;
    static unsigned short PerSecondIndex;
    static unsigned short SecondIndex;
    static short OutputValue;
    static short Outputbak;
    
    long slTemp;
    unsigned long i = 0;

    //initate filter queue, do only one time
    if(Bit_RESET == bInitFilter)
    {
        bInitFilter = Bit_SET;
        
        PerSecondIndex = 0;
        for(i = 0; i < ONESECONDSAMCNT; i++)
        {
            sSamQuePerSec[i] = sDACIn;
        }
        
        SecondIndex = 0;
        for(i = 0; i < DAFILTERSECCNT; i++)
        {
            sDAQueInSec[i] = sDACIn;
        }

        OutputValue = sDACIn;
        Outputbak = sDACIn;
    }

    //when liquid height is change largly, reset the filter ,let the da output follow the situation.
    
    //put new data in the queue
    sSamQuePerSec[PerSecondIndex++] = sDACIn;
    
    if(PerSecondIndex >= ONESECONDSAMCNT)
    {
        PerSecondIndex = 0;
        
        slTemp = 0;
        for(i = 0; i < ONESECONDSAMCNT; i++)
        {
            slTemp += sSamQuePerSec[i];
        }
        slTemp /= ONESECONDSAMCNT;

        sDAQueInSec[SecondIndex++] = slTemp;
        if(SecondIndex >= DAFILTERSECCNT)
            SecondIndex = 0;

        slTemp = 0;
        for(i = 0; i < DAFILTERSECCNT; i++)
        {
            slTemp += sDAQueInSec[i];
        }
        slTemp /= DAFILTERSECCNT;
        
        OutputValue = slTemp;

        if(abs(Outputbak - OutputValue) > 3)
        {
            Outputbak = OutputValue;
        }
    }
    
    return (unsigned short)Outputbak;
}

#endif

#define ADTIMEINTERVAL 100

uint16_t DAOutPutStabilised(uint16_t usRate, uint16_t usDACOutput)
{
    static uint16_t usBaseRate = 0xffff;
    static uint16_t usDABak;
    static uint8_t  ucDirBak = 0;     //0表示相等，1表示变小，2表示增大
    static uint16_t usLargeThan5PercCnt = 0;
    static uint16_t usLargeThan1PercCnt = 0;

    uint16_t usRateDelta;
    uint8_t ucDir;

    //第一次运行的时候获取一个油量比率
    if (usBaseRate > 101)    //由于usRate是浮点转换过来，有误差，所以增加1
    {
        usBaseRate = usRate;
        usDABak = usDACOutput;
        ucDirBak = 0;
        usLargeThan5PercCnt = 0;
        usLargeThan1PercCnt = 0;
        
        return usDABak;
    }

    if(usBaseRate == usRate)
    {
        ucDirBak = 0;
        usLargeThan5PercCnt = 0;
        usLargeThan1PercCnt = 0;
        
        return usDABak;
    }
    else if(usBaseRate > usRate)
    {
        ucDir = 1;
        usRateDelta = usBaseRate - usRate;
    }
    else
    {
        ucDir = 2;
        usRateDelta = usRate - usBaseRate;
    }

    //当方向改变，不会改变输出，也不会改变判断的基准
    if(ucDir != ucDirBak)
    {
        ucDirBak = ucDir;
        usLargeThan5PercCnt = 0;
        usLargeThan1PercCnt = 0;
        
        return usDABak;
    }
    else
    {
        usLargeThan5PercCnt = (usRateDelta > 5) ? (usLargeThan5PercCnt + 1) : 0;

        usLargeThan1PercCnt = (usRateDelta > 1) ? (usLargeThan1PercCnt + 1) : 0;

        if((usLargeThan1PercCnt > (5000/ADTIMEINTERVAL)) || (usLargeThan5PercCnt > (6000/ADTIMEINTERVAL)))
        {
            usBaseRate = usRate;
            usDABak = usDACOutput;
            //ucDirBak = 0;
            usLargeThan5PercCnt = 0;
            usLargeThan1PercCnt = 0;
            
            return usDACOutput;
        }
        else
        {
            return usDABak;
        }
    }
}



//#define BufLen 960
//float DivideValue = 10;
//#define DelLen (UserParam.FilterCycle/3)
//uint8_t HighFilterbufNum = 0;
//uint32_t HighFilterbuf[10] = {0};    //高滤波系数均值缓存区
//uint32_t AdcBuf[BufLen] = {0};       //队列从大到小
//uint16_t Index[BufLen] = {0};        //索引为0的就是要删除的元素
//uint32_t SoomthBuf[10] = {0};        //滑动均值缓冲区
//uint32_t MonotonyDataBuf[10] = {0};  //研究采样值单调性缓存区
//float DalidValueZone = 0;
//BitAction FilterStartFlag = Bit_RESET;

//typedef enum
//{
//    NeitherInNorOut = 0x00,
//    JustIn = 0x01,
//    JustOut = 0x02,
//    InAlsoOut = 0x03,
//    FirstIn = 0x04,
//    FirstOut = 0x08,
//    Finish = 0x0C,
//}ElementState_TypeDef;


float CapFilter(void)
{
    uint8_t RestartFlag = 0;
	uint8_t HighFilterbufCount = 0;
	uint8_t SoomthBufCount = 0;
	uint8_t MonotonyDataBufCount = 0;
    uint8_t UpTrend = 0;
	uint8_t DownTrend = 0;
    ElementState_TypeDef ElementState = NeitherInNorOut;
    uint8_t elementstate;
    uint16_t i, InPosition;
    static float ftemp = 0.0;
    uint32_t CapBuf[2] = {0};
	uint32_t InsertCap;
	uint32_t SumBuf = 0;
    
    RunVar.CapFromPCap = RunVar.RawCap;

    if(RunVar.CapFromPCap > ProductPara.CapMax)
        RunVar.CapFromPCap = ProductPara.CapMax;
    else if(RunVar.CapFromPCap < ProductPara.CapMin)
        RunVar.CapFromPCap = ProductPara.CapMin;

    if(0 == UserParam.FilterLevel)
    {
        FilterStartFlag = Bit_RESET;
        ftemp = RunVar.CapFromPCap;
    }
    else
    {
        if(Bit_RESET == FilterStartFlag)
        {
            FilterStartFlag = Bit_SET;
			HighFilterbufCount = 0;
			SoomthBufCount = 0;
			MonotonyDataBufCount = 0;
            HighFilterbuf[HighFilterbufCount++] = RunVar.CapFromPCap;
            SoomthBuf[SoomthBufCount++] = RunVar.CapFromPCap;
            MonotonyDataBuf[MonotonyDataBufCount++] = RunVar.CapFromPCap;

            for(i = 0; i < UserParam.FilterCycle; i++)
//			for(i = 0; i < 120; i++)
			{
				AdcBuf[i] = RunVar.CapFromPCap;
				Index[i] = i;
			}
			
        }
        else
        {
            if(SoomthBufCount < 10)
            {
                SoomthBuf[SoomthBufCount++] = RunVar.CapFromPCap;
                if(RestartFlag)
                    return ftemp;
            }
            else
            {
                SoomthBufCount = 0;
                for(i=0, SumBuf=0; i<10; i++)
                    SumBuf += SoomthBuf[i];
                MonotonyDataBuf[MonotonyDataBufCount++] = SumBuf/10;
                if(MonotonyDataBuf[(MonotonyDataBufCount+9)%10]>MonotonyDataBuf[(MonotonyDataBufCount+8)%10])
                {
                    UpTrend++;
                    DownTrend = 0;
                }
                else if(MonotonyDataBuf[(MonotonyDataBufCount+9)%10]<MonotonyDataBuf[(MonotonyDataBufCount+8)%10])
                {
                    DownTrend++;
                    UpTrend = 0;
                }
                if(MonotonyDataBufCount>=10)
                    MonotonyDataBufCount = 0;

                if( ((UpTrend>=10) && (DalidValueZone < (MonotonyDataBuf[9]-MonotonyDataBuf[0]))) ||
                    ((DownTrend>=10) && (DalidValueZone < (MonotonyDataBuf[0]-MonotonyDataBuf[9]))))
                {
                    for(i=0, SumBuf=0; i<10; i++)
                        SumBuf += MonotonyDataBuf[i];
                    ftemp = SumBuf/10;
                    RestartFlag = 1;
                    return ftemp;
                }
                else if(RestartFlag)
                {
                    RestartFlag = 0;
					UpTrend = 0;
					DownTrend = 0;
                    FilterStartFlag = Bit_RESET;
                    return ftemp;
                }
            }
            
            if(HighFilterbufCount < HighFilterbufNum)
            {
                HighFilterbuf[HighFilterbufCount++] = RunVar.CapFromPCap;
                return ftemp;
            }
            else
            {
                HighFilterbufCount = 0;
                HighFilterbuf[HighFilterbufCount++] = RunVar.CapFromPCap;
                for(i=0, SumBuf=0; i<HighFilterbufNum; i++)
                {
                    SumBuf += HighFilterbuf[i];
                }
            }
            
            ElementState = NeitherInNorOut;
            InsertCap = SumBuf/HighFilterbufNum;
            
            CapBuf[0] = InsertCap;
            CapBuf[1] = UserParam.FilterCycle-1;
            
            for(i = 0; i < UserParam.FilterCycle; i++)
//			for(i = 0; i < 120; i++)
            {
                if(Finish != (ElementState & Finish))
                {
                    if((Index[i] == 0) && (FirstOut != (ElementState & FirstOut)))
                    {
                        ElementState |= JustOut;
                    }
                    if((AdcBuf[i] <= InsertCap) && (FirstIn != (ElementState & FirstIn)))
                    {
                        InPosition = i;
                        ElementState |= JustIn;
                    }
                }
                elementstate = (ElementState & InAlsoOut);
                switch(elementstate)
                {
                    case NeitherInNorOut:
                        Index[i] = Index[i] - 1;
                        break;
                        
                    case JustIn:
                        ElementState |= FirstIn;
                        CapBuf[0] = CapBuf[0] + AdcBuf[i];
                        AdcBuf[i] = CapBuf[0] - AdcBuf[i];
                        CapBuf[0] = CapBuf[0] - AdcBuf[i];
                        
                        CapBuf[1] = CapBuf[1] + Index[i];
                        Index[i]  = CapBuf[1] - Index[i];
                        CapBuf[1] = CapBuf[1] - Index[i];

                        Index[i] = Index[i] -1;
                        if(InPosition == i)
                        {
                            AdcBuf[InPosition] = InsertCap;
                            Index[InPosition] = UserParam.FilterCycle-1;
                        }
                        break;
                        
                    case JustOut:
                        ElementState |= FirstOut;
                        if((AdcBuf[i + 1] > InsertCap) && (i < UserParam.FilterCycle-1))
                        {
                            AdcBuf[i] = AdcBuf[i+1];
                            Index[i] = Index[i+1] - 1;
                        }
                        else
                        {
                            AdcBuf[i] = InsertCap;
                            Index[i] = UserParam.FilterCycle-1;
                        }
                        break;

                    case InAlsoOut:
                        if(FirstIn == (ElementState & FirstIn))
                        {
                            CapBuf[0] = CapBuf[0] + AdcBuf[i];
                            AdcBuf[i] = CapBuf[0] - AdcBuf[i];
                            CapBuf[0] = CapBuf[0] - AdcBuf[i];
                            
                            CapBuf[1] = CapBuf[1] + Index[i];
                            Index[i]  = CapBuf[1] - Index[i];
                            CapBuf[1] = CapBuf[1] - Index[i];

                            Index[i] = Index[i] -1;
                            AdcBuf[InPosition] = InsertCap;
                            Index[InPosition] = UserParam.FilterCycle-1;
                        }
                        else if(FirstOut == (ElementState & FirstOut))
                        {
                            Index[i] = Index[i] - 1;
                        }
                        else
                        {
                            AdcBuf[i] = InsertCap;
                            Index[i] = UserParam.FilterCycle-1;
                        }
                        ElementState &= NeitherInNorOut;
                        ElementState |= Finish;
                        break;
                }
            }
        }
        
        for(i = (UserParam.FilterCycle/(uint8_t)DivideValue), SumBuf = 0 ; 
            i < UserParam.FilterCycle - (UserParam.FilterCycle/(uint8_t)DivideValue); i++)
//		for(i=12; i<108; i++)
        {
//			if(i == 12)
//				SumBuf = 0;
			
            SumBuf += AdcBuf[i];
        }
        ftemp = SumBuf / (UserParam.FilterCycle - UserParam.FilterCycle/(uint8_t)DivideValue*2);
    }
    
    return ftemp;
}



void RunVar_Init(void)
{
    RunVar.CalState = Def_State;
    RunVar.RawCapBak = 0xffffffff;
    RunVar.HeightBak = 500000.5;
}


void App_Filter_Task (void *parameter)
{
//    OS_ERR Err; 
    rt_err_t result=0;
	float fTemp;
    BitAction bNeetResetPcap = Bit_RESET;
	PCap_Init();
    Capture_TimerInit();
    DA_Init();

    while(1)
    {

#if IWDG_EN > 0
        fwdgt_counter_reload();
#endif
//        OSTaskSemPend(0, OS_OPT_PEND_BLOCKING, NULL, &Err);
		result=rt_sem_take(filter_sem,	/* 获取信号量 */
                0); 	/* 等待时间：100ms */
		
        if (result == RT_EOK)
        {
            bNeetResetPcap = IsNeetResetPcap();                                 //是否需要重启PCap
            
            if(Bit_SET == bNeetResetPcap)
            {
                bNeetResetPcap = Bit_RESET;
                resetPcap();                                                    //重启
                RunVar_Init();
                Capture_TimerInit();
                continue;
            }
                     

            fTemp = CapFilter();

            fTemp = fTemp * UserParam.UserFlowK + UserParam.UserFlowB;
            RunVar.LiquidHeightRate = (fTemp - ProductPara.CapMin)/(ProductPara.CapMax - ProductPara.CapMin);
            RunVar.LiquidHeight = (uint32_t)(RunVar.LiquidHeightRate * 65535);
            
            if(ProductPara.LiquitHeightforDAMax > 0.1)
            {
                if(RunVar.LiquidHeight >= ProductPara.LiquitHeightforDAMax)
                {
                    fTemp = 1.0;
                }
                else
                {
                    fTemp = RunVar.LiquidHeight / ProductPara.LiquitHeightforDAMax;
                }
            }
            else
            {
                fTemp = RunVar.LiquidHeightRate;
            }

            RunVar.DAForFloater = CalcuFloaterDACode((uint16_t)(fTemp * 100.0), &ProductPara.Floater);
            
            RunVar.DAForFloater = DAOutPutStabilised((uint16_t)(fTemp * 100.0),
                                                      RunVar.DAForFloater);
            
            if(Bit_RESET == DAOilDebug.bDADebugEn)
            {
                DA_Write(RunVar.DAForFloater);
            }
            if(Bit_RESET == DAOutDebug.bDADebugEn)
            {
//				DA2_Write(0x6ff);
                DA2_Write((uint16_t)(RunVar.LiquidHeightRate * (ProductPara.DAMax - ProductPara.DAMin) + ProductPara.DAMin));
            }   		
			else if(DAOutDebug.bDADebugEn == Bit_SET)
            {
                DA2_Write((unsigned short)DAOutDebug.usDACode);
            }
        }
		
    }
}

static int FilterProcessInit(void)
{
	rt_thread_t filter_thread;

	filter_thread = rt_thread_create("FilterThread",
								App_Filter_Task,
								RT_NULL,
								256,
								2,
								1);

	RT_ASSERT(filter_thread != RT_NULL);
	rt_thread_startup(filter_thread);
	
	return RT_EOK;
	
}
INIT_PREV_EXPORT(FilterProcessInit);


