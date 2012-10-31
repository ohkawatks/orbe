
#ifndef ECUCAN_H_
#define ECUCAN_H_

#include "basic_types.h"
#include <xcan.h>
#include <xparameters.h>
#include <xstatus.h>
#include <stdio.h>
#include <string.h>

#define Socket_initialize 	EcuCan_initialize
#define Socket_send 		EcuCan_send
#define Socket_recv 		EcuCan_recv

#define GIOP_HDR 12

void 	EcuCan_initialize(void);
size_t	EcuCan_send(u8* start, size_t length);
size_t	EcuCan_recv(u8* start, size_t length);
static XStatus SendFrame(XCan *InstancePtr, Xuint8 send_length, Xuint8 *send_buff);
static XStatus RecvFrame(XCan *InstancePtr, Xuint32	*canID, Xuint32 *dlc, Xuint32 current_length);
Xuint32	getLength();


#endif /* ECUCAN_H_ */
