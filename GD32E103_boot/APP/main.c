/*!
    \file    main.c
    \brief   GPIO running led

    \version 2021-03-23, V2.0.0, demo for GD32F30x
*/

/*
    Copyright (c) 2021, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x.h"
#include "gd32f303c_eval.h"
#include "systick.h"
#include "gdfmop.h"
#include "Dmaconfig.h"
#include "GD32F303_Systick.h"


//Datatype define
typedef void (*pFunction) (void);
typedef struct{
    __IO u8  start;  //Upgrade start flag
    __IO u8  up_ok;  //Upgrade succeed flag
    __IO u16 pkgsn;  //Current package sn
    __IO u16 pkgnum; //Total package number
    __IO u32 chksum; //Local data checksum
    __IO u32 orgchk; //Orignical data checksum
}_UPGRADE_PARA;

//Macro define
//Define upgrade command
#define FUNC41_UPGRADE  0x41

#define CMD_UPGRADE_START   0x0001
#define CMD_UPGRADE_CLRCODE 0x0002
#define CMD_UPGRADE_PAYLOAD 0x0003
#define CMD_UPGRADE_RUNNEW  0x0004

#define CMD_UPGRADE_CHGPARA 0x00FF
#define CMD_UPGRADE_RD_PARA 0x0100
#define CMD_UPGRADE_CLRPARA 0x0101

#define UPGRADE_FALG    0xFF

#define	MARSK_ARM_STACKTOP_CHK		0x2FFE0000
#define	MARSK_ARM_STACK_VALUE		0x20000000
#define	MARSK_GD32F303_FWDRST 		0x20000000
#define	MARSK_GD32F303_CLRALLRSTFLG	0x01000000

//Variables define
pFunction Jump_To_Application;

static u8 flg_boot1s_ot = 0;	//Boot overtime 1s flag
static S_COMM_PARA commpara = { 0x41, 0x03, 0x03, 0x02 };	//
/*Baudrate code
1:2400      2:4800
3:9600      4:19200
5:38400     6:57600
7:115200    
{ 0x01, 0x07, 0x03, 0x01 };
Parity:
1:Odd
2:Even
3:None

Modbus datatype
1:RTU
2:ASCII
*/

#define UPLOADDATA_LEN 512
//Receive data cycle buffer
#define MODBUS_RCVBUFSIZE		2048
static u8 mdbus_rcvbuf[MODBUS_RCVBUFSIZE];
//Send data parameter
#define MODBUS_SNDBUFSIZE		1088
static u8 mdbus_sndbuf[MODBUS_SNDBUFSIZE];
//Parse data parameter
#define MODBUS_PARSEBUFSIZE		1088
static u8 mdbus_parsebuf[MODBUS_PARSEBUFSIZE];
//Temparary data buffer
#define TMPBUF_SIZE 2048
#define DWTMPBUF_SIZE   (TMPBUF_SIZE >> 2)
static u8 gtmpbuf[TMPBUF_SIZE];

static _UPGRADE_PARA s_upp;
static __IO u32 destflashaddr;


//Local functions declare
static void GD32F303BOOT_ResourceInit( void );
static void GD32F303_TestLedInit( void );
static bool CB_DisplayLedBlink( void* pdat );

static void Boot_ParseReceiveModbusData( u8* psrc, u32 rcvlen, u8 msgtype );

static bool GD32F303BOOT_1sOverTimePorcess( void* pdat );
static bool GD32F303BOOT_ClearAppFlashArea( void );
static bool GD32F303BOOT_UpGradePayloadProcess( u8* psrc, u16 datlen );
static void GD32F303BOOT_UpgradePackagePayloadDecoding( u8* psrc, u16 srclen );
static void GD32F303BOOT_EraseUpgradeFlag( void );
static void JumpToUserApplication( void );


//Implement functions
/*******************************************************************************
Name:   
Func:   
Para:   
Retn:	
*******************************************************************************/
#ifdef USE_FDWGD
static void GD32F303_FDWDGInit( void )
{
	rcu_osci_on(RCU_IRC40K);
    while(SUCCESS != rcu_osci_stab_wait(RCU_IRC40K));
	
	/*	FWDG clock is independent lsi 40KHz, DIV128 IS 312.5Hz, that is 3.2 ms per clock
	reload value is 1000, delay time 2500 * 3.2ms = 8 s	*/
	fwdgt_config( 2500, FWDGT_PSC_DIV256 );
	fwdgt_enable();

}
#endif



