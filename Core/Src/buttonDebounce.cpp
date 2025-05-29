/*
 * buttonDebounce.cpp
 *
 *  Created on: May 23, 2025
 *      Author: Sentry
 */

#include "buttonDebounce.h"

buttonDebounce::buttonDebounce(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	this->GPIOPort = GPIOx;
	this->GPIOPin = GPIO_Pin;
}

void buttonDebounce::debounce(){
	this->buttonShortPressed = 0;
	this->longOneCyclePressed = 0;

	this->reading = HAL_GPIO_ReadPin(this->GPIOPort, this->GPIOPin);

	if(state){
		this->buttonShortPressed = 0;
		this->buttonLongPressed = 0;
		this->longOneCycleLast = 0;
	}

	if(this->reading != this->lastReading){
		this->lastTime = HAL_GetTick();
	}

	if(this->lastTime + 20 < HAL_GetTick()){
		if(this->state != reading){
			state = reading;
		}
	}
	this->lastReading = this->reading;

    if (!state && !buttonPressed) {
        buttonPressed = 1;
        pressStartTime = HAL_GetTick();
    }

    if (state && buttonPressed) {
    	buttonPressed = 0;
        uint32_t pressDuration = HAL_GetTick() - pressStartTime;

        if (pressDuration < longPressTime) {
            this->buttonShortPressed = 1;
        }
    }


    if(buttonPressed){
        uint32_t pressDuration = HAL_GetTick() - pressStartTime;
        if (pressDuration > longPressTime) {
        	this->buttonLongPressed = 1;
        	if(!this->longOneCycleLast){
        		this->longOneCyclePressed = 1;
        		this->longOneCycleLast = 1;
        	}
        }
    }



}

uint32_t buttonDebounce::isShort(){
	if(this->buttonShortPressed){
		return 1;
	}
	return 0;
}


uint32_t buttonDebounce::isLong(){
	if(this->buttonLongPressed){
		return 1;
	}
	return 0;
}

uint32_t buttonDebounce::isLongOneCycle(){
	if(this->longOneCyclePressed){
		return 1;
	}
	return 0;
}

uint32_t buttonDebounce::directState(){
	return !HAL_GPIO_ReadPin(this->GPIOPort, this->GPIOPin);
}

