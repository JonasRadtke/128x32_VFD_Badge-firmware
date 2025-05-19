#include "frameBuffer.h"


frameBuffer::frameBuffer() {
	
}

uint8_t frameBuffer::getPixelFromVram(uint32_t x, uint32_t y) {
	return this->buffer.at(x).at(y);
}

const uint32_t evenGridBits[12] = {0x800000, 0x200000, 0x80000, 0x20000, 0x8000, 0x2000, 0x800, 0x200, 0x80, 0x20, 0x8, 0x2};
const uint32_t oddGridBits[12]  = {0x40000, 0x100000, 0x400000, 0x1000, 0x4000, 0x10000, 0x40, 0x100, 0x400, 0x1, 0x4, 0x10};

void frameBuffer::frameBufferToOutBuffer(uint8_t outSin1Ptr[][30], uint8_t outSin2Ptr[][30]) {

	// Es gibt 44 Grids, mit je 3 Pixel pro Reihe, insgesamt 32 Reihen

	for (uint32_t grid = 0; grid < 43; grid++) {

		uint32_t gridEven = grid % 2;
		uint32_t gridX3	= grid * 3;

		uint32_t tempbyteSin1[8] = { 0 };
		uint32_t tempbyteSin2[8] = { 0 };

		for (uint32_t y = 0; y < 32; y++) {
			
			uint32_t yMod4X3 = (y % 4) * 3;
			uint32_t yDiv4 = y / 4;

			for (uint32_t x = 0; x < 3; x++) {

				uint32_t xx = gridX3 + x;
				uint32_t index = yMod4X3 + x;

				// 100% S1 und S2, 66% S2, 33% S1, 0% none
				if (!gridEven) {
					switch (this->buffer[xx][y]){
					case 0: tempbyteSin1[yDiv4] |= evenGridBits[index]; tempbyteSin2[yDiv4] |= evenGridBits[index]; break; // 100%
					case 1: tempbyteSin2[yDiv4] |= evenGridBits[index]; break; // 66%
					case 2: tempbyteSin1[yDiv4] |= evenGridBits[index]; break; // 33%
					case 3:  break; // Off
					default: ; break;
					}
				}
				else{
					switch (this->buffer[xx][y]){
					case 0: tempbyteSin1[yDiv4] |= oddGridBits[index]; tempbyteSin2[yDiv4] |= oddGridBits[index];  break; // 100%
					case 1: tempbyteSin2[yDiv4] |= oddGridBits[index]; break; // 66%
					case 2: tempbyteSin1[yDiv4] |= oddGridBits[index]; break; // 33%
					case 3:  break; // Off
					default: ; break;
					}
				}

	/*
				if (!this->buffer[xx][y]) {

					uint32_t index = yMod4X3 + x;
					if (!gridEven) {
						tempbyteSin1[yDiv4] |= evenGridBits[index];
					/*	switch ((((y%4)*3)) + x) {
						case 0: tempbyte[y / 4] |= (1 << 23); break;
						case 1: tempbyte[y / 4] |= (1 << 21); break;
						case 2: tempbyte[y / 4] |= (1 << 19); break;
						case 3: tempbyte[y / 4] |= (1 << 17); break;
						case 4: tempbyte[y / 4] |= (1 << 15); break;
						case 5: tempbyte[y / 4] |= (1 << 13); break;
						case 6: tempbyte[y / 4] |= (1 << 11); break;
						case 7: tempbyte[y / 4] |= (1 << 9); break;
						case 8: tempbyte[y / 4] |= (1 << 7); break;
						case 9: tempbyte[y / 4] |= (1 << 5); break;
						case 10: tempbyte[y / 4] |= (1 << 3); break;
						case 11: tempbyte[y / 4] |= (1 << 1); break;
						default: ; break;
						}
					}
					else {
						tempbyteSin1[yDiv4] |= oddGridBits[index];
						/*
						switch ((((y % 4) * 3)) + x) {
						case 0: tempbyte[y / 4] |= (1 << 18); break;
						case 1: tempbyte[y / 4] |= (1 << 20); break;
						case 2: tempbyte[y / 4] |= (1 << 22); break;
						case 3: tempbyte[y / 4] |= (1 << 12); break;
						case 4: tempbyte[y / 4] |= (1 << 14); break;
						case 5: tempbyte[y / 4] |= (1 << 16); break;
						case 6: tempbyte[y / 4] |= (1 << 6); break;
						case 7: tempbyte[y / 4] |= (1 << 8); break;
						case 8: tempbyte[y / 4] |= (1 << 10); break;
						case 9: tempbyte[y / 4] |= (1 << 0); break;
						case 10: tempbyte[y / 4] |= (1 << 2); break;
						case 11: tempbyte[y / 4] |= (1 << 4); break;
						default:; break;
						}
					}
				}
*/
			}
		
		}


		// Ziel ist outSin1Ptr
		for (uint32_t bytes = 0; bytes < 24; bytes+=3) {
			uint32_t tempValSin1 = tempbyteSin1[bytes/3];
			uint32_t tempValSin2 = tempbyteSin2[bytes/3];
			outSin1Ptr[grid][bytes] = (tempValSin1 >> 16) & 0xFF;
			outSin1Ptr[grid][bytes + 1] = (tempValSin1 >> 8) & 0xFF;
			outSin1Ptr[grid][bytes + 2] = tempValSin1 & 0xFF;

			outSin2Ptr[grid][bytes] = (tempValSin2 >> 16) & 0xFF;
			outSin2Ptr[grid][bytes + 1] = (tempValSin2 >> 8) & 0xFF;
			outSin2Ptr[grid][bytes + 2] = tempValSin2 & 0xFF;
		}

	}





}

void frameBuffer::drawPixelinVram(uint32_t x, uint32_t y, uint8_t color) {
	this->buffer[x][y] = color;
}

void frameBuffer::clearFrameBuffer(uint8_t color) {
	for (uint32_t x = 0; x < 129; x++) {
		for (uint32_t y = 0; y < 32; y++) {
			this->buffer[x][y] = color;
		}
	}
}


/*
void frameBuffer::drawPixelinVram(uint32_t x, uint32_t y, uint8_t color) {
	this->buffer.at(x).at(y) = color;
}

void frameBuffer::clearFrameBuffer(uint8_t color) {
	for (uint32_t x = 0; x < this->buffer.size(); x++) {
		for (uint32_t y = 0; y < this->buffer.at(x).size(); y++) {
			this->buffer.at(x).at(y) = color;
		}
	}
}
*/
uint32_t frameBuffer::draw_char(uint32_t x, uint32_t y, const uint8_t font[]) {
	uint8_t width = font[0];  // Erstes Byte = Zeichenbreite
	for (uint8_t row = 0; row < 6; row++) {
		uint8_t line = font[row + 1];  // Pixelzeilen ab Index 1
		for (uint8_t col = 0; col < width; col++) {
			if (line & (1 << (width - 1 - col))) {  // Bit pr�fen
				if ((x + col) < 128 && (y + row) < 32) {
				//	vram[x + col][y + row] = 0;
					this->buffer.at(x + col).at(y + row) = 0;
				}
			}
		}
	}
	return width;  // Breite zur�ckgeben
}

void frameBuffer::drawString(std::string text, uint32_t x, uint32_t y) {
	for (char c : text) {
		if (x > this->buffer.size() - 3) break;
		if (y > this->buffer.at(x).size() - 6) break;
		int ascii = static_cast<unsigned char>(c);
		x = x + 1 + draw_char(x, y, font3x5[ascii - 32]);
	}
}
