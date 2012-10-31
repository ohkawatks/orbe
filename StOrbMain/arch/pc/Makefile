#
# $Id: Makefile 1056 2009-02-05 03:25:19Z takeshi $
# Copyright (C) 2008,2009 AIST, National Institute of Advanced Industrial Science and Technology 
#
CC = gcc

#TYPE = -DSOCKET_CHANNEL_USE_UART
#TYPE = -DSOCKET_CHANNEL_USE_UDP
TYPE = -DSOCKET_CHANNEL_USE_STDINOUT -DENABLE_CHECK_REQUEST_ID -DGIOP_FULL_ANALYSIS

ENDIAN		= -DENDIAN=1

##INCLUDE	= -I. -I./CORBA -I../transport
INCLUDE		= -I. 
GCC_OPTIONS	= -Wall -Wextra -Wformat=2 -Wstrict-aliasing=2 				\
			  -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion		\
			  -Wfloat-equal -Wpointer-arith 

CFLAGS		= $(GCC_OPTIONS) -g $(INCLUDE) $(TYPE) $(ENDIAN)

IDL				= ../../idl/rm11.idl

IDLC_DIR		= ../../idlc
SRC_DIR			= ../../src
TRANSPORT_DIR	= $(SRC_DIR)/transport
ORB_DIR			= $(SRC_DIR)/orbe
CONFIG_DIR		= $(SRC_DIR)/config

HEADERS = CORBA_Object.h Emac.h Giop.h Orbe.h Socket.h version.h xbasic_types.h OrbeConfig.h swapd.h rm11.h
SRCS = main.c Giop.c Orbe.c StdInOut.c log.c version.c OrbeConfig.c OTable.c swapd.c Timer.c rm11.c rm11Impl.c
OBJS = main.o Giop.o Orbe.o StdInOut.o log.o version.o OrbeConfig.o OTable.o swapd.o Timer.o rm11.o rm11Impl.o

.PRECIOUS: $(HEADERS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@


all: server

client:
	@echo "*** make target 'client' is not available now."

server:
	make idl
	make symlinks
	make objs
	$(CC) -o Orbe $(CFLAGS) $(OBJS)

idl: $(IDL)
	(cd $(IDLC_DIR); make)
	test -x $(IDLC_DIR)/main.exe && $(IDLC_DIR)/main.exe $(IDL) || env RUBYLIB=$(IDLC_DIR) ruby $(IDLC_DIR)/main.rb $(IDL)

objs: $(HEADERS) $(OBJS)

clean:
	@rm -f Orbe *.o *.bak *.o

cleandata:
	@rm -f data.bin

distclean: clean cleandata
	@for f in *.{c,h};										\
	do														\
		if test -h $$f; then rm -f $$f; fi;					\
	done;													
	rm -f rm11.h rm11.c rm11Impl.c

symlinks:
	@if test ! -f ./Orbe.h; then							\
		for f in $(TRANSPORT_DIR)/*.{c,h};					\
		do													\
			echo "symlinking... $$f";						\
			ln -s $$f . ;									\
		done;												\
		\
		for f in $(ORB_DIR)/*.{c,h};						\
		do													\
			echo "symlinking... $$f";						\
			ln -s $$f . ;									\
		done;												\
		for f in $(CONFIG_DIR)/*.{c,h};						\
		do													\
			echo "symlinking... $$f";						\
			ln -s $$f . ;									\
		done;												\
	fi;	

conv: 
	@for f in *.{c,h};										\
	do														\
		sed '/DEBUG(.*);/s/\\n/\\r\\n/' < $$f > $$f.sed;	\
		mv $$f $$f.orig;									\
		mv $$f.sed $$f;										\
	done;

undo:
	@for f in *.orig;										\
	do														\
		name=`expr substr $$f 1 \\( length $$f - 5 \\)`;	\
		mv -f $$f $$name;									\
	done;
