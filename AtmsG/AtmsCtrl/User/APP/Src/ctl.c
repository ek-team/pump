#include "ctl.h"
#include "sys.h"
#include "stdlib.h"

void Pump(uint8_t IO) 
{
  if(IO == 0) //暂停
  {
    HAL_GPIO_WritePin(PUMP_EN_GPIO, PUMP_EN_GPIO_PIN, GPIO_PIN_RESET);
  }
  if(IO == 1) //启动
  {
    HAL_GPIO_WritePin(PUMP_EN_GPIO, PUMP_EN_GPIO_PIN, GPIO_PIN_SET);
  }
  if(IO == 2) //五指握拳1
  {
    N_SET(2,0);
    vTaskDelay(20);
    N_SET(1,1);
    vTaskDelay(20);
    N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
    vTaskDelay(20);
    N_SET(8,0);
    Pump(1);
    vTaskDelay(HOLD_TIME);N_SET(1,2);
  }
  if(IO == 3) //五指伸直1
  {
    N_SET(3,1);N_SET(4,1);
    vTaskDelay(20);
    N_SET(1,0);
    vTaskDelay(20);
    N_SET(2,1);
    vTaskDelay(20);
    N_SET(8,0);
    Pump(1);
    vTaskDelay(HOLD_TIME);N_SET(3,2);N_SET(4,2);N_SET(2,2);
  }
  if(IO == 4) //分指握拳
  {		
    N_SET(1,1);
    vTaskDelay(20);
    N_SET(2,0);
    vTaskDelay(20);
    N_SET(8,0);
    Pump(1);
    vTaskDelay(HOLD_TIME);N_SET(1,2);
  }
  if(IO == 5) //分指伸直
  {
    N_SET(1,1);
    N_SET(8,1);
    N_SET(2,0);
    if(DB.HandVal[1] + DB.HandVal[2] + DB.HandVal[3] + DB.HandVal[4] + DB.HandVal[5] == 5)
    {
      N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
    }
    else
    {
      N_SET(3,0);vTaskDelay(100);
      N_SET(4,0);vTaskDelay(100);
      N_SET(5,0);vTaskDelay(100);
      N_SET(6,0);vTaskDelay(100);
      N_SET(7,0);vTaskDelay(100);
    }
    Pump(1);
    vTaskDelay(500);
    N_SET(1,0);
    N_SET(2,0);
    N_SET(8,0);
  }
  if(IO == 6) //五指握拳背伸
  {
    N_SET(8,1);
    vTaskDelay(20);
    N_SET(2,0);
    vTaskDelay(20);
    N_SET(1,1);
    vTaskDelay(20);
    N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
    vTaskDelay(20);
    Pump(1);
    vTaskDelay(HOLD_TIME);N_SET(8,2);N_SET(1,2);
  }
  if(IO == 7) //五指伸直背伸
  {
    N_SET(3,1);N_SET(4,1);
    vTaskDelay(20);
    N_SET(1,0);
    vTaskDelay(20);
    N_SET(2,1);
    vTaskDelay(20);
    N_SET(8,0);
    vTaskDelay(20);
    Pump(1);
    vTaskDelay(HOLD_TIME);N_SET(3,2);N_SET(4,2);N_SET(2,2);
  }
}

void Escape(uint8_t EN)
{
  if(EN == 1)
  {
    N_SET(1,1);
    vTaskDelay(10);
    N_SET(2,1);
    N_SET(8,1);
    N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
  }
  if(EN == 0)
  {
    N_SET(1,0);
    vTaskDelay(10);
    N_SET(2,0);
    N_SET(8,0);
    N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
  }
}

void CTL0()  //无动作
{
}

