/*
 * MN12832L.cpp
 *
 *  Created on: May 17, 2025
 *      Author: Sentry
 */

#include "MN12832L.h"

MN12832L::MN12832L() {
//	this->outBuffer[0][10] = 0x01;
/*
	this->outBuffer[0][9] = 0xa8;
	this->outBuffer[2][9] = 0xa8;
	this->outBuffer[4][9] = 0xa8;
	this->outBuffer[6][9] = 0xa8;
	this->outBuffer[8][9] = 0xa8;
	this->outBuffer[10][9] = 0xa8;
	this->outBuffer[12][9] = 0xa8;
	this->outBuffer[14][9] = 0xa8;
	this->outBuffer[16][9] = 0xa8;
	this->outBuffer[18][9] = 0xa8;
	this->outBuffer[20][9] = 0xa8;
	this->outBuffer[22][9] = 0xa8;
	this->outBuffer[24][9] = 0xa8;
	this->outBuffer[26][9] = 0xa8;
	this->outBuffer[28][9] = 0xa8;
	this->outBuffer[30][9] = 0xa8;
	this->outBuffer[32][9] = 0xa8;
	this->outBuffer[34][9] = 0xa8;
	this->outBuffer[36][9] = 0xa8;
	this->outBuffer[38][9] = 0xa8;
	this->outBuffer[40][9] = 0xa8;
	this->outBuffer[42][9] = 0xa8;

	this->outBuffer[1][9] = 0x54;
	this->outBuffer[3][9] = 0x54;
	this->outBuffer[5][9] = 0x54;
	this->outBuffer[7][9] = 0x54;
	this->outBuffer[9][9] = 0x54;
	this->outBuffer[11][9] = 0x54;
	this->outBuffer[13][9] = 0x54;
	this->outBuffer[15][9] = 0x54;
	this->outBuffer[17][9] = 0x54;
	this->outBuffer[19][9] = 0x54;
	this->outBuffer[21][9] = 0x54;
	this->outBuffer[23][9] = 0x54;
	this->outBuffer[25][9] = 0x54;
	this->outBuffer[27][9] = 0x54;
	this->outBuffer[29][9] = 0x54;
	this->outBuffer[31][9] = 0x54;
	this->outBuffer[33][9] = 0x54;
	this->outBuffer[35][9] = 0x54;
	this->outBuffer[37][9] = 0x54;
	this->outBuffer[39][9] = 0x54;
	this->outBuffer[41][9] = 0x54;
	this->outBuffer[43][9] = 0x54;
*/
/*
	this->outBuffer[0][9] = 0xa8;
	this->outBuffer[2][9] = 0xa8;
	this->outBuffer[4][9] = 0xa8;
	this->outBuffer[6][9] = 0xa8;
	this->outBuffer[8][9] = 0xa8;
	this->outBuffer[10][9] = 0xa8;
	this->outBuffer[12][9] = 0xa8;
	this->outBuffer[14][9] = 0xa8;
	this->outBuffer[16][9] = 0xa8;
	this->outBuffer[18][9] = 0xa8;
	this->outBuffer[20][9] = 0xa8;
	this->outBuffer[22][9] = 0xa8;
	this->outBuffer[24][9] = 0xa8;
	this->outBuffer[26][9] = 0xa8;
	this->outBuffer[28][9] = 0xa8;
	this->outBuffer[30][9] = 0xa8;
	this->outBuffer[32][9] = 0xa8;
	this->outBuffer[34][9] = 0xa8;
	this->outBuffer[36][9] = 0xa8;
	this->outBuffer[38][9] = 0xa8;
	this->outBuffer[40][9] = 0xa8;
	this->outBuffer[42][9] = 0xa8;

	this->outBuffer[1][9] =  	0x54;
	this->outBuffer[3][9] =  	0x54;
	this->outBuffer[5][9] = 	 0x54;
	this->outBuffer[7][9] =		0x54;
	this->outBuffer[9][9] = 	0x54;
	this->outBuffer[11][9] = 0x54;
	this->outBuffer[13][9] = 0x54;
	this->outBuffer[15][9] = 0x54;
	this->outBuffer[17][9] = 0x54;
	this->outBuffer[19][9] = 0x54;
	this->outBuffer[21][9] = 0x54;
	this->outBuffer[23][9] = 0x54;
	this->outBuffer[25][9] = 0x54;
	this->outBuffer[27][9] = 0x54;
	this->outBuffer[29][9] = 0x54;
	this->outBuffer[31][9] = 0x54;
	this->outBuffer[33][9] = 0x54;
	this->outBuffer[35][9] = 0x54;
	this->outBuffer[37][9] = 0x54;
	this->outBuffer[39][9] = 0x54;
	this->outBuffer[41][9] = 0x54;
	this->outBuffer[43][9] = 0x54;


	this->outBuffer[0][24] = 0x80;
	this->outBuffer[0][29] = 0x10;
	this->outBuffer[1][24] = 0xC0;
	this->outBuffer[2][24] = 0x60;
	this->outBuffer[3][24] = 0x30;
	this->outBuffer[4][24] = 0x18;
	this->outBuffer[5][24] = 0x0c;
*/





//	this->outBuffer[42][0] = 0x80;
//	this->outBuffer[42][23] = 0x20;
//	this->outBuffer[43][0] = 0x20;
//	this->outBuffer[43][23] = 0x08;


}

void MN12832L::init(){
	this->enableDisplay(false);
	this->enableFilamentVoltage(false);
	this->enableDisplayVoltage(false);
	LL_SPI_Enable(SPI2);
	this->clearBuffer();
}


