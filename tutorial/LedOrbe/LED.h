/*
 * LED.h
 * input file   : LED.idl
 * generated at : Mon Feb 09 15:09:24 +0900 2009
 */

#ifndef _LED_H_
#define _LED_H_
#include "CORBA_Object.h"

typedef CORBA_Object LED;
extern void LED_turnOn(LED o, CORBA_long ID,  CORBA_Environment *ev);
extern void LED_turnOff(LED o, CORBA_long ID,  CORBA_Environment *ev);
extern CORBA_boolean LED_getState(LED o, CORBA_long ID,  CORBA_Environment *ev);


extern void dispatch(Req *request, Reply *reply, Giop_Hdr *header);

#endif /* _LED_H_ */
