/*
 * $Id: UdpLinux.h 1063 2009-02-09 02:24:33Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>

#define UDP_PORT 5656
#define BUF_SIZE 1024

#define Socket_initialize UdpLinux_initialize
#define Socket_send UdpLinux_send
#define Socket_recv UdpLinux_recv

void 	UdpLinux_initialize();
size_t 	UdpLinux_send(unsigned char* start, unsigned int length);
size_t 	UdpLinux_recv(unsigned char* start, unsigned int length);
