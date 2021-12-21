/*
 *
 *      Copyright (c) 1996-2010 IntervalZero, Inc.  All rights reserved.
 *
 *      RtxProp.h -- RTX Properties API data types and function prototypes.
 *
 *   
 */
#pragma once
#define _RTX_PROPERTIES_

#ifdef RTXPROP_EXPORTS
#define RTXPROP_API __declspec(dllexport)
#else
#define RTXPROP_API __declspec(dllimport)
#endif

#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif

//Editions
#define PRODUCT_NO_EDITION		 0
#define PRODUCT_RT_SOLO              1
#define PRODUCT_RT_ENTRY             2
#define PRODUCT_RT_BASIC             3
#define PRODUCT_RT_PROFESSIONAL      4
#define PRODUCT_RT_PREMIUM           5
#define PRODUCT_RT_ULTIMATE          6

//This define matches windows.h define for PRODUCT_UNLICENSED
#define PRODUCT_UNLICENSED		0xABCDABCD

// General
#define GENERAL_ENABLE	1
#define GENERAL_DISABLE	0

//Debugging
#define DBG_USER_SUPPORT	0
#define DBG_KRNL_SUPPORT	1
#define DBG_NO_SUPPORT		2

//Processor Types
#define PROCESSOR_PIC	"UP"
#define PROCESSOR_APIC	"UP APIC"
#define PROCESSOR_MP	"MP"

//maximum number of RTSS processors
#if (!defined(MAXNUM_RTPROCESSORS))
#define MAXNUM_RTPROCESSORS 0x1F
#endif

//maximum number of RTSS+Windows processors
#if (!defined(MAXNUM_TOTALPROCESSORS))
#define MAXNUM_TOTALPROCESSORS 0x20
#endif

//Hal Timer Period
//PIC systems only support values of 100 and above
#define HAL_TIMER_PERIOD_UNKNOWN	0
#define HAL_TIMER_PERIOD_1			1
#define HAL_TIMER_PERIOD_2			2
#define HAL_TIMER_PERIOD_5			5
#define HAL_TIMER_PERIOD_10			10
#define HAL_TIMER_PERIOD_20			20
#define HAL_TIMER_PERIOD_50			50
#define HAL_TIMER_PERIOD_100		100 
#define HAL_TIMER_PERIOD_200		200
#define HAL_TIMER_PERIOD_500		500	//default
#define HAL_TIMER_PERIOD_1000		1000

//Tracing
#define TRACE_ENABLE  0
#define TRACE_DISABLE 1

//Starvation - this is defined in RTX.h
#ifndef __RTX_H__
typedef enum _STARVATION_TIMEOUT_BEHAVIOR {
    STARVATION_DISABLE = 0,
    STARVATION_FREEZE,
    STARVATION_YIELD
} STARVATION_TIMEOUT_BEHAVIOR;
#endif

//Prioirity Inversion protocol available
#define PRIORITY_INVERSION_DISABLED	0
#define PRIORITY_INVERSION_LIMITED_DEMOTION	1
#define PRIORITY_INVERSION_TIERED_DEMOTION	2

//Shutdown Print Handling
#define HANDLE_PRINT_ALWAYS			0 //default-only option for MP dedicated
#define HANDLE_PRINT_ON_REQUEST		1
#define HANDLE_PRINT_NEVER			2
#define HANDLE_PRINT_UNKNOWN		3

//RTX Start Types
#define RTX_BOOT_START				0
#define RTX_DEMAND_START			1
#define RTX_PNP_BOOT_START			2

//RTX Device Dispositions
#define DEVICE_DISPOSITION_UNKNOWN		0
#define DEVICE_DISPOSITION_EXCLUSIVE	1
#define DEVICE_DISPOSITION_SHARED		2

#define COMPONENT_CUSTOM		0x10000000
//Driver Status
#define COMPONENT_SUBSYSTEM		0x00110111
#define COMPONENT_TCPIP			0x00100000
#define COMPONENT_W32			0x00001000
#define COMPONENT_SERVER		0x00000100
#define COMPONENT_RTX_RTSS		0x00000010
#define COMPONENT_HAL_EXT		0x00000001
#define ALL_DRIVERS_STOPPED		0x00000000
#define COMPONENT_BASE_DRIVERS	0x00000111

#define ARE_ALL_DRIVERS_STOPPED( data ) \
		((data) == ALL_DRIVERS_STOPPED)

#define ARE_ALL_BASE_DRIVERS_RUNNING( data ) \
		(((data) & (0x0000FFFF)) == COMPONENT_BASE_DRIVERS)

#define IS_COMPONENT_RUNNING(part, data)\
		(((data) & (part*0x0000000F)) == part)

#define IS_COMPONENT_START_PENDING(part, data)\
		(((data) & (part*0x0000000F)) == part*SERVICE_START_PENDING)

#define IS_COMPONENT_STOPPED(part, data)\
		(((data) & (part*0x0000000F)) == 0)

#define IS_COMPONENT_STOP_PENDING(part, data)\
		(((data) & (part*0x0000000F)) == part*SERVICE_STOP_PENDING)

