#ifndef __HTX_H
#define __HTX_H

#include "stm32f1xx_hal.h"
#include "main.h"

void Send_Zero(void);
void Send_Startup(uint16_t Cmd,uint8_t Cs);
void Send_State(uint8_t State,uint8_t Cs);

#endif
