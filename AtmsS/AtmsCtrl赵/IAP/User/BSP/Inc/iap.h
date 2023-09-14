#ifndef __IAP_H__
#define	__IAP_H__

#include "main.h"
#include "flash.h"
#include "stm32f1xx_hal.h"

typedef void (*pIapFun_TypeDef)(void);  //定义一个函数类型的参数

#define IAP_ADDR       0x08000000  	    //IAP程序起始地址
#define APP_ADDR       0x08004000  	    //应用程序起始地址

#define IAP_DATA_ADDR  0x0803F820       //IAP数据保存地址
#define APP_FLASH_LEN  40*1024u         //定义固件最大容量

static uint16_t ulBuf_Flash_App[1024];

void Clear_IAP_Data(void);
void Read_IAP_Data(void);
void IAP_ExecuteApp(uint32_t appxaddr);
uint8_t Calculate_XOR(uint8_t *Data,uint16_t Len);
void IAP_Write_App_Bin(uint32_t appxaddr,uint8_t *appbuf,uint32_t applen);

extern uint8_t APP_Flag;
extern uint16_t APP_Crc;
extern uint16_t APP_Len;

#endif
