#ifndef VESA_H
#define VESA_H
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
#endif