void CTL1()  //分指-手套操
{
  U1_Tx_Pack_Long(0x08,DB.PressValDis);
  vTaskDelay(50);
  U1_Tx_Pack_Long(0x09,DB.ChipTmp);
  vTaskDelay(50);
  if(DB.GymFist == 1 && DB.HandTimer == 4) //握拳二阶段
  {
    DB.GymFist = 0;
    Pump(4);
    N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
    vTaskDelay(HOLD_TIME);N_SET(3,2);N_SET(4,2);N_SET(5,2);N_SET(6,2);N_SET(7,2);
    if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x02);
    if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x04);
  }
  if(DB.HandState == 1)
  {
    if(DB.Para[9] == 1 && DB.Para[10] == 1 && DB.Para[11] == 1 && DB.Para[12] == 1 && DB.Para[13] == 1) //握拳一阶段
    {
      DB.HandState = 0;
      Pump(4);
      N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
      vTaskDelay(HOLD_TIME);N_SET(4,2);N_SET(5,2);N_SET(6,2);N_SET(7,2);
      if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x1C);
      if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x23);
      DB.GymFist = 1;
    }
    else
    {
      DB.HandState = 0;
      Pump(4);
      if(DB.Para[9] == 1)  N_SET(3,1);
      if(DB.Para[10] == 1) N_SET(4,1);
      if(DB.Para[11] == 1) N_SET(5,1);
      if(DB.Para[12] == 1) N_SET(6,1);
      if(DB.Para[13] == 1) N_SET(7,1);
      vTaskDelay(HOLD_TIME);
      if(DB.Para[9] == 1)  N_SET(3,2);
      if(DB.Para[10] == 1) N_SET(4,2);
      if(DB.Para[11] == 1) N_SET(5,2);
      if(DB.Para[12] == 1) N_SET(6,2);
      if(DB.Para[13] == 1) N_SET(7,2);
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
  U1_Tx_Pack_Long(0x08,DB.PressValDis);
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
        if(DB.HandVal[i] == 0)
        {
          N_SET(i+2,1);
        }
      }
      if((DB.DiffFingerVal[i] < -20 && DB.FingerVal[i] > 30) || DB.FingerVal[i] <= 30)
      {
        N_SET(i+2,0);
      }
    }
    if(DB.HandVal[1] + DB.HandVal[2] + DB.HandVal[3] + DB.HandVal[4] + DB.HandVal[5] > 0)
    {
      DB.HandAim = 1;
    }
    else
    {
      DB.HandAim = 0;
    }
  }
  else
  {
    if(ADC_Val[HAND_ADC] > 1800 && ADC_Val[HAND_ADC] < 3200)
    {
      DB.HandAim = 1;
      HAL_GPIO_WritePin(ADC_PW_GPIO,ADC_PW_GPIO_PIN,GPIO_PIN_RESET);
      N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
    }
    if(ADC_Val[HAND_ADC] < 1700)
    {
      DB.HandAim = 0;
      HAL_GPIO_WritePin(ADC_PW_GPIO,ADC_PW_GPIO_PIN,GPIO_PIN_SET);
      N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
    }
  }
  if(FingerTmp != DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1)
  {
    Pump(4);
    FingerTmp = DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1;
    if(DB.EmerVal != 0) U1_Tx_Pack(0x0A,FingerTmp);
  }
  if(DB.HandAim != OldHandAim)
  {
    OldHandAim = DB.HandAim;
    vTaskDelay(50);
    U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
    if(DB.HandAim == 0)
    {
      if(DB.Para[8] == 1) U1_Tx_Pack(0x04,0x00);
      N_SET(1,1);
      vTaskDelay(20);
      N_SET(8,1);
      N_SET(2,0);
      vTaskDelay(20);
      Pump(1); 
      vTaskDelay(300);
      N_SET(1,0);
      vTaskDelay(20);
      N_SET(2,0);
      vTaskDelay(20);
      N_SET(8,0);
    }
    if(DB.HandAim == 1)
    {
      Pump(4);//握拳
      if(DB.Para[8] == 0) U1_Tx_Pack(0x04,0x01);
    }
  }
  vTaskDelay(HOLD_TIME);
  if(DB.HandVal[1] == 1) N_SET(3,2);
  if(DB.HandVal[2] == 1) N_SET(4,2);
  if(DB.HandVal[3] == 1) N_SET(5,2);
  if(DB.HandVal[4] == 1) N_SET(6,2);
  if(DB.HandVal[5] == 1) N_SET(7,2);
}

void CTL3()  //分指-功能
{
  U1_Tx_Pack_Long(0x08,DB.PressValDis);
  vTaskDelay(50);
  U1_Tx_Pack_Long(0x09,DB.ChipTmp);
  vTaskDelay(50);
  if(DB.HandState == 1)
  {
    DB.HandState = 0;
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
          N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
          vTaskDelay(HOLD_TIME);N_SET(3,2);N_SET(4,2);N_SET(5,2);N_SET(6,2);N_SET(7,2);
					
          if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x10); //左手
          if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x14); //右手
          U1_Tx_Pack(0x02,++DB.Count);               //发送次数
          break;
        }
        if(i == 1)
        {
          N_SET(3,1);N_SET(4,1);
          vTaskDelay(HOLD_TIME);N_SET(3,2);N_SET(4,2);
					
          if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x0E); //左手
          if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x12); //右手
          U1_Tx_Pack(0x02,++DB.Count);               //发送次数
          break;
        }
        if(i == 2)
        {
          N_SET(3,1);N_SET(4,1);N_SET(5,1);
          vTaskDelay(HOLD_TIME);N_SET(3,2);N_SET(4,2);N_SET(5,2);
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
//	static int16_t SaveBuf[2] = {0};
//	if(DB.AntiCount > 0 && DB.Count >= 2) 
//	{
//	  DB.AntiCount++;
//	
//		if(DB.AntiCount == 2)
//		{
//			uint16_t LoadBuf[2] = {0};
//	    STMFLASH_Read(0x0803E000,LoadBuf,2);
//	    DB.AntiCal1 = (int16_t)LoadBuf[0];
//		  DB.AntiCal2 = (int16_t)LoadBuf[1];
//		  DB.AntiSumP1 = 0;
//		  DB.AntiSumP2 = 0;
//		  DB.AntiSumP3 = 0;
//		}
//		//P1段分析
//		if(DB.AntiCount >= 15 && DB.AntiCount <= 25)
//	  {
//	    DB.AntiSumP1 += DB.PressVal;//伸直痉挛检测
//	  }
//		//P2段分析
//		if(DB.AntiCount >= 45 && DB.AntiCount <= 51)
//	  {
//	    DB.AntiSumP2 += DB.PressVal;
//	  }
//		//P3段分析
//		if(DB.AntiCount >= 52 && DB.AntiCount <= 60)
//	  {
//	    DB.AntiSumP3 += DB.PressVal;//握拳痉挛检测
//	  }
//		if(DB.AntiCount == 26)
//		{
//			U1_Tx_Pack_Long(0x0C,DB.AntiSumP1);
//		  if(DB.AntiSumP1 - DB.AntiCal1 >= 20)
//		  {
//			  vTaskDelay(50);
//			  U1_Tx_Pack(0x06,0x02);
//		  }
//		}
//		if(DB.AntiCount == 61)
//		{
//			U1_Tx_Pack_Long(0x0C,DB.AntiSumP3 - DB.AntiSumP2);
//		  if(DB.AntiSumP3 - DB.AntiSumP2 - DB.AntiCal2 <= -20)
//		  {
//				vTaskDelay(50);
//			  U1_Tx_Pack(0x06,0x02);
//		  }
//			if(DB.Count >= 3)
//			{
//				if(DB.StudyFlag == 1)
//				{
//			    SaveBuf[0] = DB.AntiSumP1;
//			    SaveBuf[1] = DB.AntiSumP3 - DB.AntiSumP2;
//					DB.StudyFlag = 2;
//				}
//				else if(DB.StudyFlag == 2)
//				{
//				  SaveBuf[0] = (SaveBuf[0] + DB.AntiSumP1)/2;
//					SaveBuf[1] = (SaveBuf[1] + DB.AntiSumP3 - DB.AntiSumP2)/2;
//					DB.StudyFlag = 3;
//				}
//				else if(DB.StudyFlag == 3)
//				{
//					SaveBuf[0] = (SaveBuf[0] + DB.AntiSumP1)/2;
//					SaveBuf[1] = (SaveBuf[1] + DB.AntiSumP3 - DB.AntiSumP2)/2;
//					STMFLASH_Write(0x0803E000 + 0,(uint16_t*)&SaveBuf[0],1);
//			    STMFLASH_Write(0x0803E000 + 2,(uint16_t*)&SaveBuf[1],1);
//					DB.StudyFlag = 0;
//				}
//			}
//			DB.AntiCount = 0;
//		}
//	}
  U1_Tx_Pack_Long(0x08,DB.PressValDis);
  vTaskDelay(50);
  U1_Tx_Pack_Long(0x09,DB.ChipTmp);
  vTaskDelay(50);
  if(DB.HandState == 1)
  {
	  DB.HandState = 0;
		if(DB.Para[26] == 1)
		{
			Pump(6);
			if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x15);
		  if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x16);
	  }
		else
		{
		  Pump(2);
			if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x02);   //左手
		  if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x04);   //右手
		}
		U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
		DB.AntiCount = 1;
	}
	if(DB.HandState == 2)
	{
		DB.HandState = 0;
		if(DB.Para[26] == 1)
		{
			Pump(7);
	  }
		else
		{
		  Pump(3);
		}
		if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x01);
		if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x03);
	}
}

