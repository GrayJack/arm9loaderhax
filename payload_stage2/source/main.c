#include "screen.h"
#include "flush.h"
#include "elf.h"

#include <ctr9/io.h>
#include <ctr9/io/ctr_fatfs.h>
#include <ctr9/ctr_system.h>
#include <ctr9/ctr_screen.h>
#include <ctr9/i2c.h>
#include <ctrelf.h>


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

static const char * find_file(const char *path)
{
	f_chdrive("SD:");
	if (ctr_sd_interface_inserted() && (FR_OK == f_stat(path, NULL)))
	{
		return "SD:";
	}
	else
	{
		f_chdrive("CTRNAND:");
		if (FR_OK == f_stat(path, NULL))
		{
			return "CTRNAND:";
		}
	}

	return NULL;
}

int main()
{
	FATFS fs1;
	FATFS fs2;
	FIL payload;
	ctr_nand_interface nand;
	ctr_nand_crypto_interface ctrnand;
	ctr_sd_interface sd;
	ctr_fatfs_initialize(&nand, &ctrnand, NULL, &sd);

	f_mount(&fs1, "SD:", 0); //This never fails due to deferred mounting
	f_mount(&fs2, "CTRNAND:", 0); //This never fails due to deferred mounting

	const char * drive = find_file("/arm9loaderhax.bin");
	if (drive)
	{
		f_chdrive(drive);
		if(f_open(&payload, "/arm9loaderhax.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK)
		{

			setFramebuffers();
			ownArm11();
			clearScreens();
			i2cWriteRegister(3, 0x22, 0x2);
			ctr_screen_enable_backlight(CTR_SCREEN_BOTH);

			Elf32_Ehdr header;
			load_header(&header, &payload);
			f_lseek(&payload, 0);
			if (check_elf(&header))
			{
				load_segments(&header, &payload);
				((void (*)(void))(header.e_entry))();
			}
			else
			{
				unsigned int br;
				f_read(&payload, (void*)PAYLOAD_ADDRESS, f_size(&payload), &br);
				flush_all_caches();
				((void (*)(void))PAYLOAD_ADDRESS)();
			}
		}
	}

	ctr_system_poweroff();
	return 0;
}

