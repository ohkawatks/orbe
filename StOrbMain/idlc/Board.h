/*
 * Board.h
 * input file   : Board.idl
 * generated at : Tue May 22 11:39:20 +0900 2012
 */

#ifndef _BOARD_H_
#define _BOARD_H_
#include "CORBA_Object.h"

typedef CORBA_Object Board;
extern void Board_initialize(Board o, CORBA_Environment * ev);
extern CORBA_octet Board_getStone(Board o, CORBA_octet x, CORBA_octet y,
				  CORBA_Environment * ev);
extern void Board_putStone(Board o, CORBA_octet x, CORBA_octet y,
			   CORBA_octet stoneType, CORBA_Environment * ev);
extern CORBA_octet Board_checkConnect6(Board o, CORBA_Environment * ev);
extern CORBA_octet Board_checkPattern6(Board o, CORBA_octet pattern,
				       CORBA_Environment * ev);
extern void Board_popLastPosition(Board o, CORBA_octet * x,
				  CORBA_octet * y, CORBA_octet * direction,
				  CORBA_Environment * ev);


extern void dispatch(Req * request, Reply * reply, Giop_Hdr * header);

#endif				/* _BOARD_H_ */
