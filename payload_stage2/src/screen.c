#include "screen.h"
#include <stdint.h> // for uint32_t

#define TOP_SCREEN_SIZE	(400 * 240 * 3 / 4)
#define BOT_SCREEN_SIZE	(320 * 240 * 3 / 4)

void setFramebuffers(void)
{
	//Gateway
	*(volatile uint32_t*)0x80FFFC0 = 0x18300000;  // framebuffer 1 top left
	*(volatile uint32_t*)0x80FFFC4 = 0x18300000;  // framebuffer 2 top left
	*(volatile uint32_t*)0x80FFFC8 = 0x18300000;  // framebuffer 1 top right
	*(volatile uint32_t*)0x80FFFCC = 0x18300000;  // framebuffer 2 top right
	*(volatile uint32_t*)0x80FFFD0 = 0x18346500;  // framebuffer 1 bottom
	*(volatile uint32_t*)0x80FFFD4 = 0x18346500;  // framebuffer 2 bottom
	*(volatile uint32_t*)0x80FFFD8 = 1;  // framebuffer select top
	*(volatile uint32_t*)0x80FFFDC = 1;  // framebuffer select bottom

	//CakeBrah
	*(volatile uint32_t*)0x23FFFE00 = 0x18300000;
	*(volatile uint32_t*)0x23FFFE04 = 0x18300000;
	*(volatile uint32_t*)0x23FFFE08 = 0x18346500;
}

void clearScreens(void)
{
	for(uint32_t i = 0; i < (TOP_SCREEN_SIZE); i++)
	{
		*((volatile uint32_t*)0x18300000 + i) = 0;
	}

	for(uint32_t i = 0; i < (BOT_SCREEN_SIZE); i++)
	{
		*((volatile uint32_t*)0x18346500 + i) = 0;
	}
}
