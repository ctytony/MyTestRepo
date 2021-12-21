// Copyright (c) 1996-2016 IntervalZero Inc.  All rights reserved.

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _DRVUTL_H
#define _DRVUTL_H

#ifdef UNDER_RTSS
#undef printf
#define printf	RtPrintf
#undef wprintf
#define wprintf RtWprintf
#endif

#ifndef RTNPROC
#define RTNPROC __declspec ( dllimport )
#endif

#ifndef RTNDPROC
#define RTNDPROC __declspec(dllexport)
#endif
//********************************************
// must match definitions in
// netioc.h
//********************************************
#ifndef ENIOCBASE

#define ENIOCBASE       ((unsigned short )('e' << 8))


/* Values for raw ethernet ioctls.  These select the action inside
 * the link layer ioctl routine.
 */
#define ENIOCEND        (ENIOCBASE+10)
#define ENIOCNORMAL     ENIOCBASE       /* accept broadcast and specific */
#define ENIOCPROMISC    (ENIOCBASE+1)   /* accept all undamaged packets */
#define ENIOCALL        (ENIOCBASE+2)   /* accept ALL packets */
#define ENIOCRESET      (ENIOCBASE+3)
#define ENIOCWHATRU     (ENIOCBASE+4)   /* return device name from ndevsw */
#define ENIOADDMULTI	(ENIOCBASE+5)	/* add multicast address */
#define	ENIODELMULTI	(ENIOCBASE+6)	/* delete multicast address */
#define ENIOLINKSTATUS  (ENIOCBASE+7)	/* Link Status*/

#define ENIOSHUTDOWN	(ENIOCBASE+80)	/* Shutdown*/
#endif
//*********************************************

typedef volatile long CriticalLock;

#define RTND_DEVICE_PREFIX "rtnd"
#define RTND_SIZE 20

#pragma pack(push,4)
typedef struct {
	unsigned char szDeviceName[RTND_SIZE];
	unsigned long DllHandle;
	void *pRtndInitialize;
	void *pRtndConfigure;
	void *pRtndTransmit;
	void *pRtndIoctl;
	void *pRtndUpDown;
	unsigned char szIpv4Address[20];
	unsigned char szIpv6Address[50];
	unsigned char szNetMask[20];
	unsigned char szDefaultGateway[20];
	unsigned long dwMtu;
	unsigned long IPv6Router;
	unsigned long IPv6RouterAdv;
	unsigned long IPv6RouterFwd;

	unsigned long FilterDllHandle;
	void *pRtnMACRxFilter;
	void *pRtnMACTxFilter;
	void *pRtnMACUpDownFilter;
	void *pRtnMACIOCTLFilter;
	void *pRtnMACConfigFilter;

	int	RxFilterState;
	int TxFilterState;
	int	UpDownFilterState;
	int IOCTLFilterState;
	int ConfigFilterState;
	unsigned char szIpv6prefix[5];
} RtndConfigStruct;
#pragma pack(pop)

//Enum Data Types//

typedef  enum _RTND_MEDIA_CONNECT_STATE {
		RtndMediaConnectStateUnknown,
		RtndMediaConnectStateConnected,
		RtndMediaConnectStateDisConnected
} RTND_MEDIA_CONNECT_STATE, *PRTND_MEDIA_CONNECT_STATE;

typedef  enum _RTND_MEDIA_DUPLEX_STATE {
		RtndMediaDuplexStateUnknown,
		RtndMediaDuplexStateHalf,
		RtndMediaDuplexStateFull
} RTND_MEDIA_DUPLEX_STATE, *PRTND_MEDIA_DUPLEX_STATE;

typedef enum _RTND_MEDIA_SPEED {
		RtndMediaSpeedUnknown,
		RtndMediaSpeed10,
		RtndMediaSpeed100,
		RtndMediaSpeed1000
} RTND_MEDIA_SPEED, *PRTND_MEDIA_SPEED;


//
//Stack Macros
//
#define	RtLIST_HEAD(name, type)						\
struct name {								\
	struct type *First;			\
}

#define	RtLIST_ENTRY(type)						\
struct {								\
	struct type *Next;				\
	struct type **Prev;		\
}


#define	RtLIST_EMPTY(head)	((head)->First == NULL)

#define	RtLIST_FIRST(head)	((head)->First)


#define	RtLIST_INIT(head) do {						\
	RtLIST_FIRST((head)) = NULL;					\
} while (0)

#define	RtLIST_INSERT_NEXT(listelm, elm, field) do {			\
	if ((RtLIST_NEXT((elm), field) = RtLIST_NEXT((listelm), field)) != NULL)\
		RtLIST_NEXT((listelm), field)->field.Prev =		\
		    &RtLIST_NEXT((elm), field);				\
	RtLIST_NEXT((listelm), field) = (elm);				\
	(elm)->field.Prev = &RtLIST_NEXT((listelm), field);		\
} while (0)


