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

  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;//ADC��ͨ��֮���д���,ʱ��Ҫ�ӳ�
	
  sConfig.Channel = TMP_ADC_DMA_CH;
  sConfig.Rank = TMP_ADC + 1;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
	
  sConfig.Channel = PRESS_ADC_DMA_CH;
  sConfig.Rank = PRESS_ADC + 1;
  HAL_ADC_ConfigChannel(&hadc, &sConfig);
	
  HAL_ADCEx_Calibration_Start(&hadc);
  HAL_ADC_Start_DMA(&hadc,ADC_Val,ADC_NUM);                    //����ADת����ʹ��DMA������ж�
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)                  //ADC�����ʼ������(HAL���ڲ�����)
{
  GPIO_InitTypeDef GPIO_InitStruct;                            //����GPIO��ʼ���ṹ��
  if(hadc->Instance==ADCx)
  {
    ADC_CLK_ENABLE();                                          //����ʱ��ʹ��
    ADC_DMA_CLK_ENABLE();                                      //DMAʱ��ʹ��
    PRESS_ADC_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = PRESS_ADC_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(PRESS_ADC_GPIO, &GPIO_InitStruct);
		
    hdma_adc.Instance = ADC_DMA_CH;                            //DMA�����ʼ������
    hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc.Init.Mode = DMA_CIRCULAR;
    hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
    HAL_DMA_Init(&hdma_adc);

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc);                  //����DMA
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)               //ADC���跴��ʼ������(HAL���ڲ�����)
{
  if(hadc->Instance==ADCx)
  {
    ADC_CLK_DISABLE();                                        //����ADC����ʱ��
    HAL_DMA_DeInit(hadc->DMA_Handle);                         //DMA���跴��ʼ��
  }
} 
