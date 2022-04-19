#ifndef _SYS_CFG_H_
#define _SYS_CFG_H_

/* REMAP宏是作为是否支持IAP功能的开关，如果支持，则该宏要大于0。
另外，如果REMAP大于0，那么需要在编译器link选型哪里设置
vector table 地址为8003000，memory region 中的rom地址从8003000开始*/
#define REMAP    1
#define MODBUS_CAN_OPT    0

/* 看门狗开关 */
#define IWDG_EN  1

/*ID长度选择，旧的设备ID是12位的，新的设备ID是11位的
则使用11位ID。*/
#define DEFAULT_DEV_ID_LEN		11

/* 本宏是用来设置是否允许读取进出累计 ，测试使用，如果
允许，则在命令04，地址0x0008和0x000A处读取进出累计值，该值
不保存在Flash里 */
#define READ_IN_OUT_SUM         1

#endif

