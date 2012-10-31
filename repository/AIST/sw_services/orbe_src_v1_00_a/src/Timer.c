/*
 * $Id: Timer.c 862 2008-10-24 02:13:57Z takaya $
 * Copyright (C) 2008 AIST, National Institute of Advanced Industrial Science and Technology
 */

#ifdef TIMER
#include <stdio.h>
#include <string.h>
#include "Timer.h"

static int		TIMER_DEVICE_ID;
static XTmrCtr	xtmrctr;
static int		counter = 0;

typedef struct {
	u8 msg[20];
	u32 time;
} struct_log;

struct_log s_log[MAX_LOG]; 

void Timer_Initialize()
{
	XTmrCtr_Initialize(&xtmrctr, TIMER_DEVICE_ID);
	XTmrCtr_Start(&xtmrctr, 0);
}

void Timer(u8* String)
{
	s_log[counter].time =XTmrCtr_GetValue(&xtmrctr, 0);
	memcpy(s_log[counter++].msg, String, strlen((char*)String));
}

void Timer_Start(u8 id)
{
	s_log[id].start_time = XTmrCtr_GetValue(&xtmrctr, 0);
}

void Timer_End(u8 id, u8* String)
{
	u32 end_t;
	
	s_log[id].end_time =XTmrCtr_GetValue(&xtmrctr, 0);
	memcpy(s_log[id].msg, String, strlen(String));
}

void Show_Log()
{
	int i;
	for (i = 0; i < MAX_LOG; i++) {
		xil_printf("%s = %d\r\n", s_log[i].msg, s_log[i].time);
	}
	bzero(&s_log, sizeof(struct_log) * MAX_LOG);
	counter = 0;
}
#endif /* TIMER */