//Exception types
#define EXCEPTION_DIVIDED_BY_ZERO (0x00)
#define EXCEPTION_DEBUG (0x01)
#define EXCEPTION_INT3 (0x03)
#define EXCEPTION_UNKNOWN (0x04)
#define EXCEPTION_BOUND_CHECK (0x05)
#define EXCEPTION_INVALID_OPCODE (0x06)
#define EXCEPTION_DOUBLE_FAULT (0x08)
#define EXCEPTION_STACK_FAULT (0x0C)
#define EXCEPTION_GP_FAULT (0x0D)
#define EXCEPTION_PAGE_FAULT (0x0E)
#define EXCEPTION_NPX_ERROR (0x10)
#define EXCEPTION_SIMD_NPX_ERROR (0x13)

//buffer size defines
#define MID_BUFFER			128
#define LARGE_BUFFER		200
#define SMALL_BUFFER		50

#define DEVICE_NOT_IN_INF				0
#define DEVICE_IN_INF_NO_FILTER			1
#define DEVICE_IN_INF_FILTERED_IO_PORT	2

//
// Object types.
//
#define OT_PROCESS			1
#define OT_THREAD			2
#define OT_EVENT			3
#define OT_SEMAPHORE		4
#define OT_MUTEX			5
#define OT_SHAREDMEMORY		6
#define OT_TIMER			7
#define OT_INTERRUPT		8
#define OT_SHUTDOWN			9
#define OT_FILE				10
#define OT_ATTACHMENT		11
#define OT_LIBRARY			12
#define OT_PROCLIB          13
#define OT_WAITABLETIMER	14
#define OT_PCISLOT          15
#define OT_LAST_TYPE		15

#ifdef UNICODE
#define RTXPROP_LICENSE_FEATURE_INFO    RTXPROP_LICENSE_FEATURE_INFOW
#define PRTXPROP_LICENSE_FEATURE_INFO    PRTXPROP_LICENSE_FEATURE_INFOW
#define RTXLICENSEDUSERINFO		RTXLICENSEDUSERINFOW
#define PRTXLICENSEDUSERINFO	PRTXLICENSEDUSERINFOW
#define MPSYSTEMINFO	MPSYSTEMINFOW
#define PMPSYSTEMINFO	PMPSYSTEMINFOW
#define DEVICEINFO		DEVICEINFOW
#define	PDEVICEINFO		PDEVICEINFOW
#define	DEVICEINFO_DATA		DEVICEINFO_DATAW
#define PDEVICEINFO_DATA	PDEVICEINFO_DATAW
#define RTSSAPPINFO		RTSSAPPINFOW
#define PRTSSAPPINFO		PRTSSAPPINFOW
#define RTDLLINFO	RTDLLINFOW
#define PRTDLLINFO	PRTDLLINFOW
#define OBJINFO OBJINFOW
#define POBJINFO POBJINFOW
#else
#define RTXPROP_LICENSE_FEATURE_INFO    RTXPROP_LICENSE_FEATURE_INFOA
#define PRTXPROP_LICENSE_FEATURE_INFO    PRTXPROP_LICENSE_FEATURE_INFOA
#define RTXLICENSEDUSERINFO		RTXLICENSEDUSERINFOA
#define PRTXLICENSEDUSERINFO	PRTXLICENSEDUSERINFOA
#define MPSYSTEMINFO	MPSYSTEMINFOA
#define PMPSYSTEMINFO	PMPSYSTEMINFOA
#define DEVICEINFO		DEVICEINFOA
#define	PDEVICEINFO		PDEVICEINFOA
#define	DEVICEINFO_DATA		DEVICEINFO_DATAA
#define PDEVICEINFO_DATA	PDEVICEINFO_DATAA
#define RTSSAPPINFO		RTSSAPPINFOA
#define PRTSSAPPINFO		PRTSSAPPINFOA
#define RTDLLINFO	RTDLLINFOA
#define PRTDLLINFO	PRTDLLINFOA
#define OBJINFO OBJINFOA
#define POBJINFO POBJINFOA

#endif //!UNICODE


// This enumeration is the return type of function RtcplGetLicenseInfo().
typedef enum _RTXPROP_LICENSE_INFO_STATUS
{
    // The license information for a given feature was successfully obtained.
    LICENSE_SUCCESS = 0,

    // An expected licensing-related error occurred.
    LICENSE_FAILURE,

    // A Windows API call failed.  Call GetLastError() to obtain the error code.
    LICENSE_WINDOWS_ERROR,

    // The pathname to the license file cannot be obtained.
    LICENSE_FILE_PATHNAME_FAILURE,

    // The license file cannot be accessed.
    LICENSE_FILE_ACCESS_FAILURE,

    // The Properties library failed to lock the license file serialization mutex.
    LICENSE_MUTEX_FAILURE,

    // A low-level licensing API call failed.
    LICENSE_LICENSING_ERROR,
} RTXPROP_LICENSE_INFO_STATUS;


// Data Structures

