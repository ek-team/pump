#include "usart.h"

uint8_t U1_Rx_Buf[MAX_RX_SIZE];
uint8_t U2_Rx_Buf[MAX_RX_SIZE];
uint8_t U3_Rx_Buf[MAX_RX_SIZE];

RxMsg_T U1_Rx_Msg;
RxMsg_T U2_Rx_Msg;
RxMsg_T U3_Rx_Msg;

UART_HandleTypeDef husart1;																			//定义USART1的结构体
UART_HandleTypeDef husart2;																			//定义USART2的结构体
UART_HandleTypeDef husart3;																			//定义USART3的结构体

DMA_HandleTypeDef hdma_usart1_tx;																//定义USART1的DMA发送结构体				
DMA_HandleTypeDef hdma_usart1_rx;																//定义USART1的DMA接收结构体
DMA_HandleTypeDef hdma_usart2_tx;																//定义USART2的DMA发送结构体				
DMA_HandleTypeDef hdma_usart2_rx;																//定义USART2的DMA接收结构体				
DMA_HandleTypeDef hdma_usart3_tx;																//定义USART3的DMA发送结构体				
DMA_HandleTypeDef hdma_usart3_rx;																//定义USART3的DMA接收结构体				

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

    hdma_usart1_tx.Instance = USART1_Tx_DMA_CH;									//设置DMA发送通道
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;				//数据从内存到外设
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;						//禁用外设地址递增
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;								//启用内存地址递增
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //设置外设数据长度为单个字节
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//设置内存数据长度为单个字节
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;											//设置为普通模式(不覆盖)
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_HIGH;						//设置为高优先级
    HAL_DMA_Init(&hdma_usart1_tx);															//初始化DMA发送

    __HAL_LINKDMA(huart,hdmatx,hdma_usart1_tx);									//建立DMA发送连接

		HAL_NVIC_SetPriority(USART1_Tx_DMA_CH_IRQn, 1, 1);					//设置DMA的抢占优先级和响应优先级
		HAL_NVIC_EnableIRQ(USART1_Tx_DMA_CH_IRQn);									//启用DMA中断

    hdma_usart1_rx.Instance = USART1_Rx_DMA_CH;									//设置DMA接收通道
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;				//数据从外设到内存
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;						//禁用外设地址递增
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;								//启用内存地址递增
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //设置外设数据长度为单个字节
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//设置内存数据长度为单个字节
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;											//设置为普通模式(不覆盖)
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;						//设置为高优先级
    HAL_DMA_Init(&hdma_usart1_rx);															//初始化DMA接收

    __HAL_LINKDMA(huart,hdmarx,hdma_usart1_rx);									//建立DMA接收连接

		HAL_NVIC_SetPriority(USART1_Rx_DMA_CH_IRQn, 1, 0);					//设置DMA的抢占优先级和响应优先级   
		HAL_NVIC_EnableIRQ(USART1_Rx_DMA_CH_IRQn);									//启用DMA中断 

		__HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);										//启用串口空闲中断
  }
	
	if(huart->Instance == USART2)																	//如果是USART2
  {
    __HAL_RCC_USART2_CLK_ENABLE();															//使能USART2时钟	
  
    GPIO_InitStruct.Pin = USART2_Tx_GPIO_PIN;										//设置USART2发送口的IO口
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;											//设置为复用推挽输出模式
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;								//设置为高速模式(低速2MHz,中速10MHz,高速50MHz)
    HAL_GPIO_Init(USART2_Tx_GPIO, &GPIO_InitStruct);						//初始化USART2发送口

    GPIO_InitStruct.Pin = USART2_Rx_GPIO_PIN;										//设置USART2接收口的IO口
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;											//设置为输入模式
    GPIO_InitStruct.Pull = GPIO_NOPULL;													//设置为浮空模式
    HAL_GPIO_Init(USART2_Rx_GPIO, &GPIO_InitStruct);						//初始化USART2接收口
		
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 1);										//设置USART2的抢占优先级和响应优先级
    HAL_NVIC_EnableIRQ(USART2_IRQn);														//启用USART2中断

    hdma_usart2_tx.Instance = USART2_Tx_DMA_CH;									//设置DMA发送通道
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;				//数据从内存到外设
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;						//禁用外设地址递增
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;								//启用内存地址递增
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //设置外设数据长度为单个字节
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//设置内存数据长度为单个字节
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;											//设置为普通模式(不覆盖)
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_HIGH;						//设置为高优先级
    HAL_DMA_Init(&hdma_usart2_tx);															//初始化DMA发送

    __HAL_LINKDMA(huart,hdmatx,hdma_usart2_tx);									//建立DMA发送连接

		HAL_NVIC_SetPriority(USART2_Tx_DMA_CH_IRQn, 1, 1);					//设置DMA的抢占优先级和响应优先级   
		HAL_NVIC_EnableIRQ(USART2_Tx_DMA_CH_IRQn);									//启用DMA中断

    hdma_usart2_rx.Instance = USART2_Rx_DMA_CH;									//设置DMA接收通道
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;				//数据从外设到内存
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;						//禁用外设地址递增
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;								//启用内存地址递增
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //设置外设数据长度为单个字节
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//设置内存数据长度为单个字节
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;											//设置为普通模式(不覆盖)
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_HIGH;						//设置为高优先级
    HAL_DMA_Init(&hdma_usart2_rx);															//初始化DMA接收

    __HAL_LINKDMA(huart,hdmarx,hdma_usart2_rx);									//建立DMA接收连接

		HAL_NVIC_SetPriority(USART2_Rx_DMA_CH_IRQn, 1, 1);					//设置DMA的抢占优先级和响应优先级    
		HAL_NVIC_EnableIRQ(USART2_Rx_DMA_CH_IRQn);									//启用DMA中断 

		__HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);										//启用串口空闲中断
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

    hdma_usart3_tx.Instance = USART3_Tx_DMA_CH;									//设置DMA发送通道
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;				//数据从内存到外设
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;						//禁用外设地址递增
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;								//启用内存地址递增
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //设置外设数据长度为单个字节
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//设置内存数据长度为单个字节
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;											//设置为普通模式(不覆盖)
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_HIGH;						//设置为高优先级
    HAL_DMA_Init(&hdma_usart3_tx);															//初始化DMA发送

    __HAL_LINKDMA(huart,hdmatx,hdma_usart3_tx);									//建立DMA发送连接

		HAL_NVIC_SetPriority(USART3_Tx_DMA_CH_IRQn, 1, 1);					//设置DMA的抢占优先级和响应优先级    
		HAL_NVIC_EnableIRQ(USART3_Tx_DMA_CH_IRQn);									//启用DMA中断 

    hdma_usart3_rx.Instance = USART3_Rx_DMA_CH;									//设置DMA接收通道
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;				//数据从外设到内存
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;						//禁用外设地址递增
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;								//启用内存地址递增
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //设置外设数据长度为单个字节
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//设置内存数据长度为单个字节
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;											//设置为普通模式(不覆盖)
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_HIGH;						//设置为高优先级
    HAL_DMA_Init(&hdma_usart3_rx);															//初始化DMA接收

    __HAL_LINKDMA(huart,hdmarx,hdma_usart3_rx);									//建立DMA接收连接

		HAL_NVIC_SetPriority(USART3_Rx_DMA_CH_IRQn, 1, 1);					//设置DMA的抢占优先级和响应优先级   
		HAL_NVIC_EnableIRQ(USART3_Rx_DMA_CH_IRQn);									//启用DMA中断

		__HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);										//启用串口空闲中断
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

    HAL_DMA_DeInit(huart->hdmarx);															//重置DMA接收
  }
	if(huart->Instance == USART2)																	//如果是USART2
  {
    __HAL_RCC_USART2_CLK_DISABLE();															//禁用USART2时钟	
  
    HAL_GPIO_DeInit(USART2_Tx_GPIO, USART2_Tx_GPIO_PIN);				//重置USART2发送口
    HAL_GPIO_DeInit(USART2_Rx_GPIO, USART2_Rx_GPIO_PIN);				//重置USART2接收口

    HAL_NVIC_DisableIRQ(USART2_IRQn);														//禁用USART2中断

    HAL_DMA_DeInit(huart->hdmarx);															//重置DMA接收
  }
	if(huart->Instance == USART3)																	//如果是USART3
  {
    __HAL_RCC_USART3_CLK_DISABLE();															//禁用USART3时钟	
  
    HAL_GPIO_DeInit(USART3_Tx_GPIO, USART3_Tx_GPIO_PIN);				//重置USART3发送口
    HAL_GPIO_DeInit(USART3_Rx_GPIO, USART3_Rx_GPIO_PIN);				//重置USART3接收口

    HAL_NVIC_DisableIRQ(USART3_IRQn);														//禁用USART3中断

    HAL_DMA_DeInit(huart->hdmarx);															//重置DMA接收
  }
}

