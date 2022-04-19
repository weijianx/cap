/*************************************************************************************************
                                            gdfmop.h
                            This file is the FLASH operator header file
                        Edited by Xieyuyin 2019-09-28 Soway technology Co., Ltd
*************************************************************************************************/
#ifndef __GDFMOP_H
#define __GDFMOP_H
//Include files
#include "Includes.h"

/*************************************************************************************************
Chip flash started: 0x08000000
Boot occupy falsh:  12K Bytes
Applicatioon falsh: 0x08003000
Back Company infor: 0x08003000 ~ 0x080031FF (512 Bytes)
Back parameter inf: 0x08003200 ~ 0x080037FF

Company infor user: 0x08003800 ~ 0x080039FF (512 Bytes)
Parameter inf user: 0x08003A00 ~ 0x08003FFF
Application start:  0x08004000
*************************************************************************************************/


//Define 128k flash operate macro
	#define SOWAYAPP_FLASH_PAGES        112		//128 - 16 = 112
	#define SOWAYFLASH_PAGE_SIZE        1024
	#define SOWAYFLASH_SRCPARA_ADDR     0x08003000  //Backup parameter 2K bytes
	#define SOWAYFLASH_OP_BASE_ADDR     0x08003800  //System parameter 2K bytes
	#define MAGNET_PARAMETER_ADDR       0x08003A00	//Magnet parameter address
	#define SOWAYAPP_START_ADDR         0x08004000  //Soway application start address
	#define CHIPFLASH_MAXIMUM_ADDR      0x0801FFFF  //According chip type modify
	#define CHIPFLASH_EDGE_ADDR         0X08020000
//Define GD32F103CBT6 Mcro ok
 

#ifdef GD32F103C8T6
//Define 64k flash operate macro
	#define SOWAYAPP_FLASH_PAGES        48			//64 - 16 = 48
	#define SOWAYFLASH_PAGE_SIZE        1024
	#define SOWAYFLASH_SRCPARA_ADDR     0x08003000  //Backup parameter 2K bytes
	#define SOWAYFLASH_OP_BASE_ADDR     0x08003800  //System parameter 2K bytes
	#define MAGNET_PARAMETER_ADDR       0x08003A00	//Magnet parameter address
	#define SOWAYAPP_START_ADDR         0x08004000  //Soway application start address
	#define CHIPFLASH_MAXIMUM_ADDR      0x0800FFFF  //According chip type modify
	#define CHIPFLASH_EDGE_ADDR         0X08010000
//Define GD32F103CBT6 Mcro ok
#endif

//Functions declare
bool GDFLASHOP_EraseSpecifiedFlashPages( u32 startaddr, u16 npages );
bool GDFLASHOP_WriteData2SpecifiedFlash( u32 startaddr, u32 *psrc, u16 wlen );
bool GDFLASHOP_ReadDataFromChipFlash( u8* pdest, u32 op_addr, u32 dwbufsize, u32 dwlen );

#endif
