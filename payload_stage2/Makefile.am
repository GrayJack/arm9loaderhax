include $(top_srcdir)/common.mk

bin_PROGRAMS = payload_stage2.elf payload_stage2.bin screen_init.bin
payload_stage2_elf_CCASFLAGS = -I$(top_builddir)/payload_stage2/
payload_stage2_elf_CFLAGS=$(AM_CFLAGS) $(C9FLAGS) -T$(srcdir)/payload_stage2.ld -L${prefix}/lib -I${prefix}/include/freetype2
payload_stage2_elf_SOURCES = src/elf.c src/elf.h src/flush.h src/flush.s src/main.c src/screen.c src/screen.h src/screen_init.s
payload_stage2_elf_LDADD = $(AM_LDADD) -lctr9 -lctrelf -lfreetype -lm

EXTRA_payload_stage2_elf_DEPENDENCIES=screen_init.bin
BUILT_SOURCES=screen_init.bin

payload_stage2.bin$(EXEEXT): $(builddir)/payload_stage2.elf$(EXEEXT)
	$(OBJCOPY) $(OCFLAGS) -O binary $^ $@

$(top_builddir)/screen_init/screen_init.elf$(EXEEXT):
	(cd $(top_builddir)/screen_init/ && $(MAKE) $(AM_MAKEFLAGS) screen_init.elf$(EXEEXT) )

screen_init.bin$(EXEEXT): $(top_builddir)/screen_init/screen_init.elf$(EXEEXT)
	$(OBJCOPY) $(OCFLAGS) -O binary $^ $@

EXTRA_DIST=payload_stage2.ld

