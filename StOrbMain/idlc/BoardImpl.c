/*
 * BoardImpl.c
 * input file	: Board.idl
 * generated at : Tue May 22 11:39:20 +0900 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CORBA_Object.h"
#include "Giop.h"
#include "Orbe.h"
#include "Board.h"

void Board_initialize(Board o, CORBA_Environment * ev)
{
    /* need implementation */
}

CORBA_octet Board_getStone(Board o, CORBA_octet x, CORBA_octet y,
			   CORBA_Environment * ev)
{
    /* need implementation */
}

void Board_putStone(Board o, CORBA_octet x, CORBA_octet y,
		    CORBA_octet stoneType, CORBA_Environment * ev)
{
    /* need implementation */
}

CORBA_octet Board_checkConnect6(Board o, CORBA_Environment * ev)
{
    /* need implementation */
}

CORBA_octet Board_checkPattern6(Board o, CORBA_octet pattern,
				CORBA_Environment * ev)
{
    /* need implementation */
}

void Board_popLastPosition(Board o, CORBA_octet * x, CORBA_octet * y,
			   CORBA_octet * direction, CORBA_Environment * ev)
{
    /* need implementation */
}
