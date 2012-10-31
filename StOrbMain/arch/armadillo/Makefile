#
# $Id: Makefile 1020 2009-02-04 00:21:26Z hideki $
# Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology 
#
CC = arm-linux-gcc
#CC = gcc

TYPE = -DSOCKET_CHANNEL_USE_UDP_LINUX

ENDIAN = -DENDIAN=1

INCLUDE = -I. 

CFLAGS = -g -O2 $(INCLUDE) $(TYPE) $(ENDIAN) -DUDP_DEBUG -D__ARM__ # -DGIOP_FULL_ANALYSIS

PC_DIR = ../pc
ORB_DIR = ../../src/orbe
TRANSPORT_DIR = ../../src/transport
CONFIG_DIR = ../../src/config
IDLC_DIR = ../../idlc
IDL = ../../idl/rm11.idl

HEADERS = CORBA_Object.h Giop.h Orbe.h OTable.h Socket.h xbasic_types.h OrbeConfig.h UdpLinux.h rm11.h
SRCS = main.c Giop.c Orbe.c OrbeConfig.c OTable.c Timer.c UdpLinux.c rm11.c rm11Impl.c log.c swapd.c
OBJS = main.o Giop.o Orbe.o OrbeConfig.o OTable.o Timer.o UdpLinux.o rm11.o rm11Impl.o log.o swapd.o

.PRECIOUS: $(HEADERS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

all: 
	make idl
	make symlinks
	make objs
	$(CC) -o Orbe $(CFLAGS) $(OBJS)

objs: $(HEADERS) $(OBJS)

idl: $(IDL)
	(cd $(IDLC_DIR); make)
	env RUBYLIB=$(IDLC_DIR) ruby $(IDLC_DIR)/main.rb $(IDL)

clean:
	@rm -f Orbe *.o *.bak *.o

cleandata:
	@rm -f data.bin

distclean: clean cleandata
	@for f in *.{c,h};										\
	do														\
		if test -h $$f; then rm -f $$f; fi;					\
	done;

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
		for f in $(PC_DIR)/*.{c,h};						\
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
