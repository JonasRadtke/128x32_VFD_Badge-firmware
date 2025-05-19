/*
 * vfdBadge.cpp
 *
 *  Created on: May 16, 2025
 *      Author: Sentry
 */

#include "vfdBadge.h"

	extern TIM_HandleTypeDef htim6;
	MN12832L display;

vfdBadge::vfdBadge() {
	this->enableFilamentVoltage(false);
	this->enableDisplayVoltage(false);
}

void vfdBadge::init(){
	this->setChargerConfig();
	this->charger.ResetWatchDog();
		vRam.clearFrameBuffer(3);
		display.init();
		initSDCard();

		this->charger.activateBoostMode(true);
		this->enableFilamentVoltage(true);
		this->enableDisplayVoltage(true);
		HAL_TIM_Base_Start_IT(&htim6);
}

void vfdBadge::run(){

	if(displayRefreshTask.task(HAL_GetTick(), 100)){
		loadImage(&this->vRam, bmpI);
		bmpI++;
		if (bmpI > 9) {
		bmpI = 1;
		}
	    uint8_t (*ptr)[30] = display.outBuffer;  // ptr zeigt auf ein Array mit 32 int-Elementen
	    vRam.frameBufferToOutBuffer(ptr);
	}

/*	if(framebufferTask.task(HAL_GetTick(), 1000)){

		vRam.drawString("Loading... Hello WORLD TEST 123!", 3, 3);
		vRam.drawString("Loading... Hello WORLD TEST 123!", 3, 10);
		vRam.drawString("Loading... Hello WORLD TEST 123!", 3, 17);
		vRam.drawString("Loading... Hello WORLD TEST 123!", 3, 24);

	    uint8_t (*ptr)[30] = display.outBuffer;  // ptr zeigt auf ein Array mit 32 int-Elementen
	    vRam.frameBufferToOutBuffer(ptr);

	}*/



	// Get Status and Reset Watchdog
	if(statusThread.task(HAL_GetTick(), 1000)){
		this->getStatus();
		this->charger.ResetWatchDog();
		if(this->powerSupplyConnected){
			this->charger.enableCharging();
		}
		else{
			this->charger.disableCharging();
		}
	}

}


void vfdBadge::setChargerConfig(){
	this->charger.inputPowerSel2400();
	this->charger.setInputCurrentLimit(2000);
	this->charger.setChargeCurrentLimit(1400);
	this->charger.setPrechargeLimit(40);
	this->charger.setTerminationLimit(60);
	this->charger.chargeBitEnable(1);
	this->charger.readWriteRegister(0x04, 0xF8, 0x40); // 4.2V
	return;
}

void vfdBadge::getStatus(){
	this->charging = this->charger.getCharingAndPower();
	this->charger.readChargerStatus();
	this->powerSupplyConnected = this->charger.getPowerSupply();
}

void vfdBadge::enableDisplayVoltage(bool on){
	if (on){
		LL_GPIO_SetOutputPin(DIS_V_ENABLE_GPIO_Port, DIS_V_ENABLE_Pin);
	}
	else{
		LL_GPIO_ResetOutputPin(DIS_V_ENABLE_GPIO_Port, DIS_V_ENABLE_Pin);
	}
}

void vfdBadge::enableFilamentVoltage(bool on){
	if (on){
		LL_GPIO_SetOutputPin(DIS_FILAMENT_V_EN_GPIO_Port, DIS_FILAMENT_V_EN_Pin);
	}
	else{
		LL_GPIO_ResetOutputPin(DIS_FILAMENT_V_EN_GPIO_Port, DIS_FILAMENT_V_EN_Pin);
	}
}
