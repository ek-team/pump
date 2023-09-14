#include "usart.h"

uint8_t U1_Rx_Buf[MAX_RX_SIZE];
uint8_t U2_Rx_Buf[MAX_RX_SIZE];
uint8_t U3_Rx_Buf[MAX_RX_SIZE];

RxMsg_T U1_Rx_Msg;
RxMsg_T U2_Rx_Msg;
RxMsg_T U3_Rx_Msg;

UART_HandleTypeDef husart1;																			//����USART1�Ľṹ��
UART_HandleTypeDef husart2;																			//����USART2�Ľṹ��
UART_HandleTypeDef husart3;																			//����USART3�Ľṹ��

DMA_HandleTypeDef hdma_usart1_tx;																//����USART1��DMA���ͽṹ��				
DMA_HandleTypeDef hdma_usart1_rx;																//����USART1��DMA���սṹ��
DMA_HandleTypeDef hdma_usart2_tx;																//����USART2��DMA���ͽṹ��				
DMA_HandleTypeDef hdma_usart2_rx;																//����USART2��DMA���սṹ��				
DMA_HandleTypeDef hdma_usart3_tx;																//����USART3��DMA���ͽṹ��				
DMA_HandleTypeDef hdma_usart3_rx;																//����USART3��DMA���սṹ��				

