#include "screen.h"
#include "flush.h"
#include "elf.h"

#include <ctr9/io.h>
#include <ctr9/io/ctr_drives.h>
#include <ctr9/ctr_system.h>
#include <ctr9/ctr_screen.h>
#include <ctr9/i2c.h>
#include <ctr9/sha.h>
#include <ctr9/ctr_interrupt.h>

#include <ctrelf.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>

#define PAYLOAD_ADDRESS		0x23F00000
#define A11_PAYLOAD_LOC		0x1FFF4C80  //keep in mind this needs to be changed in the ld script for screen_init too
#define A11_ENTRY			0x1FFFFFF8

extern uint8_t screen_init_bin[];
extern uint32_t screen_init_bin_size;

void ctr_libctr9_init(void)
{
	//Do nothing yet. Initialize IO later.
}

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

static const char *find_file(const char *path, const char* drives[], size_t number_of_drives)
{
	for (size_t i = 0; i < number_of_drives; ++i)
	{
		ctr_drives_chdrive(drives[i]);
		struct stat st;
		if (stat(path, &st) == 0)
		{
			return drives[i];
		}
	}
	return NULL;
}

inline static void vol_memcpy(volatile void *dest, volatile void *sorc, size_t size)
{
	volatile uint8_t *dst = dest;
	volatile uint8_t *src = sorc;
	while(size--)
		dst[size] = src[size];
}

static const char *drives[] = {"SD:", "CTRNAND:", "TWLN:", "TWLP:"};

typedef void (*main_func)(int, char*[]);

void emergency_mode(uint32_t *registers, void *data)
{
	ctr_sd_interface sd;
	ctr_sd_interface_initialize(&sd);
	ctr_io_read_sector(&sd, (void*)PAYLOAD_ADDRESS, 0x100000, 0, 0x100000);
	flush_all_caches();
	ctr_sd_interface_destroy(&sd);
	((main_func)PAYLOAD_ADDRESS)(0, NULL);
}

int main()
{
	//backup the OTP hash, before we lose it due to libctr9 using sha functions
	//for twl
	uint8_t otp_sha[32];
	vol_memcpy(otp_sha, REG_SHAHASH, 0x20);

	ctr_interrupt_prepare();
	ctr_interrupt_set(CTR_INTERRUPT_DATABRT, emergency_mode, NULL);
	ctr_interrupt_set(CTR_INTERRUPT_UNDEF, emergency_mode, NULL);
	ctr_interrupt_set(CTR_INTERRUPT_PREABRT, emergency_mode, NULL);

	ctr_drives_initialize();

	const char * drive = find_file("/arm9loaderhax.bin", drives, 4);
	if (drive)
	{
		setFramebuffers();
		ownArm11();
		clearScreens();
		i2cWriteRegister(3, 0x22, 0x2);
		ctr_screen_enable_backlight(CTR_SCREEN_BOTH);

		ctr_drives_chdrive(drive);
		FILE *payload = fopen("/arm9loaderhax.bin", "rb");
		if(payload)
		{
			//Restore the OTP hash
			vol_memcpy(REG_SHAHASH, otp_sha, 0x20);

			Elf32_Ehdr header;
			load_header(&header, payload);
			fseek(payload, 0, SEEK_SET);

			int argc = 1;
			char *payload_source = (char*)0x30000004;
			strcpy(payload_source, drive);
			strcat(payload_source, "/arm9loaderhax.bin");
			char **argv = (char**)0x30000000;
			argv[0] = payload_source;

			if (check_elf(&header))
			{
				load_segments(&header, payload);
				((main_func)(header.e_entry))(argc, argv);
			}
			else
			{
				struct stat st;
				fstat(fileno(payload), &st);
				fread((void*)PAYLOAD_ADDRESS, st.st_size, 1, payload);
				flush_all_caches();
				((main_func)PAYLOAD_ADDRESS)(argc, argv);
			}
		}
	}

	ctr_system_poweroff();
	return 0;
}

