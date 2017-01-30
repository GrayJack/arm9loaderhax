# Arm9LoaderHax for Nintendo 3DS

## Explanation

This is a fork of delebile's A9LH implementation, documented [here](http://3dbrew.org/wiki/3DS_System_Flaws) and also presented [in this conference](https://media.ccc.de/v/32c3-7240-console_hacking), which provides ARM9 code execution directly at the console boot, exploiting a vulnerability present in 9.6+ version of New3DS arm9loader.

This exploit was found by **plutoo** and **yellows8**.

## Usage

It loads an **arm9loaderhax.bin** (always init the screens) ARM9 payload from the root of the SD card, CTRNAND, TWLN, TWLP, respectively, at address 0x23F00000 if the file is a binary.
This means that it offers a BRAHMA-like setup, and as such has compatibility with every payload BRAHMA can run.

You can also run code on the ARM11 by writing its memory address to 0x1FFFFFF8, 0x1FFFFFFC, or from making sense of 3 words passed via PXI (address, argc, argv).

There is an emergency mode, which will load a payload (**emergency.bin**) from SD card if an exception is triggered, to memory address 0x25F00000 if the file is a binary, but if that fails due to another bug, it will try again, this time reading directly from the SD card (**emergency_regs.bin**).
The emergency loading has not been tested, but at least adding the code did not cause an O3DS/N3DS to fail to load anything else.

**Note:** You need to change the extention of '*.elf' to '*.bin'. This implementation is smart enough to tell whether a file is ELF or not.

## Installation

Use the [SafeA9LHInstaller 2.6.2](https://github.com/AuroraWright/SafeA9LHInstaller/releases/tag/v2.6.2) (later version doesn't work)

## Software Update

Use the [SafeA9LHInstaller 2.6.2](https://github.com/AuroraWright/SafeA9LHInstaller/releases/tag/v2.6.2) (later version doesn't work)


## Setup

####**Required Enviroment**

* devkitARM
* [libctru](https://github.com/smealum/ctrulib)
* [libctrelf](https://github.com/gemarcano/libctrelf)
* [libctr_core](https://github.com/gemarcano/libctr_core)
* [libctr9](https://github.com/gemarcano/libctr9)
* [libctr11](https://github.com/gemarcano/libctr11)
* Python with PyCcrypto (2.7 or 3.x should work)
* GCC or MinGW (Only needed to compile the buildt-in tool, you can download [a pre-compiled windows build here](https://mega.nz/#!j0RkxLjb!4Am-3yDAR9g4VDxY93pWhXVYNDiylSW1cKJntOLfDWU), place it in **common** folder)
* Autotools (as in, automake/autoconf - mandatory)

####**Building**

```
autoreconf -if
./configure --host arm-none-eabi --prefix=$CTRARM9
make
```

**Note:** The variable CTRARM11 is required for compiling, since it's used for finding the libraries used for screen init.

## Issues/Bugs

Please report these to the issue tracker at the repository, and these will be addressed as soon as possible, if not at least acknowledged. The more detailed the reports, the more likely they are to be addressed quickly.
In particular, the following information can be quite useful when debugging bugs:

 - Type of 2/3DS system;
 - Operating system being used to compile;
 - Release/commit of library in use;
 - Steps to reproduce issue;
 - Expected behavior;
 - Actual behavior;
 - Any modifications to the library, or extensions.


## Credits

*Copyright 2016, Jason Dellaluce/Gabriel Marcano*


*Licensed under GPLv2 or any later version, refer to the license.txt file included.*

* Smealum and contributors for libctru
* Normmatt for sdmmc.c and .h, and also for .ld files and the log from XDS by ichfly that provided us some of the information needed to get screen init
* Gemarcano for libctr9, libctr11, libctrelf and libctr_core
* Christophe Devine for the SHA codes
* Archshift for i2c.c and .h
* Megazig for crypto.c and .h
* Patois for original BRAHMA code
* Smealum, Derrek, Plutoo for publishing the exploit
* Yellows8 and Plutoo as ideators of it
* [3dbrew community](http://3dbrew.org/)
* bilis/b1l1s for his screen init code, and work on inegrating it into stage 2
* dark_samus for work on integrating screen init into stage 2
