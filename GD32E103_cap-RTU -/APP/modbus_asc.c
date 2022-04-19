#include "bsp.h"
#include "math.h"
//#include "os_cfg_app.h"

float Def_Liquid_level;         //出厂累计流量
float Def_Time_waste;           //出厂累计时间
uint8_t RcvDataBuf[64];
uint8_t SendBuf[DATAPACKET_LEN];   //存储从机响应消息帧
uint8_t SendLen;
uint8_t Dev_Id[DEFAULT_DEV_ID_LEN] = {0};

//CRC_16正规查询表
/* CRC低位字节值表*/
const uint8_t auchCRCLo[] =
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

/* CRC高位字节值表*/
const uint8_t auchCRCHi[] =
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


uint16_t MBRTU_GetCRC16(uint8_t *uint8_tMsg, uint32_t uint8_tMsgLen);


void Array_Merge(uint8_t *o, uint8_t *p, uint8_t *q, uint8_t m, uint8_t n)
{
    uint8_t i, j;

    for (i = 0; i < m; i++)
    {
        q[i] = o[i];
    }

    for (j = 0; j < n ;j++)
    {
        q[m+j] = p[j];
    }
}

uint32_t MBRTU_GetSlaveAddr(uint8_t *uint8_tMsg)
{
    return(uint8_tMsg[0]);
}

uint32_t MBRTU_GetFunCode(uint8_t *uint8_tMsg)
{
    return(uint8_tMsg[1]);
}


//////////////////////////////////////////////////////////////////////////////////////////////
//description描述：MBRTU_SendMsg() 发送数据帧 modbus RTU模式
//input输入      ：
//output输出     ：
//////////////////////////////////////////////////////////////////////////////////////////////

void MBRTU_SendMsg(uint8_t *uint8_tMsg, uint32_t uint8_tMsgLen)
{
    if((MB_ADDRESS_BROADCAST != MBRTU_GetSlaveAddr(uint8_tMsg)) && (UartComm.Echo == ECHO_ENABLE))
    {
        uint16_t uint16_tCRC = MBRTU_GetCRC16( uint8_tMsg, uint8_tMsgLen );
        uint8_tMsg[uint8_tMsgLen] = ( uint8_t )( uint16_tCRC & 0x00FF );         //低位在前
        uint8_tMsg[uint8_tMsgLen+1] = ( uint8_t )(( uint16_tCRC >> 8 ) & 0x00FF ); //高位在后
        UART1_SendData( uint8_tMsg, uint8_tMsgLen + 2 );       //发送数据
    }
}

void MBRTU_SendMsg_NoLimit(uint8_t *uint8_tMsg, uint32_t uint8_tMsgLen)
{
    if (UartComm.Echo == ECHO_ENABLE)
    {
        uint16_t uint16_tCRC = MBRTU_GetCRC16(uint8_tMsg, uint8_tMsgLen);
        uint8_tMsg[uint8_tMsgLen] = (uint8_t)(uint16_tCRC & 0x00FF);             //低位在前
        uint8_tMsg[uint8_tMsgLen+1] = (uint8_t)((uint16_tCRC >> 8) & 0x00FF);      //高位在后
        UART1_SendData(uint8_tMsg, uint8_tMsgLen + 2);           //发送数据
    }
}

void MBRTU_SendErr(uint8_t ErrCode)
{
    SendLen = 1;
    SendBuf[SendLen++] |= 0x80;
    SendBuf[SendLen++] = ErrCode;

    MBRTU_SendMsg(SendBuf, SendLen);
}


//////////////////////////////////////////////////////////////////////////////////////////
//description描述：MBRTU_GetCRC16() 获取字符序列的CRC校验码
//input输入      ：Msg，MsgLen十六进制字符序列及其长度
//output输出     ：16位CRC码
///////////////////////////////////////////////////////////////////////////////////////////
uint16_t MBRTU_GetCRC16(uint8_t *uint8_tMsg, uint32_t uint8_tMsgLen)
{
    uint8_t CRCLo = 0xFF ; // 高CRC字节初始化
    uint8_t CRCHi = 0xFF ; // 低CRC字节初始化
    unsigned uIndex ; // CRC循环中的索引

    while (uint8_tMsgLen--)    // 传输消息缓冲区
    {
        uIndex = CRCLo ^ *uint8_tMsg++; // 计算CRC
        CRCLo  = CRCHi ^ auchCRCLo[uIndex];
        CRCHi  = auchCRCHi[uIndex];
    }

    return (CRCHi << 8) | CRCLo;
}

//CRC校验
uint32_t MBRTU_CheckCRC16(uint8_t *uint8_tMasterRequestData, uint32_t uint8_tMsgLen)
{
    return ((0 == MBRTU_GetCRC16(uint8_tMasterRequestData, uint8_tMsgLen)) ? 1 : 0);              //对最后两字节为CRC高的数据帧再求CRC得到的结果为0
}


