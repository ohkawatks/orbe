/*
 * $Id: Socket.h 1061 2009-02-05 06:21:36Z hideki $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include "basic_types.h" 

#ifdef SOCKET_CHANNEL_USE_UART
#include "Uart.h"
#endif

#ifdef SOCKET_CHANNEL_USE_UDP
#include "Udp.h"
#endif

#ifdef SOCKET_CHANNEL_USE_UDP_LINUX
#include "UdpLinux.h"
#endif

#ifdef SOCKET_CHANNEL_USE_STDINOUT
#include "StdInOut.h"
#endif

#ifdef SOCKET_CHANNEL_USE_CAN
#include "Can.h"
#endif

#ifdef SOCKET_CHANNEL_USE_ECU_CAN
#include "EcuCan.h"
#endif

#endif // SOCKET_H_
