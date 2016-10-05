#include <ctr9/io.h>
#include <ctr9/ctr_cache.h>
#include <elf.h>
#include <string.h>

void load_header(Elf32_Ehdr *header, FIL *file)
{
	f_lseek(file, 0);
	char buffer[sizeof(*header)];
	UINT br;
	f_read(file, buffer, sizeof(buffer), &br);

	elf_load_header(header, buffer);
}

int load_segment(const Elf32_Phdr *header, FIL *file)
{
	size_t program_size = header->p_filesz;
	size_t mem_size = header->p_memsz;
	void *location = (void*)(header->p_vaddr);

	size_t type = header->p_type;

	switch (type)
	{
		case PT_LOAD:
			break;
		default:
			return 1;
	}

	int res = f_lseek(file, header->p_offset);
	UINT br;

	res = f_read(file, location, program_size, &br);
	memset(program_size + (char*)location, 0, mem_size - program_size);

	ctr_cache_clean_data_range(location, (char*)location + mem_size);
	ctr_cache_flush_instruction_range(location, (char*)location + mem_size);
	ctr_cache_drain_write_buffer();
	return 0;
}

int load_segments(const Elf32_Ehdr *header, FIL *file)
{
	int res = 0;
	size_t pnum = header->e_phnum;
	char buffer[pnum][header->e_phentsize];
	UINT br;

	f_lseek(file, header->e_phoff);
	res = f_read(file, buffer, sizeof(buffer), &br);

	if (res)
		return res;

	for (size_t i = 0; i < pnum; ++i)
	{
		Elf32_Phdr pheader;
		elf_load_program_header(&pheader, buffer[i]);
		res = load_segment(&pheader, file);
		if (res)
			return res;
	}

	return res;
}

bool check_elf(Elf32_Ehdr *header)
{

	if (!(header->e_ident[EI_MAG0] == (char)0x7f &&
		header->e_ident[EI_MAG1] == 'E' &&
		header->e_ident[EI_MAG2] == 'L' &&
		header->e_ident[EI_MAG3] == 'F'))
		return false;

	if (header->e_ident[EI_CLASS] != 1)
		return false;

	if (header->e_ident[EI_DATA] != 1)
		return false;

	if (header->e_ident[EI_VERSION] != EV_CURRENT)
		return false;

	if (header->e_type != ET_EXEC)
		return false;

	if (header->e_machine != EM_ARM)
		return false;

	if (header->e_version != EV_CURRENT)
		return false;

	//if (header->e_entry == 0)
	//	return false;

	if (header->e_phoff == 0)
		return false;

	if (header->e_ehsize == 0)
		return false;

	return true;
}