void MBRTU_Fun03()  //读单个寄存器
{
	long k;
    uint32_t i;
    uint32_t Data_Buf;
    uint16_t ReadAdr = (uint16_t)RcvDataBuf[2] * 256 + RcvDataBuf[3];
    uint16_t Register_Num = (uint16_t)RcvDataBuf[4] * 256 + RcvDataBuf[5];

    SendLen = 0;
    SendBuf[SendLen++] = (MBRTU_GetSlaveAddr(RcvDataBuf)) ? UartComm.SlaveAddr : 0x00;
    SendBuf[SendLen++] = MB_FUNC_READ_HOLDING_REGISTER;     //功能码
    SendBuf[SendLen++] = Register_Num * 2;      //数据长度

    //如果读取范围溢出

    if (!(((ReadAdr >= HOLDING_REG_REGION1_BGEIN) && (ReadAdr <= HOLDING_REG_REGION1_END)
            && (ReadAdr + Register_Num <= (HOLDING_REG_REGION1_END + 1)))
            || ((ReadAdr >= HOLDING_REG_REGION2_BGEIN) && (ReadAdr <= HOLDING_REG_REGION2_END)
                && (ReadAdr + Register_Num <= (HOLDING_REG_REGION2_END + 1)))
            && (0 != Register_Num)))
    {
        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    for (k = 0; k < Register_Num; ReadAdr++, k++)
    {
        switch (ReadAdr)
        {

        case 0x0030:
            Data_Buf = UartComm.SlaveAddr;              //设备地址
            break;

        case 0x0031:
            Data_Buf = UartComm.BaudRate;               //波特率
            break;

        case 0x0032:
            Data_Buf = UartComm.Parity;                 //奇偶校验
            break;

        case 0x0033:
            Data_Buf = 0;                               //NC
            break;

        case 0x0034:
            Data_Buf = 0;                               //NC
            break;

        case 0x0035:
            Data_Buf = UserParam.FilterLevel;           //滤波系数
            break;

        case 0x0036:
            Data_Buf = ProductPara.AutoUpLoadTime;      //自动发送周期
            break;

        case 0x0037:
            Data_Buf = (uint32_t)(UserParam.UserFlowK * 100);    //输出修正系数K
            break;

        case 0x0038:
            Data_Buf = (uint32_t)(UserParam.UserFlowB * 100);    //输出修正系数B
            break;

        case 0x0039:
            Data_Buf = 0;                                   //位置报警阈值1
            break;

        case 0x003A:
            Data_Buf = 0;                                   //回差1
            break;

        case 0x003B:
            Data_Buf = 0;                                   //位置报警阈值2
            break;

        case 0x003C:
            Data_Buf = 0;                                   //回差2
            break;
            
        case 0x003D:
            Data_Buf = ProductPara.OutputUnit;              //转换单位
            break;

        case 0x003E:
            Data_Buf = ulWritePCAPXramErr;                  //PCap写固件状态
            break;

        case 0x003F:
            Data_Buf = ulWritePCAPXramErr;                  //pcap更新时间
            break;

        case 0x0040:
            Data_Buf = ulWritePCAPXramErr;                  //超出阈值状态持续时间
            break;

        case 0x0060:
            Data_Buf = UartComm.OutputMode;             //输出模式 1-RTU，2-ASCII
            break;

        case 0x0080:
            Data_Buf = 0;                               //液位高度
            break;

        case 0x0081:
            Data_Buf = 0;                               //剩余油量
            break;

        default:
            Data_Buf = 0;
            break;
        }

        for (i = 2; i > 0; i--)
        {
            SendBuf[SendLen++] = (uint8_t)(Data_Buf >> ((i - 1) * 8));
        }
    }

    MBRTU_SendMsg(SendBuf, SendLen);

}


void MBRTU_Fun04()  //读双个寄存器
{
	long k;
    uint32_t i;
    float Chn_Buf;
    //float PluseBak;
    uint32_t Data_Buf;
    uint16_t ReadAdr = (uint16_t)RcvDataBuf[2] * 256 + RcvDataBuf[3];
    uint16_t Register_Num = (uint16_t)RcvDataBuf[4] * 256 + RcvDataBuf[5];
    //uint16_t TimeCnt;
    //CPU_SR_ALLOC();

    SendLen = 0;

    SendBuf[SendLen++] = (MBRTU_GetSlaveAddr(RcvDataBuf)) ? UartComm.SlaveAddr : 0x00;
    SendBuf[SendLen++] = MB_FUNC_READ_INPUT_REGISTER;
    SendBuf[SendLen++] = Register_Num * 2;      //数据长度

//    //如果读取范围溢出
//    if(!(((ReadAdr <= INPUT_REG_REGION1_END)
//        && ((ReadAdr + Register_Num) <= (INPUT_REG_REGION1_END + 2)))
//        || ((ReadAdr >= INPUT_REG_REGION2_BGEIN) && (ReadAdr <= INPUT_REG_REGION2_END)
//        && ((ReadAdr + Register_Num) <= (INPUT_REG_REGION2_END + 2)))
//        && (0 != Register_Num) && (0 == (Register_Num & 0x01)) && (0 == (ReadAdr & 0x01))))
//    {
//        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
//        return;
//    }

    for (k = 0; k < Register_Num; ReadAdr += 2, k += 2)
    {
        switch (ReadAdr)
        {

        case 0x0000:
            Chn_Buf = RunVar.LiquidHeight;
            Data_Buf = *(uint32_t*) & Chn_Buf;       //液位高度
            break;

        case 0x0002:
			Data_Buf = (uint32_t)((RunVar.temp + 27310000) / 10000) - 2731;
//            Chn_Buf = RunVar.temp;
//            Data_Buf = *(uint32_t*) & Chn_Buf;       //液位温度
            break;
        case 0x0004:
            Chn_Buf = RunVar.TempInAir;
            Data_Buf = *(uint32_t*) & Chn_Buf;       //环境温度
            break;

        case 0x0006:
            Chn_Buf = Angle.XtoG;
            Data_Buf = *(uint32_t*) & Chn_Buf;       //X角度
            break;

        case 0x0008:
            Chn_Buf = Angle.YtoG;
            Data_Buf = *(uint32_t*) & Chn_Buf;       //Y角度
            break;

        case 0x000A:
            Chn_Buf = Angle.ZtoG;
            Data_Buf = *(uint32_t*) & Chn_Buf;       //Z角度
            break;

        case 0x0080:
            Chn_Buf = RunVar.RawCap;
            Data_Buf = *(uint32_t*) & Chn_Buf;       //原始PCAP数据
            break;

        case 0x0082:
            Chn_Buf = PCap_Status;
            Data_Buf = *(uint32_t*) & Chn_Buf;       //Pcap状态
            break;

        case 0x0084:
            Chn_Buf = ulWritePCAPXramErr;
            Data_Buf = *(uint32_t*) & Chn_Buf;       //PCap写固件状态
            break;

        default:
            Data_Buf = 0;
            break;
        }

        for (i = 4; i > 0; i--)
        {
            SendBuf[SendLen++] = (uint8_t)(Data_Buf >> ((i - 1) * 8));
        }
    }

    MBRTU_SendMsg(SendBuf, SendLen);
}

void MBRTU_Fun05()
{
    uint16_t ReadAdr = (uint16_t)RcvDataBuf[2] * 256 + RcvDataBuf[3];

    SendLen = 0;

    SendBuf[SendLen++] = (MBRTU_GetSlaveAddr(RcvDataBuf)) ? UartComm.SlaveAddr : 0x00;
    SendBuf[SendLen++] = MB_FUNC_WRITE_SINGLE_COIL;

    //如果操作范围溢出

    if ((ReadAdr < SINGLE_COIL_ADDR_BGEIN) || (ReadAdr > SINGLE_COIL_ADDR_END))
    {
        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

//    if ((ReadAdr != 0x0053) && (ProductPara.bFlashWEn == FLASH_WR_DISABLE))
//	if (ReadAdr != 0x0053)
//    {
//        MBRTU_SendErr(MB_EX_SLAVE_DEVICE_FAILURE);
//        return;
//    }

    if (!(((RcvDataBuf[4] == 0xFF) && (RcvDataBuf[5] == 0x00))
            || ((RcvDataBuf[4] == 0x00) && (RcvDataBuf[5] == 0x00))))
    {
        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_VALUE);
        return;
    }
	
	ProductPara.bFlashWEn = FLASH_WR_ENABLE;
	
    switch (ReadAdr)
    {
    case 0x0050:                                //标定
        if((RcvDataBuf[4] == 0x00) && (RcvDataBuf[5] == 0x00))//标定低液位
        {
            Cur_Param[CAPMIN] = (unsigned char)(RunVar.RawCap >> 24);
            Cur_Param[CAPMIN+1] = (unsigned char)(RunVar.RawCap >> 16);
            Cur_Param[CAPMIN+2] =(unsigned char)(RunVar.RawCap >> 8);
            Cur_Param[CAPMIN+3] = (unsigned char)RunVar.RawCap;
            RunVar.CalState |= CalibrateLow;
        }
        else if((RcvDataBuf[4] == 0xFF) && (RcvDataBuf[5] == 0x00))//标定高液位
        {
            Cur_Param[CAPMAX] = (unsigned char)(RunVar.RawCap >> 24);
            Cur_Param[CAPMAX+1] = (unsigned char)(RunVar.RawCap >> 16);
            Cur_Param[CAPMAX+2] =(unsigned char)(RunVar.RawCap >> 8);
            Cur_Param[CAPMAX+3] = (unsigned char)RunVar.RawCap;
            RunVar.CalState |= CalibrateHeight;
        }
        CapCalibrate();
        break;

    case 0x0051:                                //恢复出厂值

        if ((RcvDataBuf[4] == 0xFF) && (RcvDataBuf[5] == 0x00))
        {
            Cur_Param[0] = User_Default_Param[0] + 1;
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, RUN_ADDR_BASE, 1, Cur_Param);
//#if IWDG_EN > 0
//            fwdgt_counter_reload();
//#endif
            Rd_Dev_Param();
//#if IWDG_EN > 0
//            fwdgt_counter_reload();
//#endif
        }
        break;

    case 0x0052:  //解冻、冻结。冻结则是接收命令，但是不返回命令

        if ((RcvDataBuf[4] == 0xFF) && (RcvDataBuf[5] == 0x00))
            UartComm.Echo = ECHO_ENABLE;
        else
            UartComm.Echo = ECHO_DISABLE;
        I2C1_WNBytesMul3T(EEPROM_ADDRESS, ECHOEN, 1, &UartComm.Echo);
        break;
        
    case 0x0053:  //使能/关闭flash写操作
        if ((RcvDataBuf[4] == 0xFF) && (RcvDataBuf[5] == 0x00))
            ProductPara.bFlashWEn = FLASH_WR_ENABLE;
        else
            ProductPara.bFlashWEn = FLASH_WR_DISABLE;
        break;
    }
	
	ProductPara.bFlashWEn = FLASH_WR_DISABLE;
	
    MBRTU_SendMsg(RcvDataBuf, 6);
}

