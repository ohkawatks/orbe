/*
 * rm11.c
 * input file	: ../idl/rm11.idl
 * generated at : Fri Mar 30 10:13:02 +0900 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CORBA_Object.h"
#include "Giop.h"
#include "Orbe.h"
#include "rm11.h"


void dispatch(Req * req, Reply * reply, Giop_Hdr * header)
{
    u8 *req_ptr;
    u8 *reply_ptr;
    u8 *start;

    req_ptr = req->body;
    reply_ptr = start = reply->body;

    if (strcmp((void *) req->operation, "op1") == 0) {
	CORBA_short argin;
	CORBA_short argout;
	CORBA_short arginout;

	argin = Orbe_exchangeShort(*(u16 *) req_ptr);
	req_ptr += sizeof(CORBA_short);
	arginout = Orbe_exchangeShort(*(u16 *) req_ptr);
	req_ptr += sizeof(CORBA_short);

	CORBA_short result =
	    rm11_op1(NULL, argin, &argout, &arginout, NULL);

	memcpy(reply_ptr, (u8 *) & result, 2);
	reply_ptr += sizeof(CORBA_short);
	memcpy(reply_ptr, (u8 *) & argout, 2);
	reply_ptr += sizeof(CORBA_short);
	memcpy(reply_ptr, (u8 *) & arginout, 2);
	reply_ptr += sizeof(CORBA_short);

    }

    reply->body_size = (size_t) (reply_ptr - start);
}
