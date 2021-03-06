
//Include files
#include "Includes.h"

//Common functions implement
/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
void COMMON_DelayXus( u32 dly )
{
    s32 cnt;
    
    cnt = dly * 8;
    while (cnt > 0)
    {
        cnt --;
        cnt --;
    }
}


/*******************************************************************************
Name:   
Func:   
Para:   u16 pointer, u8 pointer, data formate:0-little enddian  1-big enddian
Retn:   
*******************************************************************************/
bool COMMON_Bits8Convert2Bits16( u16 *pdest, u8 *psrc, u8 mode )
{
    if ((!pdest) ||(!psrc))
        return false;
    
    if (mode)   //big enddian mode
    {
        *pdest = (*psrc ++) << 8;
        *pdest |= *psrc;
    }
    else
    {
        *pdest = (*psrc ++);
        *pdest |= (*psrc) << 8;
    }
    
    return true;
}

/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
bool COMMON_Bits8Convert2Bits32( u32 *pdest, u8 *psrc, u8 mode )
{
    if ((!pdest) ||(!psrc))
        return false;
    
    if (mode)
    {
        (*pdest) = (*psrc ++) << 24;
        (*pdest) |= (*psrc ++) << 16;
        (*pdest) |= (*psrc ++) << 8;
        (*pdest) |= (*psrc ++);
    }
    else
    {
        (*pdest) = (*psrc ++);
        (*pdest) |= (*psrc ++) << 8;
        (*pdest) |= (*psrc ++) << 16;
        (*pdest) |= (*psrc ++) << 24;
    }
    
    return true;
}

/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
bool COMMON_Bits16Convert2Bits8( u8 *pdest, u16 srcdat, u8 mode )
{
    if (!pdest)
        return false;
    
    if (mode)
    {
        *pdest ++ = srcdat >> 8;
        *pdest = srcdat & 0xFF;
    }
    else
    {
        *pdest ++ = srcdat & 0xFF;
        *pdest = srcdat >> 8;
    }
    
    return true;
}

/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
bool COMMON_Bits32Convert2Bits8( u8 *pdest, u32 srcdat, u8 mode )
{
    if (!pdest)
        return false;
    
    if (mode)
    {
        *pdest ++ = (srcdat >> 24) & 0xFF;
        *pdest ++ = (srcdat >> 16) & 0xFF;
        *pdest ++ = (srcdat >> 8) & 0xFF;
        *pdest = srcdat & 0xFF;
    }
    else
    {
        *pdest ++ = srcdat & 0xFF;
        *pdest ++ = (srcdat >> 8) & 0xFF;
        *pdest ++ = (srcdat >> 16) & 0xFF;
        *pdest ++ = (srcdat >> 24) & 0xFF;
    }
    
    return true;
}


/***************************************************************************************
Name:  
Func:   
Para:
Retn:
***************************************************************************************/
bool COMMON_ConverAscii2CommonMode( u8* psrc, u16 datlen)
{
    u16 i;
    u16 pdlen;
    u8 tmp, val;
    u8* ptmp;
    u8* pptr;
    
   
    if ((!psrc) || (datlen < 3))
        return false;
    
    pdlen = datlen - 3; //remove frame head and tail
    if (pdlen % 2)
        return false;
    
    pdlen /= 2;
    pptr = psrc;
    ptmp = psrc;
    psrc ++;
    for (i = 0; i < pdlen; i++)
    {
        if (COMMON_Convert1Byteto4Bits(&tmp, *psrc ++))
        {
            val = tmp << 4;
        }
        else
            return false;
        
       if (COMMON_Convert1Byteto4Bits(&tmp, *psrc ++))
        {
            val |= tmp;
        }
        else
            return false;
       
       *pptr++ = val;
    }
    pdlen --;   //remove LRC Check byte
    
    val = COMMON_GeneratorLRCCheckCode( ptmp, pdlen);
    if (val != *(ptmp + pdlen))
        return false;
    
    return true;
}

/***************************************************************************************
Name:  
Func:   
Para:
Retn:
***************************************************************************************/
bool COMMON_Convert1Byteto4Bits(u8* pdest, u8 src)
{
    if (!pdest)
        return false;
    
    if (src >= '0' && src <= '9')
    {
        *pdest = src - '0';
    }
    else if(src >= 'A' && src <= 'F')
    {
        *pdest = src - 'A' + 10;
    }
    else if(src >= 'a' && src <= 'f')
    {
        *pdest = src - 'a' + 10;
    }
    else
        return false;
    
    return true;
}


/***************************************************************************************
Name:  
Func:   
Para:
Retn:
***************************************************************************************/
u8 COMMON_GeneratorLRCCheckCode(u8* psrc, u16 len)
{
    u8 LRCChk = 0;
    
    while (len --)
    {
        LRCChk += *psrc++;
    }
    return (-LRCChk);
}


/***************************************************************************************
Name:
Func:
Para:
Retn:
***************************************************************************************/
#define TMPRCV_BUFFER_LEN   512
bool COMMON_ConvertHex2Ascii( u8 *psrc, u16 datlen )
{
    u8 i;
    u8 val_l;
    u8 val_h;
    u8 tmp[TMPRCV_BUFFER_LEN];
    u8* ptmp = tmp;
    u8* pdat;
    
    if ((!psrc) || (!datlen) || (datlen * 2 > TMPRCV_BUFFER_LEN))
        return false;
    
    pdat = psrc;
    memset(ptmp, 0, TMPRCV_BUFFER_LEN);
    i = datlen;
    while(i --)
    {
        val_h = (*pdat) >> 4;
        val_l = (*pdat) & 0x0F;
        pdat ++;
        //High 4 bits
        if (val_h <= 9)
        {
            *ptmp ++= val_h + '0';
        }
        else
        {
            *ptmp ++= val_h + '7'; //'A' - 10;
        }
        
        //Low 4 bits
        if (val_l <= 9)
        {
            *ptmp ++= val_l + '0';
        }
        else
        {
            *ptmp ++= val_l + '7'; //'A' - 10;
        }
    }
    psrc ++;
    memcpy( psrc, tmp, datlen * 2);
    
    return true;
}


/*******************************************************************************
Name:   
Func:   
Para:   
Retn:   
*******************************************************************************/
//CRC_16 scan list table
/*CRC low 8 bits table*/
const u8 CRCLowTab[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/*CRC high 8 bits table*/
const u8 CRCHighTab[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};
bool COMMON_GeneratorU16bitsCRCCheckCode( u16* crcval, u8* psrc, u32 srclen)
{
    u8 crc_l;
    u8 crc_h;
    u8 index;
    
    if ((!psrc) || (!srclen))
        return false;
    
    crc_l = 0xFF;
    crc_h = 0xFF;
    
    while (srclen --)
    {
        index = crc_l ^ (*psrc ++);
        crc_l = crc_h ^ CRCLowTab[index];
        crc_h = CRCHighTab[index];
    }
    
    *crcval = (crc_h << 8) | crc_l;
    return true;
}