void HAL_UART_MspInit(UART_HandleTypeDef* huart)								//USART��IO�ڳ�ʼ��(�˺�����HAL���ڲ�����)
{
  GPIO_InitTypeDef GPIO_InitStruct;															//����GPIO��ʼ���ṹ��
	
  if(huart->Instance == USART1)																	//�����USART1
  {
    __HAL_RCC_USART1_CLK_ENABLE();															//ʹ��USART1ʱ��	
  
    GPIO_InitStruct.Pin = USART1_Tx_GPIO_PIN;										//����USART1���Ϳڵ�IO��
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;											//����Ϊ�����������ģʽ
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;								//����Ϊ����ģʽ(����2MHz,����10MHz,����50MHz)
    HAL_GPIO_Init(USART1_Tx_GPIO, &GPIO_InitStruct);						//��ʼ��USART1���Ϳ�

    GPIO_InitStruct.Pin = USART1_Rx_GPIO_PIN;										//����USART1���տڵ�IO��
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;											//����Ϊ����ģʽ
    GPIO_InitStruct.Pull = GPIO_NOPULL;													//����Ϊ����ģʽ
    HAL_GPIO_Init(USART1_Rx_GPIO, &GPIO_InitStruct);						//��ʼ��USART1���տ�
		
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);										//����USART1����ռ���ȼ�����Ӧ���ȼ�
    HAL_NVIC_EnableIRQ(USART1_IRQn);														//����USART1�ж�

    hdma_usart1_tx.Instance = USART1_Tx_DMA_CH;									//����DMA����ͨ��
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;				//���ݴ��ڴ浽����
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;						//���������ַ����
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;								//�����ڴ��ַ����
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //�����������ݳ���Ϊ�����ֽ�
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//�����ڴ����ݳ���Ϊ�����ֽ�
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;											//����Ϊ��ͨģʽ(������)
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_HIGH;						//����Ϊ�����ȼ�
    HAL_DMA_Init(&hdma_usart1_tx);															//��ʼ��DMA����

    __HAL_LINKDMA(huart,hdmatx,hdma_usart1_tx);									//����DMA��������

		HAL_NVIC_SetPriority(USART1_Tx_DMA_CH_IRQn, 1, 1);					//����DMA����ռ���ȼ�����Ӧ���ȼ�
		HAL_NVIC_EnableIRQ(USART1_Tx_DMA_CH_IRQn);									//����DMA�ж�

    hdma_usart1_rx.Instance = USART1_Rx_DMA_CH;									//����DMA����ͨ��
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;				//���ݴ����赽�ڴ�
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;						//���������ַ����
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;								//�����ڴ��ַ����
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //�����������ݳ���Ϊ�����ֽ�
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//�����ڴ����ݳ���Ϊ�����ֽ�
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;											//����Ϊ��ͨģʽ(������)
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;						//����Ϊ�����ȼ�
    HAL_DMA_Init(&hdma_usart1_rx);															//��ʼ��DMA����

    __HAL_LINKDMA(huart,hdmarx,hdma_usart1_rx);									//����DMA��������

		HAL_NVIC_SetPriority(USART1_Rx_DMA_CH_IRQn, 1, 0);					//����DMA����ռ���ȼ�����Ӧ���ȼ�   
		HAL_NVIC_EnableIRQ(USART1_Rx_DMA_CH_IRQn);									//����DMA�ж� 

		__HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);										//���ô��ڿ����ж�
  }
	
	if(huart->Instance == USART2)																	//�����USART2
  {
    __HAL_RCC_USART2_CLK_ENABLE();															//ʹ��USART2ʱ��	
  
    GPIO_InitStruct.Pin = USART2_Tx_GPIO_PIN;										//����USART2���Ϳڵ�IO��
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;											//����Ϊ�����������ģʽ
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;								//����Ϊ����ģʽ(����2MHz,����10MHz,����50MHz)
    HAL_GPIO_Init(USART2_Tx_GPIO, &GPIO_InitStruct);						//��ʼ��USART2���Ϳ�

    GPIO_InitStruct.Pin = USART2_Rx_GPIO_PIN;										//����USART2���տڵ�IO��
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;											//����Ϊ����ģʽ
    GPIO_InitStruct.Pull = GPIO_NOPULL;													//����Ϊ����ģʽ
    HAL_GPIO_Init(USART2_Rx_GPIO, &GPIO_InitStruct);						//��ʼ��USART2���տ�
		
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 1);										//����USART2����ռ���ȼ�����Ӧ���ȼ�
    HAL_NVIC_EnableIRQ(USART2_IRQn);														//����USART2�ж�

    hdma_usart2_tx.Instance = USART2_Tx_DMA_CH;									//����DMA����ͨ��
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;				//���ݴ��ڴ浽����
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;						//���������ַ����
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;								//�����ڴ��ַ����
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //�����������ݳ���Ϊ�����ֽ�
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//�����ڴ����ݳ���Ϊ�����ֽ�
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;											//����Ϊ��ͨģʽ(������)
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_HIGH;						//����Ϊ�����ȼ�
    HAL_DMA_Init(&hdma_usart2_tx);															//��ʼ��DMA����

    __HAL_LINKDMA(huart,hdmatx,hdma_usart2_tx);									//����DMA��������

		HAL_NVIC_SetPriority(USART2_Tx_DMA_CH_IRQn, 1, 1);					//����DMA����ռ���ȼ�����Ӧ���ȼ�   
		HAL_NVIC_EnableIRQ(USART2_Tx_DMA_CH_IRQn);									//����DMA�ж�

    hdma_usart2_rx.Instance = USART2_Rx_DMA_CH;									//����DMA����ͨ��
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;				//���ݴ����赽�ڴ�
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;						//���������ַ����
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;								//�����ڴ��ַ����
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //�����������ݳ���Ϊ�����ֽ�
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//�����ڴ����ݳ���Ϊ�����ֽ�
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;											//����Ϊ��ͨģʽ(������)
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_HIGH;						//����Ϊ�����ȼ�
    HAL_DMA_Init(&hdma_usart2_rx);															//��ʼ��DMA����

    __HAL_LINKDMA(huart,hdmarx,hdma_usart2_rx);									//����DMA��������

		HAL_NVIC_SetPriority(USART2_Rx_DMA_CH_IRQn, 1, 1);					//����DMA����ռ���ȼ�����Ӧ���ȼ�    
		HAL_NVIC_EnableIRQ(USART2_Rx_DMA_CH_IRQn);									//����DMA�ж� 

		__HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);										//���ô��ڿ����ж�
  }
	
	if(huart->Instance == USART3)																	//�����USART3
  {
    __HAL_RCC_USART3_CLK_ENABLE();															//ʹ��USART3ʱ��	
  
    GPIO_InitStruct.Pin = USART3_Tx_GPIO_PIN;										//����USART3���Ϳڵ�IO��
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;											//����Ϊ�����������ģʽ
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;								//����Ϊ����ģʽ(����2MHz,����10MHz,����50MHz)
    HAL_GPIO_Init(USART3_Tx_GPIO, &GPIO_InitStruct);						//��ʼ��USART3���Ϳ�

    GPIO_InitStruct.Pin = USART3_Rx_GPIO_PIN;										//����USART3���տڵ�IO��
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;											//����Ϊ����ģʽ
    GPIO_InitStruct.Pull = GPIO_NOPULL;													//����Ϊ����ģʽ
    HAL_GPIO_Init(USART3_Rx_GPIO, &GPIO_InitStruct);						//��ʼ��USART3���տ�
		
    HAL_NVIC_SetPriority(USART3_IRQn, 1, 1);										//����USART3����ռ���ȼ�����Ӧ���ȼ�
    HAL_NVIC_EnableIRQ(USART3_IRQn);														//����USART3�ж�

    hdma_usart3_tx.Instance = USART3_Tx_DMA_CH;									//����DMA����ͨ��
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;				//���ݴ��ڴ浽����
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;						//���������ַ����
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;								//�����ڴ��ַ����
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //�����������ݳ���Ϊ�����ֽ�
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//�����ڴ����ݳ���Ϊ�����ֽ�
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;											//����Ϊ��ͨģʽ(������)
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_HIGH;						//����Ϊ�����ȼ�
    HAL_DMA_Init(&hdma_usart3_tx);															//��ʼ��DMA����

    __HAL_LINKDMA(huart,hdmatx,hdma_usart3_tx);									//����DMA��������

		HAL_NVIC_SetPriority(USART3_Tx_DMA_CH_IRQn, 1, 1);					//����DMA����ռ���ȼ�����Ӧ���ȼ�    
		HAL_NVIC_EnableIRQ(USART3_Tx_DMA_CH_IRQn);									//����DMA�ж� 

    hdma_usart3_rx.Instance = USART3_Rx_DMA_CH;									//����DMA����ͨ��
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;				//���ݴ����赽�ڴ�
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;						//���������ַ����
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;								//�����ڴ��ַ����
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; //�����������ݳ���Ϊ�����ֽ�
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;	//�����ڴ����ݳ���Ϊ�����ֽ�
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;											//����Ϊ��ͨģʽ(������)
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_HIGH;						//����Ϊ�����ȼ�
    HAL_DMA_Init(&hdma_usart3_rx);															//��ʼ��DMA����

    __HAL_LINKDMA(huart,hdmarx,hdma_usart3_rx);									//����DMA��������

		HAL_NVIC_SetPriority(USART3_Rx_DMA_CH_IRQn, 1, 1);					//����DMA����ռ���ȼ�����Ӧ���ȼ�   
		HAL_NVIC_EnableIRQ(USART3_Rx_DMA_CH_IRQn);									//����DMA�ж�

		__HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);										//���ô��ڿ����ж�
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)							//USART��IO�ӿڷ���ʼ��(�˺�����HAL���ڲ�����)
{
  if(huart->Instance == USART1)																	//�����USART1
  {
    __HAL_RCC_USART1_CLK_DISABLE();															//����USART1ʱ��	
  
    HAL_GPIO_DeInit(USART1_Tx_GPIO, USART1_Tx_GPIO_PIN);				//����USART1���Ϳ�
    HAL_GPIO_DeInit(USART1_Rx_GPIO, USART1_Rx_GPIO_PIN);				//����USART1���տ�

    HAL_NVIC_DisableIRQ(USART1_IRQn);														//����USART1�ж�

    HAL_DMA_DeInit(huart->hdmarx);															//����DMA����
  }
	if(huart->Instance == USART2)																	//�����USART2
  {
    __HAL_RCC_USART2_CLK_DISABLE();															//����USART2ʱ��	
  
    HAL_GPIO_DeInit(USART2_Tx_GPIO, USART2_Tx_GPIO_PIN);				//����USART2���Ϳ�
    HAL_GPIO_DeInit(USART2_Rx_GPIO, USART2_Rx_GPIO_PIN);				//����USART2���տ�

    HAL_NVIC_DisableIRQ(USART2_IRQn);														//����USART2�ж�

    HAL_DMA_DeInit(huart->hdmarx);															//����DMA����
  }
	if(huart->Instance == USART3)																	//�����USART3
  {
    __HAL_RCC_USART3_CLK_DISABLE();															//����USART3ʱ��	
  
    HAL_GPIO_DeInit(USART3_Tx_GPIO, USART3_Tx_GPIO_PIN);				//����USART3���Ϳ�
    HAL_GPIO_DeInit(USART3_Rx_GPIO, USART3_Rx_GPIO_PIN);				//����USART3���տ�

    HAL_NVIC_DisableIRQ(USART3_IRQn);														//����USART3�ж�

    HAL_DMA_DeInit(huart->hdmarx);															//����DMA����
  }
}

