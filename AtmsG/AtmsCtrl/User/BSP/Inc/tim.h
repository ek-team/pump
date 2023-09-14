#ifndef __TIM_H
#define __TIM_H

#include "stm32f1xx_hal.h"
#include "sys.h"

#define PWM_NA         3   
#define PWM_NB         11
#define PWM_CYCLE      20                         //周期计数
#define TIM_FREQ       400                        //时钟频率

#define HOLD_TIME      200                        //保持时间200ms
#define TIM_PRESCALER  71                         //预分频,时钟频率为:72MHz/(71+1)=1MHz
#define TIM_PERIOD     1000000/TIM_FREQ/PWM_CYCLE //时钟周期为:1MHz/1000/40=25

void N_SET(uint8_t Gate,int16_t EN);
void DMA_Init(void);
void TIM_Init(void);
void SysClk_Init(void);
extern void xPortSysTickHandler(void);
extern TIM_HandleTypeDef htimx;

#endif
