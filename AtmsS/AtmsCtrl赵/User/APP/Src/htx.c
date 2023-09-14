#include "htx.h"

void Task_Spd(void *parameter)
{
  uint8_t FG1Tmp,FG2Tmp;
  uint16_t FGCount,FGCount1,FGCount2;
  DB.Para[27] = 100;DB.Para[28] = 100;
  while(1)
  {
    if(HAL_GPIO_ReadPin(FG1_GPIO, FG1_GPIO_PIN) != FG1Tmp)
    {
      FG1Tmp = HAL_GPIO_ReadPin(FG1_GPIO, FG1_GPIO_PIN);
      if(FG1Tmp == 1) FGCount1++;
    }
    if(HAL_GPIO_ReadPin(FG2_GPIO, FG2_GPIO_PIN) != FG2Tmp)
    {
      FG2Tmp = HAL_GPIO_ReadPin(FG2_GPIO, FG2_GPIO_PIN);
      if(FG2Tmp == 1) FGCount2++;
    }
    if(FGCount++ > 200)
    {
      DB.FG1Val = FGCount1*60;
      DB.FG2Val = FGCount2*60;
      FGCount = 0;
      FGCount1 = 0;
      FGCount2 = 0;
    }
  	vTaskDelay(5);
  }
}
void Task_HTx(void *parameter)
{
  int16_t PressValDisTmp;
  while(1)
  {
    if(DB.State == 14)
    {
      PressValDisTmp = PressValDisTmp*4/5 + (DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5])/5*1/5;
      U1_Tx_Pack_Long(0x08,PressValDisTmp);
    }
    vTaskDelay(100);
    if(DB.AckFlag == 1)
    {
      DB.AckFlag = 0;
      U1_Tx_Pack(0x0B,01);
    }
  }
}

void U1_Tx_Pack_Long(uint8_t Fun,int16_t Data)
{
  static uint8_t TxBuf[MAX_TX_SIZE];
  uint16_t index = 0;
	
  TxBuf[index++] = 0x01;
  TxBuf[index++] = 0x06;
  TxBuf[index++] = 0x00;
  TxBuf[index++] = Fun;
  TxBuf[index++] = Data>>8;
  TxBuf[index++] = Data;
	
  BSP_U1_Tx(TxBuf,index);
}

void U1_Tx_Pack_LongNum(uint8_t Num,uint8_t Fun,int16_t Data)
{
  static uint8_t TxBuf[MAX_TX_SIZE];
  uint16_t index = 0;
	
  TxBuf[index++] = 0x01;
  TxBuf[index++] = 0x06;
  TxBuf[index++] = Num;
  TxBuf[index++] = Fun;
  TxBuf[index++] = Data>>8;
  TxBuf[index++] = Data;
	
  BSP_U1_Tx(TxBuf,index);
}

void U1_Tx_Pack(uint8_t Fun,uint8_t Data)
{
  static uint8_t TxBuf[MAX_TX_SIZE];
  uint16_t index = 0;
	
  TxBuf[index++] = 0x01;
  TxBuf[index++] = 0x06;
  TxBuf[index++] = 0x00;
  TxBuf[index++] = Fun;
  TxBuf[index++] = 0x00;
  TxBuf[index++] = Data;
	
  BSP_U1_Tx(TxBuf,index);
}

void U3_Tx_Pack(uint8_t Cmd)
{
  static uint8_t TxBuf[MAX_TX_SIZE];
  uint16_t index = 0;
  if(Cmd == 1) //开始上传
  {
    TxBuf[index++] = 0x01;
    TxBuf[index++] = 0x06;
    TxBuf[index++] = 0xF0;
    TxBuf[index++] = 0x00;
    TxBuf[index++] = 0x00;
    TxBuf[index++] = 0x01;
    TxBuf[index++] = 0x7B;
    TxBuf[index++] = 0x0A;
  }
  if(Cmd == 2) //停止上传
  {
    TxBuf[index++] = 0x01;
    TxBuf[index++] = 0x06;
    TxBuf[index++] = 0xF0;
    TxBuf[index++] = 0x00;
    TxBuf[index++] = 0x00;
    TxBuf[index++] = 0x00;
    TxBuf[index++] = 0xBA;
    TxBuf[index++] = 0xCA;
  }
  if(Cmd == 3) //伸直校准
  {
    TxBuf[index++] = 0x01;
    TxBuf[index++] = 0x06;
    TxBuf[index++] = 0xF0;
    TxBuf[index++] = 0x01;
    TxBuf[index++] = 0x00;
    TxBuf[index++] = 0x01;
    TxBuf[index++] = 0x2A;
    TxBuf[index++] = 0xCA;
  }
  if(Cmd == 4) //握拳校准
  {
    TxBuf[index++] = 0x01;
    TxBuf[index++] = 0x06;
    TxBuf[index++] = 0xF0;
    TxBuf[index++] = 0x01;
    TxBuf[index++] = 0x00;
    TxBuf[index++] = 0x02;
    TxBuf[index++] = 0x6A;
    TxBuf[index++] = 0xCB;
  }
  if(Cmd == 5) //读取传感器数值
  {
    if(DB.Para[2] == 1)
    {
      TxBuf[index++] = 0x01;
      TxBuf[index++] = 0x03;
      TxBuf[index++] = 0xE0;
      TxBuf[index++] = 0x00;
      TxBuf[index++] = 0x00;
      TxBuf[index++] = 0x05;
      TxBuf[index++] = 0xB2;
      TxBuf[index++] = 0x09;
    }
    if(DB.Para[2] == 0)
    {
      TxBuf[index++] = 0x01;
      TxBuf[index++] = 0x04;
      TxBuf[index++] = 0xE0;
      TxBuf[index++] = 0x00;
      TxBuf[index++] = 0x00;
      TxBuf[index++] = 0x05;
      TxBuf[index++] = 0x07;
      TxBuf[index++] = 0xC9;
    }
  }
  if(Cmd == 6) //读取校准成功位
  {
    TxBuf[index++] = 0x01;
    TxBuf[index++] = 0x03;
    TxBuf[index++] = 0xF0;
    TxBuf[index++] = 0x01;
    TxBuf[index++] = 0x00;
    TxBuf[index++] = 0x01;
    TxBuf[index++] = 0xE6;
    TxBuf[index++] = 0xCA;
  }
  BSP_U3_Tx(TxBuf,index);
}

