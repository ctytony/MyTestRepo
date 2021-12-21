#ifndef COMMONDATATYPE_H
#define COMMONDATATYPE_H

//Camera Type Define
#define  MVC300MSZ					0x7672

#include "Windows.h"

//capture and preview information
struct CapInfoStruct {
	UCHAR       *Buffer;
	ULONG       Height;
	ULONG       Width;
	ULONG       HorizontalOffset;
	ULONG       VerticalOffset;
	ULONG       Exposure;
	UCHAR       Gain[3];
	UCHAR       Control;
	UCHAR       Reserved[8];
};

struct MVUserData {
	ULONG		DataLength;
	UCHAR		*Data;
};

typedef enum tagOS
{
	UNKNOWN_OS,
	WIN98,
	WINNT4,
	WINNT5
} OS;

typedef struct _tagMVRECT
{
	LONG   left;		// 窗口左上角水平坐标
	LONG   top;			// 窗口左上角垂直坐标
	LONG   width;		// 窗口客户区宽度
	LONG   height;		// 窗口客户区高度
} MVRECT, *PMVRECT;

//return code
#define ResSuccess					0x0000
#define ResNullHandleErr			0x0001
#define ResNullPointerErr			0x0002
#define ResFileOpenErr				0x0003
#define ResNoDeviceErr				0x0004
#define ResInvalidParameterErr		0x0005
#define ResOutOfMemoryErr			0x0006
#define ResNoPreviewRunningErr		0x0007
#define ResOSVersionErr				0x0008
#define ResUsbNotAvailableErr		0x0009
#define ResNotSupportedErr			0x000a
#define ResNoSerialString			0x000b
#define ResVerificationErr			0x000c
#define ResTimeoutErr	            0x000d
#define ResScaleModeErr             0x000e
#define ResPreviewRunningErr        0x000f
#define ResNotPauseErr              0x0010
#define ResPIDERR					0x0011
#define ResNoiseOverflow			0x0012
#define ResCapComdFailErr			0x0013
#define ResReadNullErr				0x0014
#define ResOpModeErr				0x0015
#define ResSNErr					0x0016
#define ResNoStartErr				0x0017
#define ResTrgFailedErr				0x0018
#define ResUnknownErr				0x00ff

typedef int MV_RETURN_CODE;
typedef ULONG	U32, *PU32;
typedef USHORT	U16, *PU16;
typedef UCHAR	U8,  *PU8;

#define WM_USEROVERLAY WM_USER+200

//callback function pointer
typedef
VOID
(WINAPI * LPMV_CALLBACK)(LPVOID lpParam);

typedef
VOID
(WINAPI * LPMV_CALLBACK2)(LPVOID lpParam, LPVOID lpUser);

typedef 
VOID 
(WINAPI * LPMV_OUTRIGGERCALLBACK)(LPVOID lpParam, LPVOID lpUser );	//*lpParam - 0 低电平 1 高电平

#endif