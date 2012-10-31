/*
 * $Id: OrbeConfig.c 980 2008-12-17 05:12:31Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 *
 */

#include "Orbe.h"



/*
 * For UDP transport
 * 
 * 
 * 
 */

#ifdef SOCKET_CHANNEL_USE_UDP
#include	"xparameters.h"
#include	"Arp.h"
#include	"Emac.h"

static int OrbeConfig_setupTransportDevice() {
	Emac_setupDevice(XPAR_EMACLITE_0_DEVICE_ID, XPAR_INTC_0_DEVICE_ID, XPAR_INTC_0_EMACLITE_0_VEC_ID);
	
	u8 ip[] = {CONFIG_ORBE_IP_ADDRESS};
	Arp_setIP(ip);
	u8 mac[] = {CONFIG_ORBE_MAC_ADDRESS};
	Emac_setMAC(mac);
	return 0;
}
#endif

/*
 * For UART transport
 * 
 * 
 * 
 */
#ifdef SOCKET_CHANNEL_USE_UART
#include	"xparameters.h"
#include	"Uart.h"

static int OrbeConfig_setupTransportDevice() {
	Uart_setupDevice(XPAR_UARTLITE_0_DEVICE_ID, XPAR_INTC_0_DEVICE_ID, XPAR_INTC_0_UARTLITE_0_VEC_ID);
	return 0;
}
#endif


/*
 * For STDINOUT transport
 * 
 * 
 * 
 */
#if defined(SOCKET_CHANNEL_USE_STDINOUT) || defined(SOCKET_CHANNEL_USE_UDP_LINUX)
static int OrbeConfig_setupTransportDevice() {
	return 0;
}
#endif




int OrbeConfig_setup() {
	OrbeConfig_setupTransportDevice();
	
	#ifdef TIMER
	Timer_Initialize();
	#endif
	
	Orbe_setDebugFlag(1);
	Orbe_setAlive(TRUE);
	
	return 0;
}

