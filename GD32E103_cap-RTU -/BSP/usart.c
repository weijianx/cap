#include "bsp.h"
#include "stdarg.h"
#include <rtthread.h>
//void TIM7_IRQHandler(void);
void Timer1_Init(uint8_t uint8_tBaudRate);

uint8_t UART1_RXBuff[MAX_USART1_DATA_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uint8_t UART1_RXBuffLen = 0;

uint8_t UART1_TXBUFF[MAX_USART1_DATA_LEN];


uint8_t uint8_tSendIndex = 0;
uint8_t uint8_tSendNum = 0;
//extern OS_TCB  AppTaskMBTCB;
uint8_t g_cyCommMode;   //from Can.c

extern uint8_t g_cyCommMode;


BitAction UartRecvNewData = Bit_RESET;
BitAction UartRecvFrameOK = Bit_RESET;
/* �����ź������ƿ� */
rt_sem_t modbus_sem = RT_NULL;

//**************************************************************************************************
// ����         : UART1_Init����
// ��������     : 2016-09-05
// ����         : ׯ��Ⱥ
// ����         : ���ڳ�ʼ��
// �������     : pUartStructure                    �������ݽṹ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : 
// �޸�����     : ���ա������ն����贫����Э�顱�޸Ĳ����ʺ�У��λ����    2016.09.09
//**************************************************************************************************
void UART1_Init(UartCommTypeDef *pUartStructure)
{
    //GPIO�˿�����
//    GPIO_InitTypeDef GPIO_InitStructure;
//    USART_InitTypeDef USART_InitStructure;
       /* ����һ���ź��� */
	modbus_sem = rt_sem_create("modbus_sem",/* �ź������� */
                     1,     /* �ź�����ʼֵ��Ĭ����һ���ź��� */
                     RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
	
    UartRecvNewData = Bit_RESET;
    UartRecvFrameOK = Bit_RESET;

    UART1_RXBuffLen = 0;

    uint8_tSendIndex = 0;
    uint8_tSendNum = 0;

      /* enable USART0 receive interrupt */
	usart_interrupt_disable(USART0, USART_INT_RBNE);

	/* enable USART0 transmit interrupt */
	usart_interrupt_disable(USART0, USART_INT_TBE);
	
	usart_disable(USART0);
//    NVIC_UARTConfiguration();
	nvic_irq_enable(USART0_IRQn, 0, 0);

	usart_deinit(USART0);

    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* connect port to USARTx_Tx */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    /* connect port to USARTx_Rx */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
	
	/* connect port to USARTx_Crtl*/
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
	
    //USART ��ʼ������
    switch (pUartStructure->BaudRate)
    {
	case 0:
		usart_baudrate_set(USART0, 4800);//2400;
		break;
    case 1:
        usart_baudrate_set(USART0, 9600);//2400;
        break;
    case 2:
        usart_baudrate_set(USART0, 19200);//4800;
        break;                                                             
    case 3:
        usart_baudrate_set(USART0, 38400);//9600;
        break;
    case 4:
        usart_baudrate_set(USART0, 57600);//19200;
        break;
    case 5:
        usart_baudrate_set(USART0, 115200);//38400;
        break;
//    case 6:
//        usart_baudrate_set(USART0, 57600);
//        break;
//    case 7:
//        usart_baudrate_set(USART0, 115200);
        break;
    default:  
        break;
    }

//    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 1��ֹͣλ
	usart_stop_bit_set(USART0, USART_STB_1BIT);

    switch (pUartStructure->Parity)
    {
    case 1:
	    usart_parity_config(USART0, USART_PM_ODD);
	    usart_word_length_set(USART0, USART_WL_9BIT);
        break;
    case 2:
	    usart_parity_config(USART0, USART_PM_EVEN);
	    usart_word_length_set(USART0, USART_WL_9BIT);
        break;                                                               
    case 3:
	    usart_parity_config(USART0, USART_PM_NONE);
	    usart_word_length_set(USART0, USART_WL_8BIT);
        break;
    default:
        break;
    }

//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);

//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

//    USART_Init(USART1, &USART_InitStructure);   //��ʼ������
    //USART_ClearFlag(USART1, USART_FLAG_TC);
    //USART_ClearFlag(USART1, USART_ISR_TXE);
    usart_flag_clear( USART0,  USART_FLAG_TC );
    TX_OFF;

//    BSP_IntVectSet(BSP_INT_ID_USART1, UART1_IRQService);

//    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //�����ж�

//    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);

	Timer1_Init(pUartStructure->BaudRate);

}


void Delayms(uint16_t ms)
{
	uint16_t i,j;
	uint8_t k;
	for(i=0;i<ms;i++)
		for(j=0;j<0x0919;j++) k++;
}
//**************************************************************************************************
// ����         : USART0_IRQHandler
// ��������     : 2016-09-05
// ����         : ׯ��Ⱥ
// ����         : �����жϷ������
// �������     : ��                   �������ݽṹ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : 
// �޸�����     : 
//**************************************************************************************************
void USART0_IRQHandler(void)                	
{
    uint8_t UartReadTemp;
    
//    OSIntEnter();
    
//    if (RESET != USART_GetITStatus(USART1, USART_IT_TC))
//    {
//        USART_ClearITPendingBit(USART1, USART_IT_TC);

//        if (uint8_tSendIndex >= uint8_tSendNum)
//        {
//            uint8_tSendNum = 0;
//            USART_ITConfig(USART1, USART_IT_TC, DISABLE);
//            TX_OFF;
//        }
//        else
//        {
//            USART_SendData(USART1, UART1_TXBUFF[uint8_tSendIndex]);
//            uint8_tSendIndex++;
//        }
//    }
    if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TC))
    {
       usart_interrupt_flag_clear(USART0,USART_INT_FLAG_TC);
		
        if (uint8_tSendIndex >= uint8_tSendNum)
        {
            uint8_tSendNum = 0;
//            USART_ITConfig(USART1, USART_IT_TC, DISABLE);
			usart_interrupt_disable(USART0,USART_INT_TC);
            TX_OFF;
        }
        else
        {
//			Delayms(2);
			usart_data_transmit(USART0, UART1_TXBUFF[uint8_tSendIndex]);
            uint8_tSendIndex++;
			
        }
    }
    //===============UART Recv===============//
    if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
    {
//        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        usart_interrupt_flag_clear(USART0,USART_INT_FLAG_RBNE);
//        UartReadTemp = USART_ReceiveData(USART1);
        UartReadTemp =(uint8_t)usart_data_receive(USART0);
//		Delayms(2);
		
		if (UartRecvFrameOK == Bit_SET )
		{
			rt_memset(UART1_RXBuff, 0, MAX_USART1_DATA_LEN);
			UART1_RXBuffLen = 0;
		}

		if (UART1_RXBuffLen < (sizeof(UART1_RXBuff) / sizeof(UART1_RXBuff[0])))
		{
			UART1_RXBuff[UART1_RXBuffLen] = UartReadTemp;
			UART1_RXBuffLen++;
		}

		UartRecvFrameOK = Bit_RESET;

		UartRecvNewData = Bit_SET;

		timer_interrupt_disable(TIMER1, TIMER_INT_UP);
		timer_disable(TIMER1);
		timer_counter_value_config(TIMER1,0);
		timer_enable(TIMER1);
		timer_interrupt_enable(TIMER1, TIMER_INT_UP);

    }

	 if (usart_flag_get( USART0, USART_FLAG_ORERR ) == SET)
	 {
		 usart_interrupt_flag_clear(USART0,USART_INT_FLAG_RBNE_ORERR);
		 usart_data_receive(USART0);
	 }
//    if (RESET != USART_GetFlagStatus(USART1, USART_IT_ORE))
//    {
//        USART_ClearITPendingBit(USART1, USART_IT_ORE);
//    }  
//    OSIntExit();
}




