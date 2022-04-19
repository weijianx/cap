#include "bsp.h"



void CapCalibrate(void)
{
	
	if((CalibrateLow == RunVar.CalState) || (CalibrateHeight == RunVar.CalState))
	{
		LED_Open();
		rt_timer_start(LED_Short_Timer);
	}
	else if(Calibrated == RunVar.CalState)
	{
		RunVar.CalState = Def_State;
		if(ArrToHex(&Cur_Param[CAPMIN]) < ArrToHex(&Cur_Param[CAPMAX]))
		{
			FilterStartFlag = Bit_RESET;
			I2C1_WNBytesMul3T(EEPROM_ADDRESS, CAPMIN, 8, &Cur_Param[CAPMIN]);
			
		    ProductPara.CapMin = ArrToHex(&Cur_Param[CAPMIN]);
		    ProductPara.CapMax = ArrToHex(&Cur_Param[CAPMAX]);

			LED_Open();
			ulLedStatue = 2;
		    rt_timer_start(LED_Double_Timer);
		}
		else
		{
			LED_Open();
			rt_timer_start(LED_Short_Timer);;
		}
	}
}

