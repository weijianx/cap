#include "BSP.h"

extern uint8_t Dev_Id[DEFAULT_DEV_ID_LEN];


uint8_t const User_Default_Param[PRO_DEFAULT_LEN] =
{
	0x01,								//������ʼ����־λ

	0x01,								//�豸��ַ
	0x01,								//������
	0x00,								//��żУ��
	0x00,								//�˲�ϵ��
	0x00,								//�Զ���������
	0x3F, 0x80, 0x00, 0x00,			//����ϵ��K
	0x00, 0x00, 0x00, 0x00,			//����ϵ��B
	0x02,								//�����λ
	0x00,								//���ģʽ

	0x00,								//�Ƿ񶳽��豸

	0x00, 0x0C, 0x48, 0x5D,			//������ֵ
	0x00, 0x0D, 0xCF, 0x63,			//�����̵���ֵ
	0x00, 0x00, 0x00, 0x00,			//���DAֵ
	0x45, 0x3B, 0x80, 0x00,			//����DAֵ
	
	0x00, 0x00, 0x00, 0x00,			//��������
	0x00, 0x00, 0x00, 0x00,			//����������ֵ
	0x00, 0x00, 0x00, 0x00,			//���������̵���ֵ
	0x3F, 0x80, 0x00, 0x00,			//�¶�ϵ��K
	0x00, 0x00, 0x00, 0x00,			//�¶�ϵ��B
	0x3F, 0x80, 0x00, 0x00,			//�¶�ϵ��K
	0x00, 0x00, 0x00, 0x00,			//�¶�ϵ��B
	0x00, 0x00, 0x00, 0x00,
};

uint8_t Cur_Param[PRO_DEFAULT_LEN] = {0};
uint8_t Calib_Param[CALIB_DEFAULT_LEN] = {0};
uint8_t CALIB_Data[2] = {0};

ProductParaTypeDef ProductPara;
RunVarTypeDef RunVar;
UartCommTypeDef UartComm;
UserParamTypeDef UserParam;


//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
    IIC_Init();
}

//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ
//����ֵ  :����������
uint8_t I2C1_ReadByte(uint16_t DriverAddr, uint16_t ReadAddr)
{
//    CPU_SR_ALLOC();
//    CPU_CRITICAL_ENTER();

    uint8_t temp = 0;
    IIC_Start();

    if (EE_TYPE > AT24C16)
    {
        IIC_Send_Byte(DriverAddr);	   //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(ReadAddr >> 8);  //���͸ߵ�ַ
        IIC_Wait_Ack();
    }
    else
	{ 
		IIC_Send_Byte(DriverAddr + ((ReadAddr / 256) << 1));        //����������ַ0XA0,д����
	}
	
    IIC_Wait_Ack();

    IIC_Send_Byte(ReadAddr % 256);     //���͵͵�ַ

    IIC_Wait_Ack();

    IIC_Start();

    IIC_Send_Byte(0XA1);           //�������ģʽ

    IIC_Wait_Ack();

    temp = IIC_Read_Byte(0);

    IIC_Stop();//����һ��ֹͣ����

//    CPU_CRITICAL_EXIT();

    return temp;
}

//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
//DataToWrite:Ҫд�������
void I2C1_WriteByte(uint16_t DriverAddr, uint16_t WriteAddr, uint8_t DataToWrite)
{
//    CPU_SR_ALLOC();
    
    if(ProductPara.bFlashWEn == FLASH_WR_DISABLE)
    {
        return;
    }
    
//    CPU_CRITICAL_ENTER();

    IIC_Start();

    if (EE_TYPE > AT24C16)
    {
        IIC_Send_Byte(DriverAddr);	    //����д����
        IIC_Wait_Ack();
        IIC_Send_Byte(WriteAddr >> 8);  //���͸ߵ�ַ
    }
    else
    {
        IIC_Send_Byte(DriverAddr + ((WriteAddr / 256) << 1));        //����������ַ0XA0,д����
    }

    IIC_Wait_Ack();

    IIC_Send_Byte(WriteAddr % 256);     //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite);     //�����ֽ�
    IIC_Wait_Ack();
    IIC_Stop();//����һ��ֹͣ����
    Delay_Ms(2);

//    CPU_CRITICAL_EXIT();
}


//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void I2C1_ReadNBytes(uint16_t DriverAddr, uint16_t ReadAddr, uint16_t NumToRead, uint8_t *pBuffer)
{
    while (NumToRead)
    {
        *pBuffer++ = I2C1_ReadByte(DriverAddr, ReadAddr++);
        NumToRead--;
    }
}

