#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>

#include    "Socket.h"
#include    "Giop.h"
#include    "Orbe.h"

void dispatch() {
		        return;
}

int
main()
{
	char        *body = NULL;
	int         body_size = 0;
	size_t      length;
	Req         request;
	Locate_Req  locate_request;
	Socket_initialize();
	Orbe_initBuffer();                              // to clean buffer
	Orbe_initReq(&request);                         // minimum build up
	Orbe_initLocateReq(&locate_request);            // minimum build up
	Orbe_setOperation(&request, "op1", body, body_size);    // set operation and arguments

	/* put locate_request into request_buffer */
	length = Orbe_writeLocateReq(&locate_request);  // write into buffer
	Orbe_flush(length);                             // go out

	/* put request into request_buffer */
	length = Orbe_writeReq(&request);               // write into buffer
	Orbe_flush(length);                             // go out
	return 0;
}
