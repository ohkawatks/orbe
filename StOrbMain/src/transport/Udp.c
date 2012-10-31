/*
 * $Id: Udp.c 879 2008-11-04 08:07:44Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#include "Udp.h"
#include "Emac.h"
#include "Arp.h"
#include "CheckSum.h"
#include "Orbe.h"
#include <string.h>
#include <stdio.h>

static All_header send_header;
static u8 buff[UDP_BUFF_LEN];
	
void Udp_initialize()
{
	Emac_initialize();
}

/*
 * Create header based on previously received packet
 *
 */
void Udp_createHeader(unsigned int length)
{
	Udp_header		*s_udp_header;
	Ip_header		*s_ip_header;
	Ethernet_header	*s_ethernet_header;
	
	Timer("Create_header start");
	
	s_udp_header = &send_header.u_header;
	s_ip_header = &send_header.i_header;
	s_ethernet_header = &send_header.e_header;
	
	Udp_header *udp = (Udp_header*)(buff + sizeof(Ethernet_header) + sizeof(Ip_header));
	s_udp_header->source_port      = udp->destination_port;
	s_udp_header->destination_port = udp->source_port;
	s_udp_header->length           = UDP_HEADER_LEN + length;
	s_udp_header->checksum         = 0x0000; // ignore check sum
	
	// IP header
	Ip_header *ip = (Ip_header*)(buff + sizeof(Ethernet_header));
	s_ip_header->version_and_header_length = IP_VERSION_HLENGTH;
	s_ip_header->tos                       = 0x00;									// not necessary
	s_ip_header->total_length              = (IP_HEADER_LEN + UDP_HEADER_LEN + length);
	s_ip_header->identification            = ip->identification + 1;
	s_ip_header->flags_and_fragment_offset = 0x0000;								// no flagmentation
	s_ip_header->ttl                       = IP_TTL;
	s_ip_header->protocol                  = PROTO_TYPE_UDP;
	s_ip_header->checksum                  = 0x0000;								// initialize (required to calculate chksum)
	DEBUG("%x, %x", s_ip_header->source_ip, ip->destination_ip);
	memcpy(s_ip_header->source_ip, ip->destination_ip, SIZE_IP_ADDR);
	memcpy(s_ip_header->destination_ip, ip->source_ip, SIZE_IP_ADDR);

	s_ip_header->checksum = Checksum_calc((u8*)s_ip_header, sizeof(Ip_header));
	
	// Ethernet header
	Ethernet_header *ether = (Ethernet_header*)buff;
	memcpy(s_ethernet_header->destination_mac, ether->source_mac,      SIZE_ETHERNET_ADDR);
	memcpy(s_ethernet_header->source_mac,      ether->destination_mac, SIZE_ETHERNET_ADDR);
	s_ethernet_header->type = PROTO_TYPE_IP;
	
#ifdef UDP_DEBUG
	{
		xil_printf("Udp destination port = %d\r\n", s_udp_header->destination_port);
		xil_printf("Udp source port = %d\r\n", s_udp_header->source_port);
		xil_printf("Udp data length = %d\r\n", s_udp_header->length);
	}
#endif
	Timer("Create_header end");
}

size_t Udp_send(u8* start, size_t length)
{
	if ( length > (UDP_BUFF_LEN - ALL_HEADER_LEN)) { 
		DEBUG("Udp_send : length is too big. length = %d", length);
		return 0;
	}
	
	start -= ALL_HEADER_LEN;
	
	Timer("Udp_send start");

	Udp_createHeader(length);
	memcpy(start, &send_header, ALL_HEADER_LEN);
	
#ifdef UDP_DEBUG
	{
		int i;
		xil_printf("send data = ");
		for (i = 0; i < (ALL_HEADER_LEN + length); i++)
			xil_printf("%02x ", start[i]);
		xil_printf("\r\n");
	}
#endif	
	Emac_send(start, (ALL_HEADER_LEN + length));

	Timer("Udp_send end");

	return length; 
}

size_t Udp_recv(u8* start, size_t length)
{
	static unsigned int remains = 0;
	static u8 *pos;

	Timer("Udp_recv start");
	while (remains <= 0) {
		DEBUG("Udp_recv: waiting data coming...");

		Emac_recv(buff); // wait for data (block)
		Ethernet_header *ether = (Ethernet_header*)buff;
		pos = buff + ALL_HEADER_LEN;

		if (ether->type == PROTO_TYPE_ARP) { // ARP?
			DEBUG("Udp_recv: get ARP");
			Arp_reply(buff);
			continue;
		}
		if (ether->type != PROTO_TYPE_IP) { // IP?
			//xil_printf("######not IP, discard\r\n");
			continue; // not IP, discard
		}
		Ip_header *ip = (Ip_header*)(buff+sizeof(Ethernet_header));
		if (ip->protocol != PROTO_TYPE_UDP) { // UDP?
			//xil_printf("######not UDP, discard\r\n");
			continue; // not UDP, discard
		}
		if (memcmp(ether->destination_mac, Emac_getMAC(), 6) != 0) {
			//xil_printf("######differnt MAC, discard\r\n");
			continue; // differnt MAC, discard
		}
		Udp_header *udp = (Udp_header*)(buff + sizeof(Ethernet_header) + sizeof(Ip_header));
		if (udp->length > UDP_BUFF_LEN - ETHER_HEADER_LEN - IP_HEADER_LEN) {
			DEBUG("Udp_recv : too big datagram = %d", udp->length);
			continue; // Too big packet, discard
		}
		if (udp->length < UDP_HEADER_LEN) {
			DEBUG("Udp_recv : abnormal udp length. total = %d", udp->length);
			continue;
		}

		remains = udp->length - UDP_HEADER_LEN;
#ifdef UDP_DEBUG
		{
			int i;
			int length = ETHER_HEADER_LEN + IP_HEADER_LEN + udp->length;
			xil_printf("recv data = ");
			for (i = 0; i < length; i++)
				xil_printf("%02x ", buff[i]);
			xil_printf("\r\n");
			xil_printf("Udp destination port = %d\r\n", udp->destination_port);
			xil_printf("Udp source port = %d\r\n", udp->source_port);
			xil_printf("Udp length = %d\r\n", udp->length);
		}
#endif
	}
	if (length > remains) {
		DEBUG("Udp_recv : length = %d remains = %d", length, remains);
		return 0;
	}
	memcpy(start, pos, length);
	pos += length;
	remains -= length;

	Timer("Udp_recv end");
	return length;
}
