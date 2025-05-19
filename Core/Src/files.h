#pragma once

#include "structs.h"
#include "frameBuffer.h"

void initSDCard();
int loadImage(frameBuffer* vram, uint32_t bmpIndex);
