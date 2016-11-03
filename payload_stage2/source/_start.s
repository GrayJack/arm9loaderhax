.section .text.start
.align 4
.arm
.global _start
.extern flush_all_caches
_start:
	@call libc initialization routines
	adr r0, __libc_init_array_offset
	ldr r1, [r0]
	add r0, r1, r0
	blx r0

	adr r0, _fini_offset
	ldr r1, [r0]
	add r0, r1, r0
	adr r1, atexit_offset
	ldr r2, [r1]
	add r1, r2, r1
	blx r1

	adr r0, _init_offset
	ldr r1, [r0]
	add r0, r1, r0
	blx r0

	adr r0, main_offset
	ldr r1,  [r0]
	add r0, r1, r0
	blx r0

	adr r1, exit_offset
	ldr r2, [r1]
	add r1, r2, r1
	blx r1

	b . @die

__libc_init_array_offset:
.word __libc_init_array-.

_fini_offset:
.word _fini-.

atexit_offset:
.word atexit-.

_init_offset:
.word _init-.

main_offset:
.word main-.

exit_offset:
.word exit-.