#define	RtLIST_INSERT_HEAD(head, elm, field) do {				\
	if ((RtLIST_NEXT((elm), field) = RtLIST_FIRST((head))) != NULL)	\
		RtLIST_FIRST((head))->field.Prev = &RtLIST_NEXT((elm), field);\
	RtLIST_FIRST((head)) = (elm);					\
	(elm)->field.Prev = &RtLIST_FIRST((head));			\
} while (0)

#define	RtLIST_NEXT(elm, field)	((elm)->field.Next)

#define	RtLIST_REMOVE(elm, field) do {					\
	if (RtLIST_NEXT((elm), field) != NULL)				\
		RtLIST_NEXT((elm), field)->field.Prev = 		\
		    (elm)->field.le_prev;				\
	*(elm)->field.Prev = RtLIST_NEXT((elm), field);		\
} while (0)

/// RTND_REQUEST


typedef  unsigned long RTND_OID; 


#define RTND_OID_GEN_BYTES_RCV				\
							((RTND_OID) 0x00020219)

#define RTND_OID_GEN_BYTES_XMIT				\
	 						((RTND_OID) 0x0002021A)

#define RTND_OID_GEN_RCV_DISCARDS			\
	 						((RTND_OID) 0x0002021B)

#define RTND_OID_GEN_XMIT_DISCARDS			\
	 						((RTND_OID) 0x0002021C)

#define RTND_OID_GEN_RCV_OK					\
					 		((RTND_OID) 0x00020102)

#define RTND_OID_GEN_XMIT_OK				\
					 		((RTND_OID) 0x00020101)

#define RTND_OID_GEN_RCV_ERROR				\
	 						((RTND_OID) 0x00020104)

#define RTND_OID_GEN_XMIT_ERROR				\
					 		((RTND_OID) 0x00020103)

#define RTND_OID_GEN_MULTICAST_FRAMES_RCV	\
					 		((RTND_OID) 0x0002020A)

#define RTND_OID_GEN_MULTICAST_FRAMES_XMIT	\
					 		((RTND_OID) 0x00020204)

#define RTND_OID_GEN_BROADCAST_FRAMES_RCV	\
					 		((RTND_OID) 0x0002020C)

#define RTND_OID_GEN_BROADCAST_FRAMES_XMIT	\
					 		((RTND_OID) 0x00020206)
#define RTND_OID_GEN_LINK_SPEED				\
							((RTND_OID) 0x00010107)

#define RTND_OID_GEN_ALL_SUPPORTED_STAT_QUERY	\
							((RTND_OID) 0x80000000)

#define RTND_OID_GEN_STATISTICS					\
							((RTND_OID) 0x00020106)
							
#define RTND_OID_GEN_MAC_ADDRESS					\
							((RTND_OID) 0x00010205)

#define RTND_OID_GEN_MEDIA_CONNECT_STATUS			\
							((RTND_OID) 0x00010207)

#define RTND_OID_GEN_MEDIA_DUPLEX_STATE				\
							((RTND_OID) 0x0001028C)

//Flags for RTND_STATISTICS_INFO->SupportedStatistics structure

#define RTND_STATISTICS_FLAGS_VALID_BYTES_RCV                       0x00000008
#define RTND_STATISTICS_FLAGS_VALID_RCV_DISCARDS                    0x00000010
#define RTND_STATISTICS_FLAGS_VALID_RCV_ERROR                       0x00000020
#define RTND_STATISTICS_FLAGS_VALID_BYTES_XMIT                      0x00000200
#define RTND_STATISTICS_FLAGS_VALID_XMIT_ERROR                      0x00000400
#define RTND_STATISTICS_FLAGS_VALID_XMIT_DISCARDS                   0x00008000
#define RTND_STATISTICS_FLAGS_VALID_MULTICAST_BYTES_RCV             0x00020000
#define RTND_STATISTICS_FLAGS_VALID_BROADCAST_BYTES_RCV             0x00040000
#define RTND_STATISTICS_FLAGS_VALID_MULTICAST_BYTES_XMIT            0x00100000
#define RTND_STATISTICS_FLAGS_VALID_BROADCAST_BYTES_XMIT            0x00200000

#define RTND_STATISTICS_FLAGS_VALID_RCV_OK					            0x01000000
#define RTND_STATISTICS_FLAGS_VALID_XMIT_OK								0x02000000

//RTND_STATUS 
typedef  int 							\
								RTND_STATUS, *PRTND_STATUS; 
#define  RTND_STATUS_SUCCESS			\
								((RTND_STATUS)0x00000001)
#define  RTND_STATUS_INVALID_REQUEST	\
								((RTND_STATUS)0xC0000010)
#define  RTND_STATUS_INVALID_LENGTH		\
								((RTND_STATUS)0xC0000100)
#define  RTND_STATUS_FAILURE			\
								((RTND_STATUS)0xFFFFFFFF)

//Network Adapter Info Structure
typedef struct _RTND_REQUEST {
	RTND_OID			Oid;
	void*		 		InformationBuffer;
	unsigned int		InformationBufferLength;
}RTND_REQUEST, *PRTND_REQUEST;