//**************************************************************************************************
// ����         : TIMER2_IRQHandler()
// ��������     : 2016-09-05
// ����         : ׯ��Ⱥ
// ����         : TIM2�жϷ������
// �������     : ��                   �������ݽṹ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : 
// �޸�����     : 
//**************************************************************************************************
void TIMER1_IRQHandler(void)
{
//    OS_ERR  os_err;

//    OSIntEnter();
    
    if (SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_FLAG_UP))
    {
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);

        if (Bit_SET == UartRecvNewData)
        {
            UartRecvNewData = Bit_RESET;
            UartRecvFrameOK = Bit_SET;

//            TIM_ITConfig(TIM7, TIM_IT_Update, DISABLE);
            timer_interrupt_disable(TIMER1, TIMER_INT_UP);
//            TIM_Cmd(TIM7, DISABLE);
			timer_disable(TIMER1);
			g_cyCommMode = RS485_MODE;  //add frank cl

//            OSTaskSemPost((OS_TCB *)&AppTaskMBTCB, (OS_OPT) OS_OPT_POST_NONE,
//                          (OS_ERR *)&os_err);   
            rt_sem_release(modbus_sem);   //�ͷŶ�ֵ�ź��� 			
        }
        else
        {
//            TIM_ITConfig(TIM7, TIM_IT_Update, DISABLE);
//            TIM_Cmd(TIM7, DISABLE);
			timer_interrupt_disable(TIMER1, TIMER_INT_FLAG_UP);
			timer_disable(TIMER1);
        }
    }
    