// This structure holds the license information for one licensed feature.
typedef struct _RTXPROP_LICENSE_FEATURE_INFOA
{
    // Holds the version of this structure.  This is a monotonically increasing unsigned int value.
    // This version value MUST be incremented by 1 every time a change is made to this structure
    // that would cause misinterpretation of its contents by client code compiled against an
    // older version.  It is STRONGLY recommended that client code check the value of this member
    // for the expected value.  If the value is not what the client expects, there is version skew
    // between the client and the Properties library, and the client should not access any other
    // members of this structure, because doing so risks causing a program crash.
    unsigned int Version;

    // Holds the feature name from the license.  The string in this member is always entirely
    // single-byte ANSI characters.
    char FeatureName[64];

    // TRUE if the feature identified by member FeatureName is licensed, FALSE if not.
    BOOL IsFeatureLicensed;

    // TRUE if this feature's license is permanent, FALSE if not.
    BOOL IsPermanent;

    // If member IsPermanent is TRUE, then this member is undefined and should not be accessed.
    // If member IsPermanent is FALSE, then if this feature's license has expired, this member
    // is TRUE, otherwise it is FALSE.
    BOOL IsExpired;

    // Holds the major version number of the product for this feature from the license.
    unsigned int ProductMajorVersion;

    // Holds the minor version number of the product for this feature from the license.  This value
    // can be less than the actual minor version number of the installed product.
    unsigned int ProductMinorVersion;

    // If members IsPermanent and IsExpired are FALSE, this holds the expiration data in the form
    // DD-MMM-YYYY, with no leading '0's on the day number, otherwise this member is undefined.
    // This string is entirely single-byte ANSI characters
    char ExpirationDate[16];

    // If members IsPermanent and IsExpired are FALSE, this holds the number of days remaining until
    // this license expires, otherwise this member should not be accessed.
    unsigned int ExpirationDaysRemaining;

    // Holds the hostid string specifying the machine to which this feature's license is
    // node-locked.  This string is entirely single-byte ANSI characters
    char Hostid[64];

    // If member FeatureName is the string "IZRTX" (case-insensitively), this member is defined,
    // otherwise it is undefined.  When defined, this member holds the number of licensed RTX
    // processors.  If this member is 0, the number of licensed processors is unlimited, which
    // should only occur if this is an evaluation runtime license (i.e., IsPermanent is FALSE).
    unsigned int RtxProcessors;

    // The value of this member is one of the _PRODUCT_TYPE_* macros defined in RtxVersion.h.
    unsigned int ProductType;

    // Holds the options string from this feature's license, if this feature's license has an options
    // string, otherwise this array contains all '\0' bytes.  This does not include the "options="
    // prefix from the license file nor any surrounding quotes.  This string is entirely
    // single-byte ANSI characters
    char Options[65];

} RTXPROP_LICENSE_FEATURE_INFOA, * PRTXPROP_LICENSE_FEATURE_INFOA;


typedef struct _RTXPROP_LICENSE_FEATURE_INFOW
{
    // Holds the version of this structure.  This is a monotonically increasing unsigned int value.
    // This version value MUST be incremented by 1 every time a change is made to this structure
    // that would cause misinterpretation of its contents by client code compiled against an
    // older version.  It is STRONGLY recommended that client code check the value of this member
    // for the expected value.  If the value is not what the client expects, there is version skew
    // between the client and the Properties library, and the client should not access any other
    // members of this structure, because doing so risks causing a program crash.
    unsigned int Version;

    // Holds the feature name from the license.  The string in this member is always entirely
    // single-byte ANSI characters.
    wchar_t FeatureName[64];

    // TRUE if the feature identified by member FeatureName is licensed, FALSE if not.
    BOOL IsFeatureLicensed;

    // TRUE if this feature's license is permanent, FALSE if not.
    BOOL IsPermanent;

    // If member IsPermanent is TRUE, then this member is undefined and should not be accessed.
    // If member IsPermanent is FALSE, then if this feature's license has expired, this member
    // is TRUE, otherwise it is FALSE.
    BOOL IsExpired;

    // Holds the major version number of the product for this feature from the license.
    unsigned int ProductMajorVersion;

    // Holds the minor version number of the product for this feature from the license.  This value
    // can be less than the actual minor version number of the installed product.
    unsigned int ProductMinorVersion;

    // If members IsPermanent and IsExpired are FALSE, this holds the expiration data in the form
    // DD-MMM-YYYY, with no leading '0's on the day number, otherwise this member is undefined.
    // This string is entirely single-byte ANSI characters
    wchar_t ExpirationDate[16];

    // If members IsPermanent and IsExpired are FALSE, this holds the number of days remaining until
    // this license expires, otherwise this member should not be accessed.
    unsigned int ExpirationDaysRemaining;

    // Holds the hostid string specifying the machine to which this feature's license is
    // node-locked.  This string is entirely single-byte ANSI characters
    wchar_t Hostid[64];

    // If member FeatureName is the string "IZRTX" (case-insensitively), this member is defined,
    // otherwise it is undefined.  When defined, this member holds the number of licensed RTX
    // processors.  If this member is 0, the number of licensed processors is unlimited, which
    // should only occur if this is an evaluation runtime license (i.e., IsPermanent is FALSE).
    unsigned int RtxProcessors;

    // The value of this member is one of the _PRODUCT_TYPE_* macros defined in RtxVersion.h.
    unsigned int ProductType;

    // Holds the options string from this feature's license, if this feature's license has an options
    // string, otherwise this array contains all '\0' bytes.  This does not include the "options="
    // prefix from the license file nor any surrounding quotes.  This string is entirely
    // single-byte ANSI characters
    wchar_t Options[65];

} RTXPROP_LICENSE_FEATURE_INFOW, * PRTXPROP_LICENSE_FEATURE_INFOW;


