#include <stdint.h>

namespace VESA
{
	struct
	{
		int VideoModesAmmount = 0;
		const int VideoModeStructSize = 4+2+1+2+2+2+4;
	}GlobalVideoInfo;
	class VideoInformation
	{
	public:
		struct
		{
			uint32_t  Framebuffer;
			uint16_t Pitch;
			uint8_t ColorDepth;
			uint16_t Width;
			uint16_t Height;
			uint16_t Id;
			uint32_t Checksum;
		}Data;
		void LoadFromAddress(uint64_t addr);
		void WritePixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
		VideoInformation(uint64_t addr);
		VideoInformation();
	};
}

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
	}
}

void VESA::VideoInformation::LoadFromAddress(uint64_t addr)
{
	this->Data.Framebuffer = *((uint32_t*)(uint32_t*)addr);
	this->Data.Pitch = *((uint16_t*)((uint8_t*)addr+4));
	this->Data.ColorDepth = *((uint8_t*)((uint8_t*)addr+6));
	this->Data.Width = *((uint16_t*)((uint8_t*)addr+7));
	this->Data.Height = *((uint16_t*)((uint8_t*)addr+9));
	this->Data.Id = *((uint16_t*)((uint8_t*)addr+11));
	this->Data.Checksum = *((uint32_t*)((uint8_t*)addr+16));//low endian, copy from end
}

VESA::VideoInformation::VideoInformation(uint64_t addr)
{
	this->LoadFromAddress(addr);
}

void VESA::VideoInformation::WritePixel(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	uint32_t PixelOffset = y * this->Data.Pitch + (x * (this->Data.ColorDepth/8)) + 0xe0000000;
	void * addr = (void*) PixelOffset;
	uint32_t * Faddr = (uint32_t*) addr;
	*(Faddr) = (red << 16) | (green << 8) | (blue);
}

VESA::VideoInformation::VideoInformation()
{
	//default constructor
}
