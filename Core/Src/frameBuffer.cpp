#include "frameBuffer.h"


frameBuffer::frameBuffer() {
	
}

uint8_t frameBuffer::getPixelFromVram(uint32_t x, uint32_t y) {
	return this->buffer[y * 128 + x];
}

const uint32_t evenGridBits[12] = {0x800000, 0x200000, 0x80000, 0x20000, 0x8000, 0x2000, 0x800, 0x200, 0x80, 0x20, 0x8, 0x2};
const uint32_t oddGridBits[12]  = {0x40000, 0x100000, 0x400000, 0x1000, 0x4000, 0x10000, 0x40, 0x100, 0x400, 0x1, 0x4, 0x10};

void frameBuffer::frameBufferToOutBuffer(uint8_t outSin1Ptr[], uint8_t outSin2Ptr[]) {

	// Es gibt 44 Grids, mit je 3 Pixel pro Reihe, insgesamt 32 Reihen
	for (uint32_t grid = 0; grid < 43; grid++) {

		uint32_t gridEven = grid % 2;
		uint32_t gridX3	= grid * 3;
		uint32_t gridX30 = gridX3 * 10;

		uint32_t tempbyteSin1[8] = { 0 };
		uint32_t tempbyteSin2[8] = { 0 };

		uint32_t numberPixel = 0;
		if(grid == 42){
			numberPixel = 2;
		}
		else{
			numberPixel = 3;
		}

		for (uint32_t y = 0; y < 32; y++) {
			
			uint32_t yMod4X3 = (y % 4) * 3;
			uint32_t yDiv4 = y / 4;
			uint32_t y128 = y*128;

			for (uint32_t x = 0; x < numberPixel; x++) {

				uint32_t xx = gridX3 + x;
				uint32_t index = yMod4X3 + x;

				// 100% S1 und S2, 66% S2, 33% S1, 0% none
				if (!gridEven) {
					switch (this->buffer[xx + y128]){
					case 0: tempbyteSin1[yDiv4] |= evenGridBits[index]; tempbyteSin2[yDiv4] |= evenGridBits[index]; break; // 100%
					case 1: tempbyteSin2[yDiv4] |= evenGridBits[index]; break; // 66%
					case 2: tempbyteSin1[yDiv4] |= evenGridBits[index]; break; // 33%
					case 3:  break; // Off
					default: ; break;
					}
				}
				else{
					switch (this->buffer[xx + y128]){
					case 0: tempbyteSin1[yDiv4] |= oddGridBits[index]; tempbyteSin2[yDiv4] |= oddGridBits[index];  break; // 100%
					case 1: tempbyteSin2[yDiv4] |= oddGridBits[index]; break; // 66%
					case 2: tempbyteSin1[yDiv4] |= oddGridBits[index]; break; // 33%
					case 3:  break; // Off
					default: ; break;
					}
				}


			}
		
		}


		// Ziel ist outSin1Ptr
		for (uint32_t bytes = 0; bytes < 24; bytes+=3) {
			uint32_t tempValSin1 = tempbyteSin1[bytes/3];
			uint32_t tempValSin2 = tempbyteSin2[bytes/3];
			outSin1Ptr[gridX30 +bytes] = (tempValSin1 >> 16) & 0xFF;
			outSin1Ptr[gridX30 +bytes + 1] = (tempValSin1 >> 8) & 0xFF;
			outSin1Ptr[gridX30 +bytes + 2] = tempValSin1 & 0xFF;

			outSin2Ptr[gridX30 +bytes] = (tempValSin2 >> 16) & 0xFF;
			outSin2Ptr[gridX30 +bytes + 1] = (tempValSin2 >> 8) & 0xFF;
			outSin2Ptr[gridX30 +bytes + 2] = tempValSin2 & 0xFF;
		}

	}

}

void frameBuffer::drawPixelinVram(uint32_t x, uint32_t y, uint8_t color) {
	this->buffer[y*128+x] = color;
}

void frameBuffer::drawPixelinVramArray(uint32_t x, uint8_t color) {
	this->buffer[x] = color;
}

void frameBuffer::clearFrameBuffer() {
	uint32_t x = 4096;
	while(x){
		this->buffer[--x] = 3;
	}
}

void frameBuffer::fillBufferWithColor(uint8_t color) {
	for (uint32_t x = 0; x < 4128; x++) {
			this->buffer[x] = color;
	}
}

uint32_t frameBuffer::draw_char(uint32_t x, uint32_t y, const uint8_t font[]) {
	uint8_t width = font[0];  // Erstes Byte = Zeichenbreite
	for (uint8_t row = 0; row < 6; row++) {
		uint8_t line = font[row + 1];  // Pixelzeilen ab Index 1
		for (uint8_t col = 0; col < width; col++) {
			if (line & (1 << (width - 1 - col))) {  // Bit pr�fen
				if ((x + col) < 128 && (y + row) < 32) {
					this->buffer[(x + col) + (y + row) * 128 ] = 0;
				}
			}
		}
	}
	return width;  // Breite zur�ckgeben
}

void frameBuffer::drawString(std::string text, uint32_t x, uint32_t y) {
	for (char c : text) {
		if (x > 128 - 3) break;
		if (y > 32 - 6) break;
		int ascii = static_cast<unsigned char>(c);
		x = x + 1 + draw_char(x, y, font3x5[ascii - 32]);
	}
}

void frameBuffer::draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t color) {
	for (uint32_t i = 0; i < width; i++) {
		if (x + i < 128 && y < 32) this->buffer[x + i + (y * 128)] = color;                 // obere Kante
		if (x + i < 128 && y + height - 1 < 32) this->buffer[x + i + (y + height - 1)*128] = color; // untere Kante
	}
	for (uint32_t j = 0; j < height; j++) {
		if (x < 128 && y + j < 32) this->buffer[x + (y + j) * 128] = color;                 // linke Kante
		if (x + width - 1 < 128 && y + j < 32) this->buffer[x + width - 1 + (y + j)*128] = color;   // rechte Kante
	}
}

void frameBuffer::fill_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t color) {
	for (uint32_t j = 0; j < height; j++) {
		for (uint32_t i = 0; i < width; i++) {
			if ((x + i) < 128 && (y + j) < 32) {
				this->buffer[x + i + (y + j)*128] = color;
			}
		}
	}
}

