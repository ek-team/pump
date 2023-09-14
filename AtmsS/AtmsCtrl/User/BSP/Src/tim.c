#include "tim.h"

TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim8;
DMA_HandleTypeDef hdma_tim5_ch3_up;
DMA_HandleTypeDef hdma_tim8_ch3_up;
uint32_t B_Buff[PWM_CYCLE] = {0};
uint32_t C_Buff[PWM_CYCLE] = {0};

void SysClk_Init(void)                                                   
{
  RCC_OscInitTypeDef RCC_OscInitStruct;                                  
  RCC_ClkInitTypeDef RCC_ClkInitStruct;                                 

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;             
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                               
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;              
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                          
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;                  
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;                          
  HAL_RCC_OscConfig(&RCC_OscInitStruct);                                 
		
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK  
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;             
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                    
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;                      
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;                      
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);              

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);                        
	
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);                   

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);                             
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance==TIM4)
  {
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }
}
void TIM4_Init()
{
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = PUMP_PRESCALER;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = PUMP_PERIOD;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_PWM_Init(&htim4);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.Pulse = 0;
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2);
  sConfigOC.Pulse = 0;
  HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4);

  HAL_TIM_MspPostInit(&htim4);
	
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
}

void PWM_Set(uint8_t num,uint8_t para)
{
  if(num == 1)
  {
    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,para);
  }
  if(num == 2)
  {
    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,para);
  }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
  if(htim_pwm->Instance==TIM4)
  {
    __HAL_RCC_TIM4_CLK_ENABLE();
  }
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{
  if(htim_pwm->Instance==TIM4)
  {
    __HAL_RCC_TIM4_CLK_DISABLE();
  }
}

void TIM_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim8.Instance = TIM8;
  htim8.Init.Prescaler = TIM_PRESCALER;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = TIM_PERIOD;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim8);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig);
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig);
	
	uint16_t Count = PWM_CYCLE;  
	while(--Count)
	{
	  B_Buff[Count] = GPIOB->BSRR;
	}
	__HAL_TIM_ENABLE_DMA(&htim8, TIM_DMA_UPDATE);
  HAL_DMA_Start_IT(htim8.hdma[TIM_DMA_ID_UPDATE], (uint32_t)(B_Buff), (uint32_t)(&GPIOB->BSRR), PWM_CYCLE);
	HAL_TIM_Base_Start(&htim8);

  htim5.Instance = TIM5;
  htim5.Init.Prescaler = TIM_PRESCALER;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = TIM_PERIOD;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim5);
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig);
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig);
	
	Count = PWM_CYCLE;  
	while(--Count)
	{
	   C_Buff[Count] = GPIOC->BSRR;
	}
	__HAL_TIM_ENABLE_DMA(&htim5, TIM_DMA_UPDATE);
  HAL_DMA_Start_IT(htim5.hdma[TIM_DMA_ID_UPDATE], (uint32_t)(C_Buff), (uint32_t)(&GPIOC->BSRR), PWM_CYCLE);
	HAL_TIM_Base_Start(&htim5);
}

void DMA_Init(void)
{
  __HAL_RCC_DMA2_CLK_ENABLE();
	
	HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);
	HAL_NVIC_SetPriority(DMA2_Channel2_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(DMA2_Channel2_IRQn);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM5)
  {
    __HAL_RCC_TIM5_CLK_ENABLE();
    hdma_tim5_ch3_up.Instance = DMA2_Channel2;
    hdma_tim5_ch3_up.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim5_ch3_up.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim5_ch3_up.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim5_ch3_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_tim5_ch3_up.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_tim5_ch3_up.Init.Mode = DMA_CIRCULAR;
    hdma_tim5_ch3_up.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_tim5_ch3_up);
    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC3],hdma_tim5_ch3_up);
    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_UPDATE],hdma_tim5_ch3_up);
  }
  if(htim_base->Instance==TIM8)
  {
    __HAL_RCC_TIM8_CLK_ENABLE();
    hdma_tim8_ch3_up.Instance = DMA2_Channel1;
    hdma_tim8_ch3_up.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim8_ch3_up.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim8_ch3_up.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim8_ch3_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_tim8_ch3_up.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_tim8_ch3_up.Init.Mode = DMA_CIRCULAR;
    hdma_tim8_ch3_up.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_tim8_ch3_up);
    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC3],hdma_tim8_ch3_up);
    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_UPDATE],hdma_tim8_ch3_up);
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM5)
  {
    __HAL_RCC_TIM5_CLK_DISABLE();
    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC3]);
    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_UPDATE]);
  }
  if(htim_base->Instance==TIM8)
  {
    __HAL_RCC_TIM8_CLK_DISABLE();
    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC3]);
    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_UPDATE]);
  }
}

