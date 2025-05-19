#pragma once
#include <stdint.h>

typedef struct px {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
}pixel;

#pragma pack(push, 2)

typedef struct palette_t {
	uint8_t rgbtBlue;
	uint8_t rgbtGreen;
	uint8_t rgbtRed;
	uint8_t rgbtAlpha;
} palette;

typedef struct bit4Palette_t{
	palette palette4[16];
} bit4Palette;

typedef struct tagBITMAPFILEHEADER {
	uint16_t  bfType;
	uint32_t bfSize;
	uint16_t  bfReserved1;
	uint16_t  bfReserved2;
	uint32_t bfOffBits;
} BITMAPFILEHEADER, * PBITMAPFILEHEADER;   // http://msdn.microsoft.com/en-us/library/dd183374%28en-us,VS.85%29.aspx

typedef struct tagBITMAPINFOHEADER {
	uint32_t biSize;
	int32_t  biWidth;
	int32_t  biHeight;
	uint16_t  biPlanes;
	uint16_t  biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t  biXPelsPerMeter;
	int32_t  biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BITMAPINFOHEADER, * PBITMAPINFOHEADER;   // http://msdn.microsoft.com/de-de/library/dd183376%28en-us,VS.85%29.aspx

typedef struct tagRGBTRIPLE {
	uint8_t rgbtBlue;
	uint8_t rgbtGreen;
	uint8_t rgbtRed;
} RGBTRIPLE;                         // http://msdn.microsoft.com/de-de/library/dd162939%28v=VS.85%29.aspx
#pragma pack(pop)


