include $(top_srcdir)/common.mk

bin_PROGRAMS = screen_init.elf
screen_init_elf_CFLAGS=$(AM_CFLAGS) $(C11FLAGS) -T$(srcdir)/screen_init.ld -nostartfiles -marm
#payload_stage1_elf_CCASFLAGS = -I$(top_srcdir)/data
screen_init_elf_SOURCES = src/screen_init.c src/crt0.s

EXTRA_DIST=screen_init.ld