/*******************************************************************************
Name:   
Func:   
Para:   
Retn:	
*******************************************************************************/
int main( void )
{	
	if (RCU_RSTSCK & MARSK_GD32F303_FWDRST)
	{
		RCU_RSTSCK |= MARSK_GD32F303_CLRALLRSTFLG;
		if (((*(__IO uint32_t*)SOWAYAPP_START_ADDR) & MARSK_ARM_STACKTOP_CHK ) == MARSK_ARM_STACK_VALUE) //Check stack top
            JumpToUserApplication();
	}
	
	GD32F303BOOT_ResourceInit();

	//Poll system start, never return
	while (1)
	{
#ifdef USE_FDWGD
		GD32F303_FEEDWATCHDOG();
#endif
		GD32F303_Systick_TimerProcess( );
	}
}



/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
#define	TEST_SRC_CLK	RCU_GPIOA
#define	TEST_LED_PORT	GPIOA
#define	TEST_LED_PIN	GPIO_PIN_3

static void GD32F303_TestLedInit( void )
{
	rcu_periph_clock_enable( TEST_SRC_CLK );
	gpio_init( TEST_LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, TEST_LED_PIN );
	GPIO_BC( TEST_LED_PORT ) = TEST_LED_PIN;
	GPIO_BOP( GPIOA ) = TEST_LED_PIN;
}



/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
static void GD32F303BOOT_ResourceInit( void )
{
	S_PARSE_TAG s_parsepara;
	S_COMM_BUFPARA usart_dma_para;
	
	nvic_priority_group_set( NVIC_PRIGROUP_PRE2_SUB2 );
	GD32F303_Systick_Config( );
	
	memset(&s_upp, 0, sizeof(_UPGRADE_PARA));
    s_upp.up_ok = 1;
	
	//DMA & Usart parameters init
	usart_dma_para.prcv =  mdbus_rcvbuf;
	usart_dma_para.rcvsize = MODBUS_RCVBUFSIZE;
	usart_dma_para.psnd = mdbus_sndbuf;
	usart_dma_para.sndsize = MODBUS_SNDBUFSIZE;
	
	//Parse parameters init
	s_parsepara.pbuf = mdbus_parsebuf;
	s_parsepara.bufsize = MODBUS_PARSEBUFSIZE;
	s_parsepara.pparse = Boot_ParseReceiveModbusData;
	
	//Usart and DMA config
	USART_CommunicateInitial( 0, &usart_dma_para, &commpara );		//0-USART0 1-USART1 2-USART2 ...
	DMA_Rcv_TransDataParaConfig( &usart_dma_para );
	USART_ConfigPaserParameters( &s_parsepara );
	
	//Systick test segment
	GD32F303_TestLedInit( );
	GD32F303_TimerSet( eTmr_LedBlinkTimer, CB_DisplayLedBlink, NULL, 1000 );
	
#ifdef USE_FDWGD
	GD32F303_FDWDGInit( );
#endif
	
	//Config BootLoader 1S wait
	GD32F303_TimerSet( eTmr_BootLoader, GD32F303BOOT_1sOverTimePorcess, NULL, 1000  );
}


/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
static bool ledsta = false;
static bool CB_DisplayLedBlink( void* pdat )
{	
	if (!ledsta)
	{
		GPIO_BOP( GPIOA ) = TEST_LED_PIN;
	}
	else
	{
		GPIO_BOP( GPIOA ) = TEST_LED_PIN;
	}
	
	ledsta = !(ledsta);
	return true;
}



