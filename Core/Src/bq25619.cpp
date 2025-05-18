/*
 * bg25619.cpp
 *
 *  Created on: Feb 9, 2025
 *      Author: Sentry
 */

#include "main.h"
#include "twi.h"
#include <stdint.h>
#include "bq25619.h"

#define REG_INPUT_CURRENT_LIMIT		0x00
#define REG_CHARGER_CTRL_0			0x01
#define REG_CHARGE_CURRENT_LIMIT	0x02
#define REG_PRE_TERM_CURRENT_LIMIT	0x03
#define REG_BAT_VOLTAGE_LIMIT		0x04
#define REG_CHARGER_CTRL_1			0x05
#define REG_CHARGER_CTRL_2			0x06
#define REG_CHARGER_CTRL_3			0x07
#define REG_CHARGER_STATUS_1		0x08
#define REG_CHARGER_STATUS_2		0x09
#define REG_CHARGER_STATUS_3		0x0A
#define PART_INFO					0x0B
#define REG_CHARGER_CTRL_4			0x0C

bq25619::bq25619(uint8_t adress) {
	this->twiAdress = adress;
}

void bq25619::enableCharging(){
	LL_GPIO_ResetOutputPin(CHARGER_CHARGE_EN_GPIO_Port, CHARGER_CHARGE_EN_Pin);
	return;
}
void bq25619::disableCharging(){
	LL_GPIO_SetOutputPin(CHARGER_CHARGE_EN_GPIO_Port, CHARGER_CHARGE_EN_Pin);
	return;
}
void bq25619::inputPowerSel500(){
	LL_GPIO_SetOutputPin(CHARGER_POWER_SEL_GPIO_Port, CHARGER_POWER_SEL_Pin);
	return;
}
void bq25619::inputPowerSel2400(){
	LL_GPIO_ResetOutputPin(CHARGER_POWER_SEL_GPIO_Port, CHARGER_POWER_SEL_Pin);
	return;
}

// Reg 00 Input Current Limit
void bq25619::setHizMode(uint32_t enable){
	if(enable){
		this->readWriteRegister(REG_INPUT_CURRENT_LIMIT, 0x80, 0x80);
	}
	else{
		this->readWriteRegister(REG_INPUT_CURRENT_LIMIT, 0x80, 0x00);
	}
	return;
}

void bq25619::tsIgnore(uint32_t tsIgnore){
	if(tsIgnore){
		this->readWriteRegister(REG_INPUT_CURRENT_LIMIT, 0x40, 0x40);
	}
	else{
		this->readWriteRegister(REG_INPUT_CURRENT_LIMIT, 0x40, 0x00);
	}
	return;
}

void bq25619::batsnsDisable(uint32_t disable){
	if(disable){
		this->readWriteRegister(REG_INPUT_CURRENT_LIMIT, 0x20, 0x20);
	}
	else{
		this->readWriteRegister(REG_INPUT_CURRENT_LIMIT, 0x20, 0x00);
	}
	return;
}

void bq25619::setInputCurrentLimit(uint32_t current){
	uint32_t x = current;

	if (current < 100) x = 100;
	if (current > 3200) x = 3200;

    x = (x / 100) * 100;
    uint8_t value = ((uint8_t)((current - 100) * 31 / (3200 - 100))) & 0x1F;

    this->readWriteRegister(REG_INPUT_CURRENT_LIMIT, 0x1F, value);
	return;
}

// Reg 01 Charge Control 0
void bq25619::activateBoostMode(uint32_t enable){
	if(enable){
		this->readWriteRegister(REG_CHARGER_CTRL_0, 0x20, 0x20);
	}
	else{
		this->readWriteRegister(REG_CHARGER_CTRL_0, 0x20, 0x00);
	}
	return;
}

void bq25619::setSystemMinVoltage(bq25619_system_min_voltage_t voltage){
	this->readWriteRegister(REG_CHARGER_CTRL_0, 0x0E, ((uint8_t)(voltage)) << 1);
	return;
}

void bq25619::ResetWatchDog(){
	this->readWriteRegister(REG_CHARGER_CTRL_0, 0x40, 0x40);
	return;
}

void bq25619::chargeBitEnable(uint32_t enable){
	if(enable){
		this->readWriteRegister(REG_CHARGER_CTRL_0, 0x10, 0x10);
	}
	else{
		this->readWriteRegister(REG_CHARGER_CTRL_0, 0x10, 0x10);
	}
	return;
}


// Reg 02 Charge Current Limit
void bq25619::setChargeCurrentLimit(uint32_t current){
	uint8_t value = 0;
	if(current < 1290) value = (uint8_t)(current / 20);
	else if((current >= 1290) && (current < 1360)) value = 0x3C;
	else if((current >= 1360) && (current < 1430)) value = 0x3D;
	else if((current >= 1430) && (current < 1500)) value = 0x3E;
	else if(current >= 1500) value = 0x3F;
	this->writeRegister(REG_CHARGE_CURRENT_LIMIT, value);
	return;
}


