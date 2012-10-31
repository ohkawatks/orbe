/*
 * $Id: Orbe.c 1093 2009-03-16 05:39:48Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology 
 */
#include <stdio.h>
#include <limits.h>
#include <string.h>
#if defined(__PPC__) || defined(__MICROBLAZE__) || defined(__BITSPOT__)
/*
 * Xilinx EDK environment doesn't provide strings.h which has the prototype of bzero().
 * We don't know the reason.
 */
extern void bzero (void *__s, size_t __n);
#else
#include <strings.h>
#endif

#include "Socket.h"
#include "Giop.h"
#include "Orbe.h"

extern void dispatch(Req *req, Reply *reply, Giop_Hdr *header);

#ifdef __ARM__
static u8		request_buffer[ORBE_BUFFER_SIZE] __attribute__ ((aligned(4)));						/* for incomming messages */
static u8		reply_message[BEGINNING_OFFSET + ORBE_BUFFER_SIZE] __attribute__ ((aligned(4)));		/* for outgoing messages  */
static u8*		reply_buffer __attribute__ ((aligned(4)));
#else
static u8		request_buffer[ORBE_BUFFER_SIZE];						/* for incomming messages */
static u8		reply_message[BEGINNING_OFFSET + ORBE_BUFFER_SIZE];		/* for outgoing messages  */
static u8*		reply_buffer;
#endif

static int		Orbe_debugFlag;

#ifdef __BITSPOT__
void bzero (void *__s, size_t __n) {}
#endif

void Orbe_add(CORBA_Object o)
{
	OTable_add(o);
}

void Orbe_initBuffer()
{
	u8 *p = request_buffer;

	DEBUG("Orbe_initBuffer:");

	*p++ = 'G';
	*p++ = 'I';
	*p++ = 'O';
	*p++ = 'P';
	*p++ = 0x01;
	*p++ = 0x02;
	*p++ = ENDIAN;
}

size_t Orbe_flush(size_t length)
{
	DEBUG("Orbe_flush:");
	Socket_send(request_buffer, length);
	return length;
}

size_t Orbe_writeReq(const Req *req)
{
	DEBUG("Orbe_writeReq:");
	return Giop_writeReq(request_buffer, req);
}

size_t Orbe_writeLocateReq(const Locate_Req *locate_request)
{
	DEBUG("Orbe_writeLocateReq:");
	return Giop_writeLocateReq(request_buffer, locate_request);
}

void Orbe_setOperation(Req *request, u8 *op, u8 *body, u8 body_size)
{
	DEBUG("Orbe_setOperation:");
	memcpy(request->operation, op, strlen(op)+1);
	request->body_size = body_size;
	request->body = body;
}

Locate_Req *Orbe_initLocateReq(Locate_Req *locate_request)
{
	DEBUG("Orbe_initLocateReq:");
	locate_request->request_id = Giop_getRequestId();
	locate_request->target.index = 0;
	memcpy(locate_request->target.object_key, "OBJECT_KEY", strlen("OBJECT_KEY") + 1);

	return locate_request;
}

Req *Orbe_initReq(Req *req)
{
	DEBUG("Orbe_initLReq:");
	req->request_id = Giop_getRequestId();
	req->response_flags = 0x3;
	req->target.index = 0;
	memcpy(req->target.object_key, "OBJECT_KEY", strlen("OBJECT_KEY") + 1);
	*(CORBA_long*)req->service_context = 0; /* zero sc */
	req->body_size = 0;
	req->body      = NULL;

	return req;
}

void Orbe_setObjectKey(CORBA_Object o, void* objectKey)
{
	DEBUG("Orbe_setObjectKey:");
	strncpy(o->object_key, objectKey, 255);
}

