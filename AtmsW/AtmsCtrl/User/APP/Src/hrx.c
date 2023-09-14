#include "hrx.h"

void Task_HRx(void *parameter)
{
  RxMsg_T *ptMsg;
  while(1)
  {
    if(xQueueReceive(U1_Rx_Que,(void *)&ptMsg,portMAX_DELAY) == pdPASS)
    {
      U1_Rx_UnPack(ptMsg->RxBuf,ptMsg->Len);
    }
  }
}

extern uint8_t Model;
extern uint8_t Act;
extern uint8_t State;
extern uint8_t Para[];
void Pump(uint8_t IO);

void U1_Rx_UnPack(uint8_t *RxBuff,uint8_t Len)
{
  uint16_t Frame = 0;
  uint16_t Addr = 0;
  uint16_t Value = 0;
	
  Frame = (RxBuff[0]<<8) + RxBuff[1];
  Addr  = (RxBuff[2]<<8) + RxBuff[3];
  Value = (RxBuff[4]<<8) + RxBuff[5];
	
//监听
//for(uint8_t i=0;i<Len;i++)
//{
//	printf("%02x",RxBuff[i]);
//}
  if(Frame == 0x0206 && Addr == 0x0001)
  {
    switch(Value)
    {
      case 0x0001: N_SET(1,1);break;
      case 0x0002: N_SET(1,0);break;
      case 0x0003: N_SET(2,1);break;
      case 0x0004: N_SET(2,0);break;
      case 0x0005: N_SET(3,1);break;
      case 0x0006: N_SET(3,0);break;
      case 0x0007: N_SET(4,1);break;
      case 0x0008: N_SET(4,0);break;
      case 0x0009: N_SET(5,1);break;
      case 0x000A: N_SET(5,0);break;
      case 0x000B: N_SET(6,1);break;
      case 0x000C: N_SET(6,0);break;
      case 0x000D: N_SET(7,1);break;
      case 0x000E: N_SET(7,0);break;
      case 0x000F: N_SET(8,1);break;
      case 0x0010: N_SET(8,0);break;
      case 0x0011: HAL_GPIO_WritePin(PUMP1_EN_GPIO, PUMP1_EN_GPIO_PIN, GPIO_PIN_SET);HAL_GPIO_WritePin(PUMP2_EN_GPIO, PUMP2_EN_GPIO_PIN, GPIO_PIN_SET);PWM_Set(1,100);PWM_Set(2,100);break;
      case 0x0012: HAL_GPIO_WritePin(PUMP1_EN_GPIO, PUMP1_EN_GPIO_PIN, GPIO_PIN_RESET);HAL_GPIO_WritePin(PUMP2_EN_GPIO, PUMP2_EN_GPIO_PIN, GPIO_PIN_RESET);PWM_Set(1,0);PWM_Set(2,0);break;
      default:break;
    }
  }
  if(Frame == 0x0206 && Addr == 0x1001)
  {
//  printf(" 当前页面%d",Value);
    switch(Value)
    {
      case 0x0001: DB.Model = 1; DB.Home = 1;break;
      case 0x0002: DB.Model = 2; DB.Act = 0; break;
      case 0x0003: DB.Model = 3; DB.Act = 0; break;
      case 0x0004: DB.Model = 4; DB.Act = 0; break;
      case 0x0005: DB.Model = 5; DB.Act = 0; break;
      case 0x0006: DB.Model = 6; DB.Act = 0; break;
      case 0x0007: DB.Model = 7; DB.Act = 0; break;
      case 0x0008: DB.Model = 8; DB.Act = 0; break;
      case 0x0009: DB.Model = 9; DB.Act = 0; break;
      case 0x000A: DB.Model = 10;DB.Act = 0; break;
      case 0x000B: DB.Model = 11;DB.Act = 0; break;
      case 0x000C: DB.Model = 12;DB.Act = 0; break;
      case 0x000D: DB.Model = 13;DB.Act = 0; break;
      case 0x000E: DB.Model = 14;DB.Act = 0; break;
      case 0x000F: DB.Model = 15;DB.Act = 0; break;
      case 0x0010: DB.Model = 16;DB.Act = 0; break;
      case 0x0011: DB.Act = 1;DB.AckFlag = 1;break;
      case 0x0012: DB.Act = 2;DB.AckFlag = 1;break;
      case 0x0013: DB.Act = 3;DB.AckFlag = 1;break;
      case 0x0014: DB.Act = 4;DB.AckFlag = 1;break;
      case 0x0015: DB.Act = 5;DB.AckFlag = 1;break;
      case 0x0016: DB.Model = 17;DB.Act = 0; break;
      case 0x0017: DB.Model = 18;DB.Act = 0; break;
      case 0x0018: DB.Model = 19;DB.Act = 0; break;
      default:break;
    }
    //printf("-进入页面%d-动作%d ",DB.Model,DB.Act);
  }
  if(Frame == 0x0216 && Addr == 0x1002 && Value >= 0x0032)
  {
    //printf("设置");
    for(uint8_t i = 0; i < 50; i++)
    {
      DB.Para[i] = RxBuff[i*2 + 7];
    }
  }
  if(Frame == 0x0216 && Addr == 0x100B && Value == 0x000C)
  {
    //printf("手套操配置");
    for(uint8_t i = 0; i < 6; i++)
    {
      DB.Para[i + 9] = RxBuff[i*2 + 7];
    }
  }
  if(Frame == 0x0206 && Addr == 0x0002)
  {
    DB.StudyFlag = Value;
  }
  if(Frame == 0x0206 && Addr == 0x0003)
  {
    DB.PressAim = Value;
    if(DB.PressValDis - DB.PressAim <= -10 && DB.PressValDis - DB.PressAim >= 10)
    {
      DB.PressAimFlag = 1;  //不动
    }
    else
    {
      DB.PressAimFlag = 2;  //准备阶段
      if(DB.PressAim > 0) DB.PressAimTmp = DB.PressAim - 100;
      if(DB.PressAim < 0) DB.PressAimTmp = DB.PressAim + 100;
    }
  }
  if(Frame == 0x0206 && Addr == 0x0005)
  {
    DB.SaveFlag = Value;
  }
  if(Frame == 0x0206 && Addr == 0x0006)
  {
    DB.Para[27] = Value;
    PWM_Set(1,DB.Para[27]);
  }
  if(Frame == 0x0206 && Addr == 0x0007)
  {
    DB.Para[28] = Value;
    PWM_Set(2,DB.Para[28]);
  }
  if(Frame == 0xA55A && Addr == 0x8300)
  {
    uint16_t Len = (RxBuff[6]<<8) + RxBuff[7];
    uint16_t Crc = RxBuff[8];
    uint16_t Flag = 0xA55A;
			
    STMFLASH_Write(0x0803F820,(uint16_t*)&Len,1);
    STMFLASH_Write(0x0803F820 + 2,(uint16_t*)&Crc,1);
    STMFLASH_Write(0x0803F820 + 4,(uint16_t*)&Flag,1);
		
    HAL_NVIC_SystemReset();
  }
}

void U3_Rx_UnPack(uint8_t *RxBuff,uint8_t Len)
{
  uint16_t Frame = 0;
  uint8_t Addr = 0;
	
  Frame = RxBuff[0];
  Addr = RxBuff[2];

  if(Frame == 0x01 && Addr == 0x0A)
  {
    DB.AimFlag = 1;
    DB.FingerVal[1] = (RxBuff[3]<<8) + RxBuff[4];
    DB.FingerVal[2] = (RxBuff[5]<<8) + RxBuff[6];
    DB.FingerVal[3] = (RxBuff[7]<<8) + RxBuff[8];
    DB.FingerVal[4] = (RxBuff[9]<<8) + RxBuff[10];
    DB.FingerVal[5] = (RxBuff[11]<<8) + RxBuff[12];
    for(uint8_t i = 1; i <= 5; i++)
    {
      DB.DiffFingerVal[i] = DB.FingerVal[i] - DB.LastFingerVal[i];
      DB.LastFingerVal[i] = DB.FingerVal[i];
    }
  }
  if(Frame == 0x01 && Addr == 0x02)
  {
    if(RxBuff[4] == 3)
    {
      DB.AimWinFlag = 1;
    }
  }
}
