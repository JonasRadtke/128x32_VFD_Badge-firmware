/*
 * pictueModes.cpp
 *
 *  Created on: May 22, 2025
 *      Author: Sentry
 */

#include "frameBuffer.h"
#include "crapScheduler.h"
#include "MN12832L.h"
#include "files.h"

extern MN12832L display;

void drawImageFromSd(frameBuffer *v,uint32_t folderNumber, uint32_t frameNumber){
	static uint32_t pictureIndex = 0;
	loadImage(v, folderNumber, pictureIndex);
	pictureIndex++;
	if (pictureIndex > frameNumber - 1) {
		pictureIndex = 0;
	}
    v->frameBufferToOutBuffer(display.outBuffer, display.outBuffer2);
}



void drawJurassic(frameBuffer *v){
	static uint32_t vis = 0;
	v->clearFrameBuffer();
	if(vis){
		v->drawString("System Ready ~", 3, 24);
		vis = 0;
	}
	else{
		v->drawString("System Ready", 3, 24);
		vis = 1;
	}
	v->frameBufferToOutBuffer(display.outBuffer, display.outBuffer2);
}

constexpr int IMAGE_WIDTH = 568;
constexpr int IMAGE_HEIGHT = 32;
constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 32;

void drawAfd(frameBuffer *v){
	static uint32_t scroll = 0;
	for (int y = 0; y < SCREEN_HEIGHT; ++y) {
		uint32_t y128 = y*128;
		for (int x = 0; x < SCREEN_WIDTH; ++x) {
			int image_x = scroll + x;
			int image_y = 0 + y;

			if (image_x >= IMAGE_WIDTH || image_y >= IMAGE_HEIGHT) continue;

			int bit_index = image_y * IMAGE_WIDTH + image_x;
			int byte_index = bit_index / 8;
			int bit_offset = 7 - (bit_index % 8); // MSB first

			uint32_t bit = (afd[byte_index] >> bit_offset) & 1;

			if(bit){
				v->drawPixelinVramArray(y128+x, 0);
			}
			else{
				v->drawPixelinVramArray(y128+x, 3);
			}
		}
	}

	v->frameBufferToOutBuffer(display.outBuffer, display.outBuffer2);
	scroll+=2;
	if (scroll > 439) {
		scroll = 0;
	}
}