void USART1_Init(void)																					//USART1的通信初始化
{
  USART1_GPIO_ClK_ENABLE();																	    //使能USART1的GPIO时钟
	__HAL_RCC_DMA1_CLK_ENABLE();																	//使能DMA1的时钟
	
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

void USART2_Init(void)																					//USART2的通信初始化
{
  USART2_GPIO_ClK_ENABLE();																	    //使能USART2的GPIO时钟
	__HAL_RCC_DMA1_CLK_ENABLE();																	//使能DMA1的时钟
	
  husart2.Instance = USART2;																		//设置寄存器基地址
  husart2.Init.BaudRate = USART2_BAUDRATE;											//设置波特率
  husart2.Init.WordLength = UART_WORDLENGTH_8B;									//设置为8位字长
  husart2.Init.StopBits = UART_STOPBITS_1;											//设置为1位停止位
  husart2.Init.Parity = UART_PARITY_NONE;												//设置为无校验位
  husart2.Init.Mode = UART_MODE_TX_RX;													//设置为发送和接收模式
  husart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;									//设置为不使用硬件流控制
  husart2.Init.OverSampling = UART_OVERSAMPLING_16;							//设置为过采样
  HAL_UART_Init(&husart2);																			//初始化USART2
}

void USART3_Init(void)																					//USART3的通信初始化
{
  USART3_GPIO_ClK_ENABLE();																	    //使能USART3的GPIO时钟
	__HAL_RCC_DMA1_CLK_ENABLE();																	//使能DMA1的时钟
	
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
  USART2_Init();                                                //USART2初始化
	USART3_Init();                                                //USART3初始化
	
	HAL_UART_Receive_DMA(&husart1,U1_Rx_Buf,MAX_RX_SIZE);
	HAL_UART_Receive_DMA(&husart2,U2_Rx_Buf,MAX_RX_SIZE);
	HAL_UART_Receive_DMA(&husart3,U3_Rx_Buf,MAX_RX_SIZE);
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
