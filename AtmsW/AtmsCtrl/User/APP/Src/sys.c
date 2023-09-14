#include "sys.h"
#include "ctl.h"
#include "stdlib.h"

Data_Base DB;
IWDG_HandleTypeDef hiwdg;                                             //定义看门狗

SemaphoreHandle_t U1_Tx_Sem = NULL;
SemaphoreHandle_t U2_Tx_Sem = NULL;
SemaphoreHandle_t U3_Tx_Sem = NULL;

QueueHandle_t     U1_Rx_Que = NULL;
QueueHandle_t     U2_Rx_Que = NULL;
QueueHandle_t     U3_Rx_Que = NULL;

void AppTaskCreate(void)
{
  U1_Tx_Sem = xSemaphoreCreateBinary();  //上位机发送               
  U2_Tx_Sem = xSemaphoreCreateBinary();  //电机口发送
  U3_Tx_Sem = xSemaphoreCreateBinary();  //调试口发送
	
  U1_Rx_Que = xQueueCreate(QUE_LEN, sizeof(struct RxMsg *));  //上位机接收
  U2_Rx_Que = xQueueCreate(QUE_LEN, sizeof(struct RxMsg *));  //电机口接收
  U3_Rx_Que = xQueueCreate(QUE_LEN, sizeof(struct RxMsg *));  //调试口接收
	
  xTaskCreate(Task_HRx,"Task_HRx",200,NULL,4,NULL);           //上位机接收任务
  xTaskCreate(Task_HTx,"Task_HTx",200,NULL,3,NULL);           //上位机发送任务
	
  xTaskCreate(Task_Tim,"Task_Tim",200,NULL,2,NULL);           //定时任务
  xTaskCreate(Task_Saf,"Task_Saf",200,NULL,4,NULL);           //保护任务
  xTaskCreate(Task_Act,"Task_Act",200,NULL,2,NULL);           //运动任务
  xTaskCreate(Task_Tmp,"Task_Tmp",200,NULL,1,NULL);           //温度任务
  xTaskCreate(Task_Spd,"Task_Tmp",200,NULL,1,NULL);           //转速任务
  xTaskCreate(Task_DRx,"Task_DRx",200,NULL,2,NULL);           //调试任务
}

void Send_Edition()
{
  uint8_t Date[12] = __DATE__;
  uint8_t time[2] = {0};
  if(Date[0] == 'J' && Date[1] == 'a' && Date[2] == 'n') time[0] = (Date[10] - '0')*16 + 1;  
  if(Date[0] == 'F' && Date[1] == 'e' && Date[2] == 'b') time[0] = (Date[10] - '0')*16 + 2;  
  if(Date[0] == 'M' && Date[1] == 'a' && Date[2] == 'r') time[0] = (Date[10] - '0')*16 + 3;  
  if(Date[0] == 'A' && Date[1] == 'p' && Date[2] == 'r') time[0] = (Date[10] - '0')*16 + 4;  
  if(Date[0] == 'M' && Date[1] == 'a' && Date[2] == 'y') time[0] = (Date[10] - '0')*16 + 5;  
  if(Date[0] == 'J' && Date[1] == 'u' && Date[2] == 'n') time[0] = (Date[10] - '0')*16 + 6;  
  if(Date[0] == 'J' && Date[1] == 'u' && Date[2] == 'l') time[0] = (Date[10] - '0')*16 + 7;  
  if(Date[0] == 'A' && Date[1] == 'u' && Date[2] == 'g') time[0] = (Date[10] - '0')*16 + 8;  
  if(Date[0] == 'S' && Date[1] == 'e' && Date[2] == 'p') time[0] = (Date[10] - '0')*16 + 9;  
  if(Date[0] == 'O' && Date[1] == 'c' && Date[2] == 't') time[0] = (Date[10] - '0')*16 + 10;  
  if(Date[0] == 'N' && Date[1] == 'o' && Date[2] == 'v') time[0] = (Date[10] - '0')*16 + 11;  
  if(Date[0] == 'D' && Date[1] == 'e' && Date[2] == 'c') time[0] = (Date[10] - '0')*16 + 12;
  if(Date[4] > '0')
  {
    time[1] = (Date[4] - '0')*10 + (Date[5] - '0');
  }
  else
  {
    time[1] = (Date[5] - '0');
  }
  U1_Tx_Pack_Long(0x07,time[0]*256+time[1]);
}

