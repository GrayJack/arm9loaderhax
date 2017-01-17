include $(top_srcdir)/common.mk

bin_PROGRAMS = screen_init.bin payload_stage2.elf payload_stage2.bin
payload_stage2_elf_CCASFLAGS = -I$(top_builddir)/payload_stage2/
payload_stage2_elf_CFLAGS=$(AM_CFLAGS) $(C9FLAGS) -T$(srcdir)/payload_stage2.ld -L${prefix}/lib -I${prefix}/include/freetype2
payload_stage2_elf_SOURCES = src/flush.h src/flush.s src/main.c src/screen.c src/screen.h src/screen_init.s
payload_stage2_elf_LDADD = $(AM_LDADD) -lctr9 -lctr_core -lctrelf -lfreetype -lm
BUILT_SOURCES = screen_init.bin$(EXEEXT)

payload_stage2.bin$(EXEEXT): $(builddir)/payload_stage2.elf$(EXEEXT)
	$(OBJCOPY) $(OCFLAGS) -O binary $^ $@

.PHONY: FORCE

$(top_builddir)/screen_init/screen_init.elf$(EXEEXT): FORCE
	(cd $(top_builddir)/screen_init/ && $(MAKE) $(AM_MAKEFLAGS) screen_init.elf$(EXEEXT) )

screen_init.bin$(EXEEXT): $(top_builddir)/screen_init/screen_init.elf$(EXEEXT)
	(cd $(top_builddir)/screen_init/ && $(MAKE) $(AM_MAKEFLAGS) screen_init.elf$(EXEEXT) )
	$(OBJCOPY) $(OCFLAGS) -O binary $^ $@

EXTRA_DIST=payload_stage2.ld

