
#include "led.h"
#include "bsp.h"

//OS_TMR TMR_LED_Short_Light;
//OS_TMR TMR_LED_Double_Light;

unsigned long ulLedStatue = 0;


void LED_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);
    /* connect port to USARTx_Tx */
    gpio_init( RCU_GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
	
    LED_Close();
}

void LED_Open(void)
{
    gpio_bit_reset(GPIOB, GPIO_PIN_0);
}

void LED_Close(void)
{
    gpio_bit_set(GPIOB, GPIO_PIN_0);
}

void LED_Short_Light(void* parameter)
{
    LED_Close();
}

void LED_Double_Light(void* parameter)
{
//    OS_ERR  Err;
    uint32_t OpenAgain = *(uint32_t *)parameter;

    if(OpenAgain >= 2)
    {
        *(uint32_t *)parameter = 1;
        LED_Close();
		rt_timer_start(LED_Double_Timer);
//        OSTmrStart(&TMR_LED_Double_Light, &Err);
    }
    else if(1 == OpenAgain)
    {
        *(uint32_t *)parameter = 0;
        LED_Open();
		rt_timer_start(LED_Double_Timer);
//        OSTmrStart(&TMR_LED_Double_Light, &Err);
    }
    else
    {
        LED_Close();
    }
}


