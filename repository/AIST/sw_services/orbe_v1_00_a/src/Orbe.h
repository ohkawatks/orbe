/*
 * $Id: Orbe.h 1093 2009-03-16 05:39:48Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology 
 */
#ifndef	_ORBE_H_
#define	_ORBE_H_

#include <stdio.h>
#include "swapd.h"
#ifndef __BITSPOT__
#include "Timer.h"
#include "OrbeConfig.h"
#endif
#include "CORBA_Object.h"
#include "Giop.h"


#ifdef __BITSPOT__
#define		ORBE_BUFFER_SIZE		400
#else
#define		ORBE_BUFFER_SIZE		1024
#endif

#if defined(__PPC__) || defined(__MICROBLAZE__)
#	define	PRINT	xil_printf
#	define	LF	"\r\n"
#else
#	define	PRINT	printf
#	define	LF	"\n"
extern void log_debug();
#endif

#ifdef __BITSPOT__
#define DEBUG if (Orbe_getDebugFlag()) UART_printf
#else
#define	DEBUG(...) \
	if (Orbe_getDebugFlag()) { PRINT(__VA_ARGS__); PRINT(LF); }
#endif

#ifdef SOCKET_CHANNEL_USE_UDP
#define BEGINNING_OFFSET		42	/* for ether, ip and udp headers. */
#else
#define BEGINNING_OFFSET		0
#endif

/*
 * took these macros from Ruby. Thank you, Ruby!
 */
#ifndef swap16
#define swap16(x)		((((x)&0xFF)<<8) | (((x)>>8)&0xFF))
#endif
#define swaps(x)		swap16(x)

#ifndef swap32
#define swap32(x)		((((x)&0xFF)<<24)		\
						|(((x)>>24)&0xFF)		\
						|(((x)&0x0000FF00)<<8)	\
						|(((x)&0x00FF0000)>>8)	)
#endif
#define swapl(x)		swap32(x)
#define swapf(x)		swapl(x)

#define Orbe_exchangeShort(x)	((header->marshalling)?swaps(x):(x))
#define Orbe_exchangeInt(x)		((header->marshalling)?swapl(x):(x))
#define Orbe_exchangeLong(x)	((header->marshalling)?swapl(x):(x))
#define Orbe_exchangeFloat(x)	((header->marshalling)?swapf(x):(x))
#define Orbe_exchangeDouble(x)	((header->marshalling)?swapd(x):(x))
#define Orbe_exchangeString(x)	(x+sizeof(u32))

#define Orbe_getString(s, from)															\
				{																		\
				unsigned int length = Orbe_exchangeInt(*(u32*)from);					\
				from += sizeof(CORBA_unsigned_long);									\
				s = alloca(length);														\
				memcpy(s, from, length);												\
				from += length;															\
				}

#define Orbe_putString(s, to)															\
				{																		\
				unsigned int length = strlen(s) + 1;									\
				*(u32*)to = length;														\
				to += sizeof(CORBA_unsigned_long);										\
				memcpy(to, s, length);													\
				to += length;															\
				}

#define Orbe_noneShort(x)		(x)
#define Orbe_noneInt(x)			(x)
#define Orbe_noneLong(x)		(x)
#define Orbe_noneFloat(x)		(x)
#define Orbe_noneDouble(x)		(x)
#define Orbe_noneString(x)		Orbe_exchangeString(x)

#define Orbe_getObject(x, b)															\
				{																		\
				x->length = Orbe_getRefSize(b, header);									\
				x->object_key = alloca(x->length);										\
				memcpy(x->object_key, b, x->length);									\
				b += x->length;															\
				}

#define Orbe_putObject(x, b)															\
				{																		\
				memcpy(b, x->object_key, x->length);									\
				b += x->length;															\
				}
