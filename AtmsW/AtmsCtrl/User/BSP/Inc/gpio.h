#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f1xx_hal.h"
#include "sys.h"

#define FG1_GPIO                            GPIOB
#define FG1_GPIO_PIN                        GPIO_PIN_0

#define FG2_GPIO                            GPIOB
#define FG2_GPIO_PIN                        GPIO_PIN_1

#define EMER_GPIO                           GPIOA
#define EMER_GPIO_PIN                       GPIO_PIN_7

#define HMI_PW_GPIO                         GPIOB
#define HMI_PW_GPIO_PIN                     GPIO_PIN_5

#define GATE_PW_GPIO                        GPIOC
#define GATE_PW_GPIO_PIN                    GPIO_PIN_4

#define U3_PW_GPIO                          GPIOC
#define U3_PW_GPIO_PIN                      GPIO_PIN_11

#define PUMP1_EN_GPIO                       GPIOB
#define PUMP1_EN_GPIO_PIN                   GPIO_PIN_8

#define PUMP2_EN_GPIO                       GPIOB
#define PUMP2_EN_GPIO_PIN                   GPIO_PIN_6

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

void GPIO_Init(void);

#endif