void CTL5()  //五指-随动
{
	static uint8_t OldHandAim = 0;
	static uint8_t OldFingerTmp = 0;
	uint8_t FingerTmp = 0;
	U1_Tx_Pack_Long(0x08,DB.PressValDis);
	vTaskDelay(20);
	U1_Tx_Pack_Long(0x09,DB.ChipTmp);
	vTaskDelay(20);
  U3_Tx_Pack(1);
	vTaskDelay(50);
	U3_Tx_Pack(5);
	vTaskDelay(50);
	if(DB.AimFlag == 1)
	{
		DB.AimFlag = 0;
	  for(uint8_t i=1;i<6;i++)
	  {
	    if(DB.FingerVal[3] > 50)
		  {
		    DB.HandAim = 1;
		  }
		  else
		  {
			  DB.HandAim = 0;
		  }
		}
	  if(DB.FingerVal[1] > 50 && DB.FingerVal[2] > 50 && DB.FingerVal[3] > 50 && DB.FingerVal[4] > 50 && DB.FingerVal[5] > 50)
		{
		  FingerTmp = 0x1F;
	  }
		else if(DB.FingerVal[1] < 50 && DB.FingerVal[2] < 50 && DB.FingerVal[3] < 50 && DB.FingerVal[4] < 50 && DB.FingerVal[5] < 50)
		{
			FingerTmp = 0x00;
	  }
		else
		{
		  FingerTmp = 0x10;
		}
    if(FingerTmp != OldFingerTmp)
		{
      if(DB.EmerVal != 0) U1_Tx_Pack(0x0A,FingerTmp);
			OldFingerTmp = FingerTmp;
		}
	}
	else
	{
	  if(ADC_Val[HAND_ADC] > 1800 && ADC_Val[HAND_ADC] < 3200)
	  {
	    DB.HandAim = 1;
		  HAL_GPIO_WritePin(ADC_PW_GPIO,ADC_PW_GPIO_PIN,GPIO_PIN_RESET);
	  }
	  if(ADC_Val[HAND_ADC] < 1700)
	  {
	    DB.HandAim = 0;
		  HAL_GPIO_WritePin(ADC_PW_GPIO,ADC_PW_GPIO_PIN,GPIO_PIN_SET);
	  }
	}
		
	if(DB.HandAim != OldHandAim)
	{
	  OldHandAim = DB.HandAim;
		U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
	  if(DB.HandAim == 0)
  	{
	  	Pump(3);
	  	U1_Tx_Pack(0x01,0x01);
	  	U1_Tx_Pack(0x01,0x03);
			if(DB.Para[8] == 1) U1_Tx_Pack(0x04,0x00);
			vTaskDelay(100);
	  }
	  if(DB.HandAim == 1)
	  {
		  Pump(2);
	    U1_Tx_Pack(0x01,0x02);
		  U1_Tx_Pack(0x01,0x04);
			if(DB.Para[8] == 0) U1_Tx_Pack(0x04,0x01);
			vTaskDelay(100);
		}
	}
}

