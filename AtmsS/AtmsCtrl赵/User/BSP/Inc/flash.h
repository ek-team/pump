#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f1xx_hal.h"

#define STM32_FLASH_SIZE        256  //��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN        1    //stm32оƬ����FLASHд��ʹ��(0:����;1:ʹ��)

uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //��������

void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len );	//ָ����ַ��ʼд��ָ�����ȵ�����
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len );					            //ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite );		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read( uint32_t ReadAddr, uint16_t * pBuffer, uint16_t NumToRead );   	  //��ָ����ַ��ʼ����ָ�����ȵ�����

#endif
