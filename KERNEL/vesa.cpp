#include "vesa.h"

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
	uint64_t PixelOffset = y * this->Data.Pitch + (x * (this->Data.ColorDepth/8)) + this->Data.Framebuffer;
	void * addr = (void*) PixelOffset;
	uint64_t * Faddr = (uint64_t*) addr;
	*(Faddr) = (red << 16) | (green << 8) | (blue);
}

VESA::VideoInformation::VideoInformation()
{
	//default constructor
}
