/*
 * MN12832L.h
 *
 *  Created on: May 17, 2025
 *      Author: Sentry
 */

#ifndef SRC_MN12832L_H_
#define SRC_MN12832L_H_

#include "main.h"

class MN12832L {
private:
public:
	MN12832L();
	void init();

	uint8_t outBuffer[1320];
	uint8_t outBuffer2[1320];

	void enableDisplayVoltage(bool on);
	void enableFilamentVoltage(bool on);
	void enableDisplay(bool on);

	void newFrame();
	void newSegment();
	void fillGridBytes();
	void clearBuffer();

};

#endif /* SRC_MN12832L_H_ */
