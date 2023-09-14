#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

//BSPͷ�ļ�
#include "gpio.h"
#include "usart.h"
#include "tim.h"
#include "adc.h"

//FreeRTOSͷ�ļ�
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

extern DMA_HandleTypeDef hdma_tim8_ch3_up;
extern DMA_HandleTypeDef hdma_tim5_ch3_up;
void SysTick_Handler(void)                							       //ϵͳ�δ�ʱ���жϷ�����(ϵͳ�ж���)
{
  HAL_IncTick();																				       //ϵͳ�δ�ʱ��ֵ��ʼ��
  HAL_SYSTICK_IRQHandler();															       //����ϵͳ�δ�ʱ���ж�
	
	#if (INCLUDE_xTaskGetSchedulerState == 1)							       //����FreeRTOS��ͷ�ļ�����
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)   //�����Ƿ�FreeRTOS���������ú����ʹ������ʱ��
	#endif
	xPortSysTickHandler(); 																       //��FreeRTOS������ʱ����Ϊϵͳ�δ�ʱ��
}

void DMA1_Channel1_IRQHandler(void)											       //DMA1_CH1�жϷ�����(ϵͳ�ж���)
{
  HAL_DMA_IRQHandler(&hdma_adc);									             //HAL��DMA�жϷ�����(�ú���������жϱ�־,ȡ���ж�ʹ��,ʹDMA����׼��״̬)
}

void DMA1_Channel2_IRQHandler(void)											       //DMA1_CH2�жϷ�����(ϵͳ�ж���)
{
  HAL_DMA_IRQHandler(&hdma_usart3_tx);									       //HAL��DMA�жϷ�����(�ú���������жϱ�־,ȡ���ж�ʹ��,ʹDMA����׼��״̬)
}

void DMA1_Channel3_IRQHandler(void)											       //DMA1_CH3�жϷ�����(ϵͳ�ж���)
{
  HAL_DMA_IRQHandler(&hdma_usart3_rx);									       //HAL��DMA�жϷ�����(�ú���������жϱ�־,ȡ���ж�ʹ��,ʹDMA����׼��״̬)
}

void DMA1_Channel4_IRQHandler(void)											       //DMA1_CH4�жϷ�����(ϵͳ�ж���)
{
  HAL_DMA_IRQHandler(&hdma_usart1_tx);									       //HAL��DMA�жϷ�����(�ú���������жϱ�־,ȡ���ж�ʹ��,ʹDMA����׼��״̬)
}

void DMA1_Channel5_IRQHandler(void)											       //DMA1_CH5�жϷ�����(ϵͳ�ж���)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);									       //HAL��DMA�жϷ�����(�ú���������жϱ�־,ȡ���ж�ʹ��,ʹDMA����׼��״̬)
}

void DMA1_Channel6_IRQHandler(void)											       //DMA1_CH6�жϷ�����(ϵͳ�ж���)
{
  HAL_DMA_IRQHandler(&hdma_usart2_rx);									       //HAL��DMA�жϷ�����(�ú���������жϱ�־,ȡ���ж�ʹ��,ʹDMA����׼��״̬)
}

void DMA1_Channel7_IRQHandler(void)											       //DMA1_CH7�жϷ�����(ϵͳ�ж���)
{
  HAL_DMA_IRQHandler(&hdma_usart2_tx);									       //HAL��DMA�жϷ�����(�ú���������жϱ�־,ȡ���ж�ʹ��,ʹDMA����׼��״̬)
}

void USART1_IRQHandler(void)														       //USART1�жϷ�����(ϵͳ�ж���)
{
	RxMsg_T *ptMsg;                                              //����ṹ��ָ��
	ptMsg = &U1_Rx_Msg;                                         //ָ��ָ��ȫ������
  if(__HAL_UART_GET_FLAG(&husart1, UART_FLAG_IDLE) != RESET)   //�ж��Ƿ��ǿ����ж�
	{
		__HAL_UART_CLEAR_IDLEFLAG(&husart1);								       //��տ����жϱ�־λ
		HAL_UART_DMAStop(&husart1);                                //��λDMA����ָ��
		ptMsg->Len = MAX_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);//��ȡ���ݳ���	
		memcpy(ptMsg->RxBuf,U1_Rx_Buf,ptMsg->Len);                //��ȡ��������
		ISR_QueSend(U1_Rx_Que,(void *)&ptMsg);                    //������Ϣ����
		HAL_UART_Receive_DMA(&husart1,U1_Rx_Buf,MAX_RX_SIZE);		 //����DMA����
	}
	HAL_UART_IRQHandler(&husart1);												       //HAL��UART�жϷ�����(�ú���������жϱ�־,ȡ���ж�ʹ��,�����ûص�����)
}

void USART2_IRQHandler(void)														       //USART2�жϷ�����(ϵͳ�ж���)
{
	RxMsg_T *ptMsg;                                              //����ṹ��ָ��
	ptMsg = &U2_Rx_Msg;                                         //ָ��ָ��ȫ������
  if(__HAL_UART_GET_FLAG(&husart2, UART_FLAG_IDLE) != RESET)   //�ж��Ƿ��ǿ����ж�
	{
		__HAL_UART_CLEAR_IDLEFLAG(&husart2);								       //��տ����жϱ�־λ
		HAL_UART_DMAStop(&husart2);                                //��λDMA����ָ��
		ptMsg->Len = MAX_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);//��ȡ���ݳ���
		memcpy(ptMsg->RxBuf,U2_Rx_Buf,ptMsg->Len);                //��ȡ��������
		ISR_QueSend(U2_Rx_Que,(void *)&ptMsg);                    //������Ϣ����
		HAL_UART_Receive_DMA(&husart2,U2_Rx_Buf,MAX_RX_SIZE);		 //����DMA����
	}
	HAL_UART_IRQHandler(&husart2);												       //HAL��UART�жϷ�����(�ú���������жϱ�־,ȡ���ж�ʹ��,�����ûص�����)
}

void USART3_IRQHandler(void)														       //USART3�жϷ�����(ϵͳ�ж���)
{
	RxMsg_T *ptMsg;                                              //����ṹ��ָ��
	ptMsg = &U3_Rx_Msg;                                         //ָ��ָ��ȫ������
  if(__HAL_UART_GET_FLAG(&husart3, UART_FLAG_IDLE) != RESET)   //�ж��Ƿ��ǿ����ж�
	{
		__HAL_UART_CLEAR_IDLEFLAG(&husart3);								       //��տ����жϱ�־λ
		HAL_UART_DMAStop(&husart3);                                //��λDMA����ָ��
    ptMsg->Len = MAX_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);//��ȡ���ݳ���
		memcpy(ptMsg->RxBuf,U3_Rx_Buf,ptMsg->Len);                //��ȡ��������
		ISR_QueSend(U3_Rx_Que,(void *)&ptMsg);                    //������Ϣ����
	  HAL_UART_Receive_DMA(&husart3,U3_Rx_Buf,MAX_RX_SIZE);		 //����DMA����
	}
	HAL_UART_IRQHandler(&husart3);												       //HAL��UART�жϷ�����(�ú���������жϱ�־,ȡ���ж�ʹ��,�����ûص�����)
}

void DMA2_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_tim8_ch3_up);
}

void DMA2_Channel2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_tim5_ch3_up);
}
