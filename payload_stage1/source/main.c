#include <ctr9/io.h>
#include "flush.h"
#include <ctr9/ctr_system.h>

int main()
{
	// Initialize sdcard and nand
	ctr_nand_interface nand;
	ctr_nand_interface_initialize(&nand);
	ctr_io_read_sector(&nand, (void*)0x08006000, 0x200*0x100, 0x5C000, 0x100);
	flush_all_caches();
	// Jump to secondary payload
	((void (*)())0x08006000)();
	
	return 0;
}
