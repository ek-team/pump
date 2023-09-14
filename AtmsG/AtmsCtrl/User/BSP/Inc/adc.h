#ifndef __ADC_H
#define __ADC_H

#include "stm32f1xx_hal.h"

#define ADC_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()
#define ADC_CLK_DISABLE()              __HAL_RCC_ADC1_CLK_DISABLE()
#define ADC_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx                           ADC1
#define ADC_DMA_CH                     DMA1_Channel1

#define TMP_ADC_DMA_CH                 ADC_CHANNEL_TEMPSENSOR 

#define HAND_ADC_GPIO_CLK_ENABLE() 	   __HAL_RCC_GPIOB_CLK_ENABLE()
#define HAND_ADC_GPIO                  GPIOB
#define HAND_ADC_GPIO_PIN              GPIO_PIN_1
#define HAND_ADC_DMA_CH                ADC_CHANNEL_9

#define PRESS_ADC_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define PRESS_ADC_GPIO                 GPIOC
#define PRESS_ADC_GPIO_PIN             GPIO_PIN_5
#define PRESS_ADC_DMA_CH               ADC_CHANNEL_15

#define TMP_ADC                        0
#define HAND_ADC                       1
#define PRESS_ADC                      2
#define ADC_NUM                        3

void ADC_Init(void);

extern DMA_HandleTypeDef hdma_adc;	
extern ADC_HandleTypeDef hadc;

#endif
