#include <ctr9/io.h>
#include "flush.h"
#include <ctr9/ctr_system.h>

void ctr_libctr9_init(void)
{
	//Do nothing, we do not want any of the normal libctr9 initialization
}

int main()
{
	// Initialize sdcard and nand
	ctr_nand_interface nand;
	ctr_nand_interface_initialize(&nand);
	//Reading 0x100 sectors, 131072 bytes
	ctr_io_read_sector(&nand, (void*)0x08006000, 0x200*0x200, 0x5C000, 0x200);
	ctr_nand_interface_destroy(&nand);
	flush_all_caches();
	// Jump to secondary payload
	((void (*)())0x08006000)();

	return 0;
}

