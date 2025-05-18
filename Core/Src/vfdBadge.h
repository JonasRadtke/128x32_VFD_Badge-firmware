/*
 * vfdBadge.h
 *
 *  Created on: May 16, 2025
 *      Author: Sentry
 */

#ifndef SRC_VFDBADGE_H_
#define SRC_VFDBADGE_H_

#include "main.h"
#include "bq25619.h"
#include "crapScheduler.h"
#include "MN12832L.h"
#include "frameBuffer.h"


class vfdBadge {
private:
	bq25619 charger = bq25619(0x6A);
	frameBuffer vRam;
	uint32_t charging = 0;
	uint32_t stateOfCharge = 0;
	uint32_t powerSupplyConnected = 0;

	crapScheduler statusThread;
	crapScheduler displayRefreshTask;
	crapScheduler framebufferTask;
public:
	vfdBadge();
	void init();
	void run();
	void setChargerConfig();
	void getStatus();
	void enableDisplayVoltage(bool on);
	void enableFilamentVoltage(bool on);
};

#endif /* SRC_VFDBADGE_H_ */
