include Makefile.config

#include all common source files
SRCS = Orbe.c Giop.c OrbeConfig.c OTable.c Timer.c 

#CFLAGS = -Wall -O0
#CFLAGS = -Wall -O0 -save-temps
CFLAGS = -Wall -Os

# parse options
ifeq ($(CONFIG_SOCKET_CHANNEL_USE_UDP), y)
CFLAGS += -DSOCKET_CHANNEL_USE_UDP
#CFLAGS += -DUDP_DEBUG
SRCS   += Arp.c Udp.c CheckSum.c Emac.c 
endif

ifeq ($(CONFIG_SOCKET_CHANNEL_USE_UART), y)
CFLAGS += -DSOCKET_CHANNEL_USE_UART
SRCS   += Uart.c
endif

ifeq ($(CONFIG_ORBE_TIMER), y)
CFLAGS += -DTIMER
endif

OBJS = ${SRCS:.c=.o}

COMPILER=
ARCHIVER=
CP=cp
EXTRA_COMPILER_FLAGS= 
LIB=libOrbe.a
LEVEL=0

RELEASEDIR=../../../lib/
INCLUDEDIR=../../../include/
INCLUDES=-I./include/ -I${INCLUDEDIR}

LIBSOURCES= $(SRCS)
OUTS = *.o 

libs:
	for i in $(LIBSOURCES); do \
		echo $(COMPILER) $(COMPILER_FLAGS) $(CFLAGS) $(EXTRA_COMPILER_FLAGS) $(INCLUDES) $$i; \
		$(COMPILER) $(COMPILER_FLAGS) $(CFLAGS) $(EXTRA_COMPILER_FLAGS) $(INCLUDES) $$i; \
	done
	$(ARCHIVER) -r $(RELEASEDIR)/$(LIB) $(OUTS) 
	echo make clean

include: dummy
	$(CP) -rf *.h $(INCLUDEDIR)

dummy:
clean:
	rm -rf $(OUTS) $(RELEASEDIR)/$(LIB)