void CTL6()  //五指-助力
{
	uint8_t Acc = 0;
	Acc = 12 - 2 * DB.Para[24];
  U1_Tx_Pack_Long(0x08,DB.PressValDis);
	vTaskDelay(50);
	U1_Tx_Pack_Long(0x09,DB.ChipTmp);
	vTaskDelay(50);
	if(DB.HelpStep == 0)
	{
		Pump(3);
		N_SET(2,1);
		while(DB.PressVal > -30)
		{
			vTaskDelay(10);
			if(DB.EmerVal == 0) return;
			if(DB.Act != 0) return;
		}
		if(DB.EmerVal != 0)
		{
		  Pump(1);//如果在循环期间按了急停,会被这个代码重新启动急停
		}
		N_SET(1,1);
		vTaskDelay(20);
		N_SET(2,1);
		vTaskDelay(20);
		N_SET(8,1);
		N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
		uint16_t Count = 150;
		while(DB.PressVal < -4)
	  {
		  vTaskDelay(10);
			if(DB.EmerVal == 0) return;
			if(DB.Act != 0) return;
			if(--Count == 0) break;
		}
		N_SET(1,0);
    vTaskDelay(20);
		N_SET(2,0);
    vTaskDelay(20);
		N_SET(8,0);
		N_SET(3,1);N_SET(4,1);N_SET(5,0);N_SET(6,0);N_SET(7,0);
    vTaskDelay(HOLD_TIME);N_SET(3,2);N_SET(4,2);
		uint16_t i = 200;
		while(--i)
		{
		  vTaskDelay(10); //等待数值稳定
			if(DB.EmerVal == 0) return;
			if(DB.Act != 0) return;
		}
		DB.HelpOldVal = (uint16_t)ADC_Val[PRESS_ADC];
		if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x15);     //开始等待
		if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x16);
		DB.HelpStep = 1;
	}
	if(DB.HelpStep == 1)
	{
		DB.HelpVal = (uint16_t)ADC_Val[PRESS_ADC];
		//printf(" %d-%d ",DB.HelpOldVal,DB.HelpVal);
		if((DB.HelpVal > DB.HelpOldVal + Acc) && (DB.Para[23] == 0 || DB.Para[23] == 1))
		{
      Pump(3);
			DB.HelpStep = 3;
			if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x01);
		  if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x03);
			U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
		}
		if((DB.HelpVal < DB.HelpOldVal - Acc)  && (DB.Para[23] == 0 || DB.Para[23] == 2))
		{
      Pump(2);
		  DB.HelpStep = 2;
			if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x02);
		  if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x04);
			U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
		}
	}
	if(DB.HelpStep >= 2)
	{
		uint16_t i = 100;
		while(--i)
		{
		  vTaskDelay(10);
			if(DB.EmerVal == 0) return;
			if(DB.Act != 0) return;
		}
		DB.HelpTimer++;
		if((DB.HelpTimer >= DB.Para[3] && DB.HelpStep == 2) || (DB.HelpTimer >= DB.Para[4] && DB.HelpStep == 3))
		{
			DB.HelpStep = 0;
		  DB.HelpTimer = 0;
		}
	}
}

void CTL7()  //五指-抗阻
{
	uint8_t Acc = 0;
	Acc = 12 - 2 * DB.Para[24];
	U1_Tx_Pack_Long(0x08,DB.PressValDis);
	vTaskDelay(50);
	U1_Tx_Pack_Long(0x09,DB.ChipTmp);
	vTaskDelay(50);
	if(DB.HelpStep == 0)
	{
		Pump(3);
		N_SET(2,1);
		while(DB.PressVal > -30)
		{
			vTaskDelay(10);
			if(DB.EmerVal == 0) return;
			if(DB.Act != 0) return;
		}
		if(DB.EmerVal != 0)
		{
		  Pump(1);//如果在循环期间按了急停,会被这个代码重新启动急停
		}
		N_SET(1,1);
    vTaskDelay(20);
		N_SET(2,1);
		vTaskDelay(20);
		N_SET(8,1);
		N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
		uint16_t Count = 150;
		while(DB.PressVal < -4)
	  {
		  vTaskDelay(10);
			if(DB.EmerVal == 0) return;
			if(DB.Act != 0) return;
			if(--Count == 0) break;
		}
		N_SET(1,0);
    vTaskDelay(20);
		N_SET(2,0);
		vTaskDelay(20);
		N_SET(8,0);
		N_SET(3,1);N_SET(4,1);N_SET(5,0);N_SET(6,0);N_SET(7,0);
		vTaskDelay(HOLD_TIME);N_SET(3,2);N_SET(4,2);
		uint16_t i = 200;
		while(--i)
		{
		  vTaskDelay(10); //等待数值稳定
			if(DB.EmerVal == 0) return;
			if(DB.Act != 0) return;
		}
		DB.HelpOldVal = (uint16_t)ADC_Val[PRESS_ADC];
		if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x15);     //开始等待
		if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x16);
		DB.HelpStep = 1;
	}
	if(DB.HelpStep == 1)
	{
		DB.HelpVal = (uint16_t)ADC_Val[PRESS_ADC];
		//printf(" %d-%d ",DB.HelpOldVal,DB.HelpVal);
		if(DB.HelpVal > DB.HelpOldVal + Acc)
		{
      Pump(2);
			DB.HelpStep = 3;
			if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x01);
		  if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x03);
			U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
		}
		if(DB.HelpVal < DB.HelpOldVal - Acc)
		{
      Pump(3);
		  DB.HelpStep = 2;
			if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x02);
		  if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x04);
			U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
		}
	}
	if(DB.HelpStep >= 2)
	{
		uint16_t i = 100;
		while(--i)
		{
		  vTaskDelay(10);
			if(DB.EmerVal == 0) return;
			if(DB.Act != 0) return;
		}
		DB.HelpTimer++;
		if((DB.HelpTimer >= DB.Para[3] && DB.HelpStep == 2) || (DB.HelpTimer >= DB.Para[4] && DB.HelpStep == 3))
		{
			DB.HelpStep = 0;
		  DB.HelpTimer = 0;
		}
	}
}

