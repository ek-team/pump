#include "htx.h"

void Task_HTx(void *parameter)
{
  int16_t PressValDisTmp,Count;
  while(1)
  {
    if(DB.State == 14)
    {
      PressValDisTmp = PressValDisTmp*4/5 + DB.PressValDis*1/5;
      U1_Tx_Pack_Long(0x08,PressValDisTmp);
    }
    if(DB.PressAimFlag >= 8)
    {
       Count++;
       if(Count == 11 || Count == 12)
       {
         U1_Tx_Pack_Long(0xA3,DB.PressAim);
       }
       if(Count > 110)
       {
         DB.PressAimFlag = 0;
         Count = 0;				
      }
    }
    else
    {
      Count = 0;
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

