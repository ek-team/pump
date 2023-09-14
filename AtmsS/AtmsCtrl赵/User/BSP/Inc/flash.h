#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f1xx_hal.h"

#define STM32_FLASH_SIZE        256  //所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN        1    //stm32芯片内容FLASH写入使能(0:禁用;1:使能)

uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //读出半字

void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len );	//指定地址开始写入指定长度的数据
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len );					            //指定地址开始读取指定长度数据
void STMFLASH_Write( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite );		//从指定地址开始写入指定长度的数据
void STMFLASH_Read( uint32_t ReadAddr, uint16_t * pBuffer, uint16_t NumToRead );   	  //从指定地址开始读出指定长度的数据

#endif
