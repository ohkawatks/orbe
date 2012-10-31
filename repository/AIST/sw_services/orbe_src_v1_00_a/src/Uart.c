/*
 * $Id: Uart.c 801 2008-09-24 02:28:02Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */


#include <xuartlite.h>
#include <xuartlite_l.h>
#include "Uart.h"
#include <xintc.h>
#include <string.h>
#include <stdio.h>

#ifdef __MICROBLAZE__    
#include "mb_interface.h"
#endif
#ifdef __PPC__    
#include "xexception_l.h"
#endif


static int UARTLITE_DEVICE_ID;
static int INTC_DEVICE_ID;
static int INTC_UARTLITE_VEC_ID;

int Uart_setupDevice(int uartlite_device_id, int intc_device_id, int intc_uartlite_vec_id) {
	UARTLITE_DEVICE_ID = uartlite_device_id;
	INTC_DEVICE_ID = intc_device_id;
	INTC_UARTLITE_VEC_ID = intc_uartlite_vec_id;
	return 0;
}


#define UART_RECV_BUFFER_SIZE 512

u8 recvBuffer [UART_RECV_BUFFER_SIZE];

static XUartLite uartLite;
static XIntc intc;

static void Uart_IntHandler(void *CallBackRef) {
	XUartLite_InterruptHandler(&uartLite);
}

static unsigned int currentRecvIndex;
	
size_t Uart_recv(u8* start, unsigned int length) {
	unsigned int endAddress = (unsigned int) recvBuffer + currentRecvIndex + length;
	volatile unsigned int* pNextAddress = (volatile unsigned int*) &(uartLite.ReceiveBuffer.NextBytePtr);
	while(*pNextAddress < endAddress);

	memcpy(start, recvBuffer+currentRecvIndex, length);
	currentRecvIndex += length;
	currentRecvIndex &= (UART_RECV_BUFFER_SIZE-1); // 0x1ff
	
	return length;
}

size_t Uart_send(u8* start, unsigned int length) {
	XUartLite_Send(&uartLite, start, length);
	XUartLite_Buffer* rb = &uartLite.SendBuffer;

	volatile unsigned int* remain = (volatile unsigned int*) &rb->RemainingBytes;
	while(*remain > 0); // wait for finishing send

	return length;
}

void Uart_resetRecvBuffer() {
	uartLite.ReceiveBuffer.RequestedBytes = UART_RECV_BUFFER_SIZE;
	uartLite.ReceiveBuffer.RemainingBytes = UART_RECV_BUFFER_SIZE;
	uartLite.ReceiveBuffer.NextBytePtr = recvBuffer; // ring buffer
	currentRecvIndex = 0;
}
void Uart_resetSendBuffer() {
	uartLite.SendBuffer.RequestedBytes = 0;
	uartLite.SendBuffer.RemainingBytes = 0;
	uartLite.SendBuffer.NextBytePtr = 0;
}

static void Uart_RecvHandler(void *CallBackRef, unsigned int ByteCount) {
	Uart_resetRecvBuffer();
}

static void Uart_SendHandler(void *CallBackRef, unsigned int ByteCount) {
}

int Uart_initialize() {
   XStatus Status;

    /*
     * Initialize the UartLite driver so that it is ready to use.
     */
    Status = XUartLite_Initialize(&uartLite, UARTLITE_DEVICE_ID);
    if(Status!=0) {
		print("ERROR on initializing UartLite.\r\n");
		return Status;
    }

#ifdef __MICROBLAZE__     
    /* 
     * Enable the microblaze Interrupts
     */
    microblaze_enable_interrupts();
#endif

#ifdef __PPC__ /*PPC*/     
    
    /*
     * Initialize the PPC405 exception table
     */
    XExc_Init();

    /*
     * Register the interrupt controller handler with the exception table
     */
    XExc_RegisterHandler(XEXC_ID_NON_CRITICAL_INT,
                         (XExceptionHandler)XIntc_DeviceInterruptHandler,
                         //(XExceptionHandler)interruptHandler,
                         (void*) 0);

    /*
     * Enable non-critical exceptions
     */
    XExc_mEnableExceptions(XEXC_NON_CRITICAL);
#endif
    Status = XIntc_Initialize(&intc, INTC_DEVICE_ID);
	Status = XIntc_Connect(&intc, INTC_UARTLITE_VEC_ID, Uart_IntHandler, 0);
	XIntc_Enable(&intc, INTC_UARTLITE_VEC_ID);
    Status = XIntc_Start(&intc, XIN_REAL_MODE);
    
    XUartLite_SetRecvHandler(&uartLite, Uart_RecvHandler, 0);
    XUartLite_SetSendHandler(&uartLite, Uart_SendHandler, 0);
    XUartLite_EnableInterrupt(&uartLite);
    
    // setup buffer
    Uart_resetRecvBuffer();
    Uart_resetSendBuffer();
    
    return 0;
}