void USART1_Init(void)																					//USART1��ͨ�ų�ʼ��
{
  USART1_GPIO_ClK_ENABLE();																	    //ʹ��USART1��GPIOʱ��
	__HAL_RCC_DMA1_CLK_ENABLE();																	//ʹ��DMA1��ʱ��
	
  husart1.Instance = USART1;																		//���üĴ�������ַ
  husart1.Init.BaudRate = USART1_BAUDRATE;											//���ò�����
  husart1.Init.WordLength = UART_WORDLENGTH_8B;									//����Ϊ8λ�ֳ�
  husart1.Init.StopBits = UART_STOPBITS_1;											//����Ϊ1λֹͣλ
  husart1.Init.Parity = UART_PARITY_NONE;												//����Ϊ��У��λ
  husart1.Init.Mode = UART_MODE_TX_RX;													//����Ϊ���ͺͽ���ģʽ
  husart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;									//����Ϊ��ʹ��Ӳ��������
  husart1.Init.OverSampling = UART_OVERSAMPLING_16;							//����Ϊ������
  HAL_UART_Init(&husart1);																			//��ʼ��USART1
}

void USART2_Init(void)																					//USART2��ͨ�ų�ʼ��
{
  USART2_GPIO_ClK_ENABLE();																	    //ʹ��USART2��GPIOʱ��
	__HAL_RCC_DMA1_CLK_ENABLE();																	//ʹ��DMA1��ʱ��
	
  husart2.Instance = USART2;																		//���üĴ�������ַ
  husart2.Init.BaudRate = USART2_BAUDRATE;											//���ò�����
  husart2.Init.WordLength = UART_WORDLENGTH_8B;									//����Ϊ8λ�ֳ�
  husart2.Init.StopBits = UART_STOPBITS_1;											//����Ϊ1λֹͣλ
  husart2.Init.Parity = UART_PARITY_NONE;												//����Ϊ��У��λ
  husart2.Init.Mode = UART_MODE_TX_RX;													//����Ϊ���ͺͽ���ģʽ
  husart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;									//����Ϊ��ʹ��Ӳ��������
  husart2.Init.OverSampling = UART_OVERSAMPLING_16;							//����Ϊ������
  HAL_UART_Init(&husart2);																			//��ʼ��USART2
}

