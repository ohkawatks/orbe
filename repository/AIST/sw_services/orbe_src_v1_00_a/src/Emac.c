/*
 * $Id: Emac.c 822 2008-10-08 02:56:46Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#include <xemaclite.h>
#include <xintc.h>
#include <stdio.h>
#include <string.h>
#include "Emac.h"

#ifdef __MICROBLAZE__    
#include <mb_interface.h>
#endif
#ifdef __PPC__    
#include <xexception_l.h>
#endif


static int EMACLITE_DEVICE_ID;
static int INTC_DEVICE_ID;
static int INTC_EMACLITE_VEC_ID;

/*
 * After calling Emac_setupDevice, it is necessary to call Emac_initialize.
 */
int Emac_setupDevice(int emaclite_device_id, int intc_device_id, int intc_emaclite_vec_id) {
	EMACLITE_DEVICE_ID = emaclite_device_id;
	INTC_DEVICE_ID = intc_device_id;
	INTC_EMACLITE_VEC_ID = intc_emaclite_vec_id;
	return 0;
}


static XEmacLite emacLite;
//static XIntc intc;
//static volatile int send_status = 0;

u8 mbMAC[] = {0,1,2,3,4,5};

u8* Emac_getMAC() {
	return mbMAC;
}

void Emac_setMAC(u8* mac) {
	memcpy(mbMAC, mac, 6);
}

//static void Emac_IntHandler(void *CallBackRef) {
//	XEmacLite_InterruptHandler(&emacLite);
//}

unsigned int Emac_recv(u8* start) {
	unsigned int length = 0;

#ifdef UDP_DEBUG
	xil_printf("Emac_recv\r\n");
#endif
#ifdef TIMER
	Timer("Emac_recv start");
#endif

	do {
//		XIntc_Disable(&intc, INTC_EMACLITE_VEC_ID);
		length = XEmacLite_Recv(&emacLite, start);
//		XIntc_Enable(&intc, INTC_EMACLITE_VEC_ID);
	} while (length == 0);
	
#ifdef TIMER
	Timer("Emac_recv end");
#endif
	return length;
}

unsigned int Emac_send(u8* start, unsigned int length) {
	unsigned int status;

#ifdef UDP_DEBUG
	xil_printf("Emac_send\r\n");
#endif
#ifdef TIMER
	Timer("Emac_send start");
#endif
//	XIntc_Disable(&intc, INTC_EMACLITE_VEC_ID);
	{
//		send_status = 0;
		status = XEmacLite_Send(&emacLite, start, length);
	}
//	XIntc_Enable(&intc, INTC_EMACLITE_VEC_ID);
//	for (;send_status == 0;);

#ifdef TIMER
	Timer("Emac_send end");
#endif
	return length;
}

void Emac_RecvHandler(void *CallBackRef, unsigned int ByteCount) {
#ifdef UDP_DEBUG
	xil_printf("Emac_RecvHandler\r\n");
#endif
}

void Emac_SendHandler(void *CallBackRef, unsigned int ByteCount) {
#ifdef UDP_DEBUG
	xil_printf("Emac_SendHandler\r\n");
#endif
//	send_status = 1;
}

int Emac_initialize() {
	XStatus Status;

#ifdef UDP_DEBUG
   	xil_printf("Emac_initialize\r\n");
#endif	
    Status = XEmacLite_Initialize(&emacLite, EMACLITE_DEVICE_ID);
    if(Status != XST_SUCCESS) {
		print("ERROR on initializing EmacLite.\r\n");
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
                         (void*) 0);

    /*
     * Enable non-critical exceptions
     */
    XExc_mEnableExceptions(XEXC_NON_CRITICAL);
#endif

//    Status = XIntc_Initialize(&intc, INTC_DEVICE_ID);
//	Status = XIntc_Connect(&intc, INTC_EMACLITE_VEC_ID, Emac_IntHandler, 0);
//	XIntc_Enable(&intc, INTC_EMACLITE_VEC_ID);
//    Status = XIntc_Start(&intc, XIN_REAL_MODE);
    
    XEmacLite_SetMacAddress(&emacLite, mbMAC);
    
    XEmacLite_FlushReceive(&emacLite);
    
//    XEmacLite_SetRecvHandler(&emacLite, 0, (XEmacLite_Handler)Emac_RecvHandler);
//    XEmacLite_SetSendHandler(&emacLite, 0, (XEmacLite_Handler)Emac_SendHandler);
//    XEmacLite_EnableInterrupts(&emacLite);
    XEmacLite_DisableInterrupts(&emacLite);

    // for emaclite_v1_14
    emacLite.EmacLiteConfig.RxPingPong = 1;
    
    // for emaclite_v1_11_a
    //emacLite.ConfigPtr->RxPingPong = 1;
    return 0;
}
