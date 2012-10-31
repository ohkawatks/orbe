/*
 * $Id$
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

/*
 * took from Ruby. Thank you, Ruby!
 */
int endian()
{
	static int init = 0;
	static int endian = 0;
	char *p;

	if (init) return endian;
	init = 1;
	p = &init;
	return endian = p[0]?1:0;
}
