/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u0xx_hal.h"
#include "stm32u0xx_ll_spi.h"
#include "stm32u0xx_ll_system.h"
#include "stm32u0xx_ll_gpio.h"
#include "stm32u0xx_ll_exti.h"
#include "stm32u0xx_ll_bus.h"
#include "stm32u0xx_ll_cortex.h"
#include "stm32u0xx_ll_rcc.h"
#include "stm32u0xx_ll_utils.h"
#include "stm32u0xx_ll_pwr.h"
#include "stm32u0xx_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
void TIM6_DAC_LPTIM1_IRQHandler(void);
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SD_CS_Pin LL_GPIO_PIN_4
#define SD_CS_GPIO_Port GPIOA
#define SD_CLK_Pin LL_GPIO_PIN_5
#define SD_CLK_GPIO_Port GPIOA
#define SD_MISO_Pin LL_GPIO_PIN_6
#define SD_MISO_GPIO_Port GPIOA
#define SD_MOSI_Pin LL_GPIO_PIN_7
#define SD_MOSI_GPIO_Port GPIOA
#define USER_BT_Pin LL_GPIO_PIN_11
#define USER_BT_GPIO_Port GPIOB
#define DIS_BLK_Pin LL_GPIO_PIN_12
#define DIS_BLK_GPIO_Port GPIOB
#define DIS_LAT_Pin LL_GPIO_PIN_13
#define DIS_LAT_GPIO_Port GPIOB
#define DIS_GCP_Pin LL_GPIO_PIN_14
#define DIS_GCP_GPIO_Port GPIOB
#define DIS_V_ENABLE_Pin LL_GPIO_PIN_3
#define DIS_V_ENABLE_GPIO_Port GPIOB
#define CHARGER_QON_Pin LL_GPIO_PIN_4
#define CHARGER_QON_GPIO_Port GPIOB
#define CHARGER_CHARGE_EN_Pin LL_GPIO_PIN_5
#define CHARGER_CHARGE_EN_GPIO_Port GPIOB
#define I2C_SCL_Pin LL_GPIO_PIN_6
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin LL_GPIO_PIN_7
#define I2C_SDA_GPIO_Port GPIOB
#define CHARGER_POWER_SEL_Pin LL_GPIO_PIN_8
#define CHARGER_POWER_SEL_GPIO_Port GPIOB
#define DIS_FILAMENT_V_EN_Pin LL_GPIO_PIN_9
#define DIS_FILAMENT_V_EN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
