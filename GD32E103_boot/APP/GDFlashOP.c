/*************************************************************************************************
                                            GDFLASHOP.C
                           This file is the GD FLASH operator C source file
                        Edited by Xieyuyin 2019-09-28 Soway technology Co., Ltd
*************************************************************************************************/
//Include files
#include "gdfmop.h"

//Functions implemnet
/***************************************************************************************
Name:   
Func:   
Para:   
Retn:   
***************************************************************************************/
bool GDFLASHOP_EraseSpecifiedFlashPages( u32 startaddr, u16 npages )
{
    if (!npages)
        return false;

    if (startaddr + npages * SOWAYFLASH_PAGE_SIZE > CHIPFLASH_EDGE_ADDR)
        return false;
    
//	__disable_irq();
    fmc_unlock();
	fmc_flag_clear(FMC_FLAG_END);
    fmc_flag_clear(FMC_FLAG_WPERR);
    fmc_flag_clear(FMC_FLAG_PGERR);
	
    while (npages --)
    {
#ifdef USE_IDWDG
            FEED_I_WATCHDOG();
#endif
        if (fmc_page_erase( startaddr ) !=  FMC_READY)
            return false;
        startaddr += SOWAYFLASH_PAGE_SIZE;
    }
	fmc_unlock();
	fmc_flag_clear(FMC_FLAG_END);
    fmc_flag_clear(FMC_FLAG_WPERR);
    fmc_flag_clear(FMC_FLAG_PGERR);
    fmc_lock();
//    __enable_irq();
    
    return true;
}



/***************************************************************************************
Name:   
Func:   
Para:   
Retn:   
***************************************************************************************/
bool GDFLASHOP_WriteData2SpecifiedFlash( u32 startaddr, u32 *psrc, u16 wdlen )
{
	if ((psrc == NULL) || (!wdlen))
        return false;
    
    if (startaddr + wdlen * 4 > CHIPFLASH_MAXIMUM_ADDR)
        return false;
    
     fmc_unlock();
    __disable_irq();
    while (wdlen --)
    {
        if ( fmc_word_program( startaddr, *psrc ) != FMC_READY)
            return false;
        
        psrc ++;
        startaddr += 4;
    }
    __enable_irq();
    fmc_lock();
    
    return true;
}




/***************************************************************************************
Name:   
Func:   
Para:   
Retn:   
***************************************************************************************/
bool GDFLASHOP_ReadDataFromChipFlash( u8* pdest, u32 op_addr, u32 dwbufsize, u32 dwlen )
{
	u32 val;
    
    if ((!pdest) || (dwbufsize < dwlen))
        return false;
    
    if (op_addr < SOWAYFLASH_OP_BASE_ADDR)
        return false;
    
    while (dwlen --)
    {
        val = *(u32*)op_addr;
        *pdest ++ = (u8)val;
        *pdest ++ = (u8)(val >> 8);
        *pdest ++ = (u8)(val >> 16);
        *pdest ++ = (u8)(val >> 24);
        op_addr += 4;
    }
    return true;
}