uint16_t uart_init = 0;
void MBRTU_Fun10()
{
	long k;
    uint16_t ReadAdr = (uint16_t)RcvDataBuf[2] * 256 + RcvDataBuf[3];
    uint16_t Register_Num = (uint16_t)RcvDataBuf[4] * 256 + RcvDataBuf[5];
    uint32_t index = 0;
    uint8_t uint8_tTemp[4];

    SendLen = 0;

    SendBuf[SendLen++] = (MBRTU_GetSlaveAddr(RcvDataBuf)) ? UartComm.SlaveAddr : 0x00;
    SendBuf[SendLen++] = MB_FUNC_WRITE_MULTIPLE_REGISTERS;
    SendBuf[SendLen++] = Register_Num * 2;

    //如果读取范围溢出

    if (!(((ReadAdr >= MUL_REG_REGION1_BGEIN) && (ReadAdr <= MUL_REG_REGION1_END)
            && (ReadAdr + Register_Num <= (MUL_REG_REGION1_END + 1)))
            || ((ReadAdr >= MUL_REG_REGION2_BGEIN) && (ReadAdr <= MUL_REG_REGION2_END)
                && (ReadAdr + Register_Num <= (MUL_REG_REGION2_END + 1)))
            && (0 != Register_Num) && ((Register_Num * 2) == RcvDataBuf[6])))
    {
        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

//    if (ProductPara.bFlashWEn == FLASH_WR_DISABLE)
//    {
//        MBRTU_SendErr(MB_EX_SLAVE_DEVICE_FAILURE);
//        return;
//    }
	
	ProductPara.bFlashWEn = FLASH_WR_ENABLE;
	
    for (k = 0; k < Register_Num; ReadAdr++, k++)
    {
        switch (ReadAdr)
        {

        case 0x0030:                        //设备地址
            UartComm.SlaveAddr = RcvDataBuf[8+index];
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, SLAVE_ADDR, 1, &UartComm.SlaveAddr);
            index += 2;
            break;

        case 0x0031:                        //波特率
            UartComm.BaudRate = RcvDataBuf[8+index];
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, BAUDRATE, 1, &UartComm.BaudRate);
			uart_init++;
            index += 2;
            break;

        case 0x0032:                        //奇偶校验
            UartComm.Parity = RcvDataBuf[8+index];
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, PARITY, 1, &UartComm.Parity);
			uart_init++;
            index += 2;
            break;

        case 0x0033:                      //NC
            index += 2;
            break;

        case 0x0034:                      //NC
            index += 2;
            break;

        case 0x0035:                        //滤波系数
            UserParam.FilterLevel = RcvDataBuf[8+index];
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, FILTER_LEVEL, 1, &UserParam.FilterLevel);
            Switch_Fiter(UserParam.FilterLevel);
            index += 2;
            break;

        case 0x0036:                        //自动上传时间
            ProductPara.AutoUpLoadTime = RcvDataBuf[8+index];
            if (ProductPara.AutoUpLoadTime > 240)
                ProductPara.AutoUpLoadTime = 0;
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, AUTO_UPLOAD_TIME, 1, &ProductPara.AutoUpLoadTime);
            index += 2;
            break;

        case 0x0037:                        //输出修正系数K
            UserParam.UserFlowK = ((uint16_t)RcvDataBuf[7+index] * 256 + RcvDataBuf[8+index]) / 100.0;
            floatToHexArray(UserParam.UserFlowK, uint8_tTemp, 4);
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, USER_FLOW_K, 4, uint8_tTemp);
            index += 2;
            break;

        case 0x0038:                        //输出修正系数B
            UserParam.UserFlowB = ((uint16_t)RcvDataBuf[7+index] * 256 + RcvDataBuf[8+index]) / 100.0;
            floatToHexArray(UserParam.UserFlowB, uint8_tTemp, 4);
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, USER_FLOW_B, 4, uint8_tTemp);
            index += 2;
            break;

        case 0x0039:                        //位置报警阈值1
            index += 2;
            break;

        case 0x003A:                        //回差1
            index += 2;
            break;

        case 0x003B:                        //位置报警阈值2
            index += 2;
            break;

        case 0x003C:                        //回差2
            index += 2;
            break;

        case 0x003D:                        //转换单位
            ProductPara.OutputUnit = RcvDataBuf[8+index];
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, OUTPUT_UNIT, 1, &ProductPara.OutputUnit);
            index += 2;
            break;

        case 0x003E:                        //NC
            index += 2;
            break;

        case 0x003F:                        //PCap更新时间
            index += 2;
            break;

        case 0x0040:                        //超出阈值状态持续时间
            index += 2;
            break;

        default:
            break;
        }
    }

	ProductPara.bFlashWEn = FLASH_WR_DISABLE;
	
    MBRTU_SendMsg(RcvDataBuf, 6);
}

