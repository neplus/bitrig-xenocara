# $OpenBSD: Makefile.inc,v 1.20 2014/07/17 13:53:23 jsg Exp $
.include <bsd.xconf.mk>
.include "../config.mk"

TOP=		${.CURDIR}/../../../../dist/Mesa
MESA=		${.CURDIR}/../../../../dist/Mesa/src/mesa
GALLIUM=	${.CURDIR}/../../../../dist/Mesa/src/gallium

DRI_DRIVER_INSTALL_DIR=	${X11BASE}/lib/modules/dri

COMMON_GALLIUM_SOURCES = \
	utils.c \
	dri_util.c \
	xmlconfig.c \

COMMON_SOURCES = $(COMMON_GALLIUM_SOURCES) \
	driverfuncs.c \
	meta_blit.c \
	meta_generate_mipmap.c \
	meta.c \
	megadriver_stub.c

CPPFLAGS+= -DIN_DRI_DRIVER -DGLX_DIRECT_RENDERING \
	 $(INCLUDES) $(DRIVER_DEFINES)

.if (${COMPILER_VERSION:L:Mgcc[34]*} != "")
CFLAGS += -std=c99
.endif

CXXFLAGS= ${CFLAGS}

INCLUDES = \
	-I$(TOP)/src/mesa/drivers/dri/common \
	-I$(TOP)/include \
	-I$(TOP)/include/GL/internal \
	-I$(TOP)/src \
	-I$(TOP)/src/glsl \
	-I$(TOP)/src/mesa \
	-I$(TOP)/src/mesa/main \
	-I$(TOP)/src/mapi \
	-I$(TOP)/src/mesa/math \
	-I$(TOP)/src/mesa/transform \
	-I$(TOP)/src/mesa/shader \
	-I$(TOP)/src/mesa/swrast \
	-I$(TOP)/src/mesa/swrast_setup \
	-I$(TOP)/src/egl/main \
	-I$(TOP)/src/egl/drivers/dri \
	-I${.CURDIR}/../../generated \
	-I$(X11BASE)/include \
	-I/usr/include/dev/pci/drm

.if ${MACHINE_ARCH} == "i386"
CPPFLAGS += \
	-DUSE_X86_ASM \
	-DUSE_MMX_ASM \
	-DUSE_3DNOW_ASM \
	-DUSE_SSE_ASM
.elif ${MACHINE_ARCH} == "amd64"
CPPFLAGS += -DUSE_X86_64_ASM
.endif

LDADD?=	../../mesa/libmesa/libmesa_pic.a \
	../../glsl/libglsl/libglsl_pic.a \
	-L${X11BASE}/lib  -lexpat -ldrm -lm -lstdc++ -lc
DPADD?=	../../mesa/libmesa/libmesa_pic.a

.PATH: ${MESA}/drivers/dri/common
.PATH: ${MESA}/drivers/common

.SUFFIXES:
.SUFFIXES: .a .c .cpp .o .S .s .so

.c.so:
	@echo "${COMPILE.c:O3=O1} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.c:O3=O1} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o

.cpp.so:
	@echo "${COMPILE.cc:O3=O1} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}"
	@${COMPILE.cc:O3=O1} ${PICFLAG} -DPIC ${.IMPSRC} -o ${.TARGET}.o
	@${LD} -X -r ${.TARGET}.o -o ${.TARGET}
	@rm -f ${.TARGET}.o


all: ${LIB}

OBJS+=  ${SRCS:N*.h:R:S/$/.so/g}

${LIB}:	${OBJS} $(DPADD)
	${CC} -shared ${PICFLAG} -o $@ `${LORDER} ${OBJS}|tsort -q` ${LDADD}

clean:
	rm -f ${LIB} ${OBJS}

cleandir:	clean

install: $(LIB)
	${INSTALL} ${INSTALL_COPY} -o ${LIBOWN} -g ${LIBGRP} -m ${LIBMODE} \
	${LIB} ${DESTDIR}${DRI_DRIVER_INSTALL_DIR}

.include <bsd.dep.mk>
