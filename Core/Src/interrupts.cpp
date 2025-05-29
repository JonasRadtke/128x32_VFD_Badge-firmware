/*
 * interrupts.cpp
 *
 *  Created on: Feb 14, 2025
 *      Author: Sentry
 */

#include "main.h"
#include "MN12832L.h"


extern MN12832L display;
extern volatile uint32_t wakeUpRequest;
extern volatile uint32_t sleeping;

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;

void TIM6_DAC_LPTIM1_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim6);
  display.newFrame();
}



void TIM7_LPTIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim7);
}
