#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

//BSPͷ�ļ�
#include "usart.h"

void SysTick_Handler(void)                							       //ϵͳ�δ�ʱ���жϷ�����(ϵͳ�ж���)
{
  HAL_IncTick();																				       //ϵͳ�δ�ʱ��ֵ��ʼ��
  HAL_SYSTICK_IRQHandler();															       //����ϵͳ�δ�ʱ���ж�
}

void USART1_IRQHandler(void)														       //USART1�жϷ�����(ϵͳ�ж���)
{
	HAL_UART_IRQHandler(&husart1);												       //HAL��UART�жϷ�����(�ú���������жϱ�־,ȡ���ж�ʹ��,�����ûص�����)
}
