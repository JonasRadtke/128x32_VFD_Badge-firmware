/*
 * crapScheduler.cpp
 *
 *  Created on: Apr 29, 2025
 *      Author: Sentry
 */

#include "crapScheduler.h"


crapScheduler::crapScheduler() {
}

uint32_t crapScheduler::task(uint32_t time, uint32_t taskTime){
	if(this->lastCall + taskTime < time){
		this->lastCall = time;
		return 1;
	}
	return 0;
}