//Statistics info structure
typedef struct _RTND_STATISTICS_INFO{
   unsigned long			SupportedStatistics;
   unsigned long			BytesRcv; 
   unsigned long			BytesXmit; 
   unsigned long			RcvDiscards;
   unsigned long			XmitDiscards;
   unsigned long			RcvOk;
   unsigned long			XmitOk;
   unsigned long			RcvError; 
   unsigned long			XmitError; 
   unsigned long			RcvMulticastPackets;
   unsigned long			XmitMulticastPackets;
   unsigned long			RcvBroadcastPackets; 
   unsigned long			XmitBroadcastPackets;
} RTND_STATISTICS_INFO, *PRTND_STATISTICS_INFO;

//
// Stack functions
//
RTNPROC void RtnQueueRecvPacket(void *ndptr, void *mptr, unsigned long packetsize);
RTNPROC int RtnSetDefaultGateway(void *ndptr, unsigned long Gateway);
RTNPROC void RtnAllocRecvBuffers(void *ndptr);
RTNPROC void *RtnGetPacket(void *ndptr, long Length);
RTNPROC void RtnFreeRecvBuffers(void *ndptr);
RTNPROC void RtnProcessRecvQueue(void *ndptr);
RTNPROC void RtnTransmitDone(void *ndptr);
RTNPROC void RtnSetLinkAddress(void *ndptr, unsigned char *Addr, long Length, int txpendlimit);
RTNPROC void *RtnGetDevicePtr(char *szDeviceName);
RTNPROC int RtnAddMultiRoute(char *DeviceName, int Family, unsigned long IpAddress);
RTNPROC int RtnDeleteMultiRoute(char *DeviceName, int Family, unsigned long IpAddress);

//
// Utility functions
//
RTNPROC int RtnGetMcastCount(void *ndptr);
RTNPROC void *RtnGetDeviceFromIpAddress(unsigned long IpAddress);
RTNPROC void RtnLedWrite(long Value);

RTNPROC int RtnGetPrivateProfileString (const unsigned short *FileName,
									char* pszSection,
									char* pszEntry,
									char* pszDefault,
									char* pszBuffer,
									int cbBuffer );
RTNPROC void RtnDecodePacket(void *mptr,
				  unsigned long *ndp,
				  unsigned long *data,
				  unsigned long *lendth);
RTNPROC unsigned long RtnHtoi( char* pszHex );
RTNPROC void RtnSetDataLong(void *ndptr, long Offset, unsigned long Data);
RTNPROC unsigned long RtnGetDataLong(void *ndptr, long Offset);
RTNPROC char *RtnGetDeviceName(void *ndptr);
RTNPROC unsigned long RtnGetIpAddress(void *ndptr);
RTNPROC void RtnLeaveCriticalLock(CriticalLock *pLock);
RTNPROC void RtnEnterCriticalLock(CriticalLock *pLock);
RTNPROC void RtnInitializeCriticalLock(CriticalLock *pLock);
RTNPROC void RtnDeleteCriticalLock(CriticalLock *pLock);
RTNPROC int RtnGetMsPerTick(void);

RTNPROC int RtnInstallStaticRoute(void *ndptr, const char *pTarget, 
								int prefixLen, const char *pNextHop);
RTNPROC int RtnRemoveStaticRoute(void *ndptr, const char *pTarget, 
								 int prefixLen, const char *pNextHop);
RTNPROC void RtnDisplayRoutingTable(void);
RTNPROC void RtnDisplayNbrCacheTable(void *ndptr);


RTNDPROC void RtndRequest (
							PRTND_STATUS	Status,
							char*	 		DeviceName,
							PRTND_REQUEST	RtndRequest
						  );
typedef int      BOOL;
RTNPROC BOOL RtnIsDeviceOnline(
	const char * devname);

RTNPROC BOOL RtnIsStackOnline();

RTNPROC void RtnGetArpTable(void * ptr);

RTNPROC int RtnAddArp(
					   char * pDevName, 
					   unsigned char * pLinkAddr, 
					   unsigned char *pProtoAddr, 
					   unsigned long TTL 
					  );

RTNPROC int RtnDeleteArp(
						  char * pDevName, 
						  unsigned char *pProtoAddr 
						 );

#ifndef __INDRVUTL__
RTNPROC int RtnEnumPciCards
(
	int (*fnCheckPciCard)
	(
		PPCI_COMMON_CONFIG pPciInfo,
		ULONG bus,
		PCI_SLOT_NUMBER SlotNumber,
		PVOID pCardInfo,
		int fMacAddressPresent
	),
	PVOID pCardInfo,
	int fMacAddressPresent
);
#endif

//
// Stack called (only) utility functions
//
RTNPROC int RtnInitDeviceTable(RtndConfigStruct *RtCfg);

int os_disintr(void);
void os_resintr(int lastState);
int set_ip_defttl(int s, int ttl);
int get_ip_defttl(int s, char *optval, int *optlen);



#endif //_DRVUTL_H

#ifdef __cplusplus 
}
#endif