void CTL9()  //分指-对指
{
	U1_Tx_Pack_Long(0x08,DB.PressValDis);
	vTaskDelay(50);
	U1_Tx_Pack_Long(0x09,DB.ChipTmp);
	vTaskDelay(50);
  if(DB.HandState == 1)
  {
		if(DB.Oppo[2] + DB.Oppo[3] + DB.Oppo[4] + DB.Oppo[5] + DB.Oppo[6] == DB.Para[15] + DB.Para[16] + DB.Para[17] + DB.Para[18] + DB.Para[19])
		{
			DB.Oppo[2] = 0;DB.Oppo[3] = 0;DB.Oppo[4] = 0;DB.Oppo[5] = 0;DB.Oppo[6] = 0;
		}
		for(uint8_t i=2;i<=6;i++)
		{
			if(DB.Para[i + 13] == 1 && DB.Oppo[i] == 0)
			{
				if(i<=5)
				{
					DB.Oppo[i] = 1;
					DB.StopVal = i;
					DB.HandState = 0;
					Pump(4);
					N_SET(3,1);
			    if(i == 2) N_SET(4,1);
			    if(i == 3) N_SET(5,1);
			    if(i == 4) N_SET(6,1);
			    if(i == 5) N_SET(7,1);
          vTaskDelay(HOLD_TIME);
          N_SET(3,2);
			    if(i == 2) N_SET(4,2);
			    if(i == 3) N_SET(5,2);
			    if(i == 4) N_SET(6,2);
                if(i == 5) N_SET(7,2);
					if(DB.Para[2] == 0) U1_Tx_Pack(0x01,i+3); //左手
				  if(DB.Para[2] == 1) U1_Tx_Pack(0x01,i+7); //右手
					U1_Tx_Pack(0x02,++DB.Count);              //发送次数
				}
				else
				{
					if(DB.HandTimer <= 2) //握拳一阶段
					{
						if(DB.GymFist == 0)
						{
							Pump(4);
			        if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x1C);
			        if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x23);
						}
						N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
						vTaskDelay(HOLD_TIME);N_SET(4,2);N_SET(5,2);N_SET(6,2);N_SET(7,2);
						DB.GymFist = 1;
					}
					else                 //握拳二阶段
					{
						DB.GymFist = 0;
						DB.Oppo[i] = 1;
						DB.StopVal = i;
						DB.HandState = 0;
						Pump(4);
						N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
						vTaskDelay(HOLD_TIME);N_SET(3,2);N_SET(4,2);N_SET(5,2);N_SET(6,2);N_SET(7,2);				
					  if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x02);//左手
				    if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x04);//右手
					  U1_Tx_Pack(0x02,++DB.Count);              //发送次数
					}
        }
        break;
			}
		}
	}
	if(DB.HandState == 2)
	{
		DB.HandState = 0;
    Pump(5);
		if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x01); //左手
		if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x03); //右手
	}
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
  static uint8_t OldHandAim = 0;
  U3_Tx_Pack(1);
  vTaskDelay(50);
  U3_Tx_Pack(5);
  vTaskDelay(50);
  if(DB.AimFlag == 1)
  {
    DB.AimFlag = 0;
    if(DB.FingerVal[1] > 50 || DB.FingerVal[2] > 50 || DB.FingerVal[3] > 50 || DB.FingerVal[4] > 50 || DB.FingerVal[5] > 50)
    {
      DB.HandAim = 1;
    }
    else
    {
      DB.HandAim = 0;
    }
  }
  else
  {
    if(ADC_Val[HAND_ADC] > 1800 && ADC_Val[HAND_ADC] < 3200)
    {
      DB.HandAim = 1;
      HAL_GPIO_WritePin(ADC_PW_GPIO,ADC_PW_GPIO_PIN,GPIO_PIN_RESET);
    }
    if(ADC_Val[HAND_ADC] < 1700)
    {
      DB.HandAim = 0;
      HAL_GPIO_WritePin(ADC_PW_GPIO,ADC_PW_GPIO_PIN,GPIO_PIN_SET);
    }
  }
  if(DB.HandAim != OldHandAim)
  {
    OldHandAim = DB.HandAim;
    if(DB.HandAim == 0)
    {
      Pump(3);
      if(DB.Para[8] == 1) U1_Tx_Pack(0x04,0x00);
      vTaskDelay(100);
    }
    if(DB.HandAim == 1)
    {
      Pump(2);
      if(DB.Para[8] == 0) U1_Tx_Pack(0x04,0x01);
      vTaskDelay(100);
    }
  }
}

