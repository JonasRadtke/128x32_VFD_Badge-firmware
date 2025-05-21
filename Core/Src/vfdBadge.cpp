/*
 * vfdBadge.cpp
 *
 *  Created on: May 16, 2025
 *      Author: Sentry
 */

#include "vfdBadge.h"


constexpr int IMAGE_WIDTH = 568;
constexpr int IMAGE_HEIGHT = 32;
constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 32;

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

	uint32_t zeit1 = 0;
	uint32_t zeit2 = 0;

	switch(this->displayMode){
		case 1:
			if(displayRefreshTask.task(HAL_GetTick(), 80)){
				zeit1 = HAL_GetTick();
				loadImage(&this->vRam, 0 ,bmpI);
				bmpI++;
				if (bmpI > 9) {
				bmpI = 1;
				}

			    vRam.frameBufferToOutBuffer(display.outBuffer, display.outBuffer2);
			    zeit2 = HAL_GetTick() - zeit1;
			    zeit2 = zeit2+1;

			}
			break;
		case 2:
			if(framebufferTask.task(HAL_GetTick(), 300)){
				static uint32_t vis = 0;
				vRam.clearFrameBuffer(3);
				if(vis){
					vRam.drawString("System Ready ~", 3, 24);
					vis = 0;
				}
				else{
					vRam.drawString("System Ready", 3, 24);
					vis = 1;
				}
				vRam.frameBufferToOutBuffer(display.outBuffer, display.outBuffer2);
			}
			break;
		case 3:
			if(framebufferTask.task(HAL_GetTick(), 300)){
				vRam.clearFrameBuffer(3);
					vRam.drawString("Charging...", 3, 3);

				vRam.draw_rect(3, 19, 122, 10, 0);
				vRam.fill_rect(4, 20, 120, 8, 2);

			    vRam.frameBufferToOutBuffer(display.outBuffer, display.outBuffer2);
			}
			break;
		case 4:
			if(displayRefreshTask.task(HAL_GetTick(), 50)){
				zeit1 = HAL_GetTick();
				loadImage(&this->vRam, 2 ,bmpI2);
				bmpI2++;
				if (bmpI2 > 1) {
				bmpI2 = 0;
				}

			    vRam.frameBufferToOutBuffer(display.outBuffer, display.outBuffer2);
			    zeit2 = HAL_GetTick() - zeit1;
			    zeit2 = zeit2+1;

			}
			break;
		case 5:
			if(displayRefreshTask.task(HAL_GetTick(), 25)){
			    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
			        for (int x = 0; x < SCREEN_WIDTH; ++x) {
			            int image_x = scroll + x;
			            int image_y = 0 + y;

			            if (image_x >= IMAGE_WIDTH || image_y >= IMAGE_HEIGHT) continue;

			            int bit_index = image_y * IMAGE_WIDTH + image_x;
			            int byte_index = bit_index / 8;
			            int bit_offset = 7 - (bit_index % 8); // MSB first

			            uint8_t bit = (afd[byte_index] >> bit_offset) & 1;

			            if(bit){
				            vRam.drawPixelinVram(x, y, 0);
			            }
			            else{
			            	vRam.drawPixelinVram(x, y, 3);
			            }
			        }
			    }
			    vRam.frameBufferToOutBuffer(display.outBuffer, display.outBuffer2);

				scroll+=2;
				if (scroll > 439) {
					scroll = 0;
				}
			}
			break;
		default: this->displayMode = 1;
			break;
	}








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

