/*
 * LED.c
 * input file	: LED.idl
 * generated at : Mon Feb 09 15:09:24 +0900 2009
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CORBA_Object.h"
#include "Giop.h"
#include "Orbe.h"
#include "LED.h"


void dispatch(Req *req, Reply *reply, Giop_Hdr *header)
{
    u8      *req_ptr;
    u8      *reply_ptr;
    u8      *start;

    req_ptr = req->body;
    reply_ptr = start = reply->body;

if (strcmp((void*)req->operation, "turnOn") == 0) {
	CORBA_long ID;

ID = Orbe_exchangeLong(*(u32*)req_ptr);
req_ptr += sizeof(CORBA_long);

void result = LED_turnOn(NULL, ID,  NULL);


}
if (strcmp((void*)req->operation, "turnOff") == 0) {
	CORBA_long ID;

ID = Orbe_exchangeLong(*(u32*)req_ptr);
req_ptr += sizeof(CORBA_long);

void result = LED_turnOff(NULL, ID,  NULL);


}
if (strcmp((void*)req->operation, "getState") == 0) {
	CORBA_long ID;

ID = Orbe_exchangeLong(*(u32*)req_ptr);
req_ptr += sizeof(CORBA_long);

CORBA_boolean result = LED_getState(NULL, ID,  NULL);

*reply_ptr = result;
reply_ptr += sizeof(CORBA_boolean);

}

    reply->body_size = (size_t) (reply_ptr - start);
}

