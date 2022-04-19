#include "BSP.h"



#define AD_GROUP_MAX 40
#define AD_CHANNEL_MAX 2
#define AD_DELETE_NUM  10

#define AD_FILTER_MAX 30
#define AD_FILTER_DELETE_NUM 10

/****/
ANGLE_TypeDef Angle;

__IO uint16_t  usADC1ConvertedValue[AD_GROUP_MAX][AD_CHANNEL_MAX];
uint16_t  usADC1Value[AD_CHANNEL_MAX][AD_GROUP_MAX];

//extern u32 g_cyTankOil;

/* 定义信号量控制块 */
rt_sem_t mems_sem = RT_NULL;


typedef struct
{
    unsigned long   FilterIndex;
    unsigned short  usFilterBuf[AD_FILTER_MAX];
}struct_ADCFilter;

struct_ADCFilter  ADC1Filter[AD_CHANNEL_MAX];

unsigned short  usFilterBuf[AD_FILTER_MAX];

//static void ADC_Config(void);
//static void DMA_Config(void);


/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{

    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);  
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);
//    /* enable TIMER0 clock */
//    rcu_periph_clock_enable(RCU_TIMER0);
    /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* config the GPIO as analog mode */  
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);

}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
//     gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
    /* ADC_DMA_channel deinit */
    dma_deinit(DMA0, DMA_CH0);
    
    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr  = (uint32_t)(&usADC1ConvertedValue);
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number       = AD_GROUP_MAX * AD_CHANNEL_MAX;
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;  
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);
  
    dma_circulation_enable(DMA0, DMA_CH0);
  
    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);  
//}

///*!
//    \brief      configure the ADC peripheral
//    \param[in]  none
//    \param[out] none
//    \retval     none
//*/
//void adc_config(void)
//{
    /* ADC scan mode function enable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    /* ADC trigger config */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_EXTTRIG_REGULAR_T0_CH0);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* configure the ADC sync mode */
    adc_mode_config(ADC_DAUL_REGULAL_PARALLEL);  
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 2);
  
    /* ADC regular channel config */
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_6, ADC_SAMPLETIME_239POINT5);
	adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_7, ADC_SAMPLETIME_239POINT5);
  
    /* ADC external trigger enable */
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);

   
    /* enable ADC interface */
    adc_enable(ADC0);
//    GD32F103_Systick_Delayxms(1);    
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
    /* enable ADC interface */
    /* ADC DMA function enable */
    adc_dma_mode_enable(ADC0);
}


void Temperature_Init(void)
{
    rcu_config();
    gpio_config();
	dma_config();
	
}

void InitTmpQueue(struct_ADCFilter *pADCDF, unsigned short uidata)
{
	unsigned long i;
	
    for(i = 0; i < AD_FILTER_MAX; i++)
    {
        pADCDF->usFilterBuf[i] = uidata;
    }
}



void EXTI10_15_IRQHandler(void)
{
//    OS_ERR  Err;
//    OSIntEnter();   
	
    if(RESET != exti_interrupt_flag_get(EXTI_12))
    {
        exti_interrupt_flag_clear(EXTI_12);

//        OSTaskSemPost(&AppMEMSTCB, OS_OPT_POST_NONE, &Err);
		rt_sem_release(mems_sem);   //释放二值信号量 
    }
    else if(RESET != exti_interrupt_flag_get(EXTI_10))
    {
        exti_interrupt_flag_clear(EXTI_10);
        PCap_Ready = Bit_SET;
    }

//    OSIntExit();  
}

void MMA8652_INTN_Init(void)
{
	
	mems_sem = rt_sem_create("mems_sem",			/* 信号量名字 */
							 1,     				/* 信号量初始值，默认有一个信号量 */
							 RT_IPC_FLAG_FIFO); 	/* 信号量模式 FIFO(0x00)*/
		 
	    /* enable the key clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_AF);

    /* configure button pin as input */
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_12);

   
	/* enable and set key EXTI interrupt to the lowest priority */
	nvic_irq_enable(EXTI10_15_IRQn, 1U, 3U);

	/* connect key EXTI line to key GPIO pin */
	gpio_exti_source_select(GPIO_PORT_SOURCE_GPIOB, GPIO_PIN_SOURCE_12);

	/* configure key EXTI line */
	exti_init(EXTI_12, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
	exti_interrupt_flag_clear(EXTI_12);
    
}


void MMA8652_Init(void)
{
//	CPU_SR_ALLOC();

//	CPU_CRITICAL_ENTER();

	MMA8652_INTN_Init();
	
	MMA8652_GPIO_Config();     
	
	MMA8652_Setting();
	
//	CPU_CRITICAL_EXIT();
}

