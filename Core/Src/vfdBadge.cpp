/*
 * vfdBadge.cpp
 *
 *  Created on: May 16, 2025
 *      Author: Sentry
 */

#include "vfdBadge.h"
#include "pictureModes.h"
#include "iniParser.h"




	extern TIM_HandleTypeDef htim6;
	extern TIM_HandleTypeDef htim7;
	MN12832L display;

	volatile uint32_t wakeUpRequest;
	volatile uint32_t sleeping;
	volatile uint32_t test;

vfdBadge::vfdBadge() {
	this->enableFilamentVoltage(false);
	this->enableDisplayVoltage(false);
	initSDCard();
	iniParser iniData;
	test = iniData.parseINI("/");
    std::string strValue = iniData.iniData["bmpconfig"]["folders"];
    this->numberOfBMPFolders = std::stoul(strValue); // Umwandlung in uint32_t

    loadBmpConfig(this->actualBMPFolder);

}

void vfdBadge::init(){
	this->setChargerConfig();
	this->charger.ResetWatchDog();
		vRam.clearFrameBuffer();
		display.init();
		initSDCard();

		this->charger.activateBoostMode(true);
		this->enableFilamentVoltage(true);
		this->enableDisplayVoltage(true);
		HAL_Delay(500);
		HAL_TIM_Base_Start_IT(&htim6);
		drawVersionInfo(&vRam);
		HAL_Delay(2000);
}

volatile uint32_t zeit1;
volatile uint32_t zeit4;

void vfdBadge::run(){

	userbutton.debounce();


	if(!sleeping && !running && userbutton.isLongOneCycle()){
		this->running = 1;
		this->enableFilamentVoltage(true);
		this->enableDisplayVoltage(true);
		HAL_TIM_Base_Start_IT(&htim6);
	}
	else if(userbutton.isLongOneCycle() && this->running){
		this->running = 0;
		HAL_TIM_Base_Stop_IT(&htim6);
		this->enableFilamentVoltage(false);
		this->enableDisplayVoltage(false);
	}
	if(sleeping && userbutton.directState()){
		wakeUpRequest = 1;
	}


	displayMode = 2;
	if(this->running){
		switch(this->displayMode){
			case 1:
				if(framebufferTask.task(HAL_GetTick(), this->bmpFrameTime)){
					this->animationTimeMS = this->bmpAnimationTimeMS;
					zeit1 = HAL_GetTick();
					this->lastFrame = drawImageFromSd(&vRam, this->actualBMPFolder, this->bmpFrameNumber);
					zeit4 = HAL_GetTick() - zeit1;
					zeit4 = zeit4++;
				}
				break;
			case 2:
				if(framebufferTask.task(HAL_GetTick(), 20)){
					this->animationTimeMS = 10000;
					this->lastFrame = drawAfd(&vRam);
				}
				break;
			case 3:
				if(framebufferTask.task(HAL_GetTick(), 300)){
					this->animationTimeMS = 10000;
					this->lastFrame = drawJurassic(&vRam);
				}
				break;
			default: this->displayMode = 1;
				break;
		}


		if(lastFrame || userbutton.isShort()){
			if(this->playTime.task(HAL_GetTick(), this->animationTimeMS) || userbutton.isShort()){

				if(this->displayMode == 1){
					if(this->loadNextBmpFolder()){
						this->displayMode++;
					}
				}
				else{
					this->displayMode++;
					if (this->displayMode > 3) {
						this->displayMode = 1;
					}
				}
			}
		}
	}





	// Sleep and Wake Up
	if(!this->charging && !wakeUpRequest && !this->running){
		sleeping = 1;
		this->charger.disableCharging();
		this->goingToSleep();

	}
	if(wakeUpRequest){
		wakeUpRequest = 0;
		sleeping = 0;
		this->running = 1;
		this->wakeUp();
	}

	// Get Status and Reset Watchdog
	if(batteryStatusTask.task(HAL_GetTick(), 1000)){
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

void vfdBadge::goingToSleep() {
	wakeUpRequest = 0;
	sleeping = 1;
	this->running = 0;
	HAL_TIM_Base_Stop_IT(&htim6);
	HAL_SuspendTick();
	HAL_TIM_Base_Start_IT(&htim7);
	this->enableFilamentVoltage(false);
	this->enableDisplayVoltage(false);
	closeSDCard();
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void vfdBadge::wakeUp() {
	HAL_TIM_Base_Stop_IT(&htim7);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_ResumeTick();
	this->init();
	this->wakeUpButton = 1;
}

uint32_t vfdBadge::loadNextBmpFolder(){
	this->actualBMPFolder++;
	if(this->actualBMPFolder > this->numberOfBMPFolders - 1){
		this->actualBMPFolder = 0;
	}

	loadBmpConfig(this->actualBMPFolder);

    if(this->actualBMPFolder == 0){
    	return 1;
    }
    return 0;
}

void vfdBadge::loadBmpConfig(uint32_t folderNumber){
	iniParser iniData;
	iniData.parseINI("/" + std::to_string(folderNumber) + "/");
    std::string strValue = iniData.iniData["config"]["frameTimeMs"];
    this->bmpFrameTime = std::stoul(strValue);
    strValue = iniData.iniData["config"]["numberOfFrames"];
    this->bmpFrameNumber = std::stoul(strValue);
    strValue = iniData.iniData["config"]["animationTimeMS"];
    this->bmpAnimationTimeMS = std::stoul(strValue);
}

