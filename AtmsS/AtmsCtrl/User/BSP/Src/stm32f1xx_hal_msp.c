#include "stm32f1xx_hal.h"

void HAL_MspInit(void)
{
  __HAL_RCC_AFIO_CLK_ENABLE();		                    //使能AFIO时钟

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2); //设置NVIC优先级组为NVIC_PRIORITYGROUP_2
	
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);  //系统内部中断初始化
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
