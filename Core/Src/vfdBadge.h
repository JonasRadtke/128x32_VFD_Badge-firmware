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
#include "files.h"
#include "buttonDebounce.h"

#define VersionNumberVFD ("0.0.3")

class vfdBadge {
private:
	bq25619 charger = bq25619(0x6A);
	frameBuffer vRam;
	uint32_t charging = 0;
	uint32_t stateOfCharge = 0;
	uint32_t powerSupplyConnected = 0;

	crapScheduler batteryStatusTask;
	crapScheduler framebufferTask;
	crapScheduler playTime;


	uint32_t displayMode = 1;
	uint32_t scroll = 0;

	uint32_t running = 1;
	uint32_t sleepingDelay = 0;
	uint32_t wakeUpButton = 0;

	buttonDebounce userbutton = buttonDebounce(USER_BT_GPIO_Port, USER_BT_Pin);

	uint32_t numberOfBMPFolders = 0;

	uint32_t actualBMPFolder = 0;
	uint32_t bmpFrameTime = 100;
	uint32_t bmpAnimationTimeMS = 100;
	uint32_t bmpFrameNumber = 1;

	uint32_t animationTimeMS = 100;

	uint32_t lastFrame = 0;
	uint32_t lastMode = 0;

public:
	vfdBadge();
	void init();
	void run();
	void setChargerConfig();
	void getStatus();
	void enableDisplayVoltage(bool on);
	void enableFilamentVoltage(bool on);

	void goingToSleep();
	void wakeUp();

	uint32_t loadNextBmpFolder();
	void loadBmpConfig(uint32_t folderNumber);
};

#endif /* SRC_VFDBADGE_H_ */
