#include "usart.h"

UART_HandleTypeDef husart1;																			//����USART1�Ľṹ��
UART_HandleTypeDef husart3;																			//����USART3�Ľṹ��

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
  }
	if(huart->Instance == USART3)																	//�����USART3
  {
    __HAL_RCC_USART3_CLK_DISABLE();															//����USART3ʱ��	
  
    HAL_GPIO_DeInit(USART3_Tx_GPIO, USART3_Tx_GPIO_PIN);				//����USART3���Ϳ�
    HAL_GPIO_DeInit(USART3_Rx_GPIO, USART3_Rx_GPIO_PIN);				//����USART3���տ�

    HAL_NVIC_DisableIRQ(USART3_IRQn);														//����USART3�ж�
  }
}

void USART1_Init(void)																					//USART1��ͨ�ų�ʼ��
{
  USART1_GPIO_ClK_ENABLE();																	    //ʹ��USART1��GPIOʱ��
	
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

void USART3_Init(void)																					//USART3��ͨ�ų�ʼ��
{
  USART3_GPIO_ClK_ENABLE();																	    //ʹ��USART3��GPIOʱ��
	
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
	USART3_Init();                                                //USART3��ʼ��
	HAL_UART_Receive_IT(&husart1,&U1_RxData,1);  
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