void MBRTU_Fun2B(void)
{
	uint32_t i;
	
    for (i = 0; i < (sizeof(Dev_Id) / sizeof(Dev_Id[0])); i++)
    {
        SendBuf[i] = Dev_Id[i];
    }

    MBRTU_SendMsg_NoLimit(SendBuf, (sizeof(Dev_Id) / sizeof(Dev_Id[0])));
}

void MBRTU_Fun2A(void)
{
    uint8_t i;
    
//    if (ProductPara.bFlashWEn == FLASH_WR_DISABLE)
//    {
//        MBRTU_SendErr(MB_EX_SLAVE_DEVICE_FAILURE);
//        return;
//    }

	ProductPara.bFlashWEn = FLASH_WR_ENABLE;
	
    for (i = 0; i < DEFAULT_DEV_ID_LEN; i++)
        Dev_Id[i] = RcvDataBuf[2+i];

    I2C1_WriteNBytes(EEPROM_ADDRESS, DEV_ID, DEFAULT_DEV_ID_LEN, Dev_Id);
	
	ProductPara.bFlashWEn = FLASH_WR_DISABLE;
}

void MBRTU_Fun25()
{
    uint16_t ReadAdr = (uint16_t)RcvDataBuf[2] * 256 + RcvDataBuf[3];
    uint8_t Data_Buf[4];
    SendLen = 0;

    SendBuf[SendLen++] = (MBRTU_GetSlaveAddr(RcvDataBuf)) ? UartComm.SlaveAddr : 0x00;
    SendBuf[SendLen++] = 0x25;

    //如果操作范围溢出

//    if ((ReadAdr > SPECICAL_COIL_REGION1_END)
//        && ((ReadAdr < SPECICAL_COIL_REGION2_BGEIN) || (ReadAdr > SPECICAL_COIL_REGION2_END)))
//    {
//        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
//        return;
//    }

    if (!(((RcvDataBuf[4] == 0xFF) && (RcvDataBuf[5] == 0x00))
            || ((RcvDataBuf[4] == 0x00) && (RcvDataBuf[5] == 0x00))))
    {
        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_VALUE);
        return;
    }

