#ifndef _SYS_CFG_H_
#define _SYS_CFG_H_

/* REMAP������Ϊ�Ƿ�֧��IAP���ܵĿ��أ����֧�֣���ú�Ҫ����0��
���⣬���REMAP����0����ô��Ҫ�ڱ�����linkѡ����������
vector table ��ַΪ8003000��memory region �е�rom��ַ��8003000��ʼ*/
#define REMAP    1
#define MODBUS_CAN_OPT    0

/* ���Ź����� */
#define IWDG_EN  1

/*ID����ѡ�񣬾ɵ��豸ID��12λ�ģ��µ��豸ID��11λ��
��ʹ��11λID��*/
#define DEFAULT_DEV_ID_LEN		11

/* ���������������Ƿ������ȡ�����ۼ� ������ʹ�ã����
������������04����ַ0x0008��0x000A����ȡ�����ۼ�ֵ����ֵ
��������Flash�� */
#define READ_IN_OUT_SUM         1

#endif