/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
static void Boot_ParseReceiveModbusData( u8* psrc, u32 rcvlen, u8 msgtype )
{
    u8* ptmp;
    u16 cmdid;
    u16 datlen;
    
    if ((!psrc) || (!rcvlen))
        return;
	
	if (!flg_boot1s_ot)
    {
        flg_boot1s_ot = 0;
        GD32F303_Systick_TimerKill( eTmr_BootLoader );
    }
	
    ptmp = psrc;
    if (*psrc != FUNC41_UPGRADE)
        return;
    psrc ++;

    //Get command id
    COMMON_Bits8Convert2Bits16( &cmdid, psrc, BIGENDDIAN );
    psrc += 2;
    
    //Get data length
    COMMON_Bits8Convert2Bits16( &datlen, psrc, BIGENDDIAN );
    psrc += 2;

    switch (cmdid)
    {
    case CMD_UPGRADE_START:
        *(ptmp + 3) = 0x00;
        GD32F10X_Modbus_UpgradeResponseProcess( ptmp, 1 );
        break;

    case CMD_UPGRADE_CLRCODE:   //Clear flash code area
        memset(&s_upp, 0, sizeof(_UPGRADE_PARA));
        s_upp.start = 1;
        destflashaddr = SOWAYAPP_START_ADDR;
        GD32F303BOOT_ClearAppFlashArea();
        *(ptmp + 3) = 0x00;
        GD32F10X_Modbus_UpgradeResponseProcess( ptmp, 1 );
        break;

    case CMD_UPGRADE_PAYLOAD:
        if (s_upp.start && GD32F303BOOT_UpGradePayloadProcess(psrc, datlen))
        {
            *(ptmp + 3) = 0x00;
            GD32F10X_Modbus_UpgradeResponseProcess( ptmp, 1 );
        }
        else
        {
            *(ptmp + 3) = 0x01;
             GD32F10X_Modbus_UpgradeResponseProcess( ptmp, 1 );
        }
        break;

    case CMD_UPGRADE_RUNNEW:
        if (s_upp.up_ok)
        {
            *(ptmp + 3) = 0x00;
            GD32F10X_Modbus_UpgradeResponseProcess( ptmp, 1 );
            while (USART_SendDataViaUSART0DmaChnIsBusy());
            GD32F303BOOT_EraseUpgradeFlag();
			JumpToUserApplication();
        }
        else
        {
            *(ptmp + 3) = 0x01;
             GD32F10X_Modbus_UpgradeResponseProcess( ptmp, 1 );
        }
        break;

    case CMD_UPGRADE_CHGPARA:
    {
        *(ptmp + 3) = 0x00;
        GD32F10X_Modbus_UpgradeResponseProcess( ptmp, 1 );
        commpara.BaudRate = 7;
        commpara.ModbusType = 1;
        commpara.Parity = 3;
        while ( USART_SendDataViaUSART0DmaChnIsBusy());
        USART_CommunicateParaConfig( 0, &commpara );
        break;
    }
    
    case CMD_UPGRADE_RD_PARA:
    {
        GDFLASHOP_ReadDataFromChipFlash( (ptmp + 3), MAGNET_PARAMETER_ADDR, UPLOADDATA_LEN / 4, UPLOADDATA_LEN / 4 );
        GD32F10X_Modbus_UpgradeResponseProcess( ptmp, UPLOADDATA_LEN );
        break;
    }
    
    case CMD_UPGRADE_CLRPARA:
    {
        if (GDFLASHOP_EraseSpecifiedFlashPages( SOWAYFLASH_OP_BASE_ADDR, 1 ))
        {
            *(ptmp + 3) = 0x00;
            GD32F10X_Modbus_UpgradeResponseProcess( ptmp, 1 );
        }
        else
        {
            *(ptmp + 3) = 0x01;
            GD32F10X_Modbus_UpgradeResponseProcess( ptmp, 1 );
        }
        break;
    }
    }//end switch
}


/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
static bool GD32F303BOOT_1sOverTimePorcess( void* pdat )
{
    u8 usrsetp[8];
    
    GDFLASHOP_ReadDataFromChipFlash( usrsetp, MAGNET_PARAMETER_ADDR, 2, 2 );
    if (usrsetp[1] == UPGRADE_FALG)
    {
        if (!usrsetp[2] || usrsetp[2] == 0xFF)
        {
            commpara.SlaveAddr = 0x41;
        }
        else
        {
            commpara.SlaveAddr = usrsetp[2];
        }
        
        if (usrsetp[3] > 7)
        {
            commpara.BaudRate = 3;  //default: 9600 bps
        }
        else
        {
            commpara.BaudRate = usrsetp[3];
        }
        
        if (usrsetp[4] > 3)
        {
            commpara.Parity = 3;    //3: None
        }
        else
        {
            commpara.Parity = usrsetp[4];
        }
        
        if (usrsetp[5] > 2)
        {
            commpara.ModbusType = 2;
        }
        else
        {
            commpara.ModbusType = usrsetp[5];
        }
        
        USART_CommunicateParaConfig( 0, &commpara );
    }
    else
    {
        if (((*(__IO uint32_t*)SOWAYAPP_START_ADDR) & MARSK_ARM_STACKTOP_CHK ) == MARSK_ARM_STACK_VALUE)
        {
			JumpToUserApplication();
        }
            
    }
    
    return false;
}



