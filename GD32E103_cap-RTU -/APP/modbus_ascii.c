#include "bsp.h"

extern uint8_t UART1_RXBuff[MAX_USART1_DATA_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

extern BitAction UartRecvNewData ;
extern BitAction UartRecvFrameOK ;

uint8_t UsartErrCnt;
uint8_t cyAsciiBuff[MAX_USART1_DATA_LEN];

//static uint8_t g_cyRevState = ASCII_IDLE_STATE;
static uint8_t g_cyRevBufffLen = 0;


/************************************************************************************************************************************************************************
** ��Ȩ��   2015-2025, ��������Ϊ�Ƽ���չ���޹�˾
** �ļ���:  Modbus_Ascii.h
** ����:    �½�
** �汾:    V1.0.0
** ����:    2015-07-10
** ����:    �����㷨
** ����:         
*************************************************************************************************************************************************************************
** �޸���:          No
** �汾:  		
** �޸�����:    No 
** ����:            No
*************************************************************************************************************************************************************************/
//**************************************************************************************************
// ����         : MODBUS_ASCII_HexToAscii()
// ��������     : 2015-07-24
// ����         : �½�
// ����         : ʮ��������תASCII��
// �������     : ʮ��������(uint8_t cyHexData)
// �������     : ASCII��(uint8_t *pCyAsciiBuf)
// ���ؽ��     : ��
// ע���˵��   : 
// �޸�����     :
//**************************************************************************************************
void MODBUS_ASCII_HexToAscii(uint8_t cyHexData, uint8_t *pCyAsciiBuf)
{
	uint8_t cyTemp;
	
	cyTemp = cyHexData / 16;
	if (10 > cyTemp) //0-9
	{
		*(pCyAsciiBuf + 0) = cyTemp + '0';
	}
	else
	{
		*(pCyAsciiBuf + 0) = (cyTemp - 10) + 'A';
	}

	cyTemp = cyHexData % 16;
	if (10 > cyTemp) //0-9
	{
		*(pCyAsciiBuf + 1) = cyTemp + '0';
	}
	else
	{
		*(pCyAsciiBuf + 1) = (cyTemp - 10) + 'A';
	}
}


//**************************************************************************************************
// ����         : MODBUS_ASCII_AsciiToHex()
// ��������     : 2015-07-24
// ����         : �½�
// ����         : ASCII��תʮ��������
// �������     : ASCII��(uint8_t *pCyAsciiBuf)
// �������     : ��
// ���ؽ��     : ʮ��������(uint8_t cyHexData)
// ע���˵��   : 
// �޸�����     :
//**************************************************************************************************
uint8_t MODBUS_ASCII_AsciiToHex(uint8_t *pCyAsciiBuf)
{
	uint8_t cyHexData;
	
	cyHexData = 0;
	if ('A' > *(pCyAsciiBuf + 0) ) //0-9
	{
		cyHexData += *(pCyAsciiBuf + 0) - '0';
	}
	else if ('a' > *(pCyAsciiBuf + 0) ) //��д
	{
		cyHexData += *(pCyAsciiBuf + 0) - 'A' + 10;
	}
	else
	{
		cyHexData += *(pCyAsciiBuf + 0) - 'a' + 10;
	}

	cyHexData *= 16;
	
	if ('A' > *(pCyAsciiBuf + 1) ) //0-9
	{
		cyHexData += *(pCyAsciiBuf + 1) - '0';
	}
	else if ('a' > *(pCyAsciiBuf + 1) ) //��д
	{
		cyHexData += *(pCyAsciiBuf + 1) - 'A' + 10;
	}
	else
	{
		cyHexData += *(pCyAsciiBuf + 1) - 'a' + 10;
	}
	
	return (cyHexData);
}

//**************************************************************************************************
// ����         : MODBUS_ASCII_GetLrc()
// ��������     : 2015-07-24
// ����         : �½�
// ����         : ��ȡLRCֵ
// �������     : ASCII�봮(uint8_t *pCyAsciiBuf), ���ݳ���(uint8_t cyLen)
// �������     : ��
// ���ؽ��     : LRCֵ(uint8_t cyLrcVal)
// ע���˵��   : 
// �޸�����     :
//**************************************************************************************************
uint8_t MODBUS_ASCII_GetLrc(uint8_t *pCyAsciiBuf, uint8_t cyLen)
{
	uint8_t i;
	uint8_t cyLrcVal;
	
	if (1 == (cyLen % 2) )
	{
		return 0;
	}
	
	cyLen /= 2;
	cyLrcVal = 0;
	for (i = 0; i < cyLen; i++)
	{
		cyLrcVal += MODBUS_ASCII_AsciiToHex(pCyAsciiBuf + i * 2);
	}
	//����
	cyLrcVal = ~cyLrcVal;
	cyLrcVal += 1;
	
	return (cyLrcVal);
}


//**************************************************************************************************
// ����         : MODBUS_ASCII_AsciiPacketToRtuPacket()
// ��������     : 2015-07-24
// ����         : �½�
// ����         : ASCII���ݰ�ת��RTU���ݰ�
// �������     : ASCII�봮(uint8_t *pCyAsciiBuf),  ASCII�봮������(uint8_t cyAsciiLen)
// �������     : RTU�봮(uint8_t *pCyRtuBuf),
// ���ؽ��     : 0:����������RTU�봮������(uint8_t cyRtuLen)
// ע���˵��   : 
// �޸�����     :
//**************************************************************************************************
uint8_t MODBUS_ASCII_AsciiPacketToRtuPacket(uint8_t *pCyAsciiBuf, uint8_t cyAsciiLen, uint8_t *pCyRtuBuf)
{
	uint8_t i;
	uint8_t cyRtuLen;
	
	if (1 == (cyAsciiLen % 2) )
	{
		return 0;
	}
	
	cyRtuLen = cyAsciiLen / 2;
	for (i = 0; i < cyRtuLen; i++)
	{
		*(pCyRtuBuf + i) = MODBUS_ASCII_AsciiToHex(pCyAsciiBuf + i * 2);
	}
	
	return (cyRtuLen);
}

//**************************************************************************************************
// ����         : MODBUS_ASCII_RtuPacketToAsciiPacket()
// ��������     : 2015-07-24
// ����         : �½�
// ����         : RTU���ݰ�ת��ASCII���ݰ�
// �������     : RTU�봮(uint8_t *pCyRtuBuf),  RTU�봮������(uint8_t cyRtuLen)
// �������     : ASCII�봮(uint8_t *pCyAsciiBuf),
// ���ؽ��     : ASCII�봮������(uint8_t cyAsciiLen)
// ע���˵��   : 
// �޸�����     :
//**************************************************************************************************

uint8_t MODBUS_ASCII_RtuPacketToAsciiPacket(uint8_t *pCyRtuBuf, uint8_t cyRtuLen, uint8_t *pCyAsciiBuf)
{
	uint8_t i;
	uint8_t cyAsciiLen;
	
	cyAsciiLen = cyRtuLen * 2;
	for (i = 0; i < cyRtuLen; i++)
	{
		MODBUS_ASCII_HexToAscii( *(pCyRtuBuf + i), pCyAsciiBuf + i * 2);
	}
	
	return (cyAsciiLen);
}


//**************************************************************************************************
// ����         : MODBUS_ASCII_HandlRevData()
// ��������     : 2015-07-27
// ����         : �½�
// ����         : ASCII������������
// �������     : ��������(uint8_t cyRevData)
// �������     : ��
// ���ؽ��     : ��
// ע���˵��   :
// �޸�����     :
//**************************************************************************************************
void MODBUS_ASCII_HandlRevData(uint8_t cyRevData)
{
//    OS_ERR  os_err;
    
    if(g_cyRevBufffLen > MAX_USART1_DATA_LEN)
    {
      //UsartErrCnt = 30;
      g_cyRevBufffLen = 0;
      //USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
      //USART_Cmd(USART1, DISABLE);
      return;
    }
    
    if(0x3A == cyRevData)
    {
      g_cyRevBufffLen = 0;
      UART1_RXBuff[g_cyRevBufffLen++] = cyRevData;
    }
    else if((0x0A == cyRevData)&&(3 < g_cyRevBufffLen))
    {
      if(0x0D == UART1_RXBuff[g_cyRevBufffLen - 1])
      {
        UART1_RXBuff[g_cyRevBufffLen++] = cyRevData;
        
        UartRecvFrameOK = Bit_SET;
//        OSTaskSemPost((OS_TCB *)&AppTaskMBTCB, (OS_OPT) OS_OPT_POST_NONE,
//                          (OS_ERR *)&os_err);
		  rt_sem_release(modbus_sem);   //�ͷŶ�ֵ�ź��� 
      }
    }
    else
    {
      UART1_RXBuff[g_cyRevBufffLen++] = cyRevData;
    }
    
    /*switch(g_cyRevState)
    {
        case ASCII_HEAD_STATE:  
          if (ASCII_HEAD_DATA == cyRevData)
          {
                  g_cyRevBufffLen = 0;
          }
          else if (0x0D == cyRevData)
          {
                  g_cyRevState = ASCII_END_STATE;
          }
          UART1_RXBuff[g_cyRevBufffLen] = cyRevData;
          g_cyRevBufffLen++;
          if (MAX_USART1_DATA_LEN <= g_cyRevBufffLen)
          {
                  g_cyRevState = ASCII_IDLE_STATE;
          }
          break;
          
          case ASCII_END_STATE:   
            if (ASCII_HEAD_DATA == cyRevData)
            {
                  g_cyRevBufffLen = 0;
                  g_cyRevState = ASCII_HEAD_STATE;
                  UART1_RXBuff[g_cyRevBufffLen] = cyRevData;
                  g_cyRevBufffLen++;
            }
            else if (0x0A == cyRevData)
            {
                    g_cyRevState = ASCII_IDLE_STATE;
                    
                    UART1_RXBuff[g_cyRevBufffLen] = cyRevData;
                    g_cyRevBufffLen++;
    
                    //���ͳɹ�����һ�����ݵı�־
                    UartRecvFrameOK = Bit_SET;
					
		   OSTaskSemPost((OS_TCB *)&AppTaskMBTCB, (OS_OPT) OS_OPT_POST_NONE,
                          (OS_ERR *)&os_err);
            }
            else
            {
                    g_cyRevState = ASCII_IDLE_STATE;
            }
            break; 
            
            case ASCII_IDLE_STATE:  
            if (ASCII_HEAD_DATA == cyRevData)
            {
                    g_cyRevBufffLen = 0;
                    g_cyRevState = ASCII_HEAD_STATE;
                    UART1_RXBuff[g_cyRevBufffLen] = cyRevData;
                    g_cyRevBufffLen++;
            }
            break;
           default:               
             g_cyRevState = ASCII_IDLE_STATE; 
           break;
    }*/
}

//**************************************************************************************************
// ����         : MODBUS_ASCII_CheckAscii()
// ��������     : 2015-07-27
// ����         : �½�
// ����         : �����Ƿ���Ascii��
// �������     : ASCII�봮(uint8_t *pCyAsciiBuf), ���ݳ���(uint8_t cyLen)
// �������     : ��
// ���ؽ��     : ���(0 ��ȫ�ǣ� 1 ȫ��)
// ע���˵��   : 
// �޸�����     :
//**************************************************************************************************
uint8_t MODBUS_ASCII_CheckAscii(uint8_t *pCyAsciiBuf, uint8_t cyLen)
{
	uint8_t i;
	
	for (i = 0; i < cyLen; i++)
	{
		if ('0' > *(pCyAsciiBuf + i) )
		{
			break;
		}
		
		if ( ('9' < *(pCyAsciiBuf + i) ) && ( *(pCyAsciiBuf + i) < 'A' ) )
		{
			break;
		}
		
		if ( ('F' < *(pCyAsciiBuf + i) ) && ( *(pCyAsciiBuf + i) < 'a' ) )
		{
			break;
		}
		
		if ('f' < *(pCyAsciiBuf + i) )
		{
			break;
		}
	}
	
	
	if (i == cyLen)
	{
		return (1);
	}
	
	return (0);
}

//**************************************************************************************************
// ����         : MODBUS_ASCII_RecvData()
// ��������     : 2015-07-27
// ����         : �½�
// ����         : ����һ������
// �������     : ���ݴ�(cyRecvBuff)
// �������     : ���ݳ���(uint8_t *cyLen)
// ���ؽ��     : ִ�н��(0 û�н������ݣ� 1 �������ݳ��ֲ���ASCII�룬 2 Ч������� 3 �ɹ�)
// ע���˵��   :
// �޸�����     :
//**************************************************************************************************

uint8_t MODBUS_ASCII_RecvData(uint8_t* cyRecvBuff, uint8_t *pCyLen)
{
    uint8_t cyLrc;
	
    if (((uint8_t*)NULL) == cyRecvBuff)
    {
        return 0;
    }

    if ((Bit_RESET == UartRecvFrameOK) || (0 == g_cyRevBufffLen))
    {
        return 0;
    }
    
    UartRecvFrameOK = Bit_RESET;
    
    if (0 == MODBUS_ASCII_CheckAscii(&UART1_RXBuff[1], g_cyRevBufffLen - 3) )
    {
    	//�������ݴ���
    	return 1;
    }

    cyLrc = MODBUS_ASCII_GetLrc(&UART1_RXBuff[1], g_cyRevBufffLen - 5);
    if (cyLrc != MODBUS_ASCII_AsciiToHex(&UART1_RXBuff[g_cyRevBufffLen - 4]) )
    {
    	//��������Ч�����
    	return 2;
    }

    *pCyLen = MODBUS_ASCII_AsciiPacketToRtuPacket(&UART1_RXBuff[1], g_cyRevBufffLen - 5, cyRecvBuff);
	
    return (3);
}


//**************************************************************************************************
// ����         : MODBUS_ASCII_SendData()
// ��������     : 2015-07-13
// ����         : �½�
// ����         : ����һ������
// �������     : ���ݴ�(uint8_t *cySendBuff), ���ݳ���(cyLen) (���ݳ��� С�� 123)
// �������     : ��
// ���ؽ��     : ִ�н��(0 ʧ�ܣ� ���ݳ��� �ɹ�)
// ע���˵��   :
// �޸�����     :
//**************************************************************************************************
uint8_t MODBUS_ASCII_SendData(uint8_t *cySendBuff, uint8_t cyLen)
{
    uint8_t cyLrc;
    uint8_t cyAsciiLen;
	
    if ( (0 == cyLen) || ( ((uint8_t*)NULL) == cySendBuff) )
    {
        return 0;
    }
    
    if ( (cyLen * 2 + 5) > MAX_USART1_DATA_LEN)
    {
    	return 0;
    }
    
    cyAsciiBuff[0] = ASCII_HEAD_DATA;
    cyAsciiLen = 1;
    
    cyAsciiLen += MODBUS_ASCII_RtuPacketToAsciiPacket(cySendBuff, cyLen, &cyAsciiBuff[1]);
    cyLrc = MODBUS_ASCII_GetLrc(&cyAsciiBuff[1], cyAsciiLen - 1);
    MODBUS_ASCII_HexToAscii(cyLrc, &cyAsciiBuff[cyAsciiLen]);
    cyAsciiLen += 2;
    cyAsciiBuff[cyAsciiLen] = 0x0D;
    cyAsciiLen++;
    cyAsciiBuff[cyAsciiLen] = 0x0A;
    cyAsciiLen++;
    
    return ( UART1_SendData(cyAsciiBuff, cyAsciiLen) );
}