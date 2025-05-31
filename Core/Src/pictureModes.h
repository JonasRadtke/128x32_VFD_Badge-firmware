/*
 * pictureModes.h
 *
 *  Created on: May 22, 2025
 *      Author: Sentry
 */

#ifndef SRC_PICTUREMODES_H_
#define SRC_PICTUREMODES_H_

uint32_t drawImageFromSd(frameBuffer *v,uint32_t folderNumber, uint32_t frameNumber);
uint32_t drawJurassic(frameBuffer *v);
uint32_t drawAfd(frameBuffer *v);
uint32_t drawVersionInfo(frameBuffer *v);

#endif /* SRC_PICTUREMODES_H_ */
