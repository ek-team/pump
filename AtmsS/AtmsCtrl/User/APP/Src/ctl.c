#include "ctl.h"
#include "sys.h"
#include "stdlib.h"

void Pump(uint8_t IO) 
{
  if(IO == 0) //暂停
  {
    HAL_GPIO_WritePin(PUMP1_EN_GPIO, PUMP1_EN_GPIO_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PUMP2_EN_GPIO, PUMP2_EN_GPIO_PIN, GPIO_PIN_RESET);
    PWM_Set(1,0);
    PWM_Set(2,0);
  }
  if(IO == 1) //启动
  {
    HAL_GPIO_WritePin(PUMP1_EN_GPIO, PUMP1_EN_GPIO_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(PUMP2_EN_GPIO, PUMP2_EN_GPIO_PIN, GPIO_PIN_SET);
    PWM_Set(1,DB.Para[27]);
    PWM_Set(2,DB.Para[28]);
  }
  if(IO == 4) //分指握拳
  {
    HAL_GPIO_WritePin(PUMP2_EN_GPIO, PUMP2_EN_GPIO_PIN, GPIO_PIN_SET);
    PWM_Set(2,DB.Para[28]);
    HAL_GPIO_WritePin(PUMP1_EN_GPIO, PUMP1_EN_GPIO_PIN, GPIO_PIN_RESET);
    PWM_Set(1,0);
    N_SET(1,1);N_SET(2,0);N_SET(8,0);
  }
  if(IO == 5) //分指伸直
  {
    HAL_GPIO_WritePin(PUMP1_EN_GPIO, PUMP1_EN_GPIO_PIN, GPIO_PIN_SET);
    PWM_Set(1,DB.Para[27]);
    HAL_GPIO_WritePin(PUMP2_EN_GPIO, PUMP2_EN_GPIO_PIN, GPIO_PIN_RESET);
    PWM_Set(2,0);
    N_SET(1,0);N_SET(2,0);N_SET(8,0);
    N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
  }
  if(IO == 6) //握拳背伸
  {

  }
  if(IO == 7) //伸直背伸
  {

  }
}

void Escape(uint8_t EN)
{
  if(EN == 1)
  {
    N_SET(8,1);
    N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
  }
  if(EN == 0)
  {
    N_SET(8,0);
    N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
  }
}

void CTL0()  //无动作
{
}

void CTL1()  //分指-手套操
{
  U1_Tx_Pack_Long(0x08,DB.PressValDis[0]);
  vTaskDelay(50);
  U1_Tx_Pack_Long(0x09,DB.ChipTmp);
  vTaskDelay(50);
  if(DB.GymFist == 1 && DB.HandTimer == 4) //握拳二阶段
  {
    DB.GymFist = 0;
    Pump(4);
    N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
    if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x02);
    if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x04);
  }
  if(DB.HandState == 1)
  {
    if(DB.Para[9] == 1 && DB.Para[10] == 1 && DB.Para[11] == 1 && DB.Para[12] == 1 && DB.Para[13] == 1) //握拳一阶段
    {
      DB.HandState = 0;
      Pump(4);
      N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
      if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x1C);
      if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x23);
      DB.GymFist = 1;
    }
    else
    {
      DB.HandState = 0;
      N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
      vTaskDelay(20);
      Pump(4);
      if(DB.Para[9] == 1)  N_SET(3,0);
      if(DB.Para[10] == 1) N_SET(4,0);
      if(DB.Para[11] == 1) N_SET(5,0);
      if(DB.Para[12] == 1) N_SET(6,0);
      if(DB.Para[13] == 1) N_SET(7,0);
      if(DB.Para[2] == 0)
      {
        if(DB.Para[9] == 1 && DB.Para[10] == 1 && DB.Para[11] == 0 && DB.Para[12] == 0 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x05);
        if(DB.Para[9] == 1 && DB.Para[10] == 0 && DB.Para[11] == 1 && DB.Para[12] == 0 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x06);
        if(DB.Para[9] == 1 && DB.Para[10] == 0 && DB.Para[11] == 0 && DB.Para[12] == 1 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x07);
        if(DB.Para[9] == 1 && DB.Para[10] == 0 && DB.Para[11] == 0 && DB.Para[12] == 0 && DB.Para[13] == 1) U1_Tx_Pack(0x01,0x08);
        if(DB.Para[9] == 0 && DB.Para[10] == 1 && DB.Para[11] == 0 && DB.Para[12] == 0 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x18);
        if(DB.Para[9] == 0 && DB.Para[10] == 0 && DB.Para[11] == 1 && DB.Para[12] == 0 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x19);
        if(DB.Para[9] == 0 && DB.Para[10] == 0 && DB.Para[11] == 0 && DB.Para[12] == 1 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x1A);
        if(DB.Para[9] == 0 && DB.Para[10] == 0 && DB.Para[11] == 0 && DB.Para[12] == 0 && DB.Para[13] == 1) U1_Tx_Pack(0x01,0x1B);
        if(DB.Para[9] == 0 && DB.Para[10] == 1 && DB.Para[11] == 1 && DB.Para[12] == 1 && DB.Para[13] == 1) U1_Tx_Pack(0x01,0x1C);
        if(DB.Para[9] == 1 && DB.Para[10] == 1 && DB.Para[11] == 1 && DB.Para[12] == 0 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x1D);
        if(DB.Para[9] == 1 && DB.Para[10] == 1 && DB.Para[11] == 1 && DB.Para[12] == 1 && DB.Para[13] == 1) U1_Tx_Pack(0x01,0x02);
      }
      if(DB.Para[2] == 1) 
      {
        if(DB.Para[9] == 1 && DB.Para[10] == 1 && DB.Para[11] == 0 && DB.Para[12] == 0 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x09);
        if(DB.Para[9] == 1 && DB.Para[10] == 0 && DB.Para[11] == 1 && DB.Para[12] == 0 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x0A);
        if(DB.Para[9] == 1 && DB.Para[10] == 0 && DB.Para[11] == 0 && DB.Para[12] == 1 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x0B);
        if(DB.Para[9] == 1 && DB.Para[10] == 0 && DB.Para[11] == 0 && DB.Para[12] == 0 && DB.Para[13] == 1) U1_Tx_Pack(0x01,0x0C);
        if(DB.Para[9] == 0 && DB.Para[10] == 1 && DB.Para[11] == 0 && DB.Para[12] == 0 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x1F);
        if(DB.Para[9] == 0 && DB.Para[10] == 0 && DB.Para[11] == 1 && DB.Para[12] == 0 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x20);
        if(DB.Para[9] == 0 && DB.Para[10] == 0 && DB.Para[11] == 0 && DB.Para[12] == 1 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x21);
        if(DB.Para[9] == 0 && DB.Para[10] == 0 && DB.Para[11] == 0 && DB.Para[12] == 0 && DB.Para[13] == 1) U1_Tx_Pack(0x01,0x22);
        if(DB.Para[9] == 0 && DB.Para[10] == 1 && DB.Para[11] == 1 && DB.Para[12] == 1 && DB.Para[13] == 1) U1_Tx_Pack(0x01,0x23);
        if(DB.Para[9] == 1 && DB.Para[10] == 1 && DB.Para[11] == 1 && DB.Para[12] == 0 && DB.Para[13] == 0) U1_Tx_Pack(0x01,0x24);
        if(DB.Para[9] == 1 && DB.Para[10] == 1 && DB.Para[11] == 1 && DB.Para[12] == 1 && DB.Para[13] == 1) U1_Tx_Pack(0x01,0x04);
      }
    }
  }
  if(DB.HandState == 2)
  {
    DB.HandState = 0;
    Pump(5);
    if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x01);
    if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x03);
  }
  if(DB.CountFlag == 1)
  {
    DB.CountFlag = 0;
    U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
  }
}