size_t Orbe_getRefSize(u8 *value, Giop_Hdr *header)
{
	size_t			length;
	unsigned int	num = 0;
	int				i;

	length = 4;																	/* the area of the length of type_id */
	length += (*(CORBA_long*)value = Orbe_exchangeInt(*(CORBA_long*)value));	/* the length of type_id */
	length += ALIGN(4, length);

	num = *(CORBA_long*)(value + length)
			= Orbe_exchangeInt(*(CORBA_long*)(value + length));			/* the number of TaggedProfiles */
	length += 4;												/* the area of the length of TaggedProfiles */
	for (i = 0; i < num; i++) {
		length += 4;	/* ProfileId */
		length += (*(CORBA_long*)(value + length) = Orbe_exchangeInt(*(CORBA_long *)(value + length)));
		length += 4;	/* space of the length of Profile */
	}
	return length;
}

int Orbe_readHeader(Giop_Hdr *header)
{
	u8 *p;
	int len = Socket_recv((u8 *)header, GIOP_HEADER_LENGTH); /* BLOCKED */
	if (len != GIOP_HEADER_LENGTH) {
		DEBUG("Orbe_readHeader: header is broken.");
		return -1;
	}

	p = header->magic;
	if ( *p++ != 'G' || *p++ != 'I' || *p++ != 'O' || *p++ != 'P') {
		DEBUG("Orbe_readHeader: magic \"GIOP\" not found.");
		return -1;
	}
	
	if (1 != header->majorVersion || 2 != header->minorVersion) {
		DEBUG("Orbe_readHeader: version mismatch.");
		header->type = GIOP_ORBE_PROTOCOLVERSION_MISMATCH;
	}

	if (ENDIAN != header->endian) {
		DEBUG("Orbe_readHeader: set marshalling enable.");
		header->marshalling = TRUE;
	} else {
		DEBUG("Orbe_readHeader: set marshalling disable.");
		header->marshalling = FALSE;
	}
	return Orbe_exchangeLong(header->size);
}

int Orbe_startServer()
{
	Giop_Hdr			h;
	Giop_Hdr			*header;
	Req					*req;					/* incoming */
	Locate_Req			*locate_req;			/* incoming */

	Reply_Message		reply_msg;				/* outgoing */

	u32					id;
	static u32			replied_request_id = ULONG_MAX;		/* use for request_id check */

	header = &h;
	reply_buffer = reply_message + BEGINNING_OFFSET;

#ifndef SOCKET_CHANNEL_USE_UART
	Socket_initialize();	/* discard buffer */
#endif

	do {
#ifdef SOCKET_CHANNEL_USE_UART
		Socket_initialize(); /* discard buffer */
#endif		
		int msg_size;
		int readlimit;

		readlimit = msg_size = Orbe_readHeader(header);
		if (msg_size < 0) {
			DEBUG("Orb_startServer:received broken packets.");
			continue;
		}

		if (msg_size > ORBE_BUFFER_SIZE) {
			readlimit = ORBE_BUFFER_SIZE;
			header->type = GIOP_ORBE_SERVERSIZE_ERROR;
			DEBUG("Orb_startServer:received message is too big.");
		}
		Socket_recv(request_buffer, readlimit);
		msg_size -= readlimit;
		id = Orbe_exchangeLong(*(u32*)request_buffer);

		/*
		 * skip reading
		 */
		if (msg_size) {		/* still data remain */
			int i;
			for (i = 0; i < msg_size / ORBE_BUFFER_SIZE; i++) {
				Socket_recv(request_buffer, readlimit);
			}
			Socket_recv(request_buffer, msg_size % ORBE_BUFFER_SIZE);
		}

#ifdef ENABLE_CHECK_REQUEST_ID
		if (id <= replied_request_id && replied_request_id != ULONG_MAX) {	/* probably duplicated message */
			DEBUG("Orb_startServer:received duplicated message.");
			continue;
		}
#endif
		switch (header->type) {
		case GIOP_MSGTYPE_REQUEST:
			req = Giop_readRequest(request_buffer, header);
			reply_msg.type = GIOP_MSGTYPE_REPLY;
			reply_msg.message.reply = Giop_createReply(reply_buffer, req);

			dispatch(req, reply_msg.message.reply, header);
			break;
			
		case GIOP_MSGTYPE_REPLY:
			reply_msg.type = GIOP_MSGTYPE_MESSAGEERROR;
			break;
			
		case GIOP_MSGTYPE_CANCELREQUEST:
			reply_msg.type = GIOP_MSGTYPE_MESSAGEERROR;
			break;
			
		case GIOP_MSGTYPE_LOCATEREQUEST:
			locate_req = Giop_readLocateRequest(request_buffer, header);
			reply_msg.type = GIOP_MSGTYPE_LOCATEREPLY;
			reply_msg.message.locate_reply = Giop_createLocateReply(reply_buffer, locate_req);
			break;

		case GIOP_MSGTYPE_LOCATEREPLY:
			reply_msg.type = GIOP_MSGTYPE_MESSAGEERROR;
			break;
				
		case GIOP_MSGTYPE_CLOSECONNECTION:
			reply_msg.type = GIOP_MSGTYPE_MESSAGEERROR;
			break;
				
		case GIOP_MSGTYPE_MESSAGEERROR:
			DEBUG("Orb_startServer:caught GIOP_MSGTYPE_MESSAGEERROR packets.");
			continue;	/* not reply to error message */
			break;		/* just in case for case block :) */
				
		case GIOP_MSGTYPE_FRAGMENT:
			reply_msg.type = GIOP_MSGTYPE_MESSAGEERROR;
			break;
				
		case GIOP_ORBE_SERVERSIZE_ERROR:						/* these are ORBE special values */
		case GIOP_ORBE_PROTOCOLVERSION_MISMATCH:
			reply_msg.type = GIOP_MSGTYPE_MESSAGEERROR;
			break;

		default:
			reply_msg.type = GIOP_MSGTYPE_MESSAGEERROR;
			break;
		}
		Orbe_respond(reply_buffer, reply_msg);
		bzero(reply_buffer, sizeof(reply_buffer));
		replied_request_id = id;
#ifndef __BITSPOT__
		Show_Log();
#endif
	} while( Orbe_isAlive() );
	return 0;
}