void USART3_Init(void)																					//USART3��ͨ�ų�ʼ��
{
  USART3_GPIO_ClK_ENABLE();																	    //ʹ��USART3��GPIOʱ��
	__HAL_RCC_DMA1_CLK_ENABLE();																	//ʹ��DMA1��ʱ��
	
  husart3.Instance = USART3;																		//���üĴ�������ַ
  husart3.Init.BaudRate = USART3_BAUDRATE;											//���ò�����
  husart3.Init.WordLength = UART_WORDLENGTH_8B;									//����Ϊ8λ�ֳ�
  husart3.Init.StopBits = UART_STOPBITS_1;											//����Ϊ1λֹͣλ
  husart3.Init.Parity = UART_PARITY_NONE;												//����Ϊ��У��λ
  husart3.Init.Mode = UART_MODE_TX_RX;													//����Ϊ���ͺͽ���ģʽ
  husart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;									//����Ϊ��ʹ��Ӳ��������
  husart3.Init.OverSampling = UART_OVERSAMPLING_16;							//����Ϊ������
  HAL_UART_Init(&husart3);																			//��ʼ��USART3
}

void USART_Init(void)                                           //USART��ʼ��
{
  USART1_Init();                                                //USART1��ʼ��
  USART2_Init();                                                //USART2��ʼ��
	USART3_Init();                                                //USART3��ʼ��
	
	HAL_UART_Receive_DMA(&husart1,U1_Rx_Buf,MAX_RX_SIZE);
	HAL_UART_Receive_DMA(&husart2,U2_Rx_Buf,MAX_RX_SIZE);
	HAL_UART_Receive_DMA(&husart3,U3_Rx_Buf,MAX_RX_SIZE);
}

int fputc(int ch, FILE *f)																			//�ض���⺯��printf��USART3
{
	HAL_UART_Transmit(&husart3,(uint8_t *)&ch,1,U_TIME_OUT);			//ͨ�����ڷ���һ���ַ�,chΪ�ַ��Ĵ洢��ַ,uTimeoutΪ��ʱʱ��
  return ch;
}

int fgetc(FILE * f)																							//�ض���⺯��getchar,scanf��USART3
{
  uint8_t ch = 0;
  HAL_UART_Receive(&husart3,&ch,1,U_TIME_OUT); 									//ͨ�����ڽ���һ���ַ�,chΪ�ַ��Ĵ洢��ַ,uTimeoutΪ��ʱʱ��          
  return ch;
}