void CTL2()  //分指-随动
{
  static uint8_t OldHandAim = 0;
  static uint8_t FingerTmp = 0;
  U1_Tx_Pack_Long(0x08,DB.PressValDis[0]);
  vTaskDelay(20);
  U1_Tx_Pack_Long(0x09,DB.ChipTmp);
  vTaskDelay(20);
  U3_Tx_Pack(1);
  vTaskDelay(20);
  U3_Tx_Pack(5);
  vTaskDelay(20);
  if(DB.AimFlag == 1)
  {
    DB.AimFlag = 0;
    for(uint8_t i=1;i<=5;i++)
    {
      if((DB.DiffFingerVal[i] > 20 && DB.FingerVal[i] < 150) || DB.FingerVal[i] >= 150)
      {
        DB.HandVal[i] = 1;//握拳状态
      }
      if((DB.DiffFingerVal[i] < -20 && DB.FingerVal[i] > 30) || DB.FingerVal[i] <= 30)
      {
        DB.HandVal[i] = 0;//伸直状态
      }
    }
    if(FingerTmp != DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1)
    {
      if(FingerTmp == 0)
      {
        DB.HandAim = 1;//上一次是伸直这一次则是握拳
      }
      if(FingerTmp == 31)
      {
        DB.HandAim = 0;//上一次是握拳这一次则是伸直
      }
      FingerTmp = DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1;
      if(FingerTmp == 0)
      {
        DB.HandAim = 0;//当前是伸直则是伸直
      }
      if(FingerTmp == 31)
      {
        DB.HandAim = 1;//当前是握拳则是握拳
      }
      if(DB.EmerVal != 0) U1_Tx_Pack(0x0A,FingerTmp);
		
      if(DB.HandAim == 1)
      {
        for(uint8_t i=1;i<=5;i++)
        {
          if(DB.HandVal[i] == 1)  N_SET(i+2,0);
          else N_SET(i+2,1); 
        }
        Pump(4);
      }
      if(DB.HandAim == 0) 
      {
        for(uint8_t i=1;i<=5;i++)
        {
          if(DB.HandVal[i] == 0)  N_SET(i+2,0);
          else N_SET(i+2,1); 
        }
        HAL_GPIO_WritePin(PUMP1_EN_GPIO, PUMP1_EN_GPIO_PIN, GPIO_PIN_SET);
        PWM_Set(1,100);
        HAL_GPIO_WritePin(PUMP2_EN_GPIO, PUMP2_EN_GPIO_PIN, GPIO_PIN_RESET);
        PWM_Set(2,0);
        N_SET(1,0);N_SET(2,0);N_SET(8,0);
      }
    }
  }
  if(DB.HandAim != OldHandAim)
  {
    OldHandAim = DB.HandAim;
    U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
    if(DB.HandAim == 0)
    {
      if(DB.Para[8] == 1) U1_Tx_Pack(0x04,0x00);
    }
    if(DB.HandAim == 1)
    {
      if(DB.Para[8] == 0) U1_Tx_Pack(0x04,0x01);
    }
  }
  vTaskDelay(HOLD_TIME);
}

