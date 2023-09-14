#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f1xx_hal.h"
#include "sys.h"


#define EMER_GPIO                           GPIOB
#define EMER_GPIO_PIN                       GPIO_PIN_0

#define HMI_PW_GPIO                         GPIOB
#define HMI_PW_GPIO_PIN                     GPIO_PIN_5

#define GATE_PW_GPIO                        GPIOB
#define GATE_PW_GPIO_PIN                    GPIO_PIN_6

#define U3_PW_GPIO                          GPIOB
#define U3_PW_GPIO_PIN                      GPIO_PIN_8

#define PUMP_EN_GPIO                        GPIOB
#define PUMP_EN_GPIO_PIN                    GPIO_PIN_9

#define K1_EN_GPIO                          GPIOC
#define K1_EN_GPIO_PIN                      GPIO_PIN_9

#define K2_EN_GPIO                          GPIOC
#define K2_EN_GPIO_PIN                      GPIO_PIN_8

#define K3_EN_GPIO                          GPIOC
#define K3_EN_GPIO_PIN                      GPIO_PIN_7

#define K4_EN_GPIO                          GPIOC
#define K4_EN_GPIO_PIN                      GPIO_PIN_6

#define K5_EN_GPIO                          GPIOB
#define K5_EN_GPIO_PIN                      GPIO_PIN_15

#define N1_EN_GPIO                          GPIOB
#define N1_EN_GPIO_PIN                      GPIO_PIN_14 //·§1

#define N2_EN_GPIO                          GPIOB
#define N2_EN_GPIO_PIN                      GPIO_PIN_13 //·§2

#define N8_EN_GPIO                          GPIOB
#define N8_EN_GPIO_PIN                      GPIO_PIN_12 //Ð¹Æø·§

#define FAN_EN_GPIO                         GPIOA
#define FAN_EN_GPIO_PIN                     GPIO_PIN_1

#define LED_RUN_GPIO                        GPIOB
#define LED_RUN_GPIO_PIN 				    GPIO_PIN_4

#define ADC_PW_GPIO                         GPIOA
#define ADC_PW_GPIO_PIN                     GPIO_PIN_5

void GPIO_Init(void);

#endif