void CTL8()  //分指-多功能被动
{
  uint8_t FingerTmp = 128;
  U1_Tx_Pack_Long(0x08,DB.PressValDis);
  vTaskDelay(50);
  U1_Tx_Pack_Long(0x09,DB.ChipTmp);
  vTaskDelay(50);
  if(DB.HandState == 1 && DB.Para[14] == 0)//非被动顺序
  {
    if(DB.Para[9] == 1 && DB.Para[10] == 1 && DB.Para[11] == 1 && DB.Para[12] == 1 && DB.Para[13] == 1 && DB.HandTimer <= 2) //握拳一阶段
    {
      Pump(4);
      N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
      vTaskDelay(HOLD_TIME);N_SET(4,2);N_SET(5,2);N_SET(6,2);N_SET(7,2);
      if(FingerTmp != DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1)
      {
        FingerTmp = DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1;
        if(DB.EmerVal != 0) U1_Tx_Pack(0x0A,FingerTmp);
      }
    }
    else
    {
      DB.HandState = 0;
      Pump(4);
      if(DB.Para[9] == 1)  N_SET(3,1);
      if(DB.Para[10] == 1) N_SET(4,1);
      if(DB.Para[11] == 1) N_SET(5,1);
      if(DB.Para[12] == 1) N_SET(6,1);
      if(DB.Para[13] == 1) N_SET(7,1);
      vTaskDelay(HOLD_TIME);
      if(DB.Para[9] == 1)  N_SET(3,2);
      if(DB.Para[10] == 1) N_SET(4,2);
      if(DB.Para[11] == 1) N_SET(5,2);
      if(DB.Para[12] == 1) N_SET(6,2);
      if(DB.Para[13] == 1) N_SET(7,2);
			
      if(FingerTmp != DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1)
      {
        FingerTmp = DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1;
        if(DB.EmerVal != 0) U1_Tx_Pack(0x0A,FingerTmp);
      }
    }
  }
  if(DB.HandState == 2 && DB.Para[14] == 0)
  {
    DB.HandState = 0;
    Pump(5);
    if(FingerTmp != DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1)
    {
      FingerTmp = DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1;
      if(DB.EmerVal != 0) U1_Tx_Pack(0x0A,FingerTmp);
    }
    U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
  }
  if(DB.HandState == 1 && DB.Para[14] == 1)
  {
    DB.HandState = 0;
    Pump(4);
    if(DB.Finger[1] + DB.Finger[2] + DB.Finger[3] + DB.Finger[4] + DB.Finger[5] == DB.Para[9] + DB.Para[10] + DB.Para[11] + DB.Para[12] + DB.Para[13])
    {
      DB.Finger[1] = 0;DB.Finger[2] = 0;DB.Finger[3] = 0;DB.Finger[4] = 0;DB.Finger[5] = 0;
    }
    for(uint8_t i=1;i<=5;i++)
    {
      if(DB.Para[i + 8] == 1 && DB.Finger[i] == 0)
      {
        DB.Finger[i] = 1;
        DB.StopVal = i;
        if(i<=5)
        {
          N_SET(i+2,1);
          vTaskDelay(HOLD_TIME);
          N_SET(i+2,2);
        }
        break;
      }
    }
    if(FingerTmp != DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1)
    {
      FingerTmp = DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1;
      if(DB.EmerVal != 0) U1_Tx_Pack(0x0A,FingerTmp);
    }
    U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
  }
  if(DB.HandState == 2 && DB.Para[14] == 1)
  {
    DB.HandState = 0;
    Pump(5);
    if(FingerTmp != DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1)
    {
      FingerTmp = DB.HandVal[1]*16 + DB.HandVal[2]*8 + DB.HandVal[3]*4 + DB.HandVal[4]*2 + DB.HandVal[5]*1;
      if(DB.EmerVal != 0) U1_Tx_Pack(0x0A,FingerTmp);
    }
  }
}

void CTL12()  //分指语音
{
  static uint8_t Flag;
  if((Flag == 1 && DB.HandState == 2) || (Flag == 2 && DB.HandState == 1))
  {
    Flag = 0;
    Pump(0);
    U1_Tx_Pack(0x02,++DB.Count);
  }
  if(DB.Act == 4) 
  {
    Pump(4);
    N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);
    vTaskDelay(HOLD_TIME);N_SET(3,2);N_SET(4,2);N_SET(5,2);N_SET(6,2);N_SET(7,2);
		
    DB.HandTimer = 0;
    DB.HandState = 1;
    Flag = 1;
    DB.Act = 0;
    U1_Tx_Pack(0x0A,31);
  }
  if(DB.Act == 5)
  {
    Pump(5);
    for(uint8_t i=1;i<=5;i++)
    {
      N_SET(i+2,0);
    }
    DB.HandTimer = DB.Para[3] + 1;
    DB.HandState = 2;
    Flag = 2;
    DB.Act = 0;
    U1_Tx_Pack(0x0A,0);
  }
}
void CTL13()  //五指语音
{
  static uint8_t Flag;
  if((Flag == 1 && DB.HandState == 2) || (Flag == 2 && DB.HandState == 1))
  {
    Flag = 0;
    Pump(0);
    U1_Tx_Pack(0x02,++DB.Count);
  }
  if(DB.Act == 4) 
  {
    Pump(2);
    DB.HandTimer = 0;
    DB.HandState = 1;
    Flag = 1;
    DB.Act = 0;
    U1_Tx_Pack(0x0A,31);
  }
  if(DB.Act == 5)
  {
    Pump(3);
    DB.HandTimer = DB.Para[3] + 1;
    DB.HandState = 2;
    Flag = 2;
    DB.Act = 0;
    U1_Tx_Pack(0x0A,0);
  }
}

