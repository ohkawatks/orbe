/*
 * $Id: CORBA_Object.h 1093 2009-03-16 05:39:48Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology 
 */
#ifndef _CORBA_OBJECT_H_
#define _CORBA_OBJECT_H_

#include "basic_types.h" 

typedef struct corba_environment {
	char *env;
} CORBA_Environment;


#ifndef TRUE
#  define TRUE		1
#endif

#ifndef FALSE
#  define FALSE		0
#endif

#ifndef NULL
#define NULL		0
#endif

#define OBJECT_KEY_LENGTH		256
#define STRING_DEFAULT_LENGTH	256

struct corba_object {
	int		length;					// total length (it represents type_id + sequence of TaggedProfiles)
	u8		*object_key;
};

enum TCKind {
	tk_null = 0,
	tk_void = 1,
	tk_short = 2,
	tk_long = 3,
	tk_ushort = 4,
	tk_ulong = 5,
	tk_float = 6,
	tk_double = 7,
	tk_boolean = 8,
	tk_char = 9,
	tk_octet = 10,
	tk_any = 11,
	tk_TypeCode = 12,
	tk_Principal = 13,
	tk_objref = 14,
	tk_struct = 15,
	tk_union = 16,
	tk_enum = 17,
	tk_string = 18,
	tk_sequence = 19,
	tk_array = 20,
	tk_alias = 21,
	tk_except = 22,
	tk_longlong = 23,
	tk_ulonglong = 24,
	tk_longdouble = 25,
	tk_wchar = 26,
	tk_wstring = 27,
	tk_fixed = 28,
	tk_value = 29,
	tk_value_box = 30,
	tk_native = 31,
	tk_abstract_interface = 32,
	tk_local_interface = 33,
	tk_component = 34,
	tk_home = 35,
	tk_event = 36,
	none					= 0xffffffff,
};

typedef long			CORBA_long;
typedef unsigned long	CORBA_ulong;
typedef unsigned long	CORBA_unsigned_long;
typedef short			CORBA_short;
typedef unsigned short	CORBA_ushort;
typedef unsigned short	CORBA_unsigned_short;
typedef float			CORBA_float;
typedef double			CORBA_double;
typedef char			CORBA_char;
typedef char			CORBA_boolean;
typedef char			CORBA_octet;
typedef enum TCKind		CORBA_TypeCode;
typedef CORBA_char*		CORBA_string;

typedef struct corba_object* objref_ptr;
typedef objref_ptr		CORBA_Object;

typedef struct CORBA_any {
  CORBA_TypeCode	_type;
  u8				*_value;
} CORBA_any;

#endif /* _CORBA_OBJECT_H_ */
