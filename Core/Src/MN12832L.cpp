/*
 * MN12832L.cpp
 *
 *  Created on: May 17, 2025
 *      Author: Sentry
 */

#include "MN12832L.h"

MN12832L::MN12832L() {
}

void MN12832L::init(){
	this->enableDisplay(false);
	this->enableFilamentVoltage(false);
	this->enableDisplayVoltage(false);
	LL_SPI_Enable(SPI2);
	this->clearBuffer();
}


void MN12832L::newFrame(){
		uint32_t datlen = 44;
//		HAL_SuspendTick();

		uint32_t i = 0;
		while (datlen--) {
			this->newSegment(this->outBuffer[i]);
			i++;
		}
//		HAL_ResumeTick();
}

void MN12832L::newSegment(uint8_t data[]){
		uint32_t datlen = 30;

		GPIOB->BRR = DIS_BLK_Pin;
		uint32_t i = 0;
		while (datlen--) {
			while(!LL_SPI_IsActiveFlag_TXE(SPI2));
			LL_SPI_TransmitData8(SPI2, data[i++] ); //data[i++]

		}
		while((SPI2->SR & 0x0080));
		GPIOB->BSRR = DIS_BLK_Pin;
		// Lat
		GPIOB->BSRR = DIS_LAT_Pin;
		GPIOB->BRR = DIS_LAT_Pin;

}

void MN12832L::fillGridBytes(){
	for(uint32_t grid = 0; grid < 44; grid++){
		this->outBuffer[grid][24 + grid/8] |= (1 << (7-grid%8));
		this->outBuffer[grid][24 + (grid+1)/8] |= (1 << (7-((grid+1)%8)));
	}
}

void MN12832L::clearBuffer(){
	for(uint32_t i = 0; i<44; i++){
		for(uint32_t k = 0; k<30; k++){
			this->outBuffer[i][k] = 0;
		}
	}
	this->fillGridBytes();
}





void MN12832L::enableDisplayVoltage(bool on){
	if (on){
		LL_GPIO_SetOutputPin(DIS_V_ENABLE_GPIO_Port, DIS_V_ENABLE_Pin);
	}
	else{
		LL_GPIO_ResetOutputPin(DIS_V_ENABLE_GPIO_Port, DIS_V_ENABLE_Pin);
	}
}

void MN12832L::enableFilamentVoltage(bool on){
	if (on){
		LL_GPIO_SetOutputPin(DIS_FILAMENT_V_EN_GPIO_Port, DIS_FILAMENT_V_EN_Pin);
	}
	else{
		LL_GPIO_ResetOutputPin(DIS_FILAMENT_V_EN_GPIO_Port, DIS_FILAMENT_V_EN_Pin);
	}
}

void MN12832L::enableDisplay(bool on){
	if (on){
		LL_GPIO_ResetOutputPin(DIS_BLK_GPIO_Port, DIS_BLK_Pin);
	}
	else{
		LL_GPIO_SetOutputPin(DIS_BLK_GPIO_Port, DIS_BLK_Pin);
	}
}