void CTL14()  //评估模式
{
  static int16_t PressCount1,PressCount2,PressCount3,PressCount4,PressAimCmd,Leak = 0;
  if(DB.PressAimFlag == 0) //停止
  {
    Pump(0);
  }
  if(DB.PressAimFlag == 1) //不需要运动的情况
  {
    PressCount1 = 0;
    DB.PressAimFlag = 0;
    Pump(0);
    Escape(0);
    U1_Tx_Pack_Long(0xA3,DB.PressAim);
  }
  if(DB.PressAimFlag == 2) //气压判断(一阶段)
  {
    PressCount1 = 0;
    if(DB.PressValDis > DB.PressAimTmp) //气压要变小
    {
      Pump(3);
      DB.PressAimFlag = 3;
    }
    else if(DB.PressValDis < DB.PressAimTmp) //气压要变大
    {
      Pump(2);
      DB.PressAimFlag = 4;
    }
    else
    {
      DB.PressAimFlag = 5;
    }
  }
  if(DB.PressAimFlag == 3) //气压变小中(一阶段)
  {
    if(DB.PressValDis <= DB.PressAimTmp) 
    {
      Pump(0);
      DB.PressAimFlag = 5;
    }
  }
  if(DB.PressAimFlag == 4) //气压变大中(一阶段)
  {
    if(DB.PressValDis >= DB.PressAimTmp)
    {
      Pump(0);
      DB.PressAimFlag = 5;
    }
  }
  if(DB.PressAimFlag == 5) //气压判断(二阶段)
  {
    if(DB.PressValDis < DB.PressAim) //缓慢增加气压
    {
      DB.PressAimFlag = 6;
      N_SET(2,0);
      vTaskDelay(20);
      N_SET(1,1);
      vTaskDelay(20);
      N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
      vTaskDelay(20);
      N_SET(8,0);
      Pump(1);
    }
    else //缓慢减少气压
    {
      DB.PressAimFlag = 7;
      N_SET(3,1);N_SET(4,1);
      vTaskDelay(20);
      N_SET(1,0);
      vTaskDelay(20);
      N_SET(2,1);
      vTaskDelay(20);
      N_SET(8,0);
      Pump(1);
    }
  }
  if(DB.PressAimFlag == 6)//缓慢增加气压中(二阶段)
  {
    PressCount1++;
    //printf(" %d ",PressCount2);
    if(PressCount1 > 200) //超时后全速增加气压
    {
      Pump(1);
    }
    else
    {
      Pump(1);vTaskDelay(10);Pump(0);vTaskDelay(10);
    }
    if(DB.PressValDis >= DB.PressAim) //达到目标气压后停泵
    {
      Pump(0);
      vTaskDelay(1000);
      //printf(" 1 %d %d ",DB.PressValDis,DB.PressAim);
      if(DB.PressValDis > DB.PressAim - 15 && Leak == 0) //如果1秒内不存在气压漏气的情况
      {
        DB.PressAimFlag = 10;
        Escape(0);
      }
      else
      {
        Leak = 1;
        DB.PressAimFlag = 8;
        PressAimCmd = DB.PressAim;
        Pump(0);
      }
    }
  }
  if(DB.PressAimFlag == 7)//缓慢减少气压中(二阶段)
  {
    PressCount1++;
    //printf(" %d ",PressCount2);
    if(PressCount1 > 200) //超时后全速减少气压
    {
      Pump(1);
    }
    else
    {
      Pump(1);vTaskDelay(10);Pump(0);vTaskDelay(20);
    }
    if(DB.PressValDis <= DB.PressAim) //达到目标气压后停泵
    {
      Pump(0);
      vTaskDelay(1000);
      //printf(" 2 %d %d ",DB.PressValDis,DB.PressAim);
      if(DB.PressValDis < DB.PressAim + 15 && Leak == 0) //如果1秒内不存在气压漏气的情况
      {
        DB.PressAimFlag = 10;
        Escape(0);
      }
      else
      {
        Leak = 1;
        DB.PressAimFlag = 9;
        PressAimCmd = DB.PressAim;
        Pump(0);
      }
    }
  }
  if(DB.PressAimFlag == 8)
  {
    if(DB.PressValDis - PressAimCmd > 30 && PressAimCmd == DB.PressAim) //正压下,气压如果跳到新的高度,调整补气目标值
    {
      if(PressCount3++ > 500)
      {
        PressCount3 = 0;
        PressAimCmd = DB.PressValDis;
        //printf(" A1 %d ",PressAimCmd);
      }
    }
    else 
    {
      PressCount3 = 0;
    }
    if(PressAimCmd - DB.PressValDis > 10 && PressAimCmd != DB.PressAim) //正压下,如果气压回到原始值,调整补气目标值
    {
      if(PressCount4++ > 50)
      {
        PressCount4 = 0;
        PressAimCmd = DB.PressAim;
        //printf(" A2 %d ",PressAimCmd);
      }
    }
    else 
    {
      PressCount4 = 0;
    }
    if(DB.PressValDis < PressAimCmd) //根据补气目标值调整开泵和停泵
    {
      PressCount2++;
    }
    else
    {
      Pump(0);
    }
    if(PressCount2 > 10)
    {
      Pump(1);
      PressCount2 = 0;
    }
  }
  if(DB.PressAimFlag == 9)
  {
    if(PressAimCmd - DB.PressValDis> 30 && PressAimCmd == DB.PressAim)  //负压下,气压如果跳到新的高度,调整补气目标值
    {
      if(PressCount3++ > 500)
      {
        PressCount3 = 0;
        PressAimCmd = DB.PressValDis;
        //printf(" B1 %d ",PressAimCmd);
      }
    }
    else 
    {
      PressCount3 = 0;
    }
    if(DB.PressValDis - PressAimCmd > 10 && PressAimCmd != DB.PressAim) //正压下,如果气压回到原始值,调整补气目标值
    {
      if(PressCount4++ > 50)
      {
        PressCount4 = 0;
        PressAimCmd = DB.PressAim;
        //printf(" B2 %d ",PressAimCmd);
      }
    }
    else 
    {
      PressCount4 = 0;
    }
    if(DB.PressValDis > PressAimCmd) //根据补气目标值调整开泵和停泵
    {
      PressCount2++;
    }
    else
    {
      Pump(0);
    }
    if(PressCount2 > 10)
    {
      Pump(1);
      PressCount2 = 0;
    }
  }
}

