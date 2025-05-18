/*
 * crapScheduler.h
 *
 *  Created on: Apr 29, 2025
 *      Author: Sentry
 */

#ifndef SRC_CRAPSCHEDULER_H_
#define SRC_CRAPSCHEDULER_H_
#include <stdint.h>

class crapScheduler {
private:
	uint32_t lastCall = 0;
public:
	crapScheduler();
	uint32_t task(uint32_t time, uint32_t taskTime);
};

#endif /* SRC_CRAPSCHEDULER_H_ */
