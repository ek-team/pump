#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f1xx_hal.h"

#define HMI_PW_GPIO_CLK_ENABLE() 	  __HAL_RCC_GPIOB_CLK_ENABLE()
#define HMI_PW_GPIO 						    GPIOB
#define HMI_PW_GPIO_PIN 				    GPIO_PIN_5

#define U3_PW_GPIO_CLK_ENABLE() 	  __HAL_RCC_GPIOB_CLK_ENABLE()
#define U3_PW_GPIO 						      GPIOB
#define U3_PW_GPIO_PIN 				      GPIO_PIN_8

#define LED_RUN_GPIO_CLK_ENABLE() 	__HAL_RCC_GPIOB_CLK_ENABLE()
#define LED_RUN_GPIO 						    GPIOB
#define LED_RUN_GPIO_PIN 				    GPIO_PIN_4

void GPIO_Init(void);

#endif
