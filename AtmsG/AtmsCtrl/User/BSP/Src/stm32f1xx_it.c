#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

//BSP头文件
#include "gpio.h"
#include "usart.h"
#include "tim.h"
#include "adc.h"

//FreeRTOS头文件
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

extern DMA_HandleTypeDef hdma_tim8_ch3_up;
extern DMA_HandleTypeDef hdma_tim5_ch3_up;
void SysTick_Handler(void)                							       //系统滴答定时器中断服务函数(系统中断名)
{
  HAL_IncTick();																				       //系统滴答定时器值初始化
  HAL_SYSTICK_IRQHandler();															       //启动系统滴答定时器中断
	
	#if (INCLUDE_xTaskGetSchedulerState == 1)							       //根据FreeRTOS的头文件参数
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)   //设置是否FreeRTOS调度器启用后才能使用心跳时钟
	#endif
	xPortSysTickHandler(); 																       //将FreeRTOS的心跳时钟设为系统滴答定时器
}

void DMA1_Channel1_IRQHandler(void)											       //DMA1_CH1中断服务函数(系统中断名)
{
  HAL_DMA_IRQHandler(&hdma_adc);									             //HAL库DMA中断服务函数(该函数会清空中断标志,取消中断使能,使DMA进入准备状态)
}

void DMA1_Channel2_IRQHandler(void)											       //DMA1_CH2中断服务函数(系统中断名)
{
  HAL_DMA_IRQHandler(&hdma_usart3_tx);									       //HAL库DMA中断服务函数(该函数会清空中断标志,取消中断使能,使DMA进入准备状态)
}

void DMA1_Channel3_IRQHandler(void)											       //DMA1_CH3中断服务函数(系统中断名)
{
  HAL_DMA_IRQHandler(&hdma_usart3_rx);									       //HAL库DMA中断服务函数(该函数会清空中断标志,取消中断使能,使DMA进入准备状态)
}

void DMA1_Channel4_IRQHandler(void)											       //DMA1_CH4中断服务函数(系统中断名)
{
  HAL_DMA_IRQHandler(&hdma_usart1_tx);									       //HAL库DMA中断服务函数(该函数会清空中断标志,取消中断使能,使DMA进入准备状态)
}

void DMA1_Channel5_IRQHandler(void)											       //DMA1_CH5中断服务函数(系统中断名)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);									       //HAL库DMA中断服务函数(该函数会清空中断标志,取消中断使能,使DMA进入准备状态)
}

void DMA1_Channel6_IRQHandler(void)											       //DMA1_CH6中断服务函数(系统中断名)
{
  HAL_DMA_IRQHandler(&hdma_usart2_rx);									       //HAL库DMA中断服务函数(该函数会清空中断标志,取消中断使能,使DMA进入准备状态)
}

void DMA1_Channel7_IRQHandler(void)											       //DMA1_CH7中断服务函数(系统中断名)
{
  HAL_DMA_IRQHandler(&hdma_usart2_tx);									       //HAL库DMA中断服务函数(该函数会清空中断标志,取消中断使能,使DMA进入准备状态)
}

void USART1_IRQHandler(void)														       //USART1中断服务函数(系统中断名)
{
	RxMsg_T *ptMsg;                                              //定义结构体指针
	ptMsg = &U1_Rx_Msg;                                         //指针指向全局数组
  if(__HAL_UART_GET_FLAG(&husart1, UART_FLAG_IDLE) != RESET)   //判断是否是空闲中断
	{
		__HAL_UART_CLEAR_IDLEFLAG(&husart1);								       //清空空闲中断标志位
		HAL_UART_DMAStop(&husart1);                                //复位DMA接收指针
		ptMsg->Len = MAX_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);//读取数据长度	
		memcpy(ptMsg->RxBuf,U1_Rx_Buf,ptMsg->Len);                //读取缓存数据
		ISR_QueSend(U1_Rx_Que,(void *)&ptMsg);                    //发出消息队列
		HAL_UART_Receive_DMA(&husart1,U1_Rx_Buf,MAX_RX_SIZE);		 //继续DMA接收
	}
	HAL_UART_IRQHandler(&husart1);												       //HAL库UART中断服务函数(该函数会清空中断标志,取消中断使能,并调用回调函数)
}

void USART2_IRQHandler(void)														       //USART2中断服务函数(系统中断名)
{
	RxMsg_T *ptMsg;                                              //定义结构体指针
	ptMsg = &U2_Rx_Msg;                                         //指针指向全局数组
  if(__HAL_UART_GET_FLAG(&husart2, UART_FLAG_IDLE) != RESET)   //判断是否是空闲中断
	{
		__HAL_UART_CLEAR_IDLEFLAG(&husart2);								       //清空空闲中断标志位
		HAL_UART_DMAStop(&husart2);                                //复位DMA接收指针
		ptMsg->Len = MAX_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);//读取数据长度
		memcpy(ptMsg->RxBuf,U2_Rx_Buf,ptMsg->Len);                //读取缓存数据
		ISR_QueSend(U2_Rx_Que,(void *)&ptMsg);                    //发出消息队列
		HAL_UART_Receive_DMA(&husart2,U2_Rx_Buf,MAX_RX_SIZE);		 //继续DMA接收
	}
	HAL_UART_IRQHandler(&husart2);												       //HAL库UART中断服务函数(该函数会清空中断标志,取消中断使能,并调用回调函数)
}

void USART3_IRQHandler(void)														       //USART3中断服务函数(系统中断名)
{
	RxMsg_T *ptMsg;                                              //定义结构体指针
	ptMsg = &U3_Rx_Msg;                                         //指针指向全局数组
  if(__HAL_UART_GET_FLAG(&husart3, UART_FLAG_IDLE) != RESET)   //判断是否是空闲中断
	{
		__HAL_UART_CLEAR_IDLEFLAG(&husart3);								       //清空空闲中断标志位
		HAL_UART_DMAStop(&husart3);                                //复位DMA接收指针
    ptMsg->Len = MAX_RX_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);//读取数据长度
		memcpy(ptMsg->RxBuf,U3_Rx_Buf,ptMsg->Len);                //读取缓存数据
		ISR_QueSend(U3_Rx_Que,(void *)&ptMsg);                    //发出消息队列
	  HAL_UART_Receive_DMA(&husart3,U3_Rx_Buf,MAX_RX_SIZE);		 //继续DMA接收
	}
	HAL_UART_IRQHandler(&husart3);												       //HAL库UART中断服务函数(该函数会清空中断标志,取消中断使能,并调用回调函数)
}

void DMA2_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_tim8_ch3_up);
}

void DMA2_Channel2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_tim5_ch3_up);
}