void MN12832L::newFrame(){
		uint32_t datlen = 44;
		HAL_SuspendTick();

		uint32_t i = 0;
		while (datlen--) {
			this->newSegment(this->outBuffer[i]);
			i++;
		}
		HAL_ResumeTick();
}

void MN12832L::newSegment(uint8_t data[]){
		uint32_t datlen = 30;

		GPIOB->BRR = DIS_BLK_Pin;
		uint32_t i = 0;
		while (datlen--) {
			while(!LL_SPI_IsActiveFlag_TXE(SPI2));
			LL_SPI_TransmitData8(SPI2, data[i++] ); //data[i++]

		}
		while((SPI2->SR & 0x0080));
		GPIOB->BSRR = DIS_BLK_Pin;
		// Lat
		GPIOB->BSRR = DIS_LAT_Pin;
		GPIOB->BRR = DIS_LAT_Pin;

}

void MN12832L::fillGridBytes(){
	for(uint32_t grid = 0; grid < 44; grid++){
		this->outBuffer[grid][24 + grid/8] |= (1 << (7-grid%8));
		this->outBuffer[grid][24 + (grid+1)/8] |= (1 << (7-((grid+1)%8)));
	}
}

void MN12832L::clearBuffer(){
	for(uint32_t i = 0; i<44; i++){
		for(uint32_t k = 0; k<30; k++){
			this->outBuffer[i][k] = 0;
		}
	}
	this->fillGridBytes();
}





void MN12832L::enableDisplayVoltage(bool on){
	if (on){
		LL_GPIO_SetOutputPin(DIS_V_ENABLE_GPIO_Port, DIS_V_ENABLE_Pin);
	}
	else{
		LL_GPIO_ResetOutputPin(DIS_V_ENABLE_GPIO_Port, DIS_V_ENABLE_Pin);
	}
}

void MN12832L::enableFilamentVoltage(bool on){
	if (on){
		LL_GPIO_SetOutputPin(DIS_FILAMENT_V_EN_GPIO_Port, DIS_FILAMENT_V_EN_Pin);
	}
	else{
		LL_GPIO_ResetOutputPin(DIS_FILAMENT_V_EN_GPIO_Port, DIS_FILAMENT_V_EN_Pin);
	}
}

void MN12832L::enableDisplay(bool on){
	if (on){
		LL_GPIO_ResetOutputPin(DIS_BLK_GPIO_Port, DIS_BLK_Pin);
	}
	else{
		LL_GPIO_SetOutputPin(DIS_BLK_GPIO_Port, DIS_BLK_Pin);
	}
}



/*
	uint32_t clk = 512;

	uint32_t zero 	= 0x86000000; // CLK Low, Sin1 Low, Sin2 Low
	uint32_t one 	= 0x82000400; // CLK Low, Sin1 High, Sin2 Low
	uint32_t two 	= 0x06008000; // CLK Low, Sin1 Low, Sin2 High
	uint32_t thre 	= 0x02008400; // CLK Low  Sin1 High, Sin2 High


	volatile uint32_t *set = &GPIOA->BSRR;	//  Pin Set Register Adresse
//	volatile uint32_t *clr = &GPIOA->BRR;	//  Pin Clear Register Adresse


	while (datlen--) {// Länge einen runter zählen bis null
		curbyte=*data++;	// Nächstes Byte laden

		__asm volatile(
				".syntax unified					\n\t"
				"		cmp %[dat], #0				\n\t"
				"		beq	zero%=					\n\t"
				"		cmp %[dat], #1				\n\t"
				"		beq	one%=					\n\t"
				"		cmp %[dat], #2				\n\t"
				"		beq	two%=					\n\t"


				"		str %[thre], [%[set]]		\n\t" 				// Ausgänge setzen, alle gleichzeitig, CLK immer Low den Rest abhängig
				"		b 	end%=					\n\t"
				"zero%=:							\n\t"
				"		str %[zero], [%[set]]		\n\t"
				"		b 	end%=					\n\t"
				"one%=:								\n\t"
				"		str %[one], [%[set]]		\n\t"
				"		b 	end%=					\n\t"
				"two%=:								\n\t"
				"		str %[two], [%[set]]		\n\t"
				"end%=:								\n\t"
				"		str %[clk], [%[set]]		\n\t"	// Ansonsen Ausgang auf Low


				:
				:	[dat] "r" (curbyte), [set] "r" (set), [clk] "r" (clk), [zero] "r" (zero), [one] "r" (one), [two] "r" (two), [thre] "r" (thre)
		);

	}




	//250 ns min
	//display off BLK
	// latch on
	// 300ns min
	//latch off
*/

/*
	while (datlen--){
	//	uint32_t actualByte = (uint32_t)data[iBytes];
		curbyte=*data++;
		for(int32_t iBit = 7; iBit >= 0; iBit--){
			curbyte = curbyte >> 1;
			if(curbyte & 0x01 ){
		//		LL_GPIO_ResetOutputPin(DIS_CLK_GPIO_Port, DIS_CLK_Pin); // Clock down
				//LL_GPIO_SetOutputPin(DIS_SIN1_GPIO_Port, DIS_SIN1_Pin);
			//	GPIOA->BRR = 512;
				GPIOA->BSRR = 0x02000400;
			}
			else{
		//		LL_GPIO_ResetOutputPin(DIS_CLK_GPIO_Port, DIS_CLK_Pin); // Clock down
				//LL_GPIO_ResetOutputPin(DIS_SIN1_GPIO_Port, DIS_SIN1_Pin);
			//	GPIOA->BRR = 512;
				GPIOA->BRR = 1536;
			}
		//	LL_GPIO_SetOutputPin(DIS_CLK_GPIO_Port, DIS_CLK_Pin); //  Clock up
			GPIOA->BSRR = 512;
		}

	}
*/
