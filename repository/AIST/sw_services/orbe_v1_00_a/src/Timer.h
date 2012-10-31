/*
 * $Id: Timer.h 862 2008-10-24 02:13:57Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#ifdef TIMER
#if defined(__MICROBLAZE__) || defined(__PPC__)
#	include <xtmrctr.h>
#	define MAX_LOG 40 
/*
 * Xilinx EDK environment doesn't provide strings.h which has the prototyp declaration of bzero().
 * We don't know the reason.
 */
extern void bzero (void *__s, size_t __n);

void Timer_Initialize();
void Timer(u8* String);
void Timer_Start(u8 id);
void Timer_End(u8 id, u8* String);
void Show_Log();
#endif	/* (__MICROBLASE__) || (__PPC__) */
#else
#define Timer_Initialize()		(0)
#define Timer(x)				(0)
#define Timer_Start(x)			(0)
#define Timer_End(x,y)			(0)
#define Show_Log()
#endif	/* TIMER */
