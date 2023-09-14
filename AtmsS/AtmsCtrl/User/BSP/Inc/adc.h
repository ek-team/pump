#ifndef __ADC_H
#define __ADC_H

#include "stm32f1xx_hal.h"

#define ADC_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()
#define ADC_CLK_DISABLE()              __HAL_RCC_ADC1_CLK_DISABLE()
#define ADC_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
#define ADCx                           ADC1
#define ADC_DMA_CH                     DMA1_Channel1

#define TMP_ADC_DMA_CH                 ADC_CHANNEL_TEMPSENSOR 

#define PRESS_ADC_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define PRESS_ADC_GPIO                 GPIOC

#define PRE1_ADC_GPIO_PIN              GPIO_PIN_5
#define PRE1_ADC_DMA_CH                ADC_CHANNEL_15

#define PRE2_ADC_GPIO_PIN              GPIO_PIN_3
#define PRE2_ADC_DMA_CH                ADC_CHANNEL_13

#define PRE3_ADC_GPIO_PIN              GPIO_PIN_2
#define PRE3_ADC_DMA_CH                ADC_CHANNEL_12

#define PRE4_ADC_GPIO_PIN              GPIO_PIN_1
#define PRE4_ADC_DMA_CH                ADC_CHANNEL_11

#define PRE5_ADC_GPIO_PIN              GPIO_PIN_0
#define PRE5_ADC_DMA_CH                ADC_CHANNEL_10

#define TMP_ADC                        0
#define PRE1_ADC                       1
#define PRE2_ADC                       2
#define PRE3_ADC                       3
#define PRE4_ADC                       4
#define PRE5_ADC                       5
#define ADC_NUM                        6

void ADC_Init(void);

extern DMA_HandleTypeDef hdma_adc;	
extern ADC_HandleTypeDef hadc;

#endif
