# $OpenBSD: Makefile.inc,v 1.7 2014/07/09 21:32:40 jsg Exp $
.include <bsd.xconf.mk>
.include "../config.mk"

OBJS+=  ${SRCS:N*.h:R:S/$/.so/g}

.SUFFIXES:
.SUFFIXES: .a .c .cpp .o .so

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

lib${LIB}_pic.a: ${OBJS} ${DPADD}
	@rm -f lib${LIB}_pic.a
	@${AR} cq lib${LIB}_pic.a `${LORDER} ${OBJS} | tsort -q`
	${RANLIB} lib${LIB}_pic.a


.if ${XENOCARA_BUILD_GALLIUM} == "llvm"

# Remember to keep in sync with LLVM port's version.
LLVM_VERSION=	0x305	# 3.5
LLVM_CONFIG?=	/usr/local/bin/llvm-config

# ugh...
ALLVM_CFLAGS!=		${LLVM_CONFIG} --cflags
ALLVM_CPPFLAGS!=	${LLVM_CONFIG} --cxxflags
BLLVM_CFLAGS=	${ALLVM_CFLAGS:M-[IDf]*}
BLLVM_CPPFLAGS=	${ALLVM_CPPFLAGS:M-[IDf]*}
LLVM_CFLAGS=	${BLLVM_CFLAGS:S/-DNDEBUG//}
LLVM_CPPFLAGS=	${BLLVM_CPPFLAGS:S/-DNDEBUG//}
LLVM_LIBDIR!=	${LLVM_CONFIG} --libdir
LLVM_LIBS!=	${LLVM_CONFIG} --libs engine bitwriter mcjit r600 bitreader ipo asmparser

CFLAGS+=	${LLVM_CFLAGS}
CXXFLAGS=	${LLVM_CPPFLAGS}
LDADD+=		-L${LLVM_LIBDIR} ${LLVM_LIBS}

CPPFLAGS+= \
	-DGALLIUM_LLVMPIPE \
	-DHAVE_LLVM=${LLVM_VERSION} -DLLVM_VERSION_PATCH=0

SWRAST_DRIVERS+= ../../gallium/drivers/llvmpipe/libllvmpipe_pic.a

# for r600 and radeonsi
LOCALBASE?=	/usr/local
LDADD+=		${LOCALBASE}/lib/libelf.a
.endif
