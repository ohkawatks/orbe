/*
 * $Id: CheckSum.c 801 2008-09-24 02:28:02Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#include "CheckSum.h"

u16 Checksum_calc(u8* head, unsigned int length) {
	u32 work = 0;
	u16 tmp = 0, a;
	int i;
#ifdef TIMER
	Timer("Checksum_calc start");
#endif	
	for (i = 0; i < length;) {
		tmp = head[i++];
		tmp = tmp << 8;
		tmp += head[i++];
		
		work += tmp;
	}
	
	a = work / 65536;
	work = (work & 0xFFFF) + a;
	
	a = work / 65536;
	work += a;
	
	a = (u16)(work & 0xFFFF);
	
	a = ~a;
	
#ifdef TIMER
	Timer("Checksum_calc end");
#endif
	return a;
}
