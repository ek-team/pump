#include "tim.h"

void SysClk_Init(void)                                                   //系统时钟初始化函数
{
  RCC_OscInitTypeDef RCC_OscInitStruct;                                  //定义晶振初始化结构体
  RCC_ClkInitTypeDef RCC_ClkInitStruct;                                  //定义时钟初始化结构体

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;             //设置为外部晶振
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                               //打开HSE(外部高速晶振)
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;                //HSE预分频设为1
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                           //打开PLL锁相环
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;                   //设置PLL锁相环来源为HSE
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;                           //设置PLL锁相环为9倍频
  HAL_RCC_OscConfig(&RCC_OscInitStruct);                                 //载入晶振初始化设置
		
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK  //设置HCLK,SYSCLK,PCK1(APB1),PCK2(APB2)
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;              //设置SYSCLK来源为PLL锁相环:72MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                     //设置AHB为SYSCLK的一分频:72MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;                      //APB1设为AHB的二分频:36MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;                      //APB2设为AHB的一分频:72MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);              //载入时钟初始化设置

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);                        //设置并启动系统滴答定时器,1ms发生一次中断(HAL_RCC_GetHCLKFreq()/1000000:1us发生一次中断)
	
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);                   //设置系统滴答定时器时钟源为HCLK(AHB)

  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);                              //设置系统滴答定时器中断优先级
}