// version information is usually displayed in 
// the format:
// dwMajorVersion.dwMinorVersion.dwMicroVersion 
// Build Number: dwBuildNumber
typedef struct _RTXVERSIONINFO {
	DWORD MajorVersion;
	DWORD MinorVersion;
	DWORD MicroVersion;
	DWORD BuildNumber;
} RTXVERSIONINFO, *PRTXVERSIONINFO;

//Structure used for RTX user information.
// RTXLICENSEDUSERINFO Structure
typedef struct _RTXLICENSEDUSERINFOW {
	WCHAR User [MID_BUFFER];
	WCHAR Company [MID_BUFFER];
	WCHAR License [SMALL_BUFFER];
} RTXLICENSEDUSERINFOW, *PRTXLICENSEDUSERINFOW;

typedef struct _RTXLICENSEDUSERINFOA {
	CHAR User [MID_BUFFER];
	CHAR Company [MID_BUFFER];
	CHAR License [SMALL_BUFFER];
} RTXLICENSEDUSERINFOA, *PRTXLICENSEDUSERINFOA;

// Structure used for MP System information.
// MPSYSTEM Structure
typedef struct _MPSYSTEMINFOW {
	WCHAR ProcessorType[MID_BUFFER];
	DWORD NumberProcessors;
	BOOL Shared;
} MPSYSTEMINFOW, *PMPSYSTEMINFOW;

typedef struct _MPSYSTEMINFOA {
	CHAR ProcessorType[MID_BUFFER];
	DWORD NumberProcessors;
	BOOL Shared;
} MPSYSTEMINFOA, *PMPSYSTEMINFOA;

// Structure used for SMP System information.
// SYSTEM INFO Structure
#define MODE_DEDICATED	0
#define MODE_SHARED		1

typedef struct _SYSTEMINFO {
	DWORD NumberProcessors;
	DWORD Mode;
	DWORD NumberWindowsProcessors;
	DWORD NumberRTSSProcessors;
} SYSTEMINFO, *PSYSTEMINFO;

// _INFFILETYPE
// This enumeration represents which INF file would contain a device's hardware PnP
// info if the device's ownership were changed to RTX.  Yes, there's only two
// enumerators, so this could be a bool instead of an enum, but an enum is easier to
// scale if more INF files should come along.

typedef enum _INFFILETYPE
{
    INF_RTXPNP,
    INF_RTXPNPNET
} INFFILETYPE, *PINFFILETYPE;

// Structure used for general Device information.
// DEVICEINFO Structure
typedef struct _DEVICEINFOW {
	WCHAR Name [MID_BUFFER];
	BOOL  Troublesome;
	WCHAR HardwareId [LARGE_BUFFER];
	WCHAR Device [MID_BUFFER];
	WCHAR Vendor [MID_BUFFER];
    INFFILETYPE InfFile;
} DEVICEINFOW, *PDEVICEINFOW;

typedef struct _DEVICEINFOA {
	CHAR Name [MID_BUFFER];
	BOOL  Troublesome;
	CHAR HardwareId [LARGE_BUFFER];
	CHAR Device [MID_BUFFER];
	CHAR Vendor [MID_BUFFER];
    INFFILETYPE InfFile;
} DEVICEINFOA, *PDEVICEINFOA;

#ifndef RTAPI
typedef enum _MSI_CAPABILITY {
    NotMsiMsixCapable,
    MsiCapable,
    MsixCapable,
    MsiMsixCapable
} MSI_CAPABILITY, *PMSI_CAPABILITY;
#endif

// Structure used for detailed information on a device.
// DEVICEINFO_DATA Structure
typedef struct _DEVICEINFO_DATAW {
	PDEVICEINFOW DeviceInfo;
	WCHAR Class [SMALL_BUFFER];
	WCHAR Manufacturer [MID_BUFFER];
	WCHAR Location[LARGE_BUFFER];
	DWORD Slot;
	DWORD IRQ;	//RTX only
	DWORD Disposition;	//RTX only
	MSI_CAPABILITY MSICapable; //RTX only - Supported RTX 8.1 and above
	BOOL KeepLineBasedAvailable; //RTX only - Supported RTX 8.1 and above
	BOOL FilterOutIOPortResource;  //RTX only - Supported in RTX 8.1.2 and above
} DEVICEINFO_DATAW, *PDEVICEINFO_DATAW;

typedef struct _DEVICEINFO_DATAA {
	PDEVICEINFOA DeviceInfo;
	CHAR Class [SMALL_BUFFER];
	CHAR Manufacturer [MID_BUFFER];
	CHAR Location[LARGE_BUFFER];
	DWORD Slot;
	DWORD IRQ;	//RTX only
	DWORD Disposition;	//RTX only
	MSI_CAPABILITY MSICapable; //RTX only - Supported RTX 8.1 and above
	BOOL KeepLineBasedAvailable; //RTX only - Supported RTX 8.1 and above
	BOOL FilterOutIOPortResource;  //RTX only - Supported in RTX 8.1.2 and above
} DEVICEINFO_DATAA, *PDEVICEINFO_DATAA;

// Strucutre for detailed information on RTSS applications
typedef struct _RTSSAPPINFOA {
	CHAR Process[MAX_PATH];
	DWORD ProcessId;
	DWORD Flags;
	DWORD ProcessorAffinity;
	DWORD ProcessorAffinityMask;
	DWORD Status;
} RTSSAPPINFOA, *PRTSSAPPINFOA;

