
#ifndef SRC_FRAMEBUFFER_H_
#define SRC_FRAMEBUFFER_H_

#include <array>
#include <string>
#include "fonts.h"

class frameBuffer {
private:

public:
	uint8_t buffer[4096] = {};
	frameBuffer();
	void clearFrameBuffer(void);
	void fillBufferWithColor(uint8_t color);
	void frameBufferToOutBuffer(uint8_t outSin1Ptr[], uint8_t outSin2Ptr[]);
	uint8_t getPixelFromVram(uint32_t x, uint32_t y);

	void drawPixelinVram(uint32_t x, uint32_t y, uint8_t color);
	void drawPixelinVramArray(uint32_t x, uint8_t color);
	void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t color);
	void fill_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t color);

	uint32_t draw_char(uint32_t x, uint32_t y, const uint8_t font[]);
	void drawString(std::string text, uint32_t x, uint32_t y);


};

#endif /* SRC_FRAMEBUFFER_H_ */
