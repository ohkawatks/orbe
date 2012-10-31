/*
 * $Id: StdInOut.h 828 2008-10-09 04:48:22Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#ifndef STDINOUT_H_
#define STDINOUT_H_

#define Socket_initialize StdInOut_initialize
#define Socket_send StdInOut_send
#define Socket_recv StdInOut_recv

void 	StdInOut_initialize(void);
size_t 	StdInOut_send(u8* start, unsigned int length);
size_t	StdInOut_recv(u8* start, unsigned int length);

#endif /* STDINOUT_H_ */