// Reg 03 Pre Charge / Term Current Limit
void bq25619::setPrechargeLimit(uint32_t current){
	uint8_t value = 0;
	if (current > 260) current = 260;

	value = (uint8_t)((current-20) /20);

	this->readWriteRegister(REG_PRE_TERM_CURRENT_LIMIT, 0xF0, (value << 4));
	return;
}

void bq25619::setTerminationLimit(uint32_t current){
	uint8_t value = 0;
	if (current > 260) current = 260;

	value = (uint8_t)((current-20) /20);

	this->readWriteRegister(REG_PRE_TERM_CURRENT_LIMIT, 0x0F, value);
	return;
}


// Reg 04
void bq25619::setTopOffTimer(bq25619_topoff_timer_t timer){
	this->readWriteRegister(REG_BAT_VOLTAGE_LIMIT, 0x06, ((uint8_t)(timer)) << 1);
	return;
}

void bq25619::setRecharcheThre(bq25619_recharge_thre_t voltage){
	this->readWriteRegister(REG_BAT_VOLTAGE_LIMIT, 0x01, (uint8_t)(voltage));
	return;
}


// Reg 05
void bq25619::setWachtdogTimer(bq25619_watchdog_timer_t timer){
	this->readWriteRegister(REG_CHARGER_CTRL_1, 0x30, (uint8_t)(timer << 4));
	return;
}


//Reg 06
void bq25619::setBoostVoltage(bq25619_boost_voltage_t voltage){
	this->readWriteRegister(REG_CHARGER_CTRL_2, 0x30, (uint8_t)(voltage << 4));
	return;
}


void bq25619::readChargerStatus(){
	this->statusReg1 = this->readRegister(REG_CHARGER_STATUS_1);
	this->charge_status = (bq25619_chrg_stat_t)((this->statusReg1 >> 3) & 0x03);
	this->vbus_status = (bq25619_vbus_stat_t)((this->statusReg1 >> 5) & 0x07);
	this->statusReg2 = this->readRegister(REG_CHARGER_STATUS_2);
	this->statusReg3 = this->readRegister(REG_CHARGER_STATUS_3);
	return;
}

uint32_t bq25619::getCharging(){
	uint8_t status = this->readRegister(REG_CHARGER_STATUS_1);
	this->charge_status = (bq25619_chrg_stat_t)((status >> 3) & 0x03);
	this->vbus_status = (bq25619_vbus_stat_t)((status >> 5) & 0x07);
	if ((this->charge_status == BQ25619_CHRG_STAT_PRE_CHARGE) || (this->charge_status == BQ25619_CHRG_STAT_FAST_CHARGING)){
		return 1;
	}
	return 0;
}

uint32_t bq25619::getCharingAndPower(){
	uint8_t status = this->readRegister(REG_CHARGER_STATUS_1);
	this->charge_status = (bq25619_chrg_stat_t)((status >> 3) & 0x03);
	this->vbus_status = (bq25619_vbus_stat_t)((status >> 5) & 0x07);
	if ((this->charge_status == BQ25619_CHRG_STAT_PRE_CHARGE) || (this->charge_status == BQ25619_CHRG_STAT_FAST_CHARGING)){
		return 1;
	}
	if((this->vbus_status == BQ25619_VBUS_STAT_PSEL_2400MA) || (this->vbus_status == BQ25619_VBUS_STAT_PSEL_500MA)){
		return 2;
	}
	return 0;
}

uint32_t bq25619::getPowerSupply(){
	uint8_t status = this->readRegister(REG_CHARGER_STATUS_1);
	this->charge_status = (bq25619_chrg_stat_t)((status >> 3) & 0x03);
	this->vbus_status = (bq25619_vbus_stat_t)((status >> 5) & 0x07);
	if((this->vbus_status == BQ25619_VBUS_STAT_PSEL_2400MA) || (this->vbus_status == BQ25619_VBUS_STAT_PSEL_500MA)){
		return 1;
	}
	else{
		return 0;
	}
	return 0;
}

uint32_t bq25619::getPowerGood(){
	uint8_t status = this->readRegister(REG_CHARGER_STATUS_1);
	return 	(status >> 2) & 0x02;
}


void bq25619::writeRegister(uint8_t command ,uint8_t data){
	uint8_t x[2];
	x[0] = command;
	x[1] = data;
	uint8_t *dataPtr = x;
	HAL_I2C_Master_Transmit(&hi2c1, (this->twiAdress << 1), dataPtr, 2, 100);
	return;
}

uint8_t bq25619::readRegister(uint8_t command){
	uint8_t *commandPtr = &command;
	uint8_t data;
	uint8_t *dataPtr = &data;
	HAL_I2C_Master_Transmit(&hi2c1, (this->twiAdress << 1), commandPtr, 1, 100);
	HAL_I2C_Master_Receive(&hi2c1, (this->twiAdress << 1), dataPtr, 1, 100);
	return data;
}

void bq25619::readWriteRegister(uint8_t command ,uint8_t mask, uint8_t data){
	uint8_t regValue = this->readRegister(command);
	regValue &= ~mask;
	regValue |= data;
	this->writeRegister(command, regValue);
	return;
}

