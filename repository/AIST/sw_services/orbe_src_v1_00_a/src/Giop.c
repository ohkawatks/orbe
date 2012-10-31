/*
 * $Id: Giop.c 1080 2009-02-26 06:50:20Z takaya $ 
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Orbe.h"
#include "Giop.h"

#define	Giop_setMessageType(b, type)		(*(b + 7)) = (type)
#define	Giop_setMessageSize(b, len)			(*(u32*)(b + 8)) = (len)

size_t Giop_writeReq(u8 *buffer, const Req *req)
{
	CORBA_long	length;
	u8 *p = buffer + GIOP_HEADER_LENGTH;

	DEBUG("Giop_writeRed:");

	Giop_setMessageType(buffer, GIOP_MSGTYPE_REQUEST);

	/* id */
	*(CORBA_long*)p = req->request_id;
	p += sizeof(CORBA_long);

	/* flags */
	*p = req->response_flags;
	p += sizeof(CORBA_long);

	/* Target Address */
	*(CORBA_short*)p = req->target.index;
	p += sizeof(CORBA_long);

	/* Target Address - now only object_key */
	*(CORBA_long*)p = length = strlen(req->target.object_key) + 1;
	p += sizeof(CORBA_long);
	memcpy(p, req->target.object_key, length);
	p += length;
	p += ALIGN(4, (u32)p);

	/* operation */
	*(CORBA_long*)p = length = strlen(req->operation) + 1;
	p += sizeof(CORBA_long);
	memcpy(p, req->operation, length);
	p += length;
	p += ALIGN(4, (u32)p);

	/* Service Context */
	*(CORBA_long*)p = 0;	/* zero sc */

	if (req->body_size) {
		memcpy(p, req->body, req->body_size);
		p += req->body_size;
		p += ALIGN(8, (u32)p);
	}

	return Giop_setMessageSize(buffer, p - buffer);
}

size_t Giop_writeLocateReq(u8 *buffer, const Locate_Req *locate_request)
{
	CORBA_long length;
	u8 *p = buffer + GIOP_HEADER_LENGTH;

	DEBUG("Giop_writeLocateRequest:");

	Giop_setMessageType(buffer, GIOP_MSGTYPE_LOCATEREQUEST);

	/* id */
	*(CORBA_long*)p = locate_request->request_id;
	p += sizeof(CORBA_long);

	/* Target Address */
	*(CORBA_short*)p = locate_request->target.index;
	p += sizeof(CORBA_long);

	/* Target Address - now only object_key */
	*(CORBA_long*)p = length = strlen(locate_request->target.object_key) + 1;
	p += sizeof(CORBA_long);
	memcpy(p, locate_request->target.object_key, length);
	p += length;
	p += ALIGN(4, (u32)p);

	return Giop_setMessageSize(buffer, p - buffer);
}

Reply *Giop_readReply(u8 *msg, Giop_Hdr *header)
{
	u8				*ptr;
	char			*tmp;
	static Reply	reply;

	DEBUG("Giop_readReply:");

	ptr = msg;
	reply.request_id = Orbe_exchangeLong(*(u32*)ptr);
	ptr += 4;

	reply.reply_status = Orbe_exchangeLong(*(u32*)ptr);
	ptr += 4;

	int num = reply.service_context.num = Orbe_exchangeLong(*(u32*)ptr);
	ptr += 4;
	while (num--) {
		Orbe_getString(tmp, ptr);
		DEBUG(tmp);
		ptr += ALIGN(4, (u32)ptr);
	}
	ptr += ALIGN(8, (u32)ptr);

	reply.body = ptr;
	reply.body_size = (ptr - msg) ;

	return &reply;
}

Locate_Reply *Giop_readLocate_Reply(u8 *msg, Giop_Hdr *header)
{
	u8					*ptr;
	static Locate_Reply	locate_reply;

	DEBUG("Giop_readLocate_Reply:");

	ptr = msg;
	locate_reply.request_id = Orbe_exchangeLong(*(u32*)ptr);
	ptr += 4;

	locate_reply.locate_status = Orbe_exchangeLong(*(u32*)ptr);
	ptr += 4;

	return &locate_reply;
}

u32 Giop_getRequestId()
{
	static u32 id = 0;
	return id++;
}

size_t Giop_respondReply(u8 *buffer, Reply *reply)
{
	u8 *ptr;

	DEBUG("Giop_respondReply:");

	ptr = buffer;
	memcpy(ptr, &(reply->request_id), 4);
	ptr += 4;
	memcpy(ptr, &(reply->reply_status), 4);
	ptr += 4;
	memcpy(ptr, &(reply->service_context.num), 4);
	ptr += 4;
	/* Currently service_context list must be zero. */
	return reply->header_size + reply->body_size;
}

size_t Giop_respondLocateReply(u8 *buffer, Locate_Reply *locate_reply)
{
	DEBUG("Giop_respondLocateReply:");

	memcpy(buffer,   &(locate_reply->request_id), 4);
	memcpy(buffer+4, &(locate_reply->locate_status), 4);
	return 8;
}

Req *Giop_readRequest(u8* msg, Giop_Hdr *header)
{
	u8					*ptr;
	u32					tmp;
	static Req			request;

	DEBUG("Giop_readRequest:");

	ptr = msg;
	request.request_id = Orbe_exchangeLong(*(u32*)ptr);
	ptr += 4;

	request.response_flags = *ptr;
	ptr += 4;

	Giop_TargetAddress(&request, &ptr, header);

	tmp = Orbe_exchangeLong(*(u32*)ptr);			/* the length of string */
	ptr += 4;

	strncpy((void*) request.operation, (void*) ptr, OPERATION_LENGTH);		/* max is 255 */
	ptr += strlen((void*)ptr) + 1;

	DEBUG("Giop_readRequest: request.operation is \"%s\"", request.operation);

	ptr += ALIGN(4, ptr - msg + GIOP_HEADER_LENGTH);				/* it means ptr - (msg - GIOP_HEADER_LENGTH) */
	Giop_ServiceContextList(&request, &ptr, header);

	/*
	 * border between header and body
	 */
	ptr += ALIGN(8, ptr - msg + GIOP_HEADER_LENGTH);				/* same as above */
	request.body = ptr;		/* pointing request_buffer's area */
	return &request;
}

