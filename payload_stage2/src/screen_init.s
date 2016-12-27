.global screen_init_data_begin, screen_init_data_end, screen_init_data_size

.align 4

.section .rodata.screen_init

screen_init_begin:
.incbin "screen_init.bin"
screen_init_end:

.align 4
screen_init_data_begin:
.word screen_init_begin

.align 4
screen_init_data_end:
.word screen_init_end

.align 4
screen_init_data_size:
.word screen_init_end-screen_init_begin

