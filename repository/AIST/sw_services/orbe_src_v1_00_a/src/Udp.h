/*
 * $Id: Udp.h 829 2008-10-09 06:29:11Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#ifndef UDP_H_
#define UDP_H_

#include <stdio.h>
#include "basic_types.h"
#include "OrbeConfig.h"

#define UDP_HEADER_LEN 		8
#define IP_HEADER_LEN 		20
#define ETHER_HEADER_LEN 	14
#define ALL_HEADER_LEN (UDP_HEADER_LEN + IP_HEADER_LEN + ETHER_HEADER_LEN)

#if ENDIAN == 0
// BIG Endian
#define PROTO_TYPE_ARP	0x0806
#define PROTO_TYPE_IP	0x0800
#else
// LITTLE Endian
#define PROTO_TYPE_ARP	0x0608
#define PROTO_TYPE_IP	0x0008
#endif

#define PROTO_TYPE_UDP	0x11

#define IP_VERSION_HLENGTH 0x45
#define IP_TTL 0x80

#define SIZE_IP_ADDR 4
#define SIZE_ETHERNET_ADDR 6

#define UDP_BUFF_LEN 1518

#define Socket_initialize Udp_initialize
#define Socket_send Udp_send
#define Socket_recv Udp_recv

typedef struct {
	u16 source_port;
	u16 destination_port;
	u16 length;
	u16 checksum;
} Udp_header;

typedef struct {
	u8 version_and_header_length; // 4bit + 4bit
	u8 tos;
	u16 total_length;
	u16 identification;
	u16 flags_and_fragment_offset; // 3bit + 13bit
	u8 ttl;
	u8 protocol;
	u16 checksum;
	u8 source_ip[SIZE_IP_ADDR];
	u8 destination_ip[SIZE_IP_ADDR];	
} Ip_header;

typedef struct {
	u8 destination_mac[SIZE_ETHERNET_ADDR];
	u8 source_mac[SIZE_ETHERNET_ADDR];
	u16 type;
} Ethernet_header;

typedef struct {
	Ethernet_header 	e_header;
	Ip_header 			i_header;
	Udp_header 			u_header;
} All_header;

void	Udp_initialize();
size_t 	Udp_send(u8* start, size_t length);
size_t 	Udp_recv(u8* start, size_t length);

#endif /*UDP_H_*/
