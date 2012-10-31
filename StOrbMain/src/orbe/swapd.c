/*
 * $Id: swapd.c 975 2008-12-17 01:34:13Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */
#include	<string.h>
#include	"Orbe.h"
#include	"swapd.h"

/*
 * took this function from Ruby. Thank you for Ruby!
 */
double swapd(const double d)
{
	double dtmp = d;
	unsigned long utmp[2];
	unsigned long utmp0;

	utmp[0] = 0; utmp[1] = 0;
	memcpy(utmp,&dtmp,sizeof(double));
	utmp0 = utmp[0];
	utmp[0] = swapl(utmp[1]);
	utmp[1] = swapl(utmp0);
	memcpy(&dtmp,utmp,sizeof(double));
	return dtmp;
}

#ifdef __ARM__
double swapDoubleOutput(const double d)
{
	double dtmp = d;
	unsigned long utmp[2];
	unsigned long utmp0;

	utmp[0] = 0; utmp[1] = 0;
	memcpy(utmp,&dtmp,sizeof(double));
	utmp0 = utmp[0];
	utmp[0] = utmp[1];
	utmp[1] = utmp0;
	memcpy(&dtmp,utmp,sizeof(double));
	return dtmp;
}
#endif


