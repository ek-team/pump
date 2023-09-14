#include "gpio.h"

void GPIO_Init(void)														
{
  GPIO_InitTypeDef GPIO_InitStruct;                                    //定义GPIO初始化结构体

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_AFIO_REMAP_SWJ_NOJTAG();
	
	HAL_GPIO_WritePin(HMI_PW_GPIO,HMI_PW_GPIO_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(U3_PW_GPIO,U3_PW_GPIO_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RUN_GPIO, LED_RUN_GPIO_PIN, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin   = U3_PW_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(U3_PW_GPIO, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin   = HMI_PW_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(HMI_PW_GPIO, &GPIO_InitStruct);		

	GPIO_InitStruct.Pin   = LED_RUN_GPIO_PIN;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_RUN_GPIO, &GPIO_InitStruct);	
}
