/*
 * twi.h
 *
 *  Created on: Feb 9, 2025
 *      Author: Sentry
 */

#ifndef SRC_TWI_H_
#define SRC_TWI_H_

void twiTransmit(uint8_t adress, uint8_t command ,uint8_t *pData, uint16_t size);
void twiRead(uint8_t adress, uint8_t command ,uint8_t *pData, uint16_t size);

#endif /* SRC_TWI_H_ */