void CTL3()  //分指-功能
{
  U1_Tx_Pack_Long(0x08,DB.PressValDis[0]);
  vTaskDelay(50);
  U1_Tx_Pack_Long(0x09,DB.ChipTmp);
  vTaskDelay(50);
  if(DB.HandState == 1)
  {
    DB.HandState = 0;
    N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
    vTaskDelay(20);
    Pump(4);
    if(DB.Fun[0] + DB.Fun[1] + DB.Fun[2] == DB.Para[20] + DB.Para[21] + DB.Para[22])
    {
      DB.Fun[0] = 0;DB.Fun[1] = 0;DB.Fun[2] = 0;
    }
    for(uint8_t i=0;i<=2;i++)
    {
      if(DB.Para[i + 20] == 1 && DB.Fun[i] == 0)
      {
        DB.Fun[i] = 1;
        DB.StopVal = i;
        if(i == 0)
        {
          N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
          if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x10); //左手
          if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x14); //右手
          U1_Tx_Pack(0x02,++DB.Count);               //发送次数
          break;
        }
        if(i == 1)
        {
          N_SET(3,0);N_SET(4,0);
          if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x0E); //左手
          if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x12); //右手
          U1_Tx_Pack(0x02,++DB.Count);               //发送次数
          break;
        }
        if(i == 2)
        {
          N_SET(3,0);N_SET(4,0);N_SET(5,0);
          if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x0F); //左手
          if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x13); //右手
          U1_Tx_Pack(0x02,++DB.Count);               //发送次数
          break;
        }
      }
    }
  }
  if(DB.HandState == 2)
  {
    DB.HandState = 0;
    Pump(5);
    if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x0D); //左手
    if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x11); //右手
  }
}

