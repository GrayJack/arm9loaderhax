include $(top_srcdir)/common.mk

bin_PROGRAMS = screen_init.elf
screen_init_elf_CPPFLAGS=$(AM_CPPFLAGS) -I$(CTRARM11)/include
screen_init_elf_CFLAGS=$(AM_CFLAGS) $(C11FLAGS) -T$(srcdir)/screen_init.ld -marm
#payload_stage1_elf_CCASFLAGS = -I$(top_srcdir)/data
screen_init_elf_SOURCES = src/screen_init.c
screen_init_elf_LDFLAGS = -L$(CTRARM11)/lib $(AM_LDFLAGS)
screen_init_elf_LDADD = -lctr11 -lctr_core $(AM_LDADD)

EXTRA_DIST=screen_init.ld

