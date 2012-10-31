
#ifndef CAN_H_
#define CAN_H_

#include 	<stdio.h>
#include	"can_drv.h"

#define Socket_initialize 	Can_initialize
#define Socket_send 		Can_send
#define Socket_recv 		Can_recv

#define GIOP_HDR 12

void 	Can_initialize(void);
size_t	Can_send(CAN_U8* start, size_t length);
size_t	Can_recv(CAN_U8* start, size_t length);
CAN_U32 getLength();
void CanDebug(CAN_U8* data, size_t length);

#endif /* CAN_H_ */
