#ifndef __SYS_H
#define __SYS_H

#include <string.h>
#include "stm32f1xx_hal.h"

#include "htx.h"
#include "hrx.h"
#include "gpio.h"
#include "usart.h"
#include "adc.h"
#include "tim.h"

//FreeRTOS头文件
#include "portmacro.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

void AppTaskCreate(void);
extern void Task_Tst(void *parameter);
extern void Task_Tim(void *parameter);
extern void Task_Saf(void *parameter);
extern void Task_HTx(void *parameter);
extern void Task_HRx(void *parameter);
extern void Task_Act(void *parameter);
extern void Task_Btn(void *parameter);
extern void Task_Tmp(void *parameter);
extern void Task_LED(void *parameter);
extern void Task_DRx(void *parameter);

#define IWDG_TIME               6

#define MAX_TX_SIZE							200
#define MAX_RX_SIZE							200
#define QUE_LEN      						1

typedef struct RxMsg
{
  uint8_t RxBuf[MAX_RX_SIZE];
  uint8_t Len;
}RxMsg_T;

extern uint8_t U1_Rx_Buf[MAX_RX_SIZE];	
extern uint8_t U2_Rx_Buf[MAX_RX_SIZE];
extern uint8_t U3_Rx_Buf[MAX_RX_SIZE];

extern RxMsg_T U1_Rx_Msg;
extern RxMsg_T U2_Rx_Msg;
extern RxMsg_T U3_Rx_Msg;

extern SemaphoreHandle_t U1_Tx_Sem;
extern SemaphoreHandle_t U2_Tx_Sem;
extern SemaphoreHandle_t U3_Tx_Sem;
extern SemaphoreHandle_t LED_Pwm_Sem;

extern QueueHandle_t U1_Rx_Que;
extern QueueHandle_t U2_Rx_Que;
extern QueueHandle_t U3_Rx_Que;

void ISR_SemGive(SemaphoreHandle_t Sem);
void ISR_QueSend(QueueHandle_t Que,uint8_t *Item);

void BSP_U1_Tx(uint8_t *TxBuf,uint16_t Len);
void BSP_U2_Tx(uint8_t *TxBuf,uint16_t Len);
void BSP_U3_Tx(uint8_t *TxBuf,uint16_t Len);

typedef struct
{
  uint8_t HandTimer;   //握拳和伸直计时器
  uint8_t Model;       //界面
  uint8_t Act;         //按键记录
  uint8_t AckFlag;     //回馈标识
  uint8_t State;       //运动状态
  uint8_t Count;       //运动次数
  uint8_t CountFlag;   //运动次数标志位
  uint8_t HandState;   //握拳和伸直状态
  uint8_t HandAim;     //握拳和伸直目标
  uint8_t HandVal[6];  //握拳和伸直目标五指
  uint8_t AimFlag;     //主动手套连接标志位
  uint8_t AimWinFlag;  //主动手套校准成功
  uint8_t Para[100];   //设置参数
  uint8_t Finger[6];   //被动手指标志位
  uint8_t Oppo[7];     //对指手指标志位
  uint8_t Fun[3];      //功能标志位
  int16_t PressVal;    //压力值
  int16_t PressValDis; //显示压力值
  uint8_t EmerVal;     //急停按键值
  uint8_t WaitTime;    //分指气压等待时间
	
  uint8_t HelpStep;    //助力模式的进度
  uint8_t HelpTimer;   //助力保持时间计时器
  uint16_t HelpOldVal; //助力模式上一次的压力值
  uint16_t HelpVal;    //助力模式的压力值
  uint8_t Test;        //急停按键值
  uint16_t FingerVal[6]; //主动手套传感器数值
  uint16_t LastFingerVal[6]; //主动手套传感器前一次数值
  int16_t DiffFingerVal[6]; //主动手套传感器差值
  int16_t ChipTmp;     //芯片温度
  uint8_t Home;        //首页标志位
  uint8_t SaveFlag;    //保存标志位
  uint8_t StudyFlag;   //学习标志位
  uint8_t FingerType;  //手套识别类型
  uint8_t FingerFind;  //手套识别标志位
  uint8_t GymFist;     //手套操握拳标志位
  uint8_t StopVal;     //暂停标志位
	
  uint8_t AntiCount;   //抗痉挛计数
  int16_t AntiSumP1;   //P1段累加
  int16_t AntiSumP2;   //P2段累加
  int16_t AntiSumP3;   //P3段累加
  int16_t AntiCal1;    //第一阶段计算
  int16_t AntiCal2;    //第二阶段计算
  uint8_t Step;        //运动阶段
  int16_t PressAimFlag;//目标气压标志位
  int16_t PressAim;    //目标气压
  int16_t PressAimTmp; //目标气压临时
  uint16_t Preheat;    //预热时间
}Data_Base;

extern Data_Base DB;

extern uint32_t ADC_Val[ADC_NUM];
#endif