/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
static bool GD32F303BOOT_ClearAppFlashArea( void )
{
	bool ret = true;

    if (!GDFLASHOP_EraseSpecifiedFlashPages( SOWAYAPP_START_ADDR, SOWAYAPP_FLASH_PAGES ))
	{
        ret = false;
	}
    return ret;
}



/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
static bool GD32F303BOOT_UpGradePayloadProcess( u8* psrc, u16 datlen )
{
    u8* p;
    u16 i;
    u16 sn;
    u16 total;
    
    if ((!psrc) || (datlen < 4))
        return false;
    
    //Get total package
    COMMON_Bits8Convert2Bits16( &total, psrc, BIGENDDIAN );
    psrc += 2;
    
    //Get current sn
    COMMON_Bits8Convert2Bits16( &sn, psrc, BIGENDDIAN );
    psrc += 2;
    
    datlen -= 4;    //package para 2 + 2 bytes
    if (s_upp.start == 1)
    {
        s_upp.start = 2;
        s_upp.pkgnum = total;
        COMMON_Bits8Convert2Bits32( (u32*)(&s_upp.orgchk), psrc, BIGENDDIAN );
        return true;
    }
    else if (s_upp.pkgnum != total)
    {
        return false;
    }
    else if ((s_upp.pkgsn + 1) != sn)
    {
        return false;
    }

    //Process upgrade payload
    s_upp.pkgsn = sn;
    
    //Caculate checksum
    p = psrc;
    i = datlen;
    while (i --)
    {
        s_upp.chksum += *p ++;
    }
    
    //Decoding source data
    GD32F303BOOT_UpgradePackagePayloadDecoding( psrc, datlen );
    //Write data to flash
    memcpy(gtmpbuf, psrc, datlen);
    
    if (!GDFLASHOP_WriteData2SpecifiedFlash( destflashaddr, (u32*)gtmpbuf, datlen/4 ))
        return false;
    else
        destflashaddr += datlen;
    
    if (s_upp.pkgnum == s_upp.pkgsn + 1)
    {
        if (s_upp.orgchk == s_upp.chksum)
        {
            memset(&s_upp, 0, sizeof(_UPGRADE_PARA));
            s_upp.up_ok = 1;
            GD32F303BOOT_EraseUpgradeFlag();
        }   
        else
            return false;
     }
     
     return true;
}



/***************************************************************************************
Name:   
Func:   
Para:   
Retn:  
***************************************************************************************/
const u8 enstr[] = "ShenZhen Soway Science and Technology Development Co., Ltd.    ";
static void GD32F303BOOT_UpgradePackagePayloadDecoding( u8* psrc, u16 srclen )
{
    u16 cnt;
    
    cnt = 0;
    while (srclen --)
    {
        (*psrc) ^= (enstr[cnt] + 11);
        psrc ++;
        cnt ++;
        cnt %= sizeof(enstr);
    }
}


/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
#define UPGRADE_FLAG_OFFSET 513
static void GD32F303BOOT_EraseUpgradeFlag( void )    //Erase upgrade flag
{
    memset( gtmpbuf, 0, TMPBUF_SIZE );
    GDFLASHOP_ReadDataFromChipFlash( gtmpbuf, SOWAYFLASH_OP_BASE_ADDR, DWTMPBUF_SIZE, DWTMPBUF_SIZE );
    if (gtmpbuf[UPGRADE_FLAG_OFFSET] == UPGRADE_FALG)
    {
        gtmpbuf[UPGRADE_FLAG_OFFSET] = 0x00;
        GDFLASHOP_EraseSpecifiedFlashPages( SOWAYFLASH_OP_BASE_ADDR, 1 );
        GDFLASHOP_WriteData2SpecifiedFlash( SOWAYFLASH_OP_BASE_ADDR, (u32*)gtmpbuf, DWTMPBUF_SIZE);
    }
}



/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
static void JumpToUserApplication( void )
{    
	usart_disable(USART0);
	dma_channel_disable(DMA0, DMA_CH3);
	dma_channel_disable(DMA0, DMA_CH4);
	
    /* Jump to user application */
    destflashaddr = *(__IO u32*) (SOWAYAPP_START_ADDR + 4);
    Jump_To_Application = (pFunction)destflashaddr;

    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) SOWAYAPP_START_ADDR);

    /* Jump to application */
    Jump_To_Application();
}