void CTL4()  //五指-被动
{
  //模式取消
}

void CTL5()  //五指-随动
{
  //模式取消
}

void CTL6()  //五指-助力
{
  //模式取消
}

void CTL7()  //五指-抗阻
{
  //模式取消
}

void CTL9()  //分指-对指
{
 //模式取消
}

void CTL10()  //数据校正
{
  uint8_t Tmp[6] = {0};
  uint8_t FingerTmp = 0;
  uint16_t FingerValTmp[6];
	
  U3_Tx_Pack(1);   //开始上传
  vTaskDelay(300);
  U3_Tx_Pack(5);   //读取传感器数据
  vTaskDelay(300);

  if(DB.AimFlag == 0)
  {
    U1_Tx_Pack(0x05,0x01);
    uint16_t i = 300;
    while(--i)
    {
      vTaskDelay(10);
      if(DB.Act != 0) return;
    }
  }
  if(DB.AimFlag == 1)
  {
    DB.AimFlag = 0;
    for(uint8_t i = 1;i<6;i++)
    {
      Tmp[i] = DB.FingerVal[i];
      if(DB.FingerVal[i] > 50) 
      {
        Tmp[i] = 1;
      }
      else
      {
        Tmp[i] = 0;
      }
      if(DB.FingerVal[i] != 0 && DB.FingerVal[i] != 180)
      {
        DB.FingerType = 1;//识别为数据手套
      }
      if(DB.FingerVal[i] != 0)
      {
        DB.FingerFind = 1;//识别成功
      }
  	}
    FingerTmp = Tmp[1]*16 + Tmp[2]*8 + Tmp[3]*4 + Tmp[4]*2 + Tmp[5]*1;
    U1_Tx_Pack(0x0A,FingerTmp);
    if(DB.SaveFlag == 1) //第一次握拳保存
    {
      DB.SaveFlag = 0;
      for(uint8_t i = 1;i<6;i++)
      {
        FingerValTmp[i] = DB.FingerVal[i];
      }
      U3_Tx_Pack(4);   //保存握拳
      vTaskDelay(50);
      U3_Tx_Pack(5);   //读取传感器数据
      vTaskDelay(100);
      for(uint8_t i = 1;i<6;i++)
      {
        if(FingerValTmp[i] != DB.FingerVal[i])
        {
          if((FingerValTmp[i] != 0 && FingerValTmp[i] != 180) || (DB.FingerVal[i] != 0 && DB.FingerVal[i] != 180))
          {
            DB.FingerType = 1;//识别为数据手套
          }
        }
      }
    }
    if(DB.SaveFlag == 2) //第一次伸直保存
    {
      DB.SaveFlag = 0;
      U3_Tx_Pack(3);
      if(DB.FingerFind == 1)
      {
        if(DB.FingerType == 0) U1_Tx_Pack(0x05,0x09);   //磁石手套
        if(DB.FingerType == 1) U1_Tx_Pack(0x05,0x08);   //数据手套
      }
      else
      {
        U1_Tx_Pack(0x05,0x0A);   //识别失败
      }
    }
    if(DB.SaveFlag == 3)
    {
      DB.SaveFlag = 0;
      U3_Tx_Pack(4);
    }
    if(DB.SaveFlag == 4)
    {
      DB.SaveFlag = 0;
      U3_Tx_Pack(3);
      uint8_t i = 100;
      while(--i)
      {
        vTaskDelay(10);
        if(DB.EmerVal == 0) return;
        if(DB.Act != 0) return;
      }
      U3_Tx_Pack(6);          //判断校准是否成功
      i = 30;
      while(--i)
      {
        vTaskDelay(10);
        if(DB.EmerVal == 0) return;
        if(DB.Act != 0) return;
      }
      if(DB.AimWinFlag == 1)
      {
        DB.AimWinFlag = 0;
        U1_Tx_Pack(0x05,0x06);//校准成功
        DB.State = 0;
        DB.Model = 0;
      }
      else
      {
        U1_Tx_Pack(0x05,0x07);//校准失败
      }
    }
  }
}

