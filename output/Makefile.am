SUBDIRS = hashgenerator

include $(top_srcdir)/common.mk

bin_PROGRAMS = payload_stage1.bin payload_stage2.bin

$(top_builddir)/payload_stage1/payload_stage1.bin$(EXEEXT):
	(cd $(top_builddir)/payload_stage1/ && $(MAKE) $(AM_MAKEFLAGS) payload_stage1.bin )

payload_stage1.bin$(EXEEXT): $(top_builddir)/payload_stage1/payload_stage1.bin$(EXEEXT)
	cp $^ $@
	./hashgenerator/hashGenerator payload_stage1.bin

$(top_builddir)/payload_stage/payload_stage2.bin$(EXEEXT):
	(cd $(top_builddir)/payload_stage2/ && $(MAKE) $(AM_MAKEFLAGS) payload_stage2.bin )

payload_stage2.bin$(EXEEXT): $(top_builddir)/payload_stage2/payload_stage2.bin$(EXEEXT)
	cp $^ $@
	./hashgenerator/hashGenerator payload_stage2.bin

clean: clean-recursive
	rm -rf payload_stage1.bin.sha payload_stage2.bin.sha
