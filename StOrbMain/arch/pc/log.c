/*
 * $Id: log.c 567 2008-08-05 03:23:38Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology 
 */
#include <stdio.h>
#include <stdarg.h>

// See <sys/syslog.h>
#define	LOG_DEBUG	7

void
log_debug(char * fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
}
