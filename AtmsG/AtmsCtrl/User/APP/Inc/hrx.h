#ifndef __HRX_H
#define __HRX_H

#include "sys.h"
#include "flash.h"

//FreeRTOSÍ·ÎÄ¼þ
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define H_MOTERR  0x10
#define H_FANERR  0x20
#define H_BTNERR  0x40

#define IAP_ADDR  0x08000000
#define APP_ADDR  0x08004000

void U1_Rx_UnPack(uint8_t *RxBuff,uint8_t Len);
void U3_Rx_UnPack(uint8_t *RxBuff,uint8_t Len);
void IAP_ExecuteApp(uint32_t addr);

#endif