//    if (ProductPara.bFlashWEn == FLASH_WR_DISABLE)
//    {
//        MBRTU_SendErr(MB_EX_SLAVE_DEVICE_FAILURE);
//        return;
//    }
	
	ProductPara.bFlashWEn = FLASH_WR_ENABLE;
	
    switch (ReadAdr)
    {
    case 0x0000:                                //标定
        if((RcvDataBuf[4] == 0x00) && (RcvDataBuf[5] == 0x00))//标定低液位
        {
            Cur_Param[CAPMIN] = (unsigned char)(RunVar.RawCap >> 24);
            Cur_Param[CAPMIN+1] = (unsigned char)(RunVar.RawCap >> 16);
            Cur_Param[CAPMIN+2] =(unsigned char)(RunVar.RawCap >> 8);
            Cur_Param[CAPMIN+3] = (unsigned char)RunVar.RawCap;
            RunVar.CalState |= CalibrateLow;
        }
        else if((RcvDataBuf[4] == 0xFF) && (RcvDataBuf[5] == 0x00))//标定高液位
        {
            Cur_Param[CAPMAX] = (unsigned char)(RunVar.RawCap >> 24);
            Cur_Param[CAPMAX+1] = (unsigned char)(RunVar.RawCap >> 16);
            Cur_Param[CAPMAX+2] =(unsigned char)(RunVar.RawCap >> 8);
            Cur_Param[CAPMAX+3] = (unsigned char)RunVar.RawCap;
            RunVar.CalState |= CalibrateHeight;
        }
        CapCalibrate();
        break;

    case 0x0001:    //油浮标定开始/结束
        if((RcvDataBuf[4] == 0xFF) && (RcvDataBuf[5] == 0x00))
        {
            DAOilDebug.bDADebugEn = Bit_SET;
            DAOutDebug.bDADebugEn = Bit_RESET;
        }
        else
        {
            DAOilDebug.bDADebugEn = Bit_RESET;
        }
        break;
        
    case 0x00B2:    //DA标定开始/结束
        if((RcvDataBuf[4] == 0xFF) && (RcvDataBuf[5] == 0x00))
        {
            DAOutDebug.bDADebugEn = Bit_SET;
            DAOilDebug.bDADebugEn = Bit_RESET;
        }
        else
        {
            DAOutDebug.bDADebugEn = Bit_RESET;
        }
        break;
        
    case 0x00B3:    //记录零点/满程DA码
        if(Bit_SET == DAOutDebug.bDADebugEn)
        {
            if((RcvDataBuf[4] == 0x00) && (RcvDataBuf[5] == 0x00))//零点DA 码
            {
                ProductPara.DAMin = DAOutDebug.usDACode;
                floatToHexArray(ProductPara.DAMin, Data_Buf, 4);
                I2C1_WNBytesMul3T(EEPROM_ADDRESS, DAMIN, 4, Data_Buf);
            }
            else if((RcvDataBuf[4] == 0xFF) && (RcvDataBuf[5] == 0x00))//满程DA 码
            {
                ProductPara.DAMax = DAOutDebug.usDACode;
                floatToHexArray(ProductPara.DAMax, Data_Buf, 4);
                I2C1_WNBytesMul3T(EEPROM_ADDRESS, DAMAX, 4, Data_Buf);
            }
        }
        break;
        
    case 0x0040:        //在线升级
        NVIC_SystemReset();
        break;
    }
	
	ProductPara.bFlashWEn = FLASH_WR_DISABLE;
	
    MBRTU_SendMsg(RcvDataBuf, 6);
}

 
void MBRTU_Fun26(void)
{
	long k;
    uint32_t i;
    uint32_t Data_Buf;
//    uint16_t ReadAdr = (uint16_t)RcvDataBuf[2] * 256 + RcvDataBuf[3];
	uint16_t ReadAdr = RcvDataBuf[3];
    uint16_t Register_Num = (uint16_t)RcvDataBuf[4] * 256 + RcvDataBuf[5];
    float fTemp = 0;

    SendLen = 0;

    SendBuf[SendLen++] = (MBRTU_GetSlaveAddr(RcvDataBuf)) ? UartComm.SlaveAddr : 0x00;
    SendBuf[SendLen++] = 0x26;
    SendBuf[SendLen++] = Register_Num * 2;      //数据长度

    if (!(((ReadAdr >= 0x0080) && (ReadAdr <= 0x0096) && (ReadAdr + Register_Num) <= (0x0096 + 2))
               || ((ReadAdr >= 0x00B0) && (ReadAdr <= 0x00B6) && (ReadAdr + Register_Num) <= (0x00B6 + 2))
               || ((ReadAdr <= 0x001E) && (ReadAdr + Register_Num) <= (0x001E + 2))
               && (0 != Register_Num) && (0 == (Register_Num & 0x01)) && (0 == (ReadAdr & 0x01))))
    {
        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

    for (k = 0; k < Register_Num; ReadAdr += 2, k += 2)
    {
        switch (ReadAdr)
        {

        case 0x0080:    //出厂量程
            Data_Buf = *(uint32_t*) & ProductPara.Def_Range;
            break;

        case 0x0082:    //零点采样值
            Data_Buf = *(uint32_t*) & ProductPara.CapMin;
            break;

        case 0x0084:    //满度采样值
            Data_Buf = *(uint32_t*) & ProductPara.CapMax;
            break;

        case 0x0088:    //液体温度K值
            Data_Buf = *(uint32_t*) & ProductPara.Temp_K;
            break;

        case 0x008A:    //液体温度B值
            Data_Buf = *(uint32_t*) & ProductPara.Temp_B;
            break;
        case 0x0094:    //环境温度K值
            Data_Buf = *(uint32_t*) & ProductPara.Temp2_K;
            break;

        case 0x0096:    //环境温度B值
            Data_Buf = *(uint32_t*) & ProductPara.Temp2_B;
            break;
            
        case 0x00B0:    //去极值百分比
            Data_Buf = 0;
            break;
            
        case 0x0086:    //油浮DA标定接口
            if(Bit_SET == DAOilDebug.bDADebugEn)
                Data_Buf = *(uint32_t*) & DAOilDebug.usDACode;
            else if(Bit_SET == DAOutDebug.bDADebugEn)
                Data_Buf = *(uint32_t*) & DAOutDebug.usDACode;
            else
                Data_Buf = 0;
            break;
/*
        case 0x8090:    //DA满输出对应的液位高度
            Data_Buf = *(uint32_t*) & ProductPara.LiquitHeightforDAMax;
            break;
*/
        case 0x0000:    //高度

        case 0x0002:

        case 0x0004:

        case 0x0006:

        case 0x0008:

        case 0x000A:

        case 0x000C:

        case 0x000E:
            fTemp = ProductPara.Floater.Scale[(ReadAdr - 0x9000) >> 1];
            Data_Buf = *(uint32_t*) & fTemp;
            break;

        case 0x0010:    //DA码

        case 0x0012:

        case 0x0014:

        case 0x0016:

        case 0x9108:

        case 0x910A:

        case 0x001C:

        case 0x001E:
            fTemp = ProductPara.Floater.DACode[(ReadAdr - 0x9100) >> 1];
            Data_Buf = *(uint32_t*) & fTemp;
            break;
            
        default:
            MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
            return;
        }

        for (i = 4; i > 0; i--)
        {
            SendBuf[SendLen++] = (uint8_t)(Data_Buf >> ((i - 1) * 8));
        }
    }

    MBRTU_SendMsg(SendBuf, SendLen);
}

void MBRTU_Fun27(void)
{
	long k;
    uint16_t ReadAdr = (uint16_t)RcvDataBuf[2] * 256 + RcvDataBuf[3];
    uint16_t Register_Num = (uint16_t)RcvDataBuf[4] * 256 + RcvDataBuf[5];
    uint32_t index = 0;

    SendLen = 0;

    SendBuf[SendLen++] = (MBRTU_GetSlaveAddr(RcvDataBuf)) ? UartComm.SlaveAddr : 0x00;
    SendBuf[SendLen++] = 0x27;
    SendBuf[SendLen++] = Register_Num * 2;

    if ((0 != (Register_Num & 0x01)) || (0 == Register_Num) || (0 != (ReadAdr & 0x01))
            || ((Register_Num * 2) != RcvDataBuf[6]))
    {
        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }
    else if (!(((ReadAdr >= 0x8080) && (ReadAdr <= 0x8096) && (ReadAdr + Register_Num) <= (0x8096 + 2))
               || ((ReadAdr >= 0x000B) && (ReadAdr <= 0x00C0) && (ReadAdr + Register_Num) <= (0x00C0 + 2))
               || ((ReadAdr <= 0x001E) && (ReadAdr + Register_Num) <= (0x001E + 2))))
    {
        MBRTU_SendErr(MB_EX_ILLEGAL_DATA_ADDRESS);
        return;
    }

//    if (ProductPara.bFlashWEn == FLASH_WR_DISABLE)
//    {
//        MBRTU_SendErr(MB_EX_SLAVE_DEVICE_FAILURE);
//        return;
//    }

	ProductPara.bFlashWEn = FLASH_WR_ENABLE;
	
    for (k = 0; k < Register_Num; ReadAdr += 2, k += 2)
    {
        switch (ReadAdr)
        {

        case 0x0080:    //出厂量程
            ProductPara.Def_Range = HexToFloat(&RcvDataBuf[7 + index]);
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, DEF_RANGE, 4, &RcvDataBuf[7 + index]);

            index += 4;
            break;

        case 0x0082:    //零点采样值
            ProductPara.Def_CapMin = HexToFloat(&RcvDataBuf[7 + index]);
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, DEF_CAPMIN, 4, &RcvDataBuf[7 + index]);

            index += 4;
            break;

        case 0x0084:    //满度采样值
            ProductPara.Def_CapMax = HexToFloat(&RcvDataBuf[7 + index]);
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, DEF_CAPMAX, 4, &RcvDataBuf[7 + index]);
            index += 4;
            break;

        case 0x0090:    //液面温度k值
            ProductPara.Temp_K = HexToFloat(&RcvDataBuf[7 + index]);
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, TEMP_K, 4, &RcvDataBuf[7 + index]);

            index += 4;
            break;

        case 0x0092:    //液面温度b值
            ProductPara.Temp_B = HexToFloat(&RcvDataBuf[7 + index]);
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, TEMP_B, 4, &RcvDataBuf[7 + index]);

            index += 4;
            break; 
            
        case 0x0094:    //环境温度k值
            ProductPara.Temp2_K = HexToFloat(&RcvDataBuf[7 + index]);
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, TEMP2_K, 4, &RcvDataBuf[7 + index]);

            index += 4;
            break;

        case 0x0096:    //环境温度b值
            ProductPara.Temp2_B = HexToFloat(&RcvDataBuf[7 + index]);
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, TEMP2_B, 4, &RcvDataBuf[7 + index]);

            index += 4;
            break; 