void Task_Tmp(void *parameter)
{
  uint8_t TmpCount;
  uint8_t TmpData;
  DB.ChipTmp = 20;
  while(1)
  {
    TmpCount = 0;
    TmpData = (uint8_t)BSP_Get_Tmp_Skip_Rom();
    for(uint8_t i=1;i<=5;i++)
    {
      if(TmpData == (uint8_t)BSP_Get_Tmp_Skip_Rom())
      {
        TmpCount++;
      }
      vTaskDelay(200);
    }
    if(TmpCount >= 5)
    {
      DB.ChipTmp = TmpData;
    }
    vTaskDelay(1000);
  }
}

void Task_Act(void *parameter)
{
  DB.Model = 3;
  DB.Para[0] = 1;
  DB.Para[3] = 6;
  DB.Para[4] = 6;
  DB.Para[6] = 1;
  DB.Para[24] = 2;
  for(uint8_t i=9;i<=22;i++)
  {
    DB.Para[i] = 1;
  }
  while(1)
  {
    if(DB.EmerVal == 1 && DB.Act == 1)
    {
      vTaskDelay(200);
      if(DB.Model == 2) //游戏模式 
      {
        DB.Act = 0;
        DB.State = 11;
      }
      if(DB.Model == 4) //分指手套操
      {
        DB.Act = 0;
        DB.HandState = 0;
        DB.GymFist = 0;
        Pump(5);
        DB.HandTimer = DB.Para[3] + 2;
        DB.State = 1;
        DB.CountFlag = 2;
      }
      if(DB.Model == 5) //分指随动
      {
        DB.Act = 0;
        Pump(1);
        DB.State = 2;
      }
      if(DB.Model == 7) //分指功能
      {
        DB.Act = 0;
        DB.HandState = 0;
        Pump(5);
        DB.HandTimer = DB.Para[3] + 2;
        DB.State = 3;
      }
      if(DB.Model == 8) //分指语音
      {
        DB.Act = 0;
        DB.HandState = 0;
        DB.State = 12;
      }
      if(DB.Model == 13)//五指语音
      {
        DB.Act = 0;
        DB.HandState = 0;
        DB.State = 13;
      }
      if(DB.Model == 9) //五指被动
      {
        DB.Act = 0;
        DB.HandTimer = 0;
        DB.HandState = 0;
        DB.State = 4;
      }
      if(DB.Model == 19) //分指伸展
      {
        DB.Act = 0;
        DB.HandTimer = 0;
        DB.HandState = 0;
        DB.State = 17;
      }
      if(DB.Model == 10)//五指随动
      {
        DB.Act = 0;
        Pump(1);
        DB.State = 5;
      }
      if(DB.Model == 11)//五指助力
      {
        DB.Act = 0;
        DB.HelpStep = 0;
        DB.HelpTimer = 0;
        Pump(1);
        DB.State = 6;
      }
      if(DB.Model == 12)//五指抗阻
      {
        DB.Act = 0;
        DB.HelpStep = 0;
        DB.HelpTimer = 0;
        Pump(1);
        DB.State = 7;
      }
      if(DB.Model == 17)//分指助力
      {
        DB.Act = 0;
        DB.HelpStep = 0;
        DB.HelpTimer = 0;
        DB.State = 15;
      }
      if(DB.Model == 18)//分指抗阻
      {
        DB.Act = 0;
        DB.HelpStep = 0;
        DB.HelpTimer = 0;
        DB.State = 16;
      }
      if(DB.Model == 15) //分指多功能被动
      {
        DB.Act = 0;
        DB.HandState = 0;
        Pump(5);
        DB.HandTimer = DB.Para[3] + 2;
        DB.State = 8;
      }
      if(DB.Model == 6) //分指对指
      {
        DB.Act = 0;
        DB.HandState = 0;
        DB.GymFist = 0;
        Pump(5);
        DB.HandTimer = DB.Para[3] + 2;
        DB.State = 9;
      }
      if(DB.Model == 14) //数据校正
      {
        DB.Act = 0;
        DB.AimFlag = 0;
        DB.AimWinFlag = 0;
        DB.FingerType = 0;//默认磁石手套
        DB.FingerFind = 0;
        DB.State = 10;
      }
    }
    if(DB.Model == 1)   //主界面
    {
      Pump(0);
      if(DB.Home == 1)
      {
        DB.Home = 0;
        Escape(0);
        if(DB.EmerVal == 0) U1_Tx_Pack(0x06,0x01);
      }
    }
    if(DB.Model == 16)  //评估模式
    {
      DB.State = 14;
    }
    if(DB.Model == 3)   //配置模式
    {
      Send_Edition();
      vTaskDelay(20);
      U1_Tx_Pack_Long(0x08,DB.PressValDis);
      vTaskDelay(20);
      U1_Tx_Pack_Long(0x0D,DB.FG1Val);
      vTaskDelay(20);
      U1_Tx_Pack_Long(0x0E,DB.FG2Val);
      vTaskDelay(20);
    }
    if(DB.Act == 2)     //暂停三次
    {
      DB.Act = 0;
      uint8_t tmp = 150;
      while(tmp--)
      {
        DB.State = 0;
        Pump(0);
        Escape(1);
        if(DB.Act == 1 || DB.Act == 3) break;//出现按键直接退出循环
        if(DB.Act == 2) DB.Act = 0;
      }
      Escape(0);
      DB.Fun[DB.StopVal] = 0;
      DB.Finger[DB.StopVal] = 0;
      DB.Oppo[DB.StopVal] = 0;
    }
    if(DB.Act == 3) //退出
    {
      Pump(0);
      N_SET(3,0);N_SET(4,0);N_SET(5,0);N_SET(6,0);N_SET(7,0);
      DB.Fun[0] = 0;DB.Fun[1] = 0;DB.Fun[2] = 0;
      DB.Finger[1] = 0;DB.Finger[2] = 0;DB.Finger[3] = 0;DB.Finger[4] = 0;DB.Finger[5] = 0;
      DB.Oppo[2] = 0;DB.Oppo[3] = 0;DB.Oppo[4] = 0;DB.Oppo[5] = 0;DB.Oppo[6] = 0;
      DB.CountFlag = 0;
      DB.Count = 0;
      DB.Model = 0;
      DB.State = 0;
      DB.Act = 0;
      DB.HelpStep = 0;
      DB.HelpTimer = 0;
      DB.GymFist = 0;
      DB.PressAimFlag = 0;
      Escape(1);
      vTaskDelay(500);
      uint8_t i = 100;
      DB.Act = 0;
      while(--i)
      {
        vTaskDelay(10);
        if(DB.EmerVal == 0) break;
        if(DB.Act != 0) break;
      }
      Escape(0);
    }
    vTaskDelay(1);
    if(DB.State == 0) CTL0();
    if(DB.State == 1) CTL1();
    if(DB.State == 2) CTL2();
    if(DB.State == 3) CTL3();
    if(DB.State == 4) CTL4();
    if(DB.State == 5) CTL5();
    if(DB.State == 6) CTL6();
    if(DB.State == 7) CTL7();
    if(DB.State == 8) CTL8();
    if(DB.State == 9) CTL9();
    if(DB.State == 10) CTL10();
    if(DB.State == 11) CTL11();
    if(DB.State == 12) CTL12();
    if(DB.State == 13) CTL13();
    if(DB.State == 14) CTL14();
    if(DB.State == 15) CTL15();
    if(DB.State == 16) CTL16();
    if(DB.State == 17) CTL17();
  }
}

