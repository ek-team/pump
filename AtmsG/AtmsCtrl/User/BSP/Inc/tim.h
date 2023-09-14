#ifndef __TIM_H
#define __TIM_H

#include "stm32f1xx_hal.h"
#include "sys.h"

#define PWM_NA         3   
#define PWM_NB         11
#define PWM_CYCLE      20                         //���ڼ���
#define TIM_FREQ       400                        //ʱ��Ƶ��

#define HOLD_TIME      200                        //����ʱ��200ms
#define TIM_PRESCALER  71                         //Ԥ��Ƶ,ʱ��Ƶ��Ϊ:72MHz/(71+1)=1MHz
#define TIM_PERIOD     1000000/TIM_FREQ/PWM_CYCLE //ʱ������Ϊ:1MHz/1000/40=25

void N_SET(uint8_t Gate,int16_t EN);
void DMA_Init(void);
void TIM_Init(void);
void SysClk_Init(void);
extern void xPortSysTickHandler(void);
extern TIM_HandleTypeDef htimx;

#endif
