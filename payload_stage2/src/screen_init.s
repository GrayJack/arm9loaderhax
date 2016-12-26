.global screen_init_data_begin, screen_init_data_end, screen_init_data_size

.section .rodata.screen_init

screen_init_data_begin:
.incbin "screen_init.bin"
screen_init_data_end:

screen_init_data_size:
.word screen_init_data_end-screen_init_data_begin

