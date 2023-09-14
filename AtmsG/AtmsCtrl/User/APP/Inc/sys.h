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

//FreeRTOSͷ�ļ�
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
  uint8_t HandTimer;   //��ȭ����ֱ��ʱ��
  uint8_t Model;       //����
  uint8_t Act;         //������¼
  uint8_t AckFlag;     //������ʶ
  uint8_t State;       //�˶�״̬
  uint8_t Count;       //�˶�����
  uint8_t CountFlag;   //�˶�������־λ
  uint8_t HandState;   //��ȭ����ֱ״̬
  uint8_t HandAim;     //��ȭ����ֱĿ��
  uint8_t HandVal[6];  //��ȭ����ֱĿ����ָ
  uint8_t AimFlag;     //�����������ӱ�־λ
  uint8_t AimWinFlag;  //��������У׼�ɹ�
  uint8_t Para[100];   //���ò���
  uint8_t Finger[6];   //������ָ��־λ
  uint8_t Oppo[7];     //��ָ��ָ��־λ
  uint8_t Fun[3];      //���ܱ�־λ
  int16_t PressVal;    //ѹ��ֵ
  int16_t PressValDis; //��ʾѹ��ֵ
  uint8_t EmerVal;     //��ͣ����ֵ
  uint8_t WaitTime;    //��ָ��ѹ�ȴ�ʱ��
	
  uint8_t HelpStep;    //����ģʽ�Ľ���
  uint8_t HelpTimer;   //��������ʱ���ʱ��
  uint16_t HelpOldVal; //����ģʽ��һ�ε�ѹ��ֵ
  uint16_t HelpVal;    //����ģʽ��ѹ��ֵ
  uint8_t Test;        //��ͣ����ֵ
  uint16_t FingerVal[6]; //�������״�������ֵ
  uint16_t LastFingerVal[6]; //�������״�����ǰһ����ֵ
  int16_t DiffFingerVal[6]; //�������״�������ֵ
  int16_t ChipTmp;     //оƬ�¶�
  uint8_t Home;        //��ҳ��־λ
  uint8_t SaveFlag;    //�����־λ
  uint8_t StudyFlag;   //ѧϰ��־λ
  uint8_t FingerType;  //����ʶ������
  uint8_t FingerFind;  //����ʶ���־λ
  uint8_t GymFist;     //���ײ���ȭ��־λ
  uint8_t StopVal;     //��ͣ��־λ
	
  uint8_t AntiCount;   //�����μ���
  int16_t AntiSumP1;   //P1���ۼ�
  int16_t AntiSumP2;   //P2���ۼ�
  int16_t AntiSumP3;   //P3���ۼ�
  int16_t AntiCal1;    //��һ�׶μ���
  int16_t AntiCal2;    //�ڶ��׶μ���
  uint8_t Step;        //�˶��׶�
  int16_t PressAimFlag;//Ŀ����ѹ��־λ
  int16_t PressAim;    //Ŀ����ѹ
  int16_t PressAimTmp; //Ŀ����ѹ��ʱ
  uint16_t Preheat;    //Ԥ��ʱ��
}Data_Base;

extern Data_Base DB;

extern uint32_t ADC_Val[ADC_NUM];
#endif
