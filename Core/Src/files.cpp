#include "main.h"
#include "files.h"
#include <stdio.h>
#include <string>
#include "frameBuffer.h"
#include "FatFS/ff.h"
#include "FatFS/diskio.h"

char dirNames[100][13];
uint32_t numberOfDirs;

FILE* datei;

FATFS FatFs;				/* File system object for each logical drive */
FIL File;				/* File objects */
DIR dir;					/* Directory object */
FRESULT res;
//static FILINFO fno;

void initSDCard(){
	FRESULT res;
	res = f_mount(&FatFs, "", 0);
	if (res != FR_OK) return;
}

void closeSDCard(){
	FRESULT res;
	res = f_unmount("");
	if (res != FR_OK) return;
}


volatile uint32_t zeit3;

int loadImage(frameBuffer* vram, uint32_t dir, uint32_t bmpIndex){
	std::string path;
	uint32_t i;
	uint32_t j;
	FRESULT res;
	uint32_t zeit1 = 0;
	zeit1 = HAL_GetTick();
	// New Path for f_open (/**pathname**/**index**.bmp
	path = "/" + std::to_string(dir) + "/" + std::to_string(bmpIndex) + ".bmp";



	// Datei �ffnen
	res = f_open(&File, path.c_str(), FA_READ);
	if (res != FR_OK) return res;
	zeit3 = HAL_GetTick() - zeit1;
	zeit3 = zeit3+1;


	BITMAPFILEHEADER fileheader;				// Variable FileHeader
	BITMAPFILEHEADER* headptr = &fileheader;	// Pointer darauf

	BITMAPINFOHEADER infoheader;				// Variable Infoheader
	BITMAPINFOHEADER* infoptr = &infoheader;	// Pointer darauf
	UINT readBytes;

	bit4Palette colorPalette;
	bit4Palette* colorPalettePtr = &colorPalette;

	f_read(&File, headptr, sizeof(fileheader), &readBytes);		// Fileheader in Variable einlesen
	if (headptr->bfType == 19778)								// Ist es eine BMP?
	{
//		res = f_read(&File, infoptr, sizeof(infoheader), &readBytes);	// Infoheader in Variable einlesen
//		res = f_read(&File, colorPalettePtr, sizeof(colorPalette), &readBytes);	// Paletten Header
	}
	else
	{
		f_close( &File );											// Datei schliessen
		return 0;
	}

	zeit3 = HAL_GetTick() - zeit1;
	zeit3 = zeit3+1;


	uint8_t bmpdata[2048];

	j = 0;

	
	File.fptr = fileheader.bfOffBits;
	res = f_read(&File, &bmpdata[0], 2048, &readBytes);	// Infoheader in Variable einlesen
	if (res != FR_OK) return res;

	zeit3 = HAL_GetTick() - zeit1;
	zeit3 = zeit3+1;


	for (int32_t zeile = 32; zeile > 0; zeile--)
	{
		uint32_t zeile128 = (zeile - 1) * 128;
		for (i = 0; i < 128 - 1; i += 2)
		{
			vram->drawPixelinVramArray(i + zeile128, (bmpdata[j] >> 4));
			vram->drawPixelinVramArray(i + 1 + zeile128, bmpdata[j] & 0x0F);
			j++;
		}
	}

	zeit3 = HAL_GetTick() - zeit1;
	zeit3 = zeit3+1;

	res = f_close( &File );
	if (res != FR_OK) return res;




	return 1;

}