#define Orbe_getAny(x, b)																\
				{																		\
				int ln = 0;																\
				double d = 0;															\
				x._type = Orbe_exchangeInt(*(u32*)b);									\
				b += sizeof(CORBA_TypeCode);											\
                switch(x._type) {														\
				case tk_short:															\
				case tk_ushort:															\
					x._value = (void*)Orbe_exchangeShort(*(u16*)b);						\
					b += sizeof(CORBA_short);											\
					break;																\
				case tk_long:															\
				case tk_ulong:															\
				case tk_TypeCode:														\
					x._value = (void*)Orbe_exchangeLong(*(u32*)b);						\
					b += sizeof(CORBA_long);											\
					break;																\
				case tk_float:															\
					x._value = (void*)Orbe_exchangeFloat(*(u32*)b);						\
					b += sizeof(CORBA_float);											\
					break;																\
				case tk_double:															\
					d = Orbe_exchangeDouble(*(u32*)b);									\
					x._value = &d;														\
					b += sizeof(CORBA_double);											\
					break;																\
				case tk_boolean:														\
				case tk_char:															\
				case tk_octet:															\
					x._value = (void*)(*b+0);											\
					b += sizeof(CORBA_octet);											\
					break;																\
                case tk_string:															\
						ln = Orbe_exchangeInt(*(CORBA_ulong*)(b+4));					\
						x._value = alloca(ln);											\
						strcpy(x._value, b+sizeof(CORBA_ulong)+sizeof(CORBA_ulong));	\
						b += (ln+sizeof(CORBA_ulong)+sizeof(CORBA_ulong));				\
					break;																\
				case tk_null:															\
				case tk_void:															\
                default:																\
					x._value = NULL;													\
					break;																\
                }																		\
				}


#define Orbe_putAny(x, b)																\
				{																		\
				int ln = 0;																\
				*(CORBA_TypeCode*)b = x._type;											\
				b += sizeof(CORBA_TypeCode);											\
				switch(x._type) {														\
				case tk_short:															\
				case tk_ushort:															\
					*(u16*)b = (u16)((u32)x._value & 0xffff);							\
					b += sizeof(CORBA_short);											\
					break;																\
				case tk_long:															\
				case tk_ulong:															\
				case tk_TypeCode:														\
					*(u32*)b = (u32)x._value;											\
					b += sizeof(CORBA_long);											\
					break;																\
				case tk_float:															\
					*(u32*)b = (u32)x._value;											\
					b += sizeof(CORBA_float);											\
					break;																\
				case tk_double:															\
					memcpy(b, x._value, sizeof(CORBA_double));							\
					b += sizeof(CORBA_double);											\
					break;																\
				case tk_boolean:														\
				case tk_char:															\
				case tk_octet:															\
					*b = (u32)x._value & 0xff;											\
					b += sizeof(CORBA_octet);											\
					break;																\
				case tk_string:															\
					*(u32*)b = 0;														\
					ln = *((CORBA_ulong*)x._value)+sizeof(CORBA_ulong);					\
					memcpy(b+sizeof(CORBA_ulong), (void*)x._value, ln);					\
					b += (sizeof(CORBA_ulong)+ln);										\
					break;																\
				case tk_null:															\
				case tk_void:															\
                default:																\
					x._value = NULL;													\
					break;																\
				}																		\
				}

int			Orbe_none(u8 *p);
int			Orbe_startServer(void);
u32			Orbe_respond(u8 *in, Reply_Message reply_message);
void		Orbe_buildTable(void);
int			Orbe_isAlive(void);
void		Orbe_setAlive(int alive);
void		Orbe_setDebugFlag(int debugFlag);
int			Orbe_getDebugFlag(void);
size_t		Orbe_getRefSize(u8 *value, Giop_Hdr *header);
void		Orbe_setObjectKey(CORBA_Object o, u8 *object_key);

void		Orbe_add(CORBA_Object o);
void		Orbe_initBuffer();
size_t		Orbe_flush(size_t length);
size_t		Orbe_writeReq(const Req *req);
size_t		Orbe_writeLocateReq(const Locate_Req *locate_request);
void		Orbe_setOperation(Req *request, u8 *op, u8 *body, u8 body_size);
Locate_Req	*Orbe_initLocateReq(Locate_Req *locate_request);
Req			*Orbe_initReq(Req *req);
#endif	/* _ORBE_H_ */