/*
        case 0x0090:
            ProductPara.LiquitHeightforDAMax = HexToFloat(&RcvDataBuf[7 + index]);
            
            if(ProductPara.LiquitHeightforDAMax > 65535.0)
            {
                ProductPara.LiquitHeightforDAMax = 65535.0;
            }
            else if(ProductPara.LiquitHeightforDAMax < 0.0)
            {
                ProductPara.LiquitHeightforDAMax = 0.0;
            }
            
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, HEIGHTDAMAX, 4, &RcvDataBuf[7 + index]);

            index += 4;
            break;
*/
        case 0x8086:
            if(DAOilDebug.bDADebugEn == Bit_SET)
            {
                DAOilDebug.usDACode = HexToFloat(&RcvDataBuf[7 + index]);
                DA_Write((unsigned short)DAOilDebug.usDACode);
            }
            else if(DAOutDebug.bDADebugEn == Bit_SET)
            {
                DAOutDebug.usDACode = HexToFloat(&RcvDataBuf[7 + index]);
                DA2_Write((unsigned short)DAOutDebug.usDACode);
            }
            index += 4;
            break;

        case 0x0000:    //油浮刻度

        case 0x0002:

        case 0x0004:

        case 0x0006:

        case 0x0008:

        case 0x000A:

        case 0x000C:

        case 0x000E:
            if(DAOilDebug.bDADebugEn == Bit_SET)
            {
                ProductPara.Floater.Scale[(ReadAdr - 0x9000) >> 1] = (unsigned short)HexToFloat(&RcvDataBuf[7 + index]);
                I2C1_WNBytesMul3T(EEPROM_ADDRESS, CALIB_FLOATER_SCALE_1 + 2 * ((ReadAdr - 0x9000) >> 1),
                                 2, (uint8_t *)&(ProductPara.Floater.Scale[(ReadAdr - 0x9000) >> 1]));
            }
            index += 4;
            break;
            
        case 0x0010:    //游浮刻度对应的输出码

        case 0x0012:

        case 0x0014:

        case 0x0016:

        case 0x0018:

        case 0x001A:

        case 0x001C:

        case 0x001E:
            if(DAOilDebug.bDADebugEn == Bit_SET)
            {
                ProductPara.Floater.DACode[(ReadAdr - 0x9100) >> 1] = (unsigned short)HexToFloat(&RcvDataBuf[7 + index]);
                I2C1_WNBytesMul3T(EEPROM_ADDRESS, CALIB_FLOATER_DA_1 + 2 * ((ReadAdr - 0x9100) >> 1),
                                 2, (uint8_t *)&(ProductPara.Floater.DACode[(ReadAdr - 0x9100) >> 1]));
            }
            index += 4;
            break;
            
        default:
            index += 4;
            break;
        }
    }

	ProductPara.bFlashWEn = FLASH_WR_DISABLE;
	
    MBRTU_SendMsg(RcvDataBuf, 6);
}


