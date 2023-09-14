#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

//BSP头文件
#include "usart.h"

void SysTick_Handler(void)                							       //系统滴答定时器中断服务函数(系统中断名)
{
  HAL_IncTick();																				       //系统滴答定时器值初始化
  HAL_SYSTICK_IRQHandler();															       //启动系统滴答定时器中断
}

void USART1_IRQHandler(void)														       //USART1中断服务函数(系统中断名)
{
	HAL_UART_IRQHandler(&husart1);												       //HAL库UART中断服务函数(该函数会清空中断标志,取消中断使能,并调用回调函数)
}
