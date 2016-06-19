#include "screen.h"
#include "flush.h"

#include <ctr9/io.h>
#include <ctr9/io/ctr_fatfs.h>
#include <ctr9/ctr_system.h>

#include <string.h>

#define PAYLOAD_ADDRESS		0x23F00000
#define A11_PAYLOAD_LOC		0x1FFF4C80  //keep in mind this needs to be changed in the ld script for screen_init too
#define A11_ENTRY			0x1FFFFFF8

extern uint8_t screen_init_bin[];
extern uint32_t screen_init_bin_size;

static void ownArm11()
{
	memcpy((void*)A11_PAYLOAD_LOC, screen_init_bin, screen_init_bin_size);
	*(volatile uint32_t*)A11_ENTRY = 1;
	*((volatile uint32_t*)0x1FFAED80) = 0xE51FF004;
	*((uint32_t *)0x1FFAED84) = A11_PAYLOAD_LOC;
	*((uint32_t *)0x1FFFFFF0) = 2;

	//AXIWRAM isn't cached, so this should just work
	while(*(volatile uint32_t *)A11_ENTRY);
}

int main()
{
	FATFS fs;
	FIL payload;
	ctr_nand_interface nand;
	ctr_sd_interface sd;
	ctr_fatfs_initialize(&nand, NULL, NULL, &sd);

	f_mount(&fs, "SD:", 0); //This never fails due to deferred mounting
	if(f_open(&payload, "SD:/arm9loaderhax.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK)
	{
		setFramebuffers();
		ownArm11();
		clearScreens();
		turnOnBacklight();

		unsigned int br;
		f_read(&payload, (void*)PAYLOAD_ADDRESS, f_size(&payload), &br);
		flush_all_caches();
		((void (*)(void))PAYLOAD_ADDRESS)();
	}

	ctr_system_poweroff();
	return 0;
}

