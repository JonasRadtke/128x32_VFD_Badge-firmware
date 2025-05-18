/*
 * bg25619.h
 *
 *  Created on: Feb 9, 2025
 *      Author: Sentry
 */

#ifndef SRC_BQ25619_H_
#define SRC_BQ25619_H_

#include <stdint.h>
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

typedef enum bq25619_chrg_stat {
	BQ25619_CHRG_STAT_NOT_CHARGING	= 0,
	BQ25619_CHRG_STAT_PRE_CHARGE	= 1,
	BQ25619_CHRG_STAT_FAST_CHARGING	= 2,
	BQ25619_CHRG_STAT_CHARGE_TERM	= 3,
} bq25619_chrg_stat_t;

typedef enum bq25619_vbus_stat {
	BQ25619_VBUS_STAT_NO_INPUT		= 0,
	BQ25619_VBUS_STAT_PSEL_500MA	= 1,
	BQ25619_VBUS_STAT_PSEL_2400MA	= 3,
	BQ25619_VBUS_STAT_BOOST			= 7,
} bq25619_vbus_stat_t;

// Reg 01
typedef enum bq25619_system_min_voltage {
	BQ25619_SYSTEM_MIN_2_6V			= 0,
	BQ25619_SYSTEM_MIN_2_8V			= 1,
	BQ25619_SYSTEM_MIN_3_0V			= 2,
	BQ25619_SYSTEM_MIN_3_2V			= 3,
	BQ25619_SYSTEM_MIN_3_4V			= 4,
	BQ25619_SYSTEM_MIN_3_5V			= 5,
	BQ25619_SYSTEM_MIN_3_6V			= 6,
	BQ25619_SYSTEM_MIN_3_7V			= 7,
} bq25619_system_min_voltage_t;

// Reg04
typedef enum bq25619_topoff_timer {
	BQ25619_TOPOFF_TIMER_DISABLE	= 0,
	BQ25619_TOPOFF_TIMER_15MIN		= 1,
	BQ25619_TOPOFF_TIMER_30MIN		= 2,
	BQ25619_TOPOFF_TIMER_45MIN		= 3,
} bq25619_topoff_timer_t;

typedef enum bq25619_recharge_thre {
	BQ25619_VRECHARGE_120MV			= 0,
	BQ25619_VRECHARGE_210MV			= 1,
} bq25619_recharge_thre_t;

// Reg05
typedef enum bq25619_watchdog_timer {
	BQ25619_WATCHDOG_TIMER_DISABLE	= 0,
	BQ25619_WATCHDOG_TIMER_40S		= 1,
	BQ25619_WATCHDOG_TIMER_80S		= 2,
	BQ25619_WATCHDOG_TIMER_160S		= 3,
} bq25619_watchdog_timer_t;

// Reg 07
typedef enum bq25619_boost_voltage {
	BQ25619_BOOST_VOLTAGE_4_6V		= 0,
	BQ25619_BOOST_VOLTAGE_4_75V		= 1,
	BQ25619_BOOST_VOLTAGE_5_0V		= 2,
	BQ25619_BOOST_VOLTAGE_5_15V		= 3,
} bq25619_boost_voltage_t;

class bq25619 {
private:
	uint8_t twiAdress = 0x6A;
	uint32_t statusReg1 = 0;
	uint32_t statusReg2 = 0;
	uint32_t statusReg3 = 0;
	bq25619_chrg_stat_t charge_status = BQ25619_CHRG_STAT_NOT_CHARGING;
	bq25619_vbus_stat_t vbus_status = BQ25619_VBUS_STAT_NO_INPUT;
public:
	bq25619(uint8_t adress);

	void setHizMode(uint32_t enable); //G
	void tsIgnore(uint32_t tsIgnore); //G
	void batsnsDisable(uint32_t disable); //G
	void setInputCurrentLimit(uint32_t current); //G
	void activateBoostMode(uint32_t enable); //G
	void setSystemMinVoltage(bq25619_system_min_voltage_t voltage); //G
	void ResetWatchDog(); //G
	void setChargeCurrentLimit(uint32_t current); //G
	void setPrechargeLimit(uint32_t current); //G
	void setTerminationLimit(uint32_t current); //G
	void setTopOffTimer(bq25619_topoff_timer_t timer); //G
	void setRecharcheThre(bq25619_recharge_thre_t voltage); //G
	void setWachtdogTimer(bq25619_watchdog_timer_t timer); //G
	void setBoostVoltage(bq25619_boost_voltage_t voltage); //G
	void chargeBitEnable(uint32_t enable);

	void readChargerStatus();
	uint32_t getCharging();
	uint32_t getCharingAndPower();
	uint32_t getPowerGood();
	uint32_t getPowerSupply();

	void enableCharging();
	void disableCharging();
	void inputPowerSel500();
	void inputPowerSel2400();

	void readWriteRegister(uint8_t command ,uint8_t mask, uint8_t data);
	void writeRegister(uint8_t command ,uint8_t data);
	uint8_t readRegister(uint8_t command);
};

#endif /* SRC_BQ25619_H_ */
