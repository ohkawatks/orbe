/*
 * $Id: Giop.h 1080 2009-02-26 06:50:20Z takaya $ 
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology 
 */
#ifndef GIOP_H_
#define GIOP_H_

#include "basic_types.h"
#include "CORBA_Object.h"
#ifndef __BITSPOT__
#include "OrbeConfig.h"
#endif
#include "swapd.h"

#define GIOP_HEADER_LENGTH				12

#if ENDIAN == 1
#	define GIOP_HEADER					"GIOP\001\002\001\000\000\000\000\000"
#elif ENDIAN == 0
#	define GIOP_HEADER					"GIOP\001\002\000\000\000\000\000\000"
#endif

#ifndef ALIGN
	#define ALIGN(width, value)			((width - 1) & (value)) ? (width - ((width - 1) & (value))) : 0
#endif

#define GIOP_MSGTYPE_REQUEST					0
#define GIOP_MSGTYPE_REPLY						1
#define GIOP_MSGTYPE_CANCELREQUEST				2
#define GIOP_MSGTYPE_LOCATEREQUEST				3
#define GIOP_MSGTYPE_LOCATEREPLY				4
#define GIOP_MSGTYPE_CLOSECONNECTION			5
#define GIOP_MSGTYPE_MESSAGEERROR				6
#define GIOP_MSGTYPE_FRAGMENT					7
/* for ORBE */
#define GIOP_ORBE_PROTOCOLVERSION_MISMATCH		254
#define GIOP_ORBE_SERVERSIZE_ERROR				255

#define OPERATION_LENGTH						64
#define SERVICE_CONTEXT_LENGTH					64

typedef struct giop_hdr {
	u8					magic[4];
	u8					majorVersion;
	u8					minorVersion;
	u8					endian;			/* flags */
	u8					type;
	unsigned int		size;
	int					marshalling;
} Giop_Hdr;

typedef struct target_address {
	short					index;
	short					padding;
	char					object_key[OBJECT_KEY_LENGTH];
/*	void					*profile; */
/*	void					*ior; */
} Target_Address;

typedef struct iop_service_context {
	u32						context_id;
	u8						*context_data;
} Service_Context;

typedef struct iop_service_context_list {
	u32						num;
	Service_Context			*service_context;
} Service_Context_List;

typedef struct req {
	u32						request_id;
	u8						response_flags;
	u8						reserved[3];
#ifdef GIOP_FULL_ANALYSIS
	Target_Address			target;
#endif
	u8						operation[OPERATION_LENGTH];
#ifdef GIOP_FULL_ANALYSIS
	u8						service_context[SERVICE_CONTEXT_LENGTH];
#endif
	u32						body_size;
	u8						*body;
} Req;

/* reply status */
#define	NO_EXCEPTION		0
#define	USER_EXCEPTION		1
#define	SYSTEM_EXCEPTION	2
#define	LOCATION_FORWARD	3

typedef struct reply {
	u32						request_id;
	u32						reply_status;
	Service_Context_List	service_context;
	u32						header_size;
	u32						body_size;
	u8						*body;
} Reply;

typedef struct locate_req {
	u32						request_id;
	Target_Address			target;
	/* no body */
} Locate_Req;

typedef struct locate_reply {
	u32						request_id;
	u32						locate_status;	
	/* no body if you use only OBJECT_HERE */
} Locate_Reply;

typedef struct reply_message {
	u32		type;

	union message_type {
		Req					*req;
		Reply				*reply;
		Locate_Req			*locate_request;
		Locate_Reply		*locate_reply;
	} message;
} Reply_Message;

/*
 * Starndard Service Context definitions
 */
enum IOP_ServiceId {
	TransactionService = 0,
	CodeSets = 1,
	ChainBypassCheck = 2,
	ChainBypassInfo = 3,
	LogicalThreadId = 4,
	BI_DIR_IIOP = 5,
	SendingContextRunTime = 6,
	INVOCATION_POLICIES = 7,
	FORWARDED_IDENTITY = 8,
	UnkownExceptionInfo = 9,
	RTCorbaPriority = 10,
	RTCorbaPriorityRange = 11,
	FT_GROUP_VERSION = 12,
	FT_REQUEST = 13,
	ExceptionDetailMessage = 14,
	SecurityAttributeService = 15,
	ActivityService = 16,
	RMICustomMaxStreamForma = 17,
	ACCESS_SESSION_ID = 18,
	SERVICE_SESSION_ID = 19,
	FIREWALL_PATH = 20,
	FIREWALL_PATH_RESP = 21
};
		
/* Messages */
size_t			Giop_respondReply(u8 *out, Reply *reply);
size_t			Giop_respondLocateReply(u8 *out, Locate_Reply *locate_reply);

/* Message bodies */
Req				*Giop_readRequest(u8 *in, Giop_Hdr *header);
Reply			*Giop_createReply(u8 *in, Req *req);
Locate_Req		*Giop_readLocateRequest(u8 *in, Giop_Hdr *header);
Locate_Reply	*Giop_createLocateReply(u8 *in, Locate_Req *locate_req);

/* Items */
Target_Address	*Giop_TargetAddress(Req *req, u8 **in, Giop_Hdr *header);
u8				*Giop_ServiceContextList(Req *req, u8 **in, Giop_Hdr *header);

size_t			Giop_writeReq(u8 *buffer, const Req *req);
size_t			Giop_writeLocateReq(u8 *buffer, const Locate_Req *locate_request);
Reply			*Giop_readReply(u8 *in, Giop_Hdr *header);
Locate_Reply	*Giop_readLocate_Reply(u8 *in, Giop_Hdr *header);
u32				Giop_getRequestId();
#endif /* GIOP_H_ */