typedef struct _RTSSAPPINFOW {
	WCHAR Process[MAX_PATH];
	DWORD ProcessId;
	DWORD Flags;
	DWORD ProcessorAffinity;
	DWORD ProcessorAffinityMask;
	DWORD Status;
} RTSSAPPINFOW, *PRTSSAPPINFOW;

#define LAUNCH_FLAG_QUIET		0x1		// rtssrun /q flag - no error messages
#define LAUNCH_FLAG_LOCAL		0x2		// rtssrun /l flag
#define LAUNCH_FLAG_NONPAGED	0x4		// rtssrun /n flag
#define LAUNCH_FLAG_AUTO_START	0x8		// rtssrun /b flag
#define LAUNCH_FLAG_AFFINITY	0x10	// rtssrun /p flag
#define LAUNCH_FLAG_AFFINITY_MASK		0x20	// rtssrun /a flag 
#define LAUNCH_FLAG_WAIT		0x40	// rtssrun /w flag - internal use only
#define LAUNCH_FLAG_DYNAMIC		0x80	// rtssrun /y flag

#define TERM_FLAG_QUIET			0x1				// rtssrun /q flag - no error messages
#define TERM_FLAG_UNREGISTER	0x80		//rtsskill /b
#define TERM_FLAG_AFFINITY		0x100
#define TERM_FLAG_DYNAMIC		0x200		//rtsskill /y

// Strucutre for detailed information on RTDLLs
typedef struct _RTDLLINFOA {
	CHAR Rtdll[MAX_PATH];
	DWORD Flags;
	DWORD Status;
}RTDLLINFOA, *PRTDLLINFOA;

typedef struct _RTDLLINFOW {
	WCHAR Rtdll[MAX_PATH];
	DWORD Flags;
	DWORD Status;
}RTDLLINFOW, *PRTDLLINFOW;


#define REGISTER_FLAG_QUIET		0x1		// rtssrun /q flag - no error messages
#define REGISTER_FLAG_FORCE		0x100	// rtssrun /q flag - always register
#define REGISTER_FLAG_SHARED	0x200	// rtssrun /s 

#ifdef UNICODE
#define RtcplGetRunTimeDirectory  RtcplGetRunTimeDirectoryW
#define	RtcplGetLicensedUser	RtcplGetLicensedUserW
#define RtcplGetLicenseInfo     RtcplGetLicenseInfoW
#define RtcplGetMPSystemInfo	RtcplGetMPSystemInfoW
#define RtcplSetTCPIPINIFile	RtcplSetTCPIPINIFileW
#define RtcplGetTCPIPINIFile	RtcplGetTCPIPINIFileW
#define RtcplSetTCPIPSupport	RtcplSetTCPIPSupportW


#define RtcplListLatencyCausingDevices	RtcplListLatencyCausingDevicesW
#define RtcplEnableTroublesomeDevice	RtcplEnableTroublesomeDeviceW
#define RtcplIsDeviceInPNPINFFile	RtcplIsDeviceInPNPINFFileW
#define RtcplAddDeviceToPNPINFFile	RtcplAddDeviceToPNPINFFileW
#define RtcplRemoveDeviceFromPNPINFFile	RtcplRemoveDeviceFromPNPINFFileW
#define RtcplListWindowsDevices	RtcplListWindowsDevicesW
#define RtcplListRTXDevices		RtcplListRTXDevicesW
#define RtcplGetWindowsDeviceInfo	RtcplGetWindowsDeviceInfoW
#define RtcplGetRTXDeviceInfo	RtcplGetRTXDeviceInfoW
#define RtcplEditRTXDeviceInfo	RtcplEditRTXDeviceInfoW
#define RtcplLaunchRTSSProcess RtcplLaunchRTSSProcessW
#define RtcplRegisterRTDLL RtcplRegisterRTDLLW
#define RtcplUnRegisterRTDLL RtcplUnRegisterRTDLLW
#define RtcplListRTSSProcesses RtcplListRTSSProcessesW
#define RtcplListRegisteredRTDLLs RtcplListRegisteredRTDLLsW
#define RtcplLaunchProcess RtcplLaunchProcessW
#else
#define RtcplGetRunTimeDirectory  RtcplGetRunTimeDirectoryA
#define	RtcplGetLicensedUser	RtcplGetLicensedUserA
#define RtcplGetLicenseInfo     RtcplGetLicenseInfoA
#define RtcplGetMPSystemInfo	RtcplGetMPSystemInfoA
#define RtcplSetTCPIPINIFile	RtcplSetTCPIPINIFileA
#define RtcplGetTCPIPINIFile	RtcplGetTCPIPINIFileA
#define RtcplSetTCPIPSupport	RtcplSetTCPIPSupportA

