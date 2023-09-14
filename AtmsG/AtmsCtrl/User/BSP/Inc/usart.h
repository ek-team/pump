#ifndef __USART_H
#define __USART_H

#include "stm32f1xx_hal.h"
#include "sys.h"

#define USART1_GPIO_ClK_ENABLE()		    __HAL_RCC_GPIOA_CLK_ENABLE()

#define USART1_Tx_GPIO									GPIOA
#define USART1_Tx_GPIO_PIN							GPIO_PIN_9

#define USART1_Rx_GPIO									GPIOA
#define USART1_Rx_GPIO_PIN							GPIO_PIN_10

#define USART1_Tx_DMA_CH						    DMA1_Channel4
#define USART1_Tx_DMA_CH_IRQn			      DMA1_Channel4_IRQn

#define USART1_Rx_DMA_CH						    DMA1_Channel5
#define USART1_Rx_DMA_CH_IRQn			      DMA1_Channel5_IRQn

#define USART1_BAUDRATE									115200

#define USART2_GPIO_ClK_ENABLE()		    __HAL_RCC_GPIOA_CLK_ENABLE()

#define USART2_Tx_GPIO									GPIOA
#define USART2_Tx_GPIO_PIN							GPIO_PIN_2

#define USART2_Rx_GPIO									GPIOA
#define USART2_Rx_GPIO_PIN							GPIO_PIN_3

#define USART2_Tx_DMA_CH						    DMA1_Channel7
#define USART2_Tx_DMA_CH_IRQn			      DMA1_Channel7_IRQn

#define USART2_Rx_DMA_CH						    DMA1_Channel6
#define USART2_Rx_DMA_CH_IRQn			      DMA1_Channel6_IRQn

#define USART2_BAUDRATE									115200

#define USART3_GPIO_ClK_ENABLE()		    __HAL_RCC_GPIOB_CLK_ENABLE()

#define USART3_Tx_GPIO									GPIOB
#define USART3_Tx_GPIO_PIN							GPIO_PIN_10

#define USART3_Rx_GPIO									GPIOB
#define USART3_Rx_GPIO_PIN							GPIO_PIN_11

#define USART3_Tx_DMA_CH						    DMA1_Channel2
#define USART3_Tx_DMA_CH_IRQn			      DMA1_Channel2_IRQn

#define USART3_Rx_DMA_CH						    DMA1_Channel3
#define USART3_Rx_DMA_CH_IRQn			      DMA1_Channel3_IRQn

#define USART3_BAUDRATE									57600

#define U_TIME_OUT									    1000

extern UART_HandleTypeDef husart1;
extern UART_HandleTypeDef husart2;
extern UART_HandleTypeDef husart3;

extern DMA_HandleTypeDef hdma_usart1_tx;	
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;	
extern DMA_HandleTypeDef hdma_usart2_rx;	
extern DMA_HandleTypeDef hdma_usart3_tx;	
extern DMA_HandleTypeDef hdma_usart3_rx;

void USART_Init(void);

#endif
