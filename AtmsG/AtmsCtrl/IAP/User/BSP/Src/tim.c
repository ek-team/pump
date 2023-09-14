#include "tim.h"

void SysClk_Init(void)                                                   //ϵͳʱ�ӳ�ʼ������
{
  RCC_OscInitTypeDef RCC_OscInitStruct;                                  //���徧���ʼ���ṹ��
  RCC_ClkInitTypeDef RCC_ClkInitStruct;                                  //����ʱ�ӳ�ʼ���ṹ��

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;             //����Ϊ�ⲿ����
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                               //��HSE(�ⲿ���پ���)
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;                //HSEԤ��Ƶ��Ϊ1
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                           //��PLL���໷
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;                   //����PLL���໷��ԴΪHSE
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;                           //����PLL���໷Ϊ9��Ƶ
  HAL_RCC_OscConfig(&RCC_OscInitStruct);                                 //���뾧���ʼ������
		
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK  //����HCLK,SYSCLK,PCK1(APB1),PCK2(APB2)
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;              //����SYSCLK��ԴΪPLL���໷:72MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                     //����AHBΪSYSCLK��һ��Ƶ:72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;                      //APB1��ΪAHB�Ķ���Ƶ:36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;                      //APB2��ΪAHB��һ��Ƶ:72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);              //����ʱ�ӳ�ʼ������

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);                        //���ò�����ϵͳ�δ�ʱ��,1ms����һ���ж�(HAL_RCC_GetHCLKFreq()/1000000:1us����һ���ж�)
	
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);                   //����ϵͳ�δ�ʱ��ʱ��ԴΪHCLK(AHB)

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);                              //����ϵͳ�δ�ʱ���ж����ȼ�
}
