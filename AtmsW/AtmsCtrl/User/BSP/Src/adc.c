#include "adc.h"

uint32_t ADC_Val[ADC_NUM];

ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

void ADC_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;
	
  hadc.Instance = ADCx;
  hadc.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.NbrOfConversion = ADC_NUM;
  HAL_ADC_Init(&hadc);

  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;//ADC多通道之间有串扰,时间要加长
	
  sConfig.Channel = TMP_ADC_DMA_CH;
  sConfig.Rank = TMP_ADC + 1;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
	
  sConfig.Channel = PRESS_ADC_DMA_CH;
  sConfig.Rank = PRESS_ADC + 1;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
	
  HAL_ADCEx_Calibration_Start(&hadc);
  HAL_ADC_Start_DMA(&hadc,ADC_Val,ADC_NUM);                    //启动AD转换并使能DMA传输和中断
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)                  //ADC外设初始化配置(HAL库内部调用)
{
  GPIO_InitTypeDef GPIO_InitStruct;                            //定义GPIO初始化结构体
  if(hadc->Instance==ADCx)
  {
    ADC_CLK_ENABLE();                                          //外设时钟使能
    ADC_DMA_CLK_ENABLE();                                      //DMA时钟使能
    PRESS_ADC_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = PRESS_ADC_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(PRESS_ADC_GPIO, &GPIO_InitStruct);
		
    hdma_adc.Instance = ADC_DMA_CH;                            //DMA外设初始化配置
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc.Init.Mode = DMA_CIRCULAR;
    hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_adc);

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc);                  //连接DMA
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)               //ADC外设反初始化配置(HAL库内部调用)
{
  if(hadc->Instance==ADCx)
  {
    ADC_CLK_DISABLE();                                        //禁用ADC外设时钟
    HAL_DMA_DeInit(hadc->DMA_Handle);                         //DMA外设反初始化
  }
} 