void CTL11()  //游戏
{
  //模式取消
}

void CTL8()  //分指-多功能被动
{
  //模式取消
}

void CTL12()  //分指语音
{
  //模式取消
}
void CTL13()  //五指语音
{
  //模式取消
}

void CTL14()  //评估模式
{
  vTaskDelay(10);
  if(DB.PressAimFlag == 1)
  {
    DB.PressAimFlag = 0;
    Pump(0);
    Escape(0);
    U1_Tx_Pack_Long(0xA3,DB.PressAim);//气压相同,直接结束
  }
  if(DB.PressAimFlag == 2)
  {
    if((DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5])/5 > DB.PressAimTmp) //气压变小
    {
      Pump(5);
      DB.PressAimFlag = 3;
    }
    else if((DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5])/5 < DB.PressAimTmp) //气压变大
    {
      Pump(4);
      N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
      DB.PressAimFlag = 4;
    }
    else
    {
      Pump(0);
      Escape(0);
      DB.PressAimFlag = 5;
      U1_Tx_Pack_Long(0xA3,DB.PressAim);
    }
  }
  if(DB.PressAimFlag == 3)
  {
    if((DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5])/5 <= DB.PressAimTmp)
    {
      Pump(0);
      Escape(0);
      DB.PressAimFlag = 5;
      U1_Tx_Pack_Long(0xA3,DB.PressAim);
    }
  }
  if(DB.PressAimFlag == 4)
  {
    if((DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5])/5 >= DB.PressAimTmp)//气压变小
    {
      Pump(0);
      Escape(0);
      DB.PressAimFlag = 5;
      U1_Tx_Pack_Long(0xA3,DB.PressAim);
    }
  }
}

void CTL15() //分指助力
{
  uint8_t Acc = 0;
  static int16_t OldPressValDis = 0;
  Acc = 20 - 2 * DB.Para[24];
	
  U1_Tx_Pack_Long(0x08,DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5]);
  vTaskDelay(10);
  U1_Tx_Pack_LongNum(0x01,0x08,DB.PressValDis[1]);
  vTaskDelay(10);
  U1_Tx_Pack_LongNum(0x02,0x08,DB.PressValDis[2]);
  vTaskDelay(10);
  U1_Tx_Pack_LongNum(0x03,0x08,DB.PressValDis[3]);
  vTaskDelay(10);
  U1_Tx_Pack_LongNum(0x04,0x08,DB.PressValDis[4]);
  vTaskDelay(10);
  U1_Tx_Pack_LongNum(0x05,0x08,DB.PressValDis[5]);
  vTaskDelay(10);
  U1_Tx_Pack_Long(0x09,DB.ChipTmp);
  vTaskDelay(10);
	
  if(DB.HelpStep == 0)
  {
    Pump(0);
    Escape(1);
    vTaskDelay(2000);
    Escape(0);
    DB.HelpStep = 1;
    vTaskDelay(1000);
    if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x15);     //开始等待
    if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x16);
    OldPressValDis = DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5];
  }
  if(DB.HelpStep == 1)
  {
    if((DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5] > OldPressValDis + Acc) && (DB.Para[23] == 0 || DB.Para[23] == 1))
    {
      Pump(5);
      DB.HelpStep = 3;
      if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x01);
      if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x03);
      U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
    }
    if((DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5] < OldPressValDis - Acc)  && (DB.Para[23] == 0 || DB.Para[23] == 2))
    {
      Pump(4);
      N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
      DB.HelpStep = 2;
      if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x02);
      if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x04);
      U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
    }
    OldPressValDis = DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5];
  }
  if(DB.HelpStep >= 2)
  {
    DB.HelpTimer++;
    if((DB.HelpTimer/5 >= DB.Para[3] && DB.HelpStep == 2) || (DB.HelpTimer/5 >= DB.Para[4] && DB.HelpStep == 3))
    {
      DB.HelpStep = 0;
      DB.HelpTimer = 0;
    }
  }
}

