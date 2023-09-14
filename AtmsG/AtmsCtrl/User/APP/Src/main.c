#include "main.h"
#include "hrx.h"

void BSP_Init(void)
{
  HAL_Init();                    //外设初始化
  SysClk_Init();                 //SysClk初始化
  GPIO_Init();                   //GPIO初始化
  ADC_Init();                    //ADC初始化
  USART_Init();                  //USART初始化
  DMA_Init();                    //DMA初始化
  TIM_Init();                    //TIM初始化
}

int main(void)
{
  SCB->VTOR = FLASH_BASE|0x4000; //应用程序地址
  BSP_Init();	                   //驱动初始化
  AppTaskCreate();               //创建任务
  vTaskStartScheduler();         //启用任务调度
  return 0;
}
