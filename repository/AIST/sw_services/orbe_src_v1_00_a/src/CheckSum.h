/*
 * $Id: CheckSum.h 801 2008-09-24 02:28:02Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#ifndef CHECKSUM_H_
#define CHECKSUM_H_

#include <basic_types.h> 

u16 Checksum_calc(u8* head, unsigned int length);

#endif /*CHECKSUM_H_*/
