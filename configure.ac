#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([arm9loaderhax], [0.1], [])
AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-xz subdir-objects])
AC_CONFIG_SRCDIR([payload_stage1/src/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AS
AC_PROG_MAKE_SET
AC_CHECK_TOOL([OBJCOPY],objcopy)
# Checks for libraries.
# FIXME: Replace `main' with a function in `-lctr9':
#AC_CHECK_LIB([ctr9], [ctr_nand_interface_initialize], [ ], [AC_MSG_ERROR([Error, the ctr9 library is missing!])])
# FIXME: Replace `main' with a function in `-lctrelf':
#AC_CHECK_LIB([ctrelf], [elf_load_header], [ ], [AC_MSG_ERROR([Error, the ctrelf library is missing!])])
# FIXME: Replace `main' with a function in `-lm':
#AC_CHECK_LIB([m], [exp], [ ], [AC_MSG_ERROR([Error, the math library is missing!])])

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h strings.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero memmove memset strcasecmp])

AC_CONFIG_FILES([Makefile
                 payload_stage1/Makefile
                 payload_stage2/Makefile
                 screen_init/Makefile])
AC_OUTPUT
