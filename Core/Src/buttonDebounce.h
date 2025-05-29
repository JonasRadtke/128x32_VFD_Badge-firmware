/*
 * buttonDebounce.h
 *
 *  Created on: May 23, 2025
 *      Author: Sentry
 */

#ifndef SRC_BUTTONDEBOUNCE_H_
#define SRC_BUTTONDEBOUNCE_H_

#include "main.h"


class buttonDebounce {
private:
	GPIO_TypeDef *GPIOPort;
	uint16_t GPIOPin;

	uint32_t reading = 0;
	uint32_t lastReading = 0;
	uint32_t lastTime = 0;
	uint32_t state = 1;

	uint32_t buttonShortPressed = 0;
	uint32_t buttonLongPressed = 0;
	uint32_t longOneCyclePressed = 0;

	uint32_t buttonPressed = 0;
	uint32_t pressStartTime = 0;

	uint32_t longPressTime = 1000;
	uint32_t longOneCycleLast = 0;


public:
	buttonDebounce(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
	void debounce();
	uint32_t isShort();
	uint32_t isLong();
	uint32_t isLongOneCycle();
	uint32_t directState();
};

#endif /* SRC_BUTTONDEBOUNCE_H_ */
