/*
 * Board.c
 * input file	: Board.idl
 * generated at : Tue May 22 11:39:20 +0900 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CORBA_Object.h"
#include "Giop.h"
#include "Orbe.h"
#include "Board.h"


void dispatch(Req * req, Reply * reply, Giop_Hdr * header)
{
    u8 *req_ptr;
    u8 *reply_ptr;
    u8 *start;

    req_ptr = req->body;
    reply_ptr = start = reply->body;

    if (strcmp((void *) req->operation, "initialize") == 0) {


	void result = Board_initialize(NULL, NULL);


    }
    if (strcmp((void *) req->operation, "getStone") == 0) {
	CORBA_octet x;
	CORBA_octet y;

	x = *req_ptr;
	req_ptr += sizeof(CORBA_octet);
	y = *req_ptr;
	req_ptr += sizeof(CORBA_octet);

	CORBA_octet result = Board_getStone(NULL, x, y, NULL);

	*reply_ptr = result;
	reply_ptr += sizeof(CORBA_octet);

    }
    if (strcmp((void *) req->operation, "putStone") == 0) {
	CORBA_octet x;
	CORBA_octet y;
	CORBA_octet stoneType;

	x = *req_ptr;
	req_ptr += sizeof(CORBA_octet);
	y = *req_ptr;
	req_ptr += sizeof(CORBA_octet);
	stoneType = *req_ptr;
	req_ptr += sizeof(CORBA_octet);

	void result = Board_putStone(NULL, x, y, stoneType, NULL);


    }
    if (strcmp((void *) req->operation, "checkConnect6") == 0) {


	CORBA_octet result = Board_checkConnect6(NULL, NULL);

	*reply_ptr = result;
	reply_ptr += sizeof(CORBA_octet);

    }
    if (strcmp((void *) req->operation, "checkPattern6") == 0) {
	CORBA_octet pattern;

	pattern = *req_ptr;
	req_ptr += sizeof(CORBA_octet);

	CORBA_octet result = Board_checkPattern6(NULL, pattern, NULL);

	*reply_ptr = result;
	reply_ptr += sizeof(CORBA_octet);

    }
    if (strcmp((void *) req->operation, "popLastPosition") == 0) {
	CORBA_octet x;
	CORBA_octet y;
	CORBA_octet direction;


	void result =
	    Board_popLastPosition(NULL, &x, &y, &direction, NULL);

	*reply_ptr = x;
	reply_ptr += sizeof(CORBA_octet);
	*reply_ptr = y;
	reply_ptr += sizeof(CORBA_octet);
	*reply_ptr = direction;
	reply_ptr += sizeof(CORBA_octet);

    }

    reply->body_size = (size_t) (reply_ptr - start);
}
