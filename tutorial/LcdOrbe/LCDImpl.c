// LCDImpl.c.template
// generated by ORBE Idl Compiler
// InputFile: LCD.idl
// @Thu Jul 31 16:39:14 2008

#include        "CORBA_Object.h"
#include        "LCD.h"
#include 	"xio.h"
#include 	"xparameters.h"

CORBA_octet LCD_reg = 0;

void set_param(int param) {
	int i;
	
	XIo_Out32(XPAR_LCD_BASEADDR, 0x00 | param);
	XIo_Out32(XPAR_LCD_BASEADDR, 0x40 | param);
	for (i = 0; i < 100; i++);
	XIo_Out32(XPAR_LCD_BASEADDR, 0x00 | param);
	for (i = 0; i < 100; i++);
}

void write_data(int param) {
	int i;
	
	XIo_Out32(XPAR_LCD_BASEADDR, 0x20 | param);
	XIo_Out32(XPAR_LCD_BASEADDR, 0x60 | param);
	for (i = 0; i < 100; i++);
	XIo_Out32(XPAR_LCD_BASEADDR, 0x20 | param);
	for (i = 0; i < 100; i++);
}

void LCD_init()
{
	int i;
	
	for (i = 0; i < 50*1000*15; i++); // wait 15msec	
	set_param(3);	
	for (i = 0; i < 50*1000*5; i++); // wait 5msec
	set_param(3);
	for (i = 0; i < 50*1000*1; i++); // wait 1msec
	set_param(3);
	for (i = 0; i < 50*1000*1; i++); // wait 1msec
	set_param(2);
	for (i = 0; i < 50*1000*1; i++); // wait 1msec
}

// CORBA interface

void LCD_execute(
	LCD o,
	CORBA_octet command,
	CORBA_Environment* ev)
{
	int i;
	
	set_param((command >> 4) & 0xf);

	set_param(command & 0xf);
}

void LCD_writeData(
	LCD o,
	CORBA_octet data,
	CORBA_Environment* ev)
{
	LCD_reg = data;
	write_data((data >> 4) & 0xf);
	write_data(data & 0xf);
}

CORBA_octet LCD_readData(
	LCD o,
	CORBA_Environment* ev)
{
	return LCD_reg;
}


