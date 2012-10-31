include Makefile.config

COMPILER=
ARCHIVER=
CP=cp
EXTRA_COMPILER_FLAGS= 
LIB=libOrbe.a
LEVEL=0

#include all common source files
SRCS = OrbeConfig.c
OBJS = ${SRCS:.c=.o}
LIBSOURCES= $(SRCS)
OUTS = *.o 

CFLAGS = -Wall -O3


# parse options
ifeq ($(CONFIG_SOCKET_CHANNEL_USE_UDP), y)
CFLAGS += -DSOCKET_CHANNEL_USE_UDP
	ifeq ($(COMPILER), powerpc-eabi-gcc)
		RUNLIB = libOrbe-ppc-udp.a
	else
		RUNLIB = libOrbe-mb-udp.a
	endif
endif

ifeq ($(CONFIG_SOCKET_CHANNEL_USE_UART), y)
CFLAGS += -DSOCKET_CHANNEL_USE_UART
	ifeq ($(COMPILER), powerpc-eabi-gcc)
		RUNLIB = libOrbe-ppc-uart.a
	else
		RUNLIB = libOrbe-mb-uart.a
	endif
endif

ifeq ($(CONFIG_ORBE_TIMER), y)
CFLAGS += -DTIMER
RUNLIB = libOrbe-mb-timer.a
endif


RELEASEDIR=../../../lib/
INCLUDEDIR=../../../include/
INCLUDES=-I./include/ -I${INCLUDEDIR}

libs:
	echo $(RUNLIB)
	cp $(RUNLIB) ${RELEASEDIR}/${LIB}
	for i in $(LIBSOURCES); do \
	  echo "Compiling " $$i ;\
	  $(COMPILER) $(COMPILER_FLAGS) $(CFLAGS) $(EXTRA_COMPILER_FLAGS) $(INCLUDES) $$i; \
        done
	$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${OUTS} 

include: dummy
	${CP} -rf *.h ${INCLUDEDIR}

dummy:
clean:
	rm -rf ${OUTS}