#define RtcplListLatencyCausingDevices	RtcplListLatencyCausingDevicesA
#define RtcplEnableTroublesomeDevice	RtcplEnableTroublesomeDeviceA
#define RtcplIsDeviceInPNPINFFile	RtcplIsDeviceInPNPINFFileA
#define RtcplAddDeviceToPNPINFFile	RtcplAddDeviceToPNPINFFileA
#define RtcplRemoveDeviceFromPNPINFFile		RtcplRemoveDeviceFromPNPINFFileA
#define RtcplListWindowsDevices	RtcplListWindowsDevicesA
#define RtcplListRTXDevices		RtcplListRTXDevicesA
#define RtcplGetWindowsDeviceInfo	RtcplGetWindowsDeviceInfoA
#define RtcplGetRTXDeviceInfo	RtcplGetRTXDeviceInfoA
#define RtcplEditRTXDeviceInfo	RtcplEditRTXDeviceInfoA
#define RtcplLaunchRTSSProcess RtcplLaunchRTSSProcessA
#define RtcplRegisterRTDLL RtcplRegisterRTDLLA
#define RtcplUnRegisterRTDLL RtcplUnRegisterRTDLLA
#define RtcplListRTSSProcesses RtcplListRTSSProcessesA
#define RtcplListRegisteredRTDLLs RtcplListRegisteredRTDLLsA
#define RtcplLaunchProcess RtcplLaunchProcessA
#endif // !UNICODE

//About Functions

// RtcplSetDisableLicenseWarnings()
// Sets whether or not license expiration warnings are disabled, depending on the value of parameter
// "Disable".  If successful, returns TRUE, otherwise returns FALSE, and the client should call
// GetLastError() to obtain the Windows error code.
RTXPROP_API BOOL RtcplSetDisableLicenseWarnings(BOOL Disable);

// RtcplGetDisableLicenseWarnings()
// Queries whether or not license expiration warnings are disabled.  If successful, returns TRUE and
// sets the BOOL pointed to by parameter "pDisable" to TRUE or FALSE, depending on whether warnings
// are disabled.  If unsuccessfull, returns FALSE, does not modify the referent of parameter
// "pDisable", and the client should call GetLastError() to obtain the Windows error code.
RTXPROP_API BOOL RtcplGetDisableLicenseWarnings(PBOOL pDisable);

RTXPROP_API BOOL RtcplGetRunTimeVersion(PRTXVERSIONINFO Version);
RTXPROP_API BOOL RtcplGetProductInfo(DWORD majorVersion, 
				     DWORD minorVersion, 
				     PDWORD pReturnedProductType
				     );

RTXPROP_API BOOL RtcplGetRunTimeDirectoryW(LPWSTR Buffer, PDWORD Size);
RTXPROP_API BOOL RtcplGetRunTimeDirectoryA(LPSTR Buffer, PDWORD Size);

RTXPROP_API BOOL RtcplGetLicensedUserW(PRTXLICENSEDUSERINFOW User);
RTXPROP_API BOOL RtcplGetLicensedUserA(PRTXLICENSEDUSERINFOA User);

RTXPROP_API BOOL RtcplGetSystemInfo(PSYSTEMINFO Info);

RTXPROP_API BOOL RtcplGetMPSystemInfoW (PMPSYSTEMINFOW Info);
RTXPROP_API BOOL RtcplGetMPSystemInfoA (PMPSYSTEMINFOA Info);

// RtcplGetLicenseInfo()
// This function fills in the memory pointed to by parameter "licenseInfo" with an array of
// RTXPROP_LICENSE_FEATURE_INFO structures.  Parameter "numElements" must _ALWAYS_ be non-NULL.  If
// parameter "licenseInfo" is non-NULL, parameter "numElements" must point to an unsigned int value
// that holds the number of RTXPROP_LICENSE_FEATURE_INFO objects pointed to my parameter
// "licenseInfo".  If parameter "licenseInfo" is NULL, parameter "numElements" must point to an
// unsigned int value that will be filled in with the number of RTXPROP_LICENSE_FEATURE_INFO objects
// needed to hold the license information.  Thus, calls to this function must happen in pairs: the
// first call has "licenseInfo" set to NULL to learn the amount of memory to allocate, and the
// second call has "licenseInfo" pointing to sufficient memory to hold "numElements" instances of
// RTXPROP_LICENSE_FEATURE_INFO.
//
// Returns LICENSE_SUCCESS if successful, otherwise one of the other enumerators from
// enumeration RTXPROP_LICENSE_INFO_STATUS.
//
// NOTE: This function is _NOT_ thread-safe!
RTXPROP_API RTXPROP_LICENSE_INFO_STATUS RtcplGetLicenseInfoW(RTXPROP_LICENSE_FEATURE_INFOW * licenseInfo, unsigned int * numElements);
RTXPROP_API RTXPROP_LICENSE_INFO_STATUS RtcplGetLicenseInfoA(RTXPROP_LICENSE_FEATURE_INFOA * licenseInfo, unsigned int * numElements);

//Settings Functions
RTXPROP_API BOOL RtcplSetSubsystemBootStart(DWORD Enable,DWORD Force);
RTXPROP_API BOOL RtcplGetSubsystemBootStart (PDWORD Value);

RTXPROP_API BOOL RtcplSetStarvationTimeout (DWORD Behavior, DWORD Timeout);
RTXPROP_API BOOL RtcplGetStarvationTimeout (PDWORD Behavior, PDWORD Timeout);

RTXPROP_API BOOL RtcplSetFreeStackOnTerminateThread (DWORD Enable);
RTXPROP_API BOOL RtcplGetFreeStackOnTerminateThread (PDWORD Enable);

RTXPROP_API BOOL RtcplSetHALTimerPeriod (DWORD Period);
RTXPROP_API BOOL RtcplGetHALTimerPeriod (PDWORD Period);