int red_temp, PreTemp;
uint8_t i ;
void App_MEMSAndTemp_Task (void *parameter)
{

	rt_err_t result=0;
    	BitAction bTempInitate;
//	OS_ERR Err;
	float ftemp;
	unsigned short usTemp;
//	long i, j;
    
#if MMA8652
	ACCEL_TypeDef Accel;
#endif

    bTempInitate = Bit_RESET;
	
#if MMA8652	
	MMA8652_Init();
#endif

//	Temperature_Init();
//	ADC1Filter[0].FilterIndex = 0;
//    ADC1Filter[1].FilterIndex = 0;
	
	DS18B20_Init();
	
	while(1)
	{
		
		PreTemp = DS18B20_Get_Temp();                                               //每秒读一次温度值
		
		if(i < 3)
		{
			i++;
			red_temp = PreTemp;
		}
		
		if((-1000000 > (PreTemp - red_temp)) || ((PreTemp - red_temp) > 1000000))
		{
			DS18B20_Init();                                                         //出现故障时复位一下总线
			
			fwdgt_counter_reload();
			return;
		}
		else
		{
			red_temp = PreTemp;
		}

		if(PreTemp == -27310000)                                                    //初始化18B20不成功,上电会读个85度上来，去掉    
		{
			DS18B20_Init();                                                         //出现故障时复位一下总线
			
			return;
		}
		
		if((PreTemp < -4000000) || (PreTemp > 11000000))                            //温度不在-30~110范围内丢掉
		{
			return;
		}

		RunVar.temp = PreTemp;                                                //影响读数据的准确性,所以在接收数据的过程中读取的数据丢掉
		
		rt_thread_delay(1000);
	
//#if IWDG_EN > 0
////		fwdgt_counter_reload();
//#endif	

//				
//#if MMA8652
//				
//		result=rt_sem_take(mems_sem,	/* 获取信号量 */
//                1000); 	/* 等待时间：100ms */

//		if(result == RT_EOK)
//		{
//			MMA8652_Init();
//		}
//		Multiple_read_MMA8652(&Accel);
//		
//		getAngle(&Accel, &Angle);
//#endif		

//		/******* Temperature *******/		
////		CPU_SR_ALLOC();
////        CPU_CRITICAL_ENTER();
//        for(i = 0; i < AD_GROUP_MAX; i++)
//        {
//            for(j = 0; j < AD_CHANNEL_MAX; j++)
//            {
//			    usADC1Value[j][i]=usADC1ConvertedValue[i][j];
//            }
//        }
////        CPU_CRITICAL_EXIT();
//        
//        usTemp = GetDelExtremeAndAverage(usADC1Value[0], AD_GROUP_MAX, AD_DELETE_NUM, AD_DELETE_NUM);

//        if(Bit_SET == bTempInitate)
//        {
//            InitTmpQueue(&ADC1Filter[0], usTemp);
//        }
//        
//        ADC1Filter[0].usFilterBuf[ADC1Filter[0].FilterIndex] = usTemp;
//        if (++ADC1Filter[0].FilterIndex >= AD_FILTER_MAX)
//            ADC1Filter[0].FilterIndex = 0;

//        for (j = 0; j < AD_FILTER_MAX; j++)
//        {
//            usFilterBuf[j] = ADC1Filter[0].usFilterBuf[j];
//        }

//        ftemp = GetDelExtremeAndAverage(usFilterBuf, AD_FILTER_MAX, AD_FILTER_DELETE_NUM, AD_FILTER_DELETE_NUM);

//        ftemp = ftemp * 3.3 / 4095;
//        ftemp = (ftemp - 0.5) * 100;
//        
//        RunVar.TempInSensor = ftemp * ProductPara.Temp_K + ProductPara.Temp_B;


//        usTemp = GetDelExtremeAndAverage(usADC1Value[1], AD_GROUP_MAX, AD_DELETE_NUM, AD_DELETE_NUM);

//        if(Bit_SET == bTempInitate)
//        {
//            InitTmpQueue(&ADC1Filter[1], usTemp);
//        }
//        
//        ADC1Filter[1].usFilterBuf[ADC1Filter[1].FilterIndex] = usTemp;
//        if (++ADC1Filter[1].FilterIndex >= AD_FILTER_MAX)
//            ADC1Filter[1].FilterIndex = 0;

//        for (j = 0; j < AD_FILTER_MAX; j++)
//        {
//            usFilterBuf[j] = ADC1Filter[1].usFilterBuf[j];
//        }

//        ftemp = GetDelExtremeAndAverage(usFilterBuf, AD_FILTER_MAX, AD_FILTER_DELETE_NUM, AD_FILTER_DELETE_NUM);

//        ftemp = ftemp * 3.3 / 4095;
//        ftemp = (ftemp - 0.5) * 100;
//        RunVar.TempInAir= ftemp * ProductPara.Temp2_K + ProductPara.Temp2_B;

//        bTempInitate = Bit_SET;
	}
}