void get_device_addr(void)
{
	SendLen = 0;
	SendBuf[SendLen++] = UartComm.SlaveAddr;
	SendBuf[SendLen++] = 00;
	SendBuf[SendLen++] = 00;
	SendBuf[SendLen++] = 00;
	SendBuf[SendLen++] = UartComm.SlaveAddr;

	MBRTU_SendMsg(SendBuf, SendLen);
}


//功能操作函数
void MBRTU_Function()
{
    int32_t RecvLen = 0;
    
    RecvLen = UART1_RecvData(RcvDataBuf, sizeof(RcvDataBuf) / sizeof(RcvDataBuf[0]));

    if (RecvLen <= 0)
    {
        return;
    }
	else if(MB_ADDRESS_BROADCAST == MBRTU_GetSlaveAddr(RcvDataBuf))
	{
		get_device_addr();
	}
    else if ((UartComm.SlaveAddr != MBRTU_GetSlaveAddr(RcvDataBuf)) &&
             (MB_ADDRESS_BROADCAST != MBRTU_GetSlaveAddr(RcvDataBuf)))
    {
        return;
    }
    else if (0 == MBRTU_CheckCRC16(RcvDataBuf, RecvLen))
    {
        if ((MBRTU_GetSlaveAddr(RcvDataBuf) == UartComm.SlaveAddr))
        {
            SendLen = 0;
            SendBuf[SendLen++] = MBRTU_GetSlaveAddr(RcvDataBuf);
            SendBuf[SendLen++] = 0x80 | (uint8_t)MBRTU_GetFunCode(RcvDataBuf);
            SendBuf[SendLen++] = MB_EX_MEMORY_PARITY_ERROR;

            MBRTU_SendMsg(SendBuf, SendLen);
        }
    }
    else
    {
        switch (MBRTU_GetFunCode(RcvDataBuf))
        {

        case MB_FUNC_READ_HOLDING_REGISTER:
            MBRTU_Fun03();  //读单个寄存器
            break;

        case MB_FUNC_READ_INPUT_REGISTER:
            MBRTU_Fun04();  //读双个寄存器
            break;

        case MB_FUNC_WRITE_SINGLE_COIL:
            MBRTU_Fun05();  //写线圈
            break;

        case MB_FUNC_WRITE_MULTIPLE_REGISTERS:
            MBRTU_Fun10();
            break;

        case 0x25:
            MBRTU_Fun25();
            break;
		
        case 0x26:
            MBRTU_Fun26();
            break;

        case 0x27:
            MBRTU_Fun27();
            break;

        case 0x2A:
            MBRTU_Fun2A();
            break;

        case 0x2B:
            MBRTU_Fun2B();
            break;

        default:
            SendLen = 0;
            SendBuf[SendLen++] = MBRTU_GetSlaveAddr(RcvDataBuf);
            SendBuf[SendLen++] = 0x80 | (uint8_t)MBRTU_GetFunCode(RcvDataBuf);
            SendBuf[SendLen++] = MB_EX_ILLEGAL_FUNCTION;

            MBRTU_SendMsg(SendBuf, SendLen);
            break;

        }
    }
}

