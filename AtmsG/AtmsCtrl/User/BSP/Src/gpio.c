#include "gpio.h"

void GPIO_Init(void)														
{
  GPIO_InitTypeDef GPIO_InitStruct;                                    //定义GPIO初始化结构体

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_AFIO_REMAP_SWJ_NOJTAG();
	
  HAL_GPIO_WritePin(HMI_PW_GPIO,  HMI_PW_GPIO_PIN,  GPIO_PIN_SET);
  HAL_GPIO_WritePin(GATE_PW_GPIO, GATE_PW_GPIO_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(U3_PW_GPIO, U3_PW_GPIO_PIN, GPIO_PIN_SET);
	
  HAL_GPIO_WritePin(K1_EN_GPIO,   K1_EN_GPIO_PIN,   GPIO_PIN_RESET);
  HAL_GPIO_WritePin(K2_EN_GPIO,   K2_EN_GPIO_PIN,   GPIO_PIN_RESET);
  HAL_GPIO_WritePin(K3_EN_GPIO,   K3_EN_GPIO_PIN,   GPIO_PIN_RESET);
  HAL_GPIO_WritePin(K4_EN_GPIO,   K4_EN_GPIO_PIN,   GPIO_PIN_RESET);
  HAL_GPIO_WritePin(K5_EN_GPIO,   K5_EN_GPIO_PIN,   GPIO_PIN_RESET);
  HAL_GPIO_WritePin(N1_EN_GPIO,   N1_EN_GPIO_PIN,   GPIO_PIN_RESET);
  HAL_GPIO_WritePin(N2_EN_GPIO,   N2_EN_GPIO_PIN,   GPIO_PIN_RESET);
  HAL_GPIO_WritePin(N8_EN_GPIO,   N8_EN_GPIO_PIN,   GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PUMP_EN_GPIO, PUMP_EN_GPIO_PIN, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(LED_RUN_GPIO, LED_RUN_GPIO_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(FAN_EN_GPIO,  FAN_EN_GPIO_PIN,  GPIO_PIN_SET);

  GPIO_InitStruct.Pin   = HMI_PW_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HMI_PW_GPIO, &GPIO_InitStruct);				

  GPIO_InitStruct.Pin   = GATE_PW_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP; 
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GATE_PW_GPIO, &GPIO_InitStruct);			

  GPIO_InitStruct.Pin   = U3_PW_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(U3_PW_GPIO, &GPIO_InitStruct);	
	
  GPIO_InitStruct.Pin   = PUMP_EN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PUMP_EN_GPIO, &GPIO_InitStruct);	
	
  GPIO_InitStruct.Pin   = K1_EN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;                         
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(K1_EN_GPIO, &GPIO_InitStruct);	
	
  GPIO_InitStruct.Pin   = K2_EN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;                        
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(K2_EN_GPIO, &GPIO_InitStruct);	
	
  GPIO_InitStruct.Pin   = K3_EN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP; 
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(K3_EN_GPIO, &GPIO_InitStruct);	
	
  GPIO_InitStruct.Pin   = K4_EN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(K4_EN_GPIO, &GPIO_InitStruct);	
	
  GPIO_InitStruct.Pin   = K5_EN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP; 
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(K5_EN_GPIO, &GPIO_InitStruct);	
	
  GPIO_InitStruct.Pin   = N1_EN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP; 
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(N1_EN_GPIO, &GPIO_InitStruct);	
	
  GPIO_InitStruct.Pin   = N2_EN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP; 
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(N2_EN_GPIO, &GPIO_InitStruct);	
	
  GPIO_InitStruct.Pin   = N8_EN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(N8_EN_GPIO, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin   = LED_RUN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_RUN_GPIO, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin   = ADC_PW_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP; 
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ADC_PW_GPIO, &GPIO_InitStruct);	

  GPIO_InitStruct.Pin   = FAN_EN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FAN_EN_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.Pin   = EMER_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  HAL_GPIO_Init(EMER_GPIO, &GPIO_InitStruct);
}
