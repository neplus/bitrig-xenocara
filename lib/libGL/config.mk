# $OpenBSD: config.mk,v 1.1 2014/07/09 21:32:40 jsg Exp $

MESA_MAJOR=	10
MESA_MINOR=	2
MESA_TINY=	3

CPP=	cpp -notraditional

CFLAGS+= -fms-extensions

CPPFLAGS+= \
	-DPACKAGE_VERSION=\"${MESA_MAJOR}.${MESA_MINOR}.${MESA_TINY}\" \
	-DPACKAGE_BUGREPORT=\"bugs@openbsd.org\" \
	-DSTDC_HEADERS=1 \
	-DHAVE_SYS_TYPES_H=1 \
	-DHAVE_SYS_STAT_H=1 \
	-DHAVE_STDLIB_H=1 \
	-DHAVE_STRING_H=1 \
	-DHAVE_MEMORY_H=1 \
	-DHAVE_STRINGS_H=1 \
	-DHAVE_INTTYPES_H=1 \
	-DHAVE_STDINT_H=1 \
	-DHAVE_UNISTD_H=1 \
	-DHAVE_DLFCN_H=1 \
	-DYYTEXT_POINTER=1 \
	-DHAVE_CLOCK_GETTIME=1 \
	-DHAVE_PTHREAD_PRIO_INHERIT=1 \
	-DHAVE_PTHREAD=1 \
	-DHAVE_DLOPEN \
	-DHAVE_POSIX_MEMALIGN \
	-DHAVE_LIBDRM \
	-DGLX_INDIRECT_RENDERING \
	-DGLX_DIRECT_RENDERING \
	-DUSE_EXTERNAL_DXTN_LIB=1 \
	-DHAVE_ALIAS \
	-DHAVE_MINCORE

.if ${MACHINE_ARCH} == "m68k" || ${MACHINE_ARCH} == "powerpc" || \
    ${MACHINE_ARCH} == "sparc"
PICFLAG=-fPIC
.endif