//    OSIntExit();
}




//**************************************************************************************************
// ����         : Timer7_Init()
// ��������     : 2016-09-05
// ����         : ׯ��Ⱥ
// ����         : TIM7��ʼ��
// �������     : uint8_t uint8_tBaudRate      ���ڲ�����
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : 
// �޸�����     : 
//**************************************************************************************************
void Timer1_Init(uint8_t uint8_tBaudRate)
{

#ifdef STM32F072
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;

//    //ʹ�ܶ�ʱ����ʱ��
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

//    //��λTIM2��ʱ����ʹ֮�����ʼ״̬
//    TIM_DeInit(TIM7);

//    //�ȹرն�ʱ���жϣ����յ�һ���ַ��ٴ�
//    TIM_Cmd(TIM7, DISABLE);

//    NVIC_Timer7Configuration();

//    //��ʱ������ģʽ�������˳���Ƶ����
//    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//    //��ʱ������ģʽ  ����/����/���ĶԳƼ���
//    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//    //��ʱ������

//    if (uint8_tBaudRate > 2)
//    {
//        TIM_TimeBaseInitStruct.TIM_Period = 1749;
//    }
//    else
//    {
//        TIM_TimeBaseInitStruct.TIM_Period = 6999 / (uint8_tBaudRate + 1);
//    }

//    //��ʱ��Ԥ��Ƶ����
//    //��ʱʱ�� T=(TIM_Prescaler+1)*(TIM_Period+1)/TIMxCLK
//    TIM_TimeBaseInitStruct.TIM_Prescaler = 47;

//    //��ʼ��TIM2��ʱ��
//    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseInitStruct);

//    //��ʼ�������λ������Ч���������һ�£�����һ���жϣ����Ͻ����ж�
//    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

//    BSP_IntVectSet(BSP_INT_ID_TIM7, TIM7_IRQHandler);

//    //ʹ�ܸ����ж�
//    TIM_ITConfig(TIM7, TIM_IT_Update, DISABLE);

//    TIM_Cmd(TIM7, DISABLE);

//    TIM_SetCounter(TIM7, 0);

#else
    timer_parameter_struct timer_initpara;

    /* enable the peripherals clock */
    rcu_periph_clock_enable(RCU_TIMER1);

    /* deinit a TIMER */
    timer_deinit(TIMER1);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER2 configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
//    timer_initpara.period            = 9999;
	if (uint8_tBaudRate > 2)
    {
        timer_initpara.period  = 1749;
    }
    else
    {
        timer_initpara.period  = 6999 / (uint8_tBaudRate + 1);
    }
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER1, &timer_initpara);
	
    //NVIC Config
    nvic_irq_enable(TIMER1_IRQn, 0, 1);
    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(TIMER1, TIMER_INT_FLAG_UP);
    /* enable the TIMER interrupt */
    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    /* enable a TIMER */
    timer_disable(TIMER1);
	timer_counter_value_config( TIMER1, 0 );

