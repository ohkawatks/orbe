/*
 * $Id: Emac.h 801 2008-09-24 02:28:02Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#ifndef EMAC_H_
#define EMAC_H_

#include "basic_types.h" 

int	Emac_initialize();
unsigned int Emac_send(u8* start, unsigned int length);
unsigned int Emac_recv(u8* start);

u8* 	Emac_getMAC();
void 	Emac_setMAC(u8* mac);
int		Emac_setupDevice(int emaclite_device_id, int intc_device_id, int intc_emaclite_vec_id);

#endif /*EMAC_H_*/