RTXPROP_API BOOL RtcplSetTimeQuantum (DWORD Quantum);
RTXPROP_API BOOL RtcplGetTimeQuantum (PDWORD Quantum);

RTXPROP_API BOOL RtcplSetNumberOfProcessSlots (DWORD Slots);
RTXPROP_API BOOL RtcplGetNumberOfProcessSlots (PDWORD Slots);

RTXPROP_API BOOL RtcplSetShutdownPrintHandlingBehavior (DWORD Behavior);
RTXPROP_API BOOL RtcplGetShutdownPrintHandlingBehavior (PDWORD Behavior);

RTXPROP_API BOOL RtcplSetFullCacheFlushDelay (DWORD Enable);
RTXPROP_API BOOL RtcplGetFullCacheFlushDelay (PDWORD Enable);

RTXPROP_API BOOL RtcplSetPriorityInversionProtocol (DWORD Protocol);
RTXPROP_API BOOL RtcplGetPriorityInversionProtocol (PDWORD Protocol);

//Debugging Functions
RTXPROP_API BOOL RtcplSetFreezeFaultingProcessOnException (DWORD Enable);
RTXPROP_API BOOL RtcplGetFreezeFaultingProcessOnException (PDWORD Enable);

RTXPROP_API BOOL RtcplSetKernelDebugging (DWORD Enable);
RTXPROP_API BOOL RtcplGetKernelDebugging (PDWORD Enable);

RTXPROP_API BOOL RtcplSetWriteDebugInfo (DWORD Enable);
RTXPROP_API BOOL RtcplGetWriteDebugInfo (PDWORD Enable);

//Memory Configuration Functions
RTXPROP_API BOOL RtcplSetUseLocalMemoryByDefault (DWORD Enable);
RTXPROP_API BOOL RtcplGetUseLocalMemoryByDefault (PDWORD Enable);

RTXPROP_API BOOL RtcplSetLocalMemoryPoolSize (DWORD Size);
RTXPROP_API BOOL RtcplGetLocalMemoryPoolSize (PDWORD Size);

RTXPROP_API BOOL RtcplSetMapMemoryUpperBound (DWORD Size);
RTXPROP_API BOOL RtcplGetMapMemoryUpperBound (PDWORD Size);

RTXPROP_API BOOL RtcplSetAutoExpandLocalPool(DWORD Enable);
RTXPROP_API BOOL RtcplGetAutoExpandLocalPool(PDWORD Enable);

RTXPROP_API BOOL RtcplSetLocalPoolExpansionSize(DWORD Size);
RTXPROP_API BOOL RtcplGetLocalPoolExpansionSize(PDWORD Size);

//function to sense clustered mode
RTXPROP_API BOOL RtcplGetClusterModeState(PDWORD Enabled);

//function to sensse ACPI system
RTXPROP_API BOOL RtcplGetAcpiState (PDWORD pEnabled);


//Mobile Processor Configuration Functions 
//(flags that can be ORed together)
#define ENABLE_NO_C1_IDLE	1   // 0001
#define ENABLE_NO_IDLE		2	// 0010

RTXPROP_API BOOL RtcplSetWindowsIdleState (DWORD Enable);
RTXPROP_API BOOL RtcplGetWindowsIdleState (PDWORD Enable);

RTXPROP_API BOOL RtcplListLatencyCausingDevicesW (PDEVICEINFOW Devices, PDWORD Count);
RTXPROP_API BOOL RtcplListLatencyCausingDevicesA (PDEVICEINFOA Devices, PDWORD Count);

RTXPROP_API BOOL RtcplEnableTroublesomeDeviceW (PDEVICEINFOW Device, DWORD Enable);
RTXPROP_API BOOL RtcplEnableTroublesomeDeviceA (PDEVICEINFOA Device, DWORD Enable);

//Plug and Play Functions
RTXPROP_API BOOL RtcplListWindowsDevicesW ( PDEVICEINFOW Devices,  PDWORD Count);
RTXPROP_API BOOL RtcplListWindowsDevicesA ( PDEVICEINFOA Devices,  PDWORD Count);

RTXPROP_API BOOL RtcplGetWindowsDeviceInfoW (PDEVICEINFO_DATAW DevData);
RTXPROP_API BOOL RtcplGetWindowsDeviceInfoA (PDEVICEINFO_DATAA DevData);

RTXPROP_API BOOL RtcplListRTXDevicesW ( PDEVICEINFOW Devices,  PDWORD Count);
RTXPROP_API BOOL RtcplListRTXDevicesA( PDEVICEINFOA Devices,  PDWORD Count);

RTXPROP_API BOOL RtcplGetRTXDeviceInfoW (PDEVICEINFO_DATAW DevData);
RTXPROP_API BOOL RtcplGetRTXDeviceInfoA (PDEVICEINFO_DATAA DevData);

RTXPROP_API BOOL RtcplEditRTXDeviceInfoW (PDEVICEINFO_DATAW DevData);
RTXPROP_API BOOL RtcplEditRTXDeviceInfoA (PDEVICEINFO_DATAA DevData);

RTXPROP_API BOOL RtcplAddDeviceToPNPINFFileW (PDEVICEINFOW Device);
RTXPROP_API BOOL RtcplAddDeviceToPNPINFFileA (PDEVICEINFOA Device);

