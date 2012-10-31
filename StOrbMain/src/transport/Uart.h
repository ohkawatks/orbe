/*
 * $Id: Uart.h 801 2008-09-24 02:28:02Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#ifndef UART_H_
#define UART_H_

#include <stdio.h>
#include "basic_types.h" 

#define Socket_initialize Uart_initialize
#define Socket_send Uart_send
#define Socket_recv Uart_recv

int 	Uart_initialize();
size_t 	Uart_send(u8* start, unsigned int length);
size_t	Uart_recv(u8* start, unsigned int length);

int		Uart_setupDevice(int uartlite_device_id, int intc_device_id, int intc_uartlite_vec_id);

#endif /*UART_H_*/
