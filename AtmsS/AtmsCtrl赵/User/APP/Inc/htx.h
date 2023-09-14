#ifndef __HTX_H
#define __HTX_H

#include "sys.h"

//FreeRTOSÍ·ÎÄ¼þ
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

#define H_FR1    0x5A
#define H_FR2    0xA5
#define H_FRAME  0x5AA5

#define H_READ   0x0002
#define H_WRITE  0x0003
#define H_STS    0x0500
#define H_ACK    0x0001
#define H_UPDATE 0x0300
#define H_QUERY  0x000A
#define H_TEST   0x000B

#define H_MOTERR  0x10
#define H_FANERR  0x20
#define H_BTNERR  0x40

void U1_Tx_Pack_LongNum(uint8_t Num,uint8_t Fun,int16_t Data);
void U1_Tx_Pack_Long(uint8_t Fun,int16_t Data);
void U1_Tx_Pack(uint8_t Fun,uint8_t Data);
void U3_Tx_Pack(uint8_t Cmd);

#endif
