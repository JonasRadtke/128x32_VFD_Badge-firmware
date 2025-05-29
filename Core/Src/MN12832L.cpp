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
this->newSegment();
}

void MN12832L::newSegment(){
	uint32_t gridLen = 44;
	uint32_t grid = 0;
	while(gridLen--){
		uint32_t datlen = 30;
		GPIOB->BRR = DIS_BLK_Pin;
		uint32_t i = grid*30;
		while (datlen--) {
			while(!LL_SPI_IsActiveFlag_TXE(SPI2));
			LL_SPI_TransmitData8(SPI2, this->outBuffer2[i++] );
			if(datlen == 1){
				GPIOB->BSRR = DIS_GCP_Pin;
				GPIOB->BRR = DIS_GCP_Pin;
			}
		}
		datlen = 30;
		i = grid*30;
		while (datlen--) {
			while(!LL_SPI_IsActiveFlag_TXE(SPI2));
			LL_SPI_TransmitData8(SPI2, this->outBuffer[i++] );
			if(datlen == 5){
				GPIOB->BSRR = DIS_GCP_Pin;
				GPIOB->BRR = DIS_GCP_Pin;
			}
		}
		while((SPI2->SR & 0x0080));
		GPIOB->BSRR = DIS_BLK_Pin;
		// Lat
		GPIOB->BSRR = DIS_LAT_Pin;
		GPIOB->BRR = DIS_LAT_Pin;

		grid++;
	}







}

void MN12832L::fillGridBytes(){
	for(uint32_t grid = 0; grid < 44; grid++){
		this->outBuffer[grid*30+(24 + grid/8)] |= (1 << (7-grid%8));
		this->outBuffer[grid*30+(24 + (grid+1)/8)] |= (1 << (7-((grid+1)%8)));
		this->outBuffer2[grid*30+(24 + grid/8)] |= (1 << (7-grid%8));
		this->outBuffer2[grid*30+(24 + (grid+1)/8)] |= (1 << (7-((grid+1)%8)));
	}
}

void MN12832L::clearBuffer(){
	for(uint32_t i = 0; i<1320; i++){
			this->outBuffer[i] = 0;
			this->outBuffer2[i] = 0;
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


