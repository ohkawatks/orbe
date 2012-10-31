/*
 * $Id: Arp.c 879 2008-11-04 08:07:44Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#include <stdio.h>
#include <string.h>
#include "Udp.h"
#include "Arp.h"
#include "Emac.h"
#include "Orbe.h"

static u8 mbIPx[4] = {0xc0, 0xa8, 0x01, 0x0a};	/* default */

u8 *Arp_getIP()
{
	return mbIPx;
}

void Arp_setIP(u8* ipv4)
{
	mbIPx[0] = ipv4[0];
	mbIPx[1] = ipv4[1];
	mbIPx[2] = ipv4[2];
	mbIPx[3] = ipv4[3];
	DEBUG("Arp_setIP : set IP address = %03d.%03d.%03d.%03d", mbIPx[0], mbIPx[1], mbIPx[2], mbIPx[3]);
	return;
}

u8 *Arp_init()
{
	static int init = 0;
	static u8 reply[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	/* Ether dest */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	/* Ether src */
		0x08, 0x06,							/* arp */
		0x00, 0x01,							/* Hardware(Ether) */
		0x08, 0x00,							/* Protocol(IP) */
		0x06, 0x04,							/* size(H)/size(P) */
		0x00, 0x02,							/* Reply */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	/* sender */
		0x00, 0x00, 0x00, 0x00,				/* sender */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	/* target */
		0x00, 0x00, 0x00, 0x00,				/* target */
	};

	if (!init) {
		init = 1;
		memcpy(reply + 6,  Emac_getMAC(),  6);
		memcpy(reply + 22, Emac_getMAC(),  6);
		memcpy(reply + 28, Arp_getIP(),    4);
	}
	return reply;
}

void Arp_reply(u8 *buff)
{
	if (memcmp(Arp_getIP(), buff + ETHER_HEADER_LEN + 24, 4) != 0) {
		DEBUG("Arp_reply : IP unmatch");
		return;
	}

	/*
	 * Arp_init() will allocate storage area.
	 */
	u8 *reply = Arp_init();
	memcpy(reply + 0, buff + 6, 6);
	memcpy(reply + ETHER_HEADER_LEN + 18, buff + ETHER_HEADER_LEN + 8, 10);

#ifdef UDP_DEBUG
	{
		int i;
		xil_printf("Arp data = ");
		for (i = 0; i < ETHER_HEADER_LEN + ARP_INFO_LEN; i++) {
			xil_printf("%02x ", arp_buff[i]);
		}
		xil_printf("\r\n");
	}
#endif

	Emac_send(reply, ETHER_HEADER_LEN + ARP_INFO_LEN);
}
