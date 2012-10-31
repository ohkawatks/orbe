/*
 * $Id: swapd.h 975 2008-12-17 01:34:13Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */
#ifndef	_SWAPD_H_
#define _SWAPD_H_
double swapd(const double d);
#ifdef __ARM__
double swapDoubleOutput(const double d);
#else
#define swapDoubleOutput(x) (x)
#endif
#endif