void Task_Tim(void *parameter)
{
  while(1)
  {
    if(DB.Para[3] < 3) DB.Para[3] = 7;
    if(DB.Para[4] < 3) DB.Para[4] = 7;
		
    if(DB.HandTimer >= DB.Para[3] + DB.Para[4])
    {
      DB.HandTimer = 0;
      if(DB.CountFlag == 0)
      {
        DB.CountFlag = 1;
      }
      else if(DB.CountFlag == 2)
      {
        DB.CountFlag = 0;
      }
      DB.Step = 4;
    }
    if(DB.HandTimer <= DB.Para[3]) 
    {
      if(DB.HandTimer == 1)
      {
        DB.HandState = 1;  //握拳
        DB.Step = 1;
      }
    }
    if(DB.HandTimer > DB.Para[3] && DB.HandTimer < DB.Para[3] + DB.Para[4])
    {
      if(DB.HandTimer == DB.Para[3] + 1)
      {
        DB.HandState = 2;  //伸直
        DB.Step = 3;
      }
    }
    vTaskDelay(1000);
    if(DB.State > 0) DB.HandTimer++;
    if(DB.EmerVal == 0)
    {
      HAL_GPIO_TogglePin(LED_RUN_GPIO,LED_RUN_GPIO_PIN);
    }
    else 
    {
      HAL_GPIO_WritePin(LED_RUN_GPIO,LED_RUN_GPIO_PIN,GPIO_PIN_RESET);
    }
  }
}