void N_SET(uint8_t Gate,int16_t EN)
{
	uint16_t Count = PWM_CYCLE;
	while(Count--)
	{
		if(Gate == 1 && EN == 0)
		{
			B_Buff[Count] |= (uint32_t)N1_EN_GPIO_PIN << 16;
			B_Buff[Count] &= ~N1_EN_GPIO_PIN;
		}
		if(Gate == 1 && EN == 1)
		{
			B_Buff[Count] |= N1_EN_GPIO_PIN;
			B_Buff[Count] &= ~((uint32_t)N1_EN_GPIO_PIN << 16);
		}
		if(Gate == 1 && EN == 2)
		{
			if(DB.Para[29] == 0) return;
			if(Count > PWM_NA)
			{
				B_Buff[Count] |= (uint32_t)N1_EN_GPIO_PIN << 16;
				B_Buff[Count] &= ~N1_EN_GPIO_PIN;
			}
			else
      {				
				B_Buff[Count] |= N1_EN_GPIO_PIN;
				B_Buff[Count] &= ~((uint32_t)N1_EN_GPIO_PIN << 16);
			}
		}
		if(Gate == 2 && EN == 0) 
		{  
			B_Buff[Count] |= (uint32_t)N2_EN_GPIO_PIN << 16;
			B_Buff[Count] &= ~N2_EN_GPIO_PIN;
		}
		if(Gate == 2 && EN == 1) 
		{  
			B_Buff[Count] |= N2_EN_GPIO_PIN;
			B_Buff[Count] &= ~((uint32_t)N2_EN_GPIO_PIN << 16);
		}
		if(Gate == 2 && EN == 2)
		{
			if(DB.Para[29] == 0) return;
			if(Count > PWM_NA)
			{
				B_Buff[Count] |= (uint32_t)N2_EN_GPIO_PIN << 16;
				B_Buff[Count] &= ~N2_EN_GPIO_PIN;
			}
			else
      {				
				B_Buff[Count] |= N2_EN_GPIO_PIN;
				B_Buff[Count] &= ~((uint32_t)N2_EN_GPIO_PIN << 16);
			}
		}
		if(Gate == 3 && EN == 0) 
		{  
			C_Buff[Count] |= (uint32_t)K1_EN_GPIO_PIN << 16;
			C_Buff[Count] &= ~K1_EN_GPIO_PIN;
		}
		if(Gate == 3 && EN == 1) 
		{
			C_Buff[Count] |= K1_EN_GPIO_PIN;
			C_Buff[Count] &= ~((uint32_t)K1_EN_GPIO_PIN << 16);
		}
		if(Gate == 3 && EN == 2)
		{
			if(DB.Para[29] == 0) return;
			if(Count > PWM_NB)
			{
				C_Buff[Count] |= (uint32_t)K1_EN_GPIO_PIN << 16;
				C_Buff[Count] &= ~K1_EN_GPIO_PIN;
			}
			else
      {				
				C_Buff[Count] |= K1_EN_GPIO_PIN;
				C_Buff[Count] &= ~((uint32_t)K1_EN_GPIO_PIN << 16);
			}
		}
		if(Gate == 4 && EN == 0) 
		{
			C_Buff[Count] |= (uint32_t)K2_EN_GPIO_PIN << 16;
			C_Buff[Count] &= ~K2_EN_GPIO_PIN;
		}
		if(Gate == 4 && EN == 1) 
		{
			C_Buff[Count] |= K2_EN_GPIO_PIN;
			C_Buff[Count] &= ~((uint32_t)K2_EN_GPIO_PIN << 16);
		}
		if(Gate == 4 && EN == 2)
		{
			if(DB.Para[29] == 0) return;
			if(Count > PWM_NB)
			{
				C_Buff[Count] |= (uint32_t)K2_EN_GPIO_PIN << 16;
				C_Buff[Count] &= ~K2_EN_GPIO_PIN;
			}
			else
      {				
				C_Buff[Count] |= K2_EN_GPIO_PIN;
				C_Buff[Count] &= ~((uint32_t)K2_EN_GPIO_PIN << 16);
			}
		}
		if(Gate == 5 && EN == 0) 
		{
			C_Buff[Count] |= (uint32_t)K3_EN_GPIO_PIN << 16;
			C_Buff[Count] &= ~K3_EN_GPIO_PIN;
		}
		if(Gate == 5 && EN == 1) 
		{
			C_Buff[Count] |= K3_EN_GPIO_PIN;
			C_Buff[Count] &= ~((uint32_t)K3_EN_GPIO_PIN << 16);
		}
		if(Gate == 5 && EN == 2)
		{
			if(DB.Para[29] == 0) return;
			if(Count > PWM_NB)
			{
				C_Buff[Count] |= (uint32_t)K3_EN_GPIO_PIN << 16;
				C_Buff[Count] &= ~K3_EN_GPIO_PIN;
			}
			else
      {				
				C_Buff[Count] |= K3_EN_GPIO_PIN;
				C_Buff[Count] &= ~((uint32_t)K3_EN_GPIO_PIN << 16);
			}
		}
		if(Gate == 6 && EN == 0) 
		{
			C_Buff[Count] |= (uint32_t)K4_EN_GPIO_PIN << 16;
			C_Buff[Count] &= ~K4_EN_GPIO_PIN;
		}
		if(Gate == 6 && EN == 1) 
		{
			C_Buff[Count] |= K4_EN_GPIO_PIN;
			C_Buff[Count] &= ~((uint32_t)K4_EN_GPIO_PIN << 16);
		}
		if(Gate == 6 && EN == 2)
		{
			if(DB.Para[29] == 0) return;
			if(Count > PWM_NB)
			{
				C_Buff[Count] |= (uint32_t)K4_EN_GPIO_PIN << 16;
				C_Buff[Count] &= ~K4_EN_GPIO_PIN;
			}
			else
      {				
				C_Buff[Count] |= K4_EN_GPIO_PIN;
				C_Buff[Count] &= ~((uint32_t)K4_EN_GPIO_PIN << 16);
			}
		}
		if(Gate == 7 && EN == 0) 
		{
			B_Buff[Count] |= (uint32_t)K5_EN_GPIO_PIN << 16;
			B_Buff[Count] &= ~K5_EN_GPIO_PIN;
		}
		if(Gate == 7 && EN == 1) 
		{
			B_Buff[Count] |= K5_EN_GPIO_PIN;
			B_Buff[Count] &= ~((uint32_t)K5_EN_GPIO_PIN << 16);
		}
		if(Gate == 7 && EN == 2)
		{
			if(DB.Para[29] == 0) return;
			if(Count > PWM_NB)
			{
				B_Buff[Count] |= (uint32_t)K5_EN_GPIO_PIN << 16;
				B_Buff[Count] &= ~K5_EN_GPIO_PIN;
			}
			else
      {				
				B_Buff[Count] |= K5_EN_GPIO_PIN;
				B_Buff[Count] &= ~((uint32_t)K5_EN_GPIO_PIN << 16);
			}
		}
		if(Gate == 8 && EN == 0) 
		{
			B_Buff[Count] |= (uint32_t)N8_EN_GPIO_PIN << 16;
			B_Buff[Count] &= ~N8_EN_GPIO_PIN;
		}
		if(Gate == 8 && EN == 1) 
		{
			B_Buff[Count] |= N8_EN_GPIO_PIN;
			B_Buff[Count] &= ~((uint32_t)N8_EN_GPIO_PIN << 16);
		}
		if(Gate == 8 && EN == 2)
		{
			if(DB.Para[29] == 0) return;
			if(Count > PWM_NA)
			{
				B_Buff[Count] |= (uint32_t)N8_EN_GPIO_PIN << 16;
				B_Buff[Count] &= ~N8_EN_GPIO_PIN;
			}
			else
      {				
				B_Buff[Count] |= N8_EN_GPIO_PIN;
				B_Buff[Count] &= ~((uint32_t)N8_EN_GPIO_PIN << 16);
			}
		}
	}
	vTaskDelay(1);
}

