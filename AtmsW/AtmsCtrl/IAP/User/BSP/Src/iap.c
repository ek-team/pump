#include "iap.h"

uint8_t APP_Flag = 0;
uint16_t APP_Crc = 0;
uint16_t APP_Len = 0;

void Clear_IAP_Data()
{
	 uint16_t Empty = {0xFFFF};
	 STMFLASH_Write(IAP_DATA_ADDR+4,(uint16_t*)&Empty,1);
}

void Read_IAP_Data()
{	    
	 uint16_t RxBuf[3] = {0};
	 STMFLASH_Read(IAP_DATA_ADDR,RxBuf,3);
	 if(RxBuf[2] == 0xA55A)
	 {
	    APP_Flag = 1;
		  APP_Len = RxBuf[0];
		  APP_Crc = RxBuf[1];
	 }
	 else
	 {
	   APP_Flag = 0;
		 APP_Len = 0;
		 APP_Crc = 0;
	 }
}

uint8_t Calculate_XOR(uint8_t *Data,uint16_t Len)
{
  uint8_t Crc = 0;
	for(uint16_t i=0;i<Len;i++)
	{
	   Crc ^= *(Data + i);
	}
	return Crc;
}

void IAP_Write_App_Bin(uint32_t ulStartAddr, uint8_t * pBin_Buf, uint32_t ulBufLength)
{
	uint16_t us, usCtr=0, usTemp;
	uint32_t ulAdd_Write = ulStartAddr;                               //当前写入的地址
	uint8_t * pData = pBin_Buf;
	
	for(us = 0;us < ulBufLength; us += 2)
	{						    
		usTemp = (uint16_t)pData[1]<<8;
		usTemp += (uint16_t)pData[0];	  
		pData += 2;                                                     //偏移2个字节
		ulBuf_Flash_App[usCtr++] = usTemp;	    
		if(usCtr == 1024)
		{
			usCtr = 0;
			STMFLASH_Write(ulAdd_Write,ulBuf_Flash_App,1024);	
			ulAdd_Write += 2048;                                          //偏移2048
		}
	}
	if(usCtr) STMFLASH_Write(ulAdd_Write,ulBuf_Flash_App,usCtr);      //将最后的一些内容字节写进去.  
}

void IAP_ExecuteApp(uint32_t addr)
{
  typedef void(*pfun)(void);
  static pfun jumpToApp;  
  __IO uint32_t jumpAddr;
	
	if(addr == APP_ADDR)
	{
		__set_PRIMASK(1);
    if(((*(__IO uint32_t *)addr) & 0x2FFE0000) == 0x20000000)
    {
      jumpAddr = *(__IO uint32_t *)(addr + 4);
      jumpToApp = (pfun)jumpAddr;
      __set_MSP(*(__IO uint32_t *)addr);
      jumpToApp();
    }
	}
	if(addr == IAP_ADDR)
	{
	  HAL_RCC_DeInit();
    HAL_UART_DeInit(&husart1);
	  HAL_UART_DeInit(&husart3);
    __disable_irq();
		SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL = 0;
    if (((*(__IO uint32_t *)addr) & 0x2FFE0000) == 0x20000000)
    {
      jumpAddr = *(__IO uint32_t *)(addr + 4);
      jumpToApp = (pfun)jumpAddr;
      __set_PSP(*(__IO uint32_t *)addr); 
      __set_CONTROL(0);
      __set_MSP(*(__IO uint32_t *)addr);
      jumpToApp();
    }
	}
}	
