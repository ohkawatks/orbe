#
# $Id: Makefile 1020 2009-02-04 00:21:26Z hideki $
# Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
#

TRANSPORT_DIR = ../../src/transport
ORBE_DIR = ../../src/orbe
PC_DIR = ../pc
TARGET_DIR = Debug/SRC

TRANSPORT_SRC = Can.c Can.h Socket.h
ORBE_SRC = basic_types.h CORBA_Object.h Giop.c Giop.h Orbe.c Orbe.h OTable.c \
		OTable.h swapd.c swapd.h
PC_SRC = xbasic_types.h

all: bitspot

bitspot:
	@for f in $(TRANSPORT_SRC);					\
	do											\
		cp $(TRANSPORT_DIR)/$$f $(TARGET_DIR);	\
	done;										\
	for f in $(ORBE_SRC);						\
	do											\
		cp $(ORBE_DIR)/$$f $(TARGET_DIR);		\
	done;										\
	for f in $(PC_SRC);							\
	do											\
		cp $(PC_DIR)/$$f $(TARGET_DIR);			\
	done