#endif
}



//**************************************************************************************************
// ����         : UART1_SendData()
// ��������     : 2016-09-05
// ����         : ׯ��Ⱥ
// ����         : ���ڷ�������
// �������     : uint8_t* UART1_SendBuff      �������ݻ���
//                uint32_t Len                 �������ݳ���     
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : 
// �޸�����     : 
//**************************************************************************************************
uint32_t UART1_SendData(uint8_t* UART1_SendBuff, uint32_t Len)
{
    uint32_t i = 0;

    if ((0 == Len) || (((uint8_t*)0) == UART1_SendBuff))
        return 0;

    if (uint8_tSendNum != 0)
    {
        return 0;
    }

    if (Len > (sizeof(UART1_TXBUFF) / sizeof(UART1_TXBUFF[0])))
    {
        Len = (sizeof(UART1_TXBUFF) / sizeof(UART1_TXBUFF[0]));
    }

    for (i = 0; i < Len; i++)
    {
        UART1_TXBUFF[i] = UART1_SendBuff[i];
    }

    TX_ON;
    
    usart_data_transmit(USART0, UART1_TXBUFF[0]);

    uint8_tSendIndex = 1;
    uint8_tSendNum = Len;
//    USART_ITConfig(USART1, USART_IT_TC, ENABLE);
    usart_interrupt_enable(USART0,USART_INT_TC);

    return(Len);
}



//**************************************************************************************************
// ����         : UART1_RecvData()
// ��������     : 2016-09-05
// ����         : ׯ��Ⱥ
// ����         : ���ڷ�������
// �������     : uint8_t* UART1_RecvBuff      �������ݻ���
//                uint32_t Len                 �������ݳ���     
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : 
// �޸�����     : 
//**************************************************************************************************
int32_t UART1_RecvData(uint8_t* UART1_RecvBuff, uint32_t Len)
{
    uint32_t i = 0;

    if ((0 == Len) || (((uint8_t*)NULL) == UART1_RecvBuff))
    {
        return 0;
    }

    if ((Bit_RESET == UartRecvFrameOK) || (0 == UART1_RXBuffLen))
    {
        return 0;
    }

    if (Len < UART1_RXBuffLen)
    {
        return -1;
    }

    Len = UART1_RXBuffLen;

    for (i = 0; i < Len; i++)
    {
        UART1_RecvBuff[i] = UART1_RXBuff[i];
    }

    UartRecvFrameOK = Bit_RESET;

    UART1_RXBuffLen = 0;

    return Len;
}


//**************************************************************************************************
// ����         : uprintf(const char *fmt,...)
// ��������     : 2016-09-08
// ����         : ÷����
// ����         : �򴮿ڷ���ָ����ʽ������
// �������     : �룺fmt,...    ��������ָ��
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   : ��usart��ӡ���ڵ����ݣ����ڵ���
// �޸�����     : 
//**************************************************************************************************
void uprintf(const char *fmt,...)
{
    int i,j; 
    va_list marker;
    char buff[50];
    va_start(marker, fmt);
    vsprintf(buff,fmt,marker);
    for(i=0; buff[i]; i++)
    {
        UART1_SendData((uint8_t*)(buff+i), 1);
        for(j=0xfff;j>0;j--);
    }
    va_end(marker);
}

int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}

