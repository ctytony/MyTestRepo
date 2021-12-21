/* 
 * Copyright (c) 1997-2010 IntervalZero, Inc.  All rights reserved.
 *
 * Module Name:
 *
 *	LogRecord.h
 *
 * Abstract:
 *
 * 	 RTX TimeView log record data structure.
 *
 * Environment:
 *
 *	RTX application
 *
 *	
 * Revision History:
 *
 *
 */
#include <RtxEventId.h>

#pragma once

#ifndef _RTX_LOG_RECORD_H_
#define _RTX_LOG_RECORD_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define EVENT_MASK_DEFAULT  (0xFDFFDC00)  // all events are captured except groups 4 and 16


typedef struct _LogHeader {
	LARGE_INTEGER clockRate;   // unit is Hz
	ULONG nEventsToCapture;
	ULONG percentBeforeTrigger;
	int group[TOTAL_RTX_EVENT_GROUPS];
	ULONG eventMask;
	RtxEventId   triggerEvents;
	int triggerOccurence;
	ULONG EventsCaptured;
	BOOL RollOver;
} LogHeader;


typedef struct _LogRecord {
	RtxEventId id;
	unsigned __int64 timeStamp;
	PVOID data1;
	PVOID data2;
	unsigned int cpuid;
	unsigned int Pid;
	unsigned int Tid;
} LogRecord;


#endif


#ifdef __cplusplus 
}
#endif