RTXPROP_API BOOL RtcplRemoveDeviceFromPNPINFFileW (PDEVICEINFOW Device);
RTXPROP_API BOOL RtcplRemoveDeviceFromPNPINFFileA (PDEVICEINFOA Device);

RTXPROP_API BOOL RtcplIsDeviceInPNPINFFileW (PDEVICEINFOW Device, PDWORD RetVal);
RTXPROP_API BOOL RtcplIsDeviceInPNPINFFileA (PDEVICEINFOA Device, PDWORD RetVal);


//TCP Settings Functions

RTXPROP_API BOOL RtcplIsTCPIPInstalled();

RTXPROP_API BOOL RtcplSetTCPIPSupportW (DWORD Enable, DWORD Depend, DWORD Verbose, LPWSTR INIFile);
RTXPROP_API BOOL RtcplSetTCPIPSupportA (DWORD Enable, DWORD Depend, DWORD Verbose, LPSTR INIFile);
RTXPROP_API BOOL RtcplGetTCPIPSupport (PDWORD Enable);

RTXPROP_API BOOL RtcplSetTCPIPVerboseMode (DWORD Enable);
RTXPROP_API BOOL RtcplGetTCPIPVerboseMode (PDWORD Enable);

RTXPROP_API BOOL RtcplSetTCPIPINIFileW (LPWSTR Buffer);
RTXPROP_API BOOL RtcplSetTCPIPINIFileA (LPSTR Buffer);

RTXPROP_API BOOL RtcplGetTCPIPINIFileW (LPWSTR Buffer, PDWORD Size);
RTXPROP_API BOOL RtcplGetTCPIPINIFileA (LPSTR Buffer, PDWORD Size);

RTXPROP_API BOOL  RtcplGetTCPIPAffinity (PDWORD Affinity);
RTXPROP_API BOOL  RtcplSetTCPIPAffinity (DWORD Affinity);


//RTX Subsystem Control Functions
RTXPROP_API BOOL RtcplStopSubsystemComponent(DWORD Component, DWORD Force);
RTXPROP_API BOOL RtcplStartSubsystemComponent(DWORD Component);

RTXPROP_API BOOL RtcplGetSubsystemComponentStatus ( PDWORD Status);

//Exception Handling Functions
RTXPROP_API BOOL RtcplSetExceptionStatus (DWORD ExceptionType, DWORD Enable, DWORD DebugBreak);
RTXPROP_API BOOL RtcplGetExceptionStatus (DWORD ExceptionType, PDWORD Enable, PDWORD DebugBreak);

//Process Functions
RTXPROP_API BOOL RtcplLaunchRTSSProcessA( LPSTR ApplicationName, LPSTR CommandLine, DWORD LaunchFlags, 
										 DWORD ProcessorAffinityMask, DWORD ProcessorAffinity, PDWORD ProcessorId);
RTXPROP_API BOOL RtcplLaunchRTSSProcessW( LPWSTR ApplicationName, LPWSTR CommandLine, DWORD LaunchFlags, 
										 DWORD ProcessorAffinityMask, DWORD ProcessorAffinity, PDWORD ProcessorId);
RTXPROP_API BOOL RtcplTerminateRTSSProcess( DWORD ProcessId, DWORD TerminationFlags);
RTXPROP_API BOOL RtcplRegisterRTDLLA(LPSTR RTDLLName,DWORD RegisterFlags);
RTXPROP_API BOOL RtcplRegisterRTDLLW(LPWSTR RTDLLName,DWORD RegisterFlags);
RTXPROP_API BOOL RtcplLaunchProcessA( LPSTR Cmdline, PPROCESS_INFORMATION ProcInfo, BOOL Wait);
RTXPROP_API BOOL RtcplLaunchProcessW( LPWSTR Cmdline, PPROCESS_INFORMATION ProcInfo, BOOL Wait);


RTXPROP_API BOOL RtcplUnRegisterRTDLLA(LPSTR RTDLLName);
RTXPROP_API BOOL RtcplUnRegisterRTDLLW(LPWSTR RTDLLName);

//
RTXPROP_API BOOL RtcplSetTotalNumberOfProcessors(DWORD total);
RTXPROP_API BOOL RtcplGetNumberOfProcessorsAvailable(PDWORD total);
RTXPROP_API BOOL RtcplModifyDefaultRTXConfiguration( DWORD Mode, DWORD RTX, DWORD Windows);

RTXPROP_API BOOL RtcplListRTSSProcessesA (PRTSSAPPINFOA Processes, PDWORD Count);
RTXPROP_API BOOL RtcplListRTSSProcessesW (PRTSSAPPINFOW Processes, PDWORD Count);

RTXPROP_API BOOL RtcplListRegisteredRTDLLsA (PRTDLLINFOA Rtdlls, PDWORD Count);
RTXPROP_API BOOL RtcplListRegisteredRTDLLsW (PRTDLLINFOW Rtdlls, PDWORD Count);

RTXPROP_API BOOL RtcplNotifyRtxServer(DWORD command);

RTXPROP_API BOOL RtcplSetTraceEnable (DWORD Level);
RTXPROP_API BOOL RtcplGetTraceEnable (PDWORD Level);

#ifdef __cplusplus
}
#endif

