.section .text.start
.align 4
.arm
.global _start
.extern flush_all_caches
_start:
_init:

    ldr r0, =main
    blx r0

.die:
    b .die

