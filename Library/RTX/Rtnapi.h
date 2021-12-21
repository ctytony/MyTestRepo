/*
 *
 *      Copyright (c) 1996-2016 IntervalZero, Inc.  All rights reserved.
 *
 *      Rtnapi.h -- RTX Network Stack APIs.
 *
 *   
 */

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


//
// Filter functions
//
#ifndef RTNFLTRPROC
#define RTNFLTRPROC __declspec ( dllimport )
#endif

#ifndef u32
typedef unsigned u32;
#endif /* u32 */

//Enums for Filter
typedef enum _FRAME_STATE
{
    STATE_DISMISS = 0,
    STATE_CONTINUE,
} FRAME_STATE;

typedef enum _FILTER_ID
{
    RECEIVE_FILTER = 0,
	TRANSMIT_FILTER,
    UPDOWN_FILTER,
	CONFIGURATION_FILTER,
	IOCTL_FILTER,
} FILTER_ID;

typedef enum _FILTER_STATE
{
    STATE_DISABLE = 0,
    STATE_ENABLE
} FILTER_STATE;

// This function places a frame on the receive queue for the indicated network device.
RTNFLTRPROC void RtnQueueRecvFrame(void *ndptr, void *mptr, unsigned long packetsize);

// This function configures certain NIC modes or characteristics.
RTNFLTRPROC void RtnIOCTLDriver(void *ndp,int cmd,char *addr);

// This function transmits a frame.  It is also used when an application wants to transmit a frame
// that bypasses the RTX TCP/IP stack.  NOTE: Developers must determine the MAC address of the
// destination themselves, using third-party (non-RTX) utilities.
RTNFLTRPROC int RtnTransmitFrame(void *mptr);

// This function retrieves the filter state on the specified interface.
RTNFLTRPROC int  RtnGetFilterState(u32 ipaddr,int filter);

// This function sets the filter state on the specified interface.
RTNFLTRPROC int  RtnSetFilterState(u32 ipaddr,FILTER_ID Filter,FILTER_STATE State);

// This function obtains a frame from the RtxTcpIp Stack guaranteed to be long enough to contain
// DataSize bytes of data.
RTNFLTRPROC void * RtnAllocateFrame(char *DevName, int DataSize);

// This function frees a frame previously allocated using RtnAllocateFrame.
RTNFLTRPROC void RtnFreeFrame(void *Frame);

//
// Stack called (only) utility functions
//

int UtilUpDown(void *ndptr, unsigned short flag, char *options);
int UtilIOCTL(void *ndptr, int cmd, char *addr);
int UtilConfig(void *ndptr);
int UtilTransmit(void *mptr);

//RTX Network stack specific get/setsockopt at IPPROTO_TCP level
#define TCP_ACKDELAYTIME 23
#define TCP_ACKNSEG 24
#define SO_REUSEPORT 10

#ifdef __cplusplus 
}
#endif