void Task_Saf(void *parameter)
{
  DB.EmerVal = 1;
  uint16_t SafeCount[4] = {0};
  while(1)
  {
    if(DB.Para[27] < 20) DB.Para[27] = 20;
    if(DB.Para[28] < 20) DB.Para[28] = 20;
    DB.PressValDis = (int16_t)((int32_t)ADC_Val[PRESS_ADC]*1597/1250 - 1295);
    DB.PressVal = (int16_t)DB.PressValDis/10;
    vTaskDelay(20);
    if(DB.EmerVal != HAL_GPIO_ReadPin(EMER_GPIO, EMER_GPIO_PIN))
    {
      vTaskDelay(100);
      if(DB.EmerVal != HAL_GPIO_ReadPin(EMER_GPIO, EMER_GPIO_PIN))
      {
        DB.EmerVal = HAL_GPIO_ReadPin(EMER_GPIO, EMER_GPIO_PIN);
        if(DB.EmerVal == 0)  
        {
          U1_Tx_Pack(0x06,0x01); //急停按钮被按下
          if(DB.Model != 3)      //工程师模式下不进行急停控制操作
          {
            uint8_t tmp = 100;
            while(tmp--)
            {
              DB.State = 0;
              Pump(0);
              Escape(1);
              vTaskDelay(10);
              if(HAL_GPIO_ReadPin(EMER_GPIO, EMER_GPIO_PIN) == 1) 
              {
                DB.State = 0;
                Pump(0);
                Escape(1);
                vTaskDelay(100);
                if(HAL_GPIO_ReadPin(EMER_GPIO, EMER_GPIO_PIN) == 1) 
                {
                  DB.State = 0;
                  Pump(0);
                  Escape(1);
                  vTaskDelay(1400);
                  break;//急停弹起提前跳出循环
                }
              }
            }
            Escape(0);
            DB.Fun[DB.StopVal] = 0;
            DB.Finger[DB.StopVal] = 0;
            DB.Oppo[DB.StopVal] = 0;
          }
        }
        if(DB.EmerVal == 1)
        {
          U1_Tx_Pack(0x06,0x00);
        }
      }
    }
    //SafeCount[0]130Kpa
    //SafeCount[1]80Kpa
    //SafeCount[2]90Kpa
    //SafeCount[3]100Kpa

    if(DB.PressVal < 70 || (DB.PressVal <= 80 && DB.State == 2))
    {
      SafeCount[0] = 1;SafeCount[1] = 1;SafeCount[2] = 1;SafeCount[3] = 1;
    }
    if(DB.PressVal > 130 && DB.Model != 3) //130Kpa强度高保护
    {
      if(SafeCount[0] > 0) SafeCount[0]++;
    }
    if(DB.PressVal > 80 && DB.Para[1] == 1)//抓球
    {
      if(SafeCount[1] > 0) SafeCount[1]++;
    }
    if(DB.PressVal > 90 && DB.Para[0] == 0)//强度低保护
    {
      if(SafeCount[2] > 0) SafeCount[2]++;
    }
    if(DB.PressVal > 100 && DB.Para[0] == 2)//强度中保护
    {
      if(SafeCount[3] > 0) SafeCount[3]++;
    }
    if(SafeCount[0] > 20 || SafeCount[1] > 20  || SafeCount[2] > 20  || SafeCount[3] > 20)
    {
      if(((DB.State == 1 || DB.State == 3 || DB.State == 4 || DB.State == 8 || DB.State == 9 || DB.State == 11 || DB.State == 12 || DB.State == 13) && DB.HandTimer < DB.Para[3])
      || ((DB.State == 2) && (DB.HandVal[1] == 1 || DB.HandVal[2] == 1 || DB.HandVal[3] == 1 || DB.HandVal[4] == 1 || DB.HandVal[5] == 1))
      || DB.State == 5
      || ((DB.State == 6 || DB.State == 7) && DB.HelpTimer < DB.Para[3]))
      {
        Pump(0);
        if(SafeCount[0] > 20)
        {
          if(DB.State == 4 || DB.State == 5 || DB.State == 6 || DB.State == 7 || DB.State == 13)//五指
          {
            N_SET(8,1);
            Pump(0);
            vTaskDelay(900);
          }
          else
          {
            Escape(1);
            Pump(0);
            vTaskDelay(900);
          }
          if(((DB.State == 1 || DB.State == 3 || DB.State == 4 || DB.State == 8 || DB.State == 9 || DB.State == 11 || DB.State == 12 || DB.State == 13) && DB.HandTimer < DB.Para[3])
          || ((DB.State == 2) && (DB.HandVal[1] == 1 || DB.HandVal[2] == 1 || DB.HandVal[3] == 1 || DB.HandVal[4] == 1 || DB.HandVal[5] == 1))
          || DB.State == 5
          || ((DB.State == 6 || DB.State == 7) && DB.HelpTimer < DB.Para[3]))
          {
            if(DB.State == 4 || DB.State == 5 || DB.State == 6 || DB.State == 7 || DB.State == 13)//五指
            {
              N_SET(8,1);
              Pump(0);
              vTaskDelay(900);
            }
            else
            {
              Escape(1);
              Pump(0);
              vTaskDelay(900);
            }
          }
          if(DB.State == 4 || DB.State == 5 || DB.State == 6 || DB.State == 7|| DB.State == 13)
          {
            N_SET(8,0);
            Pump(0);
          }
          else
          {
            Escape(0);
            Pump(0);
          }
        }
      }
      SafeCount[0] = 0;SafeCount[1] = 0;SafeCount[2] = 0;SafeCount[3] = 0;
    }
    if(DB.PressVal > 60 && DB.Para[6] == 0)//防痉挛启动
    {
      PWM_Set(1,DB.Para[27]/3);
    }
  }
}

