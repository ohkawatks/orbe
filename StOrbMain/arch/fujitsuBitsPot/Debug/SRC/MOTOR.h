// MOTOR.h
// generated by ORBE Idl Compiler
// InputFile: MOTOR.idl
// @Tue Nov 11 16:28:37 2008

#ifndef _MOTOR_H

#define _MOTOR_H

#include        "CORBA_Object.h"
typedef CORBA_Object MOTOR;

#include        "MOTOR.h"
extern void MOTOR_stepRight(
	MOTOR o,
	CORBA_Environment* ev);
extern void MOTOR_stepLeft(
	MOTOR o,
	CORBA_Environment* ev);
extern void MOTOR_startSpin(
	MOTOR o,
	CORBA_octet turnD,
	CORBA_ushort speed,
	CORBA_Environment* ev);
extern void MOTOR_stopSpin(
	MOTOR o,
	CORBA_Environment* ev);
extern CORBA_octet MOTOR_getPosition(
	MOTOR o,
	CORBA_Environment* ev);
extern void MOTOR_turnOn(
	MOTOR o,
	CORBA_long ID,
	CORBA_Environment* ev);
extern void MOTOR_turnOff(
	MOTOR o,
	CORBA_long ID,
	CORBA_Environment* ev);
extern CORBA_boolean MOTOR_getState(
	MOTOR o,
	CORBA_long ID,
	CORBA_Environment* ev);
#endif //_MOTOR_H
