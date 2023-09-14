#ifndef __ONE_H
#define	__ONE_H

#include "stm32f1xx_hal.h"

#define DS18B20_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOA_CLK_ENABLE()
#define DS18B20_GPIO                                GPIOA
#define DS18B20_GPIO_PIN                            GPIO_PIN_5

#define DS18B20_GPIO_LOW()                          HAL_GPIO_WritePin(DS18B20_GPIO,DS18B20_GPIO_PIN,GPIO_PIN_RESET) 
#define DS18B20_GPIO_HIGH()                         HAL_GPIO_WritePin(DS18B20_GPIO,DS18B20_GPIO_PIN,GPIO_PIN_SET)
#define DS18B20_Data_IN()	                          HAL_GPIO_ReadPin(DS18B20_GPIO,DS18B20_GPIO_PIN)

void  Tmp_Init(void);
float BSP_Get_Tmp_Skip_Rom(void);
float BSP_Get_Tmp_Match_Rom(uint8_t *DS18B20_ID);

uint8_t DS18B20_Init(void);
uint8_t DS18B20_Presence(void);
uint8_t DS18B20_Read_Bit(void);
uint8_t DS18B20_Read_Byte(void);

void DS18B20_Read_Id(uint8_t *DS18B20_ID);
void DS18B20_Mode_IPU(void);
void DS18B20_Mode_Out_PP(void);
void DS18B20_RST(void);
void DS18B20_Write_Byte(uint8_t dat);
void DS18B20_Skip_Rom(void);
void DS18B20_Match_Rom(void);

#endif