Reply *Giop_createReply(u8* out_going_msg, Req *req)
{
	size_t				len;
	static Reply		reply;

	DEBUG("Giop_createReply:");

	reply.request_id = req->request_id;
	reply.reply_status = 0;							/* NO_EXCEPTION */
	reply.service_context.num = 0;
	reply.service_context.service_context = NULL;

	len = 12;	/* 12 means total length of avobe values */

	/*
	 * border between header and body
	 */
	len += ALIGN(8, len + GIOP_HEADER_LENGTH);
	reply.body = out_going_msg + len + GIOP_HEADER_LENGTH;		/* pointing reply_buffer's area */
	reply.header_size = len;

	/*
	DEBUG("request_id = %d", reply.request_id);
	DEBUG("reply_status = %d", reply.reply_status);
	DEBUG("service_context.num = %d", reply.service_context.num);
	DEBUG("body = 0x%x, offset=%d", reply.body, reply.body - out_going_msg);
	*/
	return &reply;
}

Locate_Req *Giop_readLocateRequest(u8* msg, Giop_Hdr *header)
{
	Locate_Req *req;

	DEBUG("Giop_LocateRequest:");

	/*
	 * CAUTION:
	 * It's a fake of the actual procedure.
	 * This implementation doesn't care of the target address.
	 */
	req = (Locate_Req *)msg;
	req->request_id =Orbe_exchangeLong(*(u32*)msg);
	return req;
}

Locate_Reply *Giop_createLocateReply(u8* out_going_msg, Locate_Req *req)
{
	static Locate_Reply  reply;

	DEBUG("Giop_LocateReply:");

	reply.request_id = req->request_id;
	reply.locate_status = 1;		/* OBJECT_HERE */

	/* if OBJECT_HERE, only header without body */
	return &reply;
}

Target_Address *Giop_TargetAddress(Req *request, u8 **buffer, Giop_Hdr *header)
{
	u8				*ptr = NULL;
	Target_Address	*target;

	int				index;
	size_t			max = 0;
	size_t			len = 0;

	DEBUG("Giop_TargetAddress:");

	ptr = (u8 *)*buffer;
#ifdef GIOP_FULL_ANALYSIS
	target = &request->target;
	target->index = index = Orbe_exchangeLong(*(u32*)ptr);
	ptr += 4;

	switch (target->index) {
	case 0:
		DEBUG("Giop_TargetAddress: object_key");
		max = len = Orbe_exchangeLong(*(u32*)ptr);
		ptr += 4;
		if (len > OBJECT_KEY_LENGTH) {
			DEBUG("Giop_TargetAddress: object_key is too big (%d required).", len);
			DEBUG("procedure is contiuned.");
			max = OBJECT_KEY_LENGTH;
		}
		memcpy(target->object_key, ptr, max);
		ptr += len;
		break;
	case 1:
		DEBUG("Giop_TargetAddress: profile, not implemented.");
		break;
	case 2:
		DEBUG("Giop_TargetAddress: ior, not implemented.");
		break;
	}
#else
	target = NULL;
	index = Orbe_exchangeShort(*(u16*)ptr);
	ptr += 4;

	switch (index) {
	case 0:
		DEBUG("Giop_TargetAddress: object_key");
		max = len = Orbe_exchangeLong(*(u32*)ptr);
		ptr += 4;
		if (len > OBJECT_KEY_LENGTH) {
			DEBUG("Giop_TargetAddress: object_key is too big (%d bytes are required).", len);
			DEBUG("procedure is contiuned.");
			max = OBJECT_KEY_LENGTH;
		}
		ptr += len;
		break;
	case 1:
		DEBUG("Giop_TargetAddress: profile, not implemented.");
		break;
	case 2:
		DEBUG("Giop_TargetAddress: ior, not implemented.");
		break;
	}
#endif
	ptr += ALIGN(4, (u32)ptr);
	*buffer = ptr;
	return  target;
}

u8 *Giop_ServiceContextList(Req *request, u8 **buffer, Giop_Hdr *header)
{
	u8			*start = NULL;
	u32			num;
	u32			i;
	u32			len;
	size_t		total = 0;	/* distance from &buffer[0] */
#ifdef GIOP_FULL_ANALYSIS
	u8			*service_context = start = request->service_context;
#endif

	num = Orbe_exchangeLong(*(u32*)(*buffer + total));	/* the number of items */
	total += 4;
	for (i = 0; i < num; i++) {
		total += 4;
		len = Orbe_exchangeLong(*(u32*)(*buffer + total));		/* length */
		total += 4;
		total += len;
		total += ALIGN(4, (u32)(*buffer + total));
	}

#ifdef GIOP_FULL_ANALYSIS
	size_t limit = total;
	if (total > SERVICE_CONTEXT_LENGTH) {
		limit = SERVICE_CONTEXT_LENGTH;
		DEBUG("Giop_ServiceContextList : ServiceContextList is too big (%d bytes are required).", total);
		DEBUG("procedure is contiuned.");
	}
	memcpy(start, *buffer, limit);
#endif

	*buffer += total;
	return start;
}
