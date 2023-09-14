#include "stm32f1xx_hal.h"

void HAL_MspInit(void)
{
  __HAL_RCC_AFIO_CLK_ENABLE();		                    //ʹ��AFIOʱ��

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2); //����NVIC���ȼ���ΪNVIC_PRIORITYGROUP_2
	
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);  //ϵͳ�ڲ��жϳ�ʼ��
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
