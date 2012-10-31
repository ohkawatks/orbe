/*
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology 
 * $Id: OTable.h 858 2008-10-22 07:41:12Z takaya $
 */
#ifndef	_OTABLE_H_
#define	_OTABLE_H_

#include	"CORBA_Object.h"

#define		MAX_OBJECTS		32

int OTable_add(CORBA_Object o);
int OTable_remove(CORBA_Object o);
CORBA_Object OTable_current();
int OTable_has(CORBA_Object o);
#endif /* _OTABLE_H_ */
