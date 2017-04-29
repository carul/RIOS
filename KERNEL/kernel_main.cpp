#include <stdint.h>
#include "vesa.h"

void _start()
{
	/**
		Get ammount of previously collected VESA video modes
	*/
	uint8_t * FirstStructPointer = (uint8_t*)0x508;
	while(true)
	{

		uint32_t checksum = *((uint32_t*)FirstStructPointer+16);
		if (checksum != 0xbacabaca)
			break;
		VESA::GlobalVideoInfo.VideoModesAmmount++;
		FirstStructPointer+=VESA::GlobalVideoInfo.VideoModeStructSize;
	}
	/**
		load every VESA structure
	*/
	VESA::VideoInformation VideoModes[VESA::GlobalVideoInfo.VideoModesAmmount];
	for(int i = 0; i < VESA::GlobalVideoInfo.VideoModesAmmount; i++)
	{
		VideoModes[i].LoadFromAddress(0x508+i*VESA::GlobalVideoInfo.VideoModeStructSize);
	}
	/**
		search for basic video mode
	*/
	VESA::VideoInformation ActiveVideoMode;
	for(int i = 0; i < VESA::GlobalVideoInfo.VideoModesAmmount; i++)
	{
		if(VideoModes[i].Data.Width == 1280 && VideoModes[i].Data.Height == 720 && VideoModes[i].Data.ColorDepth == 32)
			ActiveVideoMode = VideoModes[i];
	}


	while(true)
	{
		/*
			paint screen white
			TODO: Write string + font
		*/
		for(int x = 0; x < 1280; x++)
		{
			for(int y = 0; y < 720; y++)
			{
				ActiveVideoMode.WritePixel(x, y, 255, 255, 255);
			}
		}
	}
}
