include $(top_srcdir)/common.mk

bin_PROGRAMS = payload_stage1.elf payload_stage1.bin
#payload_stage1_elf_CCASFLAGS = -I$(top_srcdir)/data
payload_stage1_elf_CFLAGS=$(AM_CFLAGS) $(C9FLAGS) -T$(srcdir)/payload_stage1.ld -L${prefix}/lib -I${prefix}/include/freetype2
payload_stage1_elf_SOURCES = src/flush.h src/main.c src/flush.s
payload_stage1_elf_LDADD = $(AM_LDADD) -lctr9 -lfreetype -lm

EXTRA_DIST=payload_stage1.ld

payload_stage1.bin$(EXEEXT): $(builddir)/payload_stage1.elf$(EXEEXT)
	$(OBJCOPY) $(OCFLAGS) -O binary $^ $@

