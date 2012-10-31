/*
 * $Id: Arp.h 879 2008-11-04 08:07:44Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#ifndef ARP_H_
#define ARP_H_

#include "basic_types.h"
#include "Udp.h"

#define ARP_INFO_LEN 28

void 	Arp_reply(u8 *whohas);
u8		*Arp_getIP();
void	Arp_setIP(u8* ipv4);

#endif /*ARP_H_*/