void CTL16()//分指抗阻
{
  uint8_t Acc = 0;
  static int16_t OldPressValDis = 0;
  Acc = 20 - 2 * DB.Para[24];
	
  U1_Tx_Pack_Long(0x08,DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5]);
  vTaskDelay(10);
  U1_Tx_Pack_LongNum(0x01,0x08,DB.PressValDis[1]);
  vTaskDelay(10);
  U1_Tx_Pack_LongNum(0x02,0x08,DB.PressValDis[2]);
  vTaskDelay(10);
  U1_Tx_Pack_LongNum(0x03,0x08,DB.PressValDis[3]);
  vTaskDelay(10);
  U1_Tx_Pack_LongNum(0x04,0x08,DB.PressValDis[4]);
  vTaskDelay(10);
  U1_Tx_Pack_LongNum(0x05,0x08,DB.PressValDis[5]);
  vTaskDelay(10);
  U1_Tx_Pack_Long(0x09,DB.ChipTmp);
  vTaskDelay(10);
	
  if(DB.HelpStep == 0)
  {
    Pump(0);
    Escape(1);
    vTaskDelay(2000);
    Escape(0);
    DB.HelpStep = 1;
    vTaskDelay(1000);
    if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x15);     //开始等待
    if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x16);
    OldPressValDis = DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5];
  }
  if(DB.HelpStep == 1)
  {
    if(DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5] > OldPressValDis + Acc)
    {
      Pump(4);
      N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
      DB.HelpStep = 3;
      if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x01);
      if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x03);
      U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
    }
    if(DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5] < OldPressValDis - Acc)
    {
      Pump(5);
      DB.HelpStep = 2;
      if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x02);
      if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x04);
      U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
    }
    OldPressValDis = DB.PressValDis[1] + DB.PressValDis[2] + DB.PressValDis[3] + DB.PressValDis[4] + DB.PressValDis[5];
  }
  if(DB.HelpStep >= 2)
  {
    DB.HelpTimer++;
    if((DB.HelpTimer/5 >= DB.Para[3] && DB.HelpStep == 2) || (DB.HelpTimer/5 >= DB.Para[4] && DB.HelpStep == 3))
    {
      DB.HelpStep = 0;
      DB.HelpTimer = 0;
    }
  }
}

void CTL17()//分指伸展
{
//  U1_Tx_Pack_Long(0x08,DB.PressValDis);
//  vTaskDelay(50);
//  U1_Tx_Pack_Long(0x09,DB.ChipTmp);
//  vTaskDelay(50);
//  if(DB.HandState == 1)
//  {
//    DB.HandState = 0;
//    Escape(1);
//    vTaskDelay(HOLD_TIME);//Escape加ECO
//    N_SET(1,2);
//    N_SET(2,2);
//    N_SET(8,2);
//    N_SET(3,2);N_SET(4,2);N_SET(5,2);N_SET(6,2);N_SET(7,2);
//    if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x15);
//    if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x16);
//    U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
//  }
//  if(DB.HandState == 2)
//  {
//    DB.HandState = 0;
//    Pump(5);
//    if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x01);
//    if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x03);
//  }
}
