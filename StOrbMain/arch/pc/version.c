/*
 * $Id: version.c 859 2008-10-22 09:40:58Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

#include	"Orbe.h"
#include	"version.h"

#define		VERSION		"ORBEngine Copyright (C) 2008 AIST"

void version()
{
	char	buffer[256];

	build_version(buffer);
	fprintf(stderr, "%s", buffer);

	exit(-1);
}

void debug_version()
{
	char	buffer[256];

	build_version(buffer);
	DEBUG(buffer);
}

void build_version(char *bp)
{

	strcpy(bp, VERSION);

#if defined(SOCKET_CHANNEL_USE_UDP)
	strcat(bp, " UDP");
#elif defined(SOCKET_CHANNEL_USE_TCP)
	strcat(bp, " TCP");
#elif defined(SOCKET_CHANNEL_USE_STDINOUT)
	strcat(bp, " STDINOUT");
#endif

#if ENDIAN == 1
	strcat(bp, " ENDIAN=1");
#else
	strcat(bp, " ENDIAN=0");
#endif

#if defined(ENABLE_CHECK_REQUEST_ID)
	strcat(bp, " ENABLE_CHECK_REQUEST_ID");
#else
	strcat(bp, " ");
#endif

#if defined(GIOP_FULL_ANALYSIS)
	strcat(bp, " GIOP_FULL_ANALYSIS");
#endif
	strcat(bp, "\n");
}
