/*
 * rm11.h
 * input file   : ../idl/rm11.idl
 * generated at : Fri Mar 30 10:13:02 +0900 2012
 */

#ifndef _RM11_H_
#define _RM11_H_
#include "CORBA_Object.h"

typedef CORBA_Object rm11;
extern CORBA_short rm11_op1(rm11 o, CORBA_short argin,
			    CORBA_short * argout, CORBA_short * arginout,
			    CORBA_Environment * ev);


extern void dispatch(Req * request, Reply * reply, Giop_Hdr * header);

#endif				/* _RM11_H_ */
