/*
 * twi.c
 *
 *  Created on: Feb 9, 2025
 *      Author: Sentry
 */

#include "main.h"
#include "twi.h"
#include <stdint.h>

extern I2C_HandleTypeDef hi2c1;

void twiTransmit(uint8_t adress, uint8_t command ,uint8_t *pData, uint16_t size){

	HAL_I2C_Master_Transmit(&hi2c1, (adress << 1), pData, size, 100);
	return;
}

void twiRead(uint8_t adress, uint8_t command ,uint8_t *pData, uint16_t size){
	uint8_t icRegister = command;
	uint8_t *registerPtr = &icRegister;
	HAL_I2C_Master_Transmit(&hi2c1, (adress << 1), registerPtr, 1, 100);
	HAL_I2C_Master_Receive(&hi2c1, (adress << 1), pData, size, 100);
	return;
}
