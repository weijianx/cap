#include "BSP.h"

DADBG_TypeDef DAOilDebug = {Bit_RESET, 0};
DADBG_TypeDef DAOutDebug = {Bit_RESET, 0};

void DA_Init(void)
{
	    /* enable the clock of peripherals */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_DAC);
	
	/* once enabled the DAC, the corresponding GPIO pin is connected to the DAC converter automatically */
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5);
}

void DA_Write(unsigned short DACode)
{

	
    if(DACode > 4095)
        DACode = 4095;
	
	dac_deinit();
    /* configure the DAC0 */
    dac_trigger_disable(DAC0);
    dac_wave_mode_config(DAC0, DAC_WAVE_DISABLE);
    dac_output_buffer_enable(DAC0);
    
    /* enable DAC0 and set data */
    dac_enable(DAC0);
    dac_data_set(DAC0, DAC_ALIGN_12B_R, DACode);	
}


void DA2_Write(unsigned short DACode)
{

    if(DACode > 4095)
        DACode = 4095;
	
	dac_deinit();
    /* configure the DAC0 */
    dac_trigger_disable(DAC1);
    dac_wave_mode_config(DAC1, DAC_WAVE_DISABLE);
    dac_output_buffer_enable(DAC1);
    
    /* enable DAC0 and set data */
    dac_enable(DAC1);
    dac_data_set(DAC1, DAC_ALIGN_12B_R, DACode);

}