void AutoUpLoadFrame(void)
{
    float temp;
    uint32_t ulTemp;
    uint32_t i = 0;

    //液位高度
    temp = RunVar.LiquidHeight;
    for (i = 0; i < 4; i++)
        SendBuf[i] = (uint8_t)((*(uint32_t*) & temp) >> ((3 - i) * 8));

    //环境温度
    temp = RunVar.TempInAir;
    for (i = 0; i < 4; i++)
        SendBuf[i+4] = (uint8_t)((*(uint32_t*) & temp) >> ((3 - i) * 8));

    //原始PCAP数据
    ulTemp = RunVar.RawCap;
    for (i = 0; i < 4; i++)
        SendBuf[i+8] = (uint8_t)(ulTemp >> ((3 - i) * 8));

    //油箱剩余油量
    temp = 0;
    for(i = 0; i < 4; i++)
        SendBuf[i+12] = (uint8_t)((*(uint32_t*)&temp)>>((3-i)*8));

    for (i = 0; i < DEFAULT_DEV_ID_LEN; i++)
        SendBuf[i+16] = Dev_Id[i];
    
    MBRTU_SendMsg_NoLimit(SendBuf, 16 + DEFAULT_DEV_ID_LEN);
}


/**@brief       将自动上传时间编码转换成时间
* @param[in]    CodeValue             编码值
* @param[out]   无
* @return       转换后的秒数
* - None		
* @note         01-被动；02-10S（缺省值）；03-20S；04-30S
*/
uint8_t AutoUpLoadTimeTransform(uint8_t CodeValue)
{
    uint8_t code = 0;
    code = CodeValue;
//    switch(code)
//    {
//    case 1: 
//        code = code - 1;                                                        //编码为01，返回值为0时关闭主动上传
//        break;

//    case 2:                                                             
//        code = code * 5;                                                        //编码为02，返回值为10时10s自动上传
//        break;
//        
//    case 3: 
//        code = (code + 1) * 5;                                                  //编码为03，返回值为20时20s自动上传                                              
//        break;
//        
//    case 4:
//        code = (code + 2) * 5;                                                  //编码为04，返回值为30时30s自动上传
//        
//    default:
//        code = 0;                                                               //默认被动上传
//        break;
//    }
    return code;
}
    
void Timer3_Init(void)
{
//	timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer3_initpara;

    rcu_periph_clock_enable(RCU_TIMER3);

    timer_deinit(TIMER3);

    /* TIMER configuration */
    timer3_initpara.prescaler         = 7199;
    timer3_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer3_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer3_initpara.period            = 9999;
    timer3_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer3_initpara.repetitioncounter = 0;
    timer_init(TIMER3,&timer3_initpara);

	nvic_irq_enable(TIMER3_IRQn, 1, 3);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER3);
//    timer_primary_output_config(TIMER0, ENABLE);
	
	timer_interrupt_disable(TIMER3, TIMER_INT_UP);
	timer_disable(TIMER3);
}


uint32_t AutoUpTime_cnt = 0;
void TIMER3_IRQHandler(void)
{
    
    if (timer_interrupt_flag_get(TIMER3, TIMER_INT_FLAG_UP))
    {
        timer_interrupt_flag_clear(TIMER3, TIMER_INT_FLAG_UP);
		AutoUpTime_cnt++;
	}

}

void  APP_MB_Task(void *p_arg)
{
//    OS_ERR      os_err;
	rt_err_t result=0;
	uint16_t uart_cnt = 0;		
    EEProm_Init();
    Rd_Dev_Param();
    UART1_Init(&UartComm);
    Timer3_Init();


    while (1)
    {

		if(uart_cnt != uart_init)
		{
			uart_cnt = uart_init;
			Rd_Dev_Param();
			UART1_Init(&UartComm);
		}
		
		result=rt_sem_take(modbus_sem,	/* 获取信号量 */
                UART_WAIT_TIME); 	/* 等待时间：100ms */
#if IWDG_EN > 0
        fwdgt_counter_reload();
#endif
        if(Bit_SET == RunVar.bInitializing)
        {
//            if((OSTimeGet(&os_err) - RunVar.uartInitTime) < (3 * OS_CFG_TICK_RATE_HZ))
//            {
//                continue;
//            }
            RunVar.bInitializing = Bit_RESET;
        }

        if (result == RT_EOK)
        {

            MBRTU_Function();
			
        }
        else if (result == -RT_ETIMEOUT)
        {
			if (0 != AutoUpLoadTimeTransform(ProductPara.AutoUpLoadTime))       
            {
				timer_interrupt_enable(TIMER3, TIMER_INT_UP);
				timer_enable(TIMER3);
//				if ((UartRunCnt - UartRunCntPre) >= (AutoUpLoadTimeTransform(ProductPara.AutoUpLoadTime) * 1000))
				if(AutoUpTime_cnt == (AutoUpLoadTimeTransform(ProductPara.AutoUpLoadTime)))
				{
					AutoUpLoadFrame();
//					UartRunCntPre = UartRunCnt;
					
					timer_interrupt_disable(TIMER3, TIMER_INT_UP);
					timer_disable(TIMER3);
					
					AutoUpTime_cnt = 0;
				}
            }
//            else
//            {
//                UartRunCntPre = UartRunCnt;
//            }
        }
		
    }
}



static int ModbusProcessInit(void)
{
	rt_thread_t mem_thread;

	mem_thread = rt_thread_create("AppThread",
								APP_MB_Task,
								RT_NULL,
								256,
								1,
								1);

	RT_ASSERT(mem_thread != RT_NULL);
	rt_thread_startup(mem_thread);
	
	return RT_EOK;
	
}
INIT_PREV_EXPORT(ModbusProcessInit);


