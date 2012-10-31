/*
 * $Id: basic_types.h 858 2008-10-22 07:41:12Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#ifndef BASIC_TYPES_H_
#define BASIC_TYPES_H_

#if defined(__MICROBLAZE__)
#	include <xbasic_types.h>
#elif defined(__PPC__)
#	include <xbasic_types.h>
#else
#	include "xbasic_types.h" 
#endif

#endif /*BASIC_TYPES_H_*/
