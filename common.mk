INCPATHS=-I$(prefix)/include/freetype2 -I$(prefix)/include

THUMBFLAGS=-mthumb #-mthumb-interwork
C9FLAGS=-mcpu=arm946e-s -march=armv5te -mlittle-endian -mword-relocations $(THUMBFLAGS)
C11FLAGS=-mcpu=mpcore -mlittle-endian -march=armv6k -mtune=mpcore -mfloat-abi=hard -mfpu=vfp

#SIZE_OPTIMIZATION = -flto
SIZE_OPTIMIZATION = -Wl,--gc-sections -ffunction-sections
AM_CPPFLAGS=$(INCPATHS)
AM_CFLAGS= -std=gnu11 -O2 -g -fomit-frame-pointer -ffast-math \
	-Wpedantic -Wall -Wextra -Wcast-align -Wcast-qual \
	-Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op \
	-Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls \
	-Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default \
	-Wundef -Wno-unused $(SIZE_OPTIMIZATION)
AM_LDFLAGS=-Wl,--use-blx,--pic-veneer,-q
OCFLAGS=--set-section-flags .bss=alloc,load,contents