void Task_DRx(void *parameter)
{
  RxMsg_T *ptMsg;
  while(1)
  {
    if(xQueueReceive(U3_Rx_Que,(void *)&ptMsg,portMAX_DELAY) == pdPASS)
    {
      U3_Rx_UnPack(ptMsg->RxBuf,ptMsg->Len);
    }
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)	       //HAL库UART发送完成中断回调函数
{
  if(huart->Instance==USART1)
  {
    ISR_SemGive(U1_Tx_Sem);
  }
  if(huart->Instance==USART2)
  {
    ISR_SemGive(U2_Tx_Sem);
  }
  if(huart->Instance==USART3)											
  {
    ISR_SemGive(U3_Tx_Sem);
  }
}

void ISR_SemGive(SemaphoreHandle_t Sem)
{
  BaseType_t pxHigherPriorityTaskWoken;								        
  xSemaphoreGiveFromISR(Sem,&pxHigherPriorityTaskWoken);      
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);			        
}

void ISR_QueSend(QueueHandle_t Que,uint8_t *Item)
{
  BaseType_t pxHigherPriorityTaskWoken;								     
  xQueueSendFromISR(Que,Item,&pxHigherPriorityTaskWoken);   
  portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);			      
}

void BSP_U1_Tx(uint8_t *TxBuf,uint16_t Len)                                            
{
  if(Len > MAX_TX_SIZE)
  {
    Len = MAX_TX_SIZE;
    //printf("串口1发送数据过长");
  }
  HAL_UART_Transmit_DMA(&husart1,TxBuf,Len);
  vTaskDelay(20);
}

void BSP_U2_Tx(uint8_t *TxBuf,uint16_t Len)                                            
{
  if(Len > MAX_TX_SIZE)
  {
    Len = MAX_TX_SIZE;
    //printf("串口2发送数据过长");
  }
  HAL_UART_Transmit_DMA(&husart2,TxBuf,Len);
  vTaskDelay(20);
}

void BSP_U3_Tx(uint8_t *TxBuf,uint16_t Len)                                            
{
  if(Len > MAX_TX_SIZE)
  {
    Len = MAX_TX_SIZE;
    //printf("串口3发送数据过长");
  }
  HAL_UART_Transmit_DMA(&husart3,TxBuf,Len);
  vTaskDelay(20);
}

