#include "frameBuffer.h"


frameBuffer::frameBuffer() {
	
}

uint8_t frameBuffer::getPixelFromVram(uint32_t x, uint32_t y) {
	return this->buffer.at(x).at(y);
}

void frameBuffer::frameBufferToOutBuffer(uint8_t outPtr[][30]) {
	for (uint32_t grid = 0; grid < 43; grid++) {

		uint32_t tempbyte[8] = { 0 };

		for (uint32_t y = 0; y < 32; y++) {
			
			for (uint32_t x = 0; x < 3; x++) {

				uint32_t xx = (grid * 3) + x;
				if (!this->buffer[xx][y]) {


					if (!(grid % 2)) {
						switch ((((y%4)*3)) + x) {
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

			}
		
		}

		for (uint32_t bytes = 0; bytes < 24; bytes+=3) {
		//	this->outBuffer[grid][bytes] = (tempbyte[bytes/3] >> 16) & 0xFF;
		//	this->outBuffer[grid][bytes +1] = (tempbyte[bytes/3] >> 8) & 0xFF;
		//	this->outBuffer[grid][bytes +2] = tempbyte[bytes/3] & 0xFF;
			outPtr[grid][bytes] = (tempbyte[bytes/3] >> 16) & 0xFF;
			outPtr[grid][bytes +1] = (tempbyte[bytes/3] >> 8) & 0xFF;
			outPtr[grid][bytes +2] = tempbyte[bytes/3] & 0xFF;
		}

	}





}

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