u32 Orbe_respond(u8 *buffer, Reply_Message reply_msg)
{
	u32		size;
	u8		*p;
	u8		*body;

	body = buffer + GIOP_HEADER_LENGTH;
	switch (reply_msg.type) {
	case GIOP_MSGTYPE_REPLY:
		DEBUG("Orbe_respond: send GIOP_MSGTYPE_REPLY");
		size = Giop_respondReply(body, reply_msg.message.reply);
		DEBUG("Orbe_respond: reply message size=%d(%d/%d)", size, reply_msg.message.reply->header_size, reply_msg.message.reply->body_size);
		break;
	case GIOP_MSGTYPE_LOCATEREPLY:
		DEBUG("Orbe_respond: send GIOP_MSGTYPE_LOCATE_REPLY");
		size = Giop_respondLocateReply(body, reply_msg.message.locate_reply);
		break;
	default:
		DEBUG("Orbe_respond: send GIOP_MSGTYPE_MESSAGEERROR");
		size = 0;		/* only header */
	}
	memcpy(buffer, GIOP_HEADER, GIOP_HEADER_LENGTH);

	/*
	 * CAUTION:
 	 * Xilinx Platform require 4 bytes alignment to fill integer values.
	 * But now, buffer pointer is started from 42 bytes-shifted position to make room of UDP header.
	 * So it's implemtented as below:
	 */
	buffer[7] = (u8)reply_msg.type;
	p = &size;
	buffer[8] = p[0];
	buffer[9] = p[1];
	buffer[10] = p[2];
	buffer[11] = p[3];

	Socket_send(buffer, size + GIOP_HEADER_LENGTH);
	return size + GIOP_HEADER_LENGTH;
}

void Orbe_setDebugFlag(int v)
{
	Orbe_debugFlag = v;
}

int Orbe_getDebugFlag()
{
	return Orbe_debugFlag;
}

static int		Orbe_life;

void Orbe_setAlive(int aliveness)
{
	Orbe_life = aliveness;
}

int Orbe_isAlive()
{
	return Orbe_life;
}