void CTL15() //分指助力
{
  static int16_t Count1,Count2,Count3;
  uint8_t Acc = 0;
  Acc = (16 - 2 * DB.Para[24]);
  Count1++;
  vTaskDelay(10);
  if(DB.HelpStep == 0)
  {
    if(DB.PressValDis < 100) 
    {
      Count3++;
    }
    else
    {
      Pump(0);
    }
    if(Count3 > 10)
    {
      N_SET(1,1);
      N_SET(2,0);
      N_SET(8,0);
      N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);//分指握拳+通气
      Pump(1);
      Count3 = 0;
    }
    if(Count1 > 20)
    {
      Count1 = 0;
      DB.HelpVal = DB.PressValDis;
      Count2++;
      if(Count2 > 5)
      {
        DB.HelpStep = 1;
        if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x15);     //开始等待
        if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x16);
        Count2 = 0;
        N_SET(1,1);
        N_SET(2,0);
        N_SET(8,0);
        N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);//分指握拳+通气+ECO
        vTaskDelay(HOLD_TIME);
        N_SET(1,2);N_SET(3,2);N_SET(4,2);N_SET(5,2);N_SET(6,2);N_SET(7,2);
      }
      DB.HelpOldVal = DB.HelpVal;
    }
  }
  if(DB.HelpStep == 1)
  {
    if(DB.PressValDis < 100) //根据补气目标值调整开泵和停泵
    {
      Count3++;
    }
    else
    {
      Pump(0);
    }
    if(Count3 > 100)
    {
      Pump(1);
      Count3 = 0;
      Count1 = 0;
      DB.HelpOldVal = 100;
    }
    if(Count1 > 100)
    {
      Count1 = 0;
      DB.HelpVal = DB.PressValDis;
      printf("-%d-%d-",DB.HelpVal,DB.HelpOldVal);
      if((DB.HelpVal > DB.HelpOldVal + Acc) && (DB.Para[23] == 0 || DB.Para[23] == 1))
      {
        Pump(5);
        DB.HelpStep = 3;
        if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x01);
        if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x03);
        U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
      }
      if((DB.HelpVal < DB.HelpOldVal - Acc)  && (DB.Para[23] == 0 || DB.Para[23] == 2))
      {
        Pump(4);
        DB.HelpStep = 2;
        if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x02);
        if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x04);
        U1_Tx_Pack(0x02,++DB.Count);                  //发送次数
      }
      DB.HelpOldVal = DB.HelpVal;
    }
  }
  if(DB.HelpStep >= 2)
  {
    uint16_t i = 100;
    while(--i)
    {
      vTaskDelay(10);
      if(DB.EmerVal == 0) return;
      if(DB.Act != 0) return;
    }
    DB.HelpTimer++;
    if((DB.HelpTimer >= DB.Para[3] && DB.HelpStep == 2) || (DB.HelpTimer >= DB.Para[4] && DB.HelpStep == 3))
    {
      DB.HelpStep = 0;
      DB.HelpTimer = 0;
      Pump(0);
      Escape(1);
      vTaskDelay(2000);
      Escape(0);
    }
  }
}

void CTL16()//分指抗阻
{
  static int16_t Count1,Count2,Count3;
  uint8_t Acc = 0;
  Acc = (16 - 2 * DB.Para[24]);
  Count1++;
  vTaskDelay(10);
  if(DB.HelpStep == 0)
  {
    if(DB.PressValDis < 100) 
    {
      Count3++;
    }
    else
    {
      Pump(0);
    }
    if(Count3 > 10)
    {
      N_SET(1,1);
      N_SET(2,0);
      N_SET(8,0);
      N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);//分指握拳+通气
      Pump(1);
      Count3 = 0;
    }
    if(Count1 > 20)
    {
      Count1 = 0;
      DB.HelpVal = DB.PressValDis;
      Count2++;
      if(Count2 > 5)
      {
        DB.HelpStep = 1;
        if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x15);     //开始等待
        if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x16);
        Count2 = 0;
        N_SET(1,1);
        N_SET(2,0);
        N_SET(8,0);
        N_SET(3,1);N_SET(4,1);N_SET(5,1);N_SET(6,1);N_SET(7,1);//分指握拳+通气+ECO
        vTaskDelay(HOLD_TIME);
        N_SET(1,2);N_SET(3,2);N_SET(4,2);N_SET(5,2);N_SET(6,2);N_SET(7,2);
        Count2 = 0;
      }
      DB.HelpOldVal = DB.HelpVal;
    }
  }
  if(DB.HelpStep == 1)
  {
    if(DB.PressValDis < 100) //根据补气目标值调整开泵和停泵
    {
      Count3++;
    }
    else
    {
      Pump(0);
    }
    if(Count3 > 100)
    {
      Pump(1);
      Count3 = 0;
      Count1 = 0;
      DB.HelpOldVal = 100;
    }
    if(Count1 > 100)
    {
      Count1 = 0;
      DB.HelpVal = DB.PressValDis;
      if(DB.HelpVal > DB.HelpOldVal + Acc)
      {
        Pump(4);
        DB.HelpStep = 3;
        if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x01);
        if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x03);
        U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
      }
      if(DB.HelpVal < DB.HelpOldVal - Acc)
      {
        Pump(5);
        DB.HelpStep = 2;
        if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x02);
        if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x04);
        U1_Tx_Pack(0x02,++DB.Count);                  //发送次数
      }
      DB.HelpOldVal = DB.HelpVal;
    }
  }
  if(DB.HelpStep >= 2)
  {
    uint16_t i = 100;
    while(--i)
    {
      vTaskDelay(10);
      if(DB.EmerVal == 0) return;
      if(DB.Act != 0) return;
    }
    DB.HelpTimer++;
    if((DB.HelpTimer >= DB.Para[3] && DB.HelpStep == 2) || (DB.HelpTimer >= DB.Para[4] && DB.HelpStep == 3))
    {
      DB.HelpStep = 0;
      DB.HelpTimer = 0;
      Pump(0);
      Escape(1);
      vTaskDelay(2000);
      Escape(0);
    }
  }
}

void CTL17()//分指伸展
{
  U1_Tx_Pack_Long(0x08,DB.PressValDis);
  vTaskDelay(50);
  U1_Tx_Pack_Long(0x09,DB.ChipTmp);
  vTaskDelay(50);
  if(DB.HandState == 1)
  {
    DB.HandState = 0;
    Escape(1);
    vTaskDelay(HOLD_TIME);//Escape加ECO
    N_SET(1,2);
    N_SET(2,2);
    N_SET(8,2);
    N_SET(3,2);N_SET(4,2);N_SET(5,2);N_SET(6,2);N_SET(7,2);
    if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x15);
    if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x16);
    U1_Tx_Pack(0x02,++DB.Count);                 //发送次数
  }
  if(DB.HandState == 2)
  {
    DB.HandState = 0;
    Pump(5);
    if(DB.Para[2] == 0) U1_Tx_Pack(0x01,0x01);
    if(DB.Para[2] == 1) U1_Tx_Pack(0x01,0x03);
  }
}