//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void I2C1_WriteNBytes(uint16_t DriverAddr, uint16_t WriteAddr, uint16_t NumToWrite, const uint8_t *pBuffer)
{
    if(ProductPara.bFlashWEn == FLASH_WR_DISABLE)
    {
        return;
    }
    
    while (NumToWrite--)
    {
        I2C1_WriteByte(DriverAddr, WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}

void I2C1_WNBytesMul3T(uint16_t DriverAddr, uint16_t WriteAddr, uint16_t NumToWrite, const uint8_t *pBuffer)
{
    if(ProductPara.bFlashWEn == FLASH_WR_DISABLE)
    {
        return;
    }
    
    while (NumToWrite--)
    {
        I2C1_WriteByte(DriverAddr, WriteAddr, *pBuffer);
        I2C1_WriteByte(DriverAddr, WriteAddr + 2 * 0x80, *pBuffer);
        I2C1_WriteByte(DriverAddr, WriteAddr + 4 * 0x80, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}


void EEProm_Init(void)
{
    AT24CXX_Init();
}

void Switch_Fiter(uint8_t value)
{
	FilterStartFlag = Bit_RESET;
	switch(value)
	{	
		case 0:	HighFilterbufNum = 1; UserParam.FilterCycle = 1; break;

		case 1:	HighFilterbufNum = 1; UserParam.FilterCycle = 120; break;

		case 2:	HighFilterbufNum = 1; UserParam.FilterCycle = 240; break;

		case 3:	HighFilterbufNum = 1; UserParam.FilterCycle = 360; break;

		
		case 4:	HighFilterbufNum = 1; UserParam.FilterCycle = 600; break;

		case 5:	HighFilterbufNum = 2; UserParam.FilterCycle = 1200; break;

		case 6:	HighFilterbufNum = 2; UserParam.FilterCycle = 1800; break;

		case 7:	HighFilterbufNum = 3; UserParam.FilterCycle = 2400; break;

		case 8:	HighFilterbufNum = 5; UserParam.FilterCycle = 4800; break;
		
		case 9:	HighFilterbufNum = 10; UserParam.FilterCycle = 9600; break;
		
		default:
			break;
	}

	UserParam.FilterCycle /= HighFilterbufNum;
}


void Check_User_Backup(void)
{
    uint32_t i = 0, errn = 0;
    uint8_t bak1 = 0, bak2 = 0;
    
    for (i = 0; i < PRO_DEFAULT_LEN; i++)
    {
        errn = 0;
        
        bak1 = I2C1_ReadByte(EEPROM_ADDRESS, i + USER_DEFAULT_PARA_BAK1);
        bak2 = I2C1_ReadByte(EEPROM_ADDRESS, i + USER_DEFAULT_PARA_BAK2);
        if (Cur_Param[i] != bak1)
            errn |= 0x01;

        if (Cur_Param[i] != bak2)
            errn |= 0x02;

        if (bak1 != bak2)
            errn |= 0x04;

        if(0x00 == errn)
            continue;

        switch (errn)
        {
        case 0x03:
        	Cur_Param[i] = bak1;
        	I2C1_WriteByte(EEPROM_ADDRESS, RUN_ADDR_BASE + i, Cur_Param[i]);
            break;
        case 0x05:
            I2C1_WriteByte(EEPROM_ADDRESS, USER_DEFAULT_PARA_BAK1 + i, Cur_Param[i]);
            break;
        case 0x06:
            I2C1_WriteByte(EEPROM_ADDRESS, USER_DEFAULT_PARA_BAK2 + i, Cur_Param[i]);
            break;
        case 0x07:
            Cur_Param[i] = User_Default_Param[i];
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, RUN_ADDR_BASE + i, 1, &Cur_Param[i]);
            break;
        }
    }    
}

void Check_Calib_Backup(void)
{
    uint32_t i = 0, errn = 0;
    uint8_t bak1 = 0, bak2 = 0;

	for(i = 0; i < CALIB_DEFAULT_LEN; i++)
	{
	    errn = 0;
	    
	    bak1 = I2C1_ReadByte(EEPROM_ADDRESS, i + CALIB_DEFAULT_PARA_BAK1);
        bak2 = I2C1_ReadByte(EEPROM_ADDRESS, i + CALIB_DEFAULT_PARA_BAK2);
        if (Calib_Param[i] != bak1)
            errn |= 0x01;

        if (Calib_Param[i] != bak2)
            errn |= 0x02;

        if (bak1 != bak2)
            errn |= 0x04;

        if(0x00 == errn)
            continue;
        
        switch (errn)
        {
        case 0x03:
            Calib_Param[i] = bak1;
            I2C1_WriteByte(EEPROM_ADDRESS, CALIB_FLOATER_SCALE_1 + i, Calib_Param[i]);
            break;
        case 0x05:
            I2C1_WriteByte(EEPROM_ADDRESS, CALIB_DEFAULT_PARA_BAK1 + i, Calib_Param[i]);
            break;
        case 0x06:
            I2C1_WriteByte(EEPROM_ADDRESS, CALIB_DEFAULT_PARA_BAK2 + i, Calib_Param[i]);
            break;
        case 0x07:
			Calib_Param[i] = 0xFF;
            I2C1_WNBytesMul3T(EEPROM_ADDRESS, CALIB_FLOATER_SCALE_1 + i , 1, &Calib_Param[i]);
            break;
        }
	}
	
}

void Rd_Dev_Param(void)
{
	int i=0;
	
    ProductPara.bFlashWEn = FLASH_WR_ENABLE;
    
    if(User_Default_Param[0] != I2C1_ReadByte(EEPROM_ADDRESS, RUN_ADDR_BASE))	//��EEPROM�����ݣ�д�����ֵ
        I2C1_WNBytesMul3T(EEPROM_ADDRESS, RUN_ADDR_BASE, USER_DEFAULT_LEN, User_Default_Param);
    I2C1_ReadNBytes(EEPROM_ADDRESS, RUN_ADDR_BASE, PRO_DEFAULT_LEN, Cur_Param);
    I2C1_ReadNBytes(EEPROM_ADDRESS, CALIB_FLOATER_SCALE_1, CALIB_DEFAULT_LEN, Calib_Param);

	Check_User_Backup();
	Check_Calib_Backup();
	
	UartComm.SlaveAddr = Cur_Param[SLAVE_ADDR];
	UartComm.BaudRate = Cur_Param[BAUDRATE];
	UartComm.Parity = Cur_Param[PARITY];
	UserParam.FilterLevel = Cur_Param[FILTER_LEVEL];
	Switch_Fiter(UserParam.FilterLevel);
	ProductPara.AutoUpLoadTime = Cur_Param[AUTO_UPLOAD_TIME];
	
	UserParam.UserFlowK = HexToFloat(&Cur_Param[USER_FLOW_K]);
	UserParam.UserFlowB = HexToFloat(&Cur_Param[USER_FLOW_B]);
	
	ProductPara.OutputUnit = Cur_Param[OUTPUT_UNIT];
	UartComm.OutputMode = Cur_Param[OUTPUT_MODE];
	UartComm.Echo = Cur_Param[ECHOEN];

    ProductPara.CapMin = ArrToHex(&Cur_Param[CAPMIN]);
    ProductPara.CapMax = ArrToHex(&Cur_Param[CAPMAX]);
	DalidValueZone = (ProductPara.CapMax - ProductPara.CapMin) * 0.01;
    ProductPara.DAMin = HexToFloat(&Cur_Param[DAMIN]);
    ProductPara.DAMax = HexToFloat(&Cur_Param[DAMAX]);
    ProductPara.Def_Range = HexToFloat(&Cur_Param[DEF_RANGE]);
    ProductPara.Def_CapMin = HexToFloat(&Cur_Param[DEF_CAPMIN]);
    ProductPara.Def_CapMax = HexToFloat(&Cur_Param[DEF_CAPMAX]);

	ProductPara.Temp_K = HexToFloat(&Cur_Param[TEMP_K]);
	ProductPara.Temp_B = HexToFloat(&Cur_Param[TEMP_B]);
	ProductPara.Temp2_K = HexToFloat(&Cur_Param[TEMP2_K]);
	ProductPara.Temp2_B = HexToFloat(&Cur_Param[TEMP2_B]);
    ProductPara.LiquitHeightforDAMax = HexToFloat(&Cur_Param[HEIGHTDAMAX]);
    
	for(i=0; i<8; i++)
	{
	    I2C1_ReadNBytes(EEPROM_ADDRESS, CALIB_FLOATER_DA_1 + 2 * i, 2, CALIB_Data);
	    ProductPara.Floater.DACode[i] = CALIB_Data[1] * 256 + CALIB_Data[0];
		I2C1_ReadNBytes(EEPROM_ADDRESS, CALIB_FLOATER_SCALE_1 + 2 * i, 2, CALIB_Data);
		ProductPara.Floater.Scale[i] = CALIB_Data[1] * 256 + CALIB_Data[0];
	}
	
    I2C1_ReadNBytes(EEPROM_ADDRESS, DEV_ID, DEFAULT_DEV_ID_LEN, Dev_Id);

    ProductPara.bFlashWEn = FLASH_WR_DISABLE;
}

