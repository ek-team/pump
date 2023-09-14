#include "usart.h"

UART_HandleTypeDef husart1;																			//定义USART1的结构体
UART_HandleTypeDef husart3;																			//定义USART3的结构体

void HAL_UART_MspInit(UART_HandleTypeDef* huart)								//USART的IO口初始化(此函数被HAL库内部调用)
{
  GPIO_InitTypeDef GPIO_InitStruct;															//定义GPIO初始化结构体
	
  if(huart->Instance == USART1)																	//如果是USART1
  {
    __HAL_RCC_USART1_CLK_ENABLE();															//使能USART1时钟	
  
    GPIO_InitStruct.Pin = USART1_Tx_GPIO_PIN;										//设置USART1发送口的IO口
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;											//设置为复用推挽输出模式
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;								//设置为高速模式(低速2MHz,中速10MHz,高速50MHz)
    HAL_GPIO_Init(USART1_Tx_GPIO, &GPIO_InitStruct);						//初始化USART1发送口

    GPIO_InitStruct.Pin = USART1_Rx_GPIO_PIN;										//设置USART1接收口的IO口
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;											//设置为输入模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;													//设置为浮空模式
    HAL_GPIO_Init(USART1_Rx_GPIO, &GPIO_InitStruct);						//初始化USART1接收口
		
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);										//设置USART1的抢占优先级和响应优先级
    HAL_NVIC_EnableIRQ(USART1_IRQn);														//启用USART1中断
  }
	
	if(huart->Instance == USART3)																	//如果是USART3
  {
    __HAL_RCC_USART3_CLK_ENABLE();															//使能USART3时钟	
  
    GPIO_InitStruct.Pin = USART3_Tx_GPIO_PIN;										//设置USART3发送口的IO口
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;											//设置为复用推挽输出模式
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;								//设置为高速模式(低速2MHz,中速10MHz,高速50MHz)
    HAL_GPIO_Init(USART3_Tx_GPIO, &GPIO_InitStruct);						//初始化USART3发送口

    GPIO_InitStruct.Pin = USART3_Rx_GPIO_PIN;										//设置USART3接收口的IO口
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;											//设置为输入模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;													//设置为浮空模式
    HAL_GPIO_Init(USART3_Rx_GPIO, &GPIO_InitStruct);						//初始化USART3接收口
		
    HAL_NVIC_SetPriority(USART3_IRQn, 1, 1);										//设置USART3的抢占优先级和响应优先级
    HAL_NVIC_EnableIRQ(USART3_IRQn);														//启用USART3中断

  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)							//USART的IO接口反初始化(此函数被HAL库内部调用)
{
  if(huart->Instance == USART1)																	//如果是USART1
  {
    __HAL_RCC_USART1_CLK_DISABLE();															//禁用USART1时钟	
  
    HAL_GPIO_DeInit(USART1_Tx_GPIO, USART1_Tx_GPIO_PIN);				//重置USART1发送口
    HAL_GPIO_DeInit(USART1_Rx_GPIO, USART1_Rx_GPIO_PIN);				//重置USART1接收口

    HAL_NVIC_DisableIRQ(USART1_IRQn);														//禁用USART1中断
  }
	if(huart->Instance == USART3)																	//如果是USART3
  {
    __HAL_RCC_USART3_CLK_DISABLE();															//禁用USART3时钟	
  
    HAL_GPIO_DeInit(USART3_Tx_GPIO, USART3_Tx_GPIO_PIN);				//重置USART3发送口
    HAL_GPIO_DeInit(USART3_Rx_GPIO, USART3_Rx_GPIO_PIN);				//重置USART3接收口

    HAL_NVIC_DisableIRQ(USART3_IRQn);														//禁用USART3中断
  }
}

void USART1_Init(void)																					//USART1的通信初始化
{
  USART1_GPIO_ClK_ENABLE();																	    //使能USART1的GPIO时钟
	
  husart1.Instance = USART1;																		//设置寄存器基地址
  husart1.Init.BaudRate = USART1_BAUDRATE;											//设置波特率
  husart1.Init.WordLength = UART_WORDLENGTH_8B;									//设置为8位字长
  husart1.Init.StopBits = UART_STOPBITS_1;											//设置为1位停止位
  husart1.Init.Parity = UART_PARITY_NONE;												//设置为无校验位
  husart1.Init.Mode = UART_MODE_TX_RX;													//设置为发送和接收模式
  husart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;									//设置为不使用硬件流控制
  husart1.Init.OverSampling = UART_OVERSAMPLING_16;							//设置为过采样
  HAL_UART_Init(&husart1);																			//初始化USART1
}

void USART3_Init(void)																					//USART3的通信初始化
{
  USART3_GPIO_ClK_ENABLE();																	    //使能USART3的GPIO时钟
	
  husart3.Instance = USART3;																		//设置寄存器基地址
  husart3.Init.BaudRate = USART3_BAUDRATE;											//设置波特率
  husart3.Init.WordLength = UART_WORDLENGTH_8B;									//设置为8位字长
  husart3.Init.StopBits = UART_STOPBITS_1;											//设置为1位停止位
  husart3.Init.Parity = UART_PARITY_NONE;												//设置为无校验位
  husart3.Init.Mode = UART_MODE_TX_RX;													//设置为发送和接收模式
  husart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;									//设置为不使用硬件流控制
  husart3.Init.OverSampling = UART_OVERSAMPLING_16;							//设置为过采样
  HAL_UART_Init(&husart3);																			//初始化USART3
}

void USART_Init(void)                                           //USART初始化
{
  USART1_Init();                                                //USART1初始化
	USART3_Init();                                                //USART3初始化
	HAL_UART_Receive_IT(&husart1,&U1_RxData,1);  
}

int fputc(int ch, FILE *f)																			//重定向库函数printf到USART3
{
	HAL_UART_Transmit(&husart3,(uint8_t *)&ch,1,U_TIME_OUT);			//通过串口发送一个字符,ch为字符的存储地址,uTimeout为超时时间
  return ch;
}

int fgetc(FILE * f)																							//重定向库函数getchar,scanf到USART3
{
  uint8_t ch = 0;
  HAL_UART_Receive(&husart3,&ch,1,U_TIME_OUT); 									//通过串口接收一个字符,ch为字符的存储地址,uTimeout为超时时间          
  return ch;
}
