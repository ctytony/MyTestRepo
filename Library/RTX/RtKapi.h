/* -*-C++-*-
 *
 *      Copyright (c) 1996-2016 IntervalZero, Inc.  All rights reserved.
 *
 *      rtkapi.h -- Real-time API data types and function prototypes
 *                  for NT drivers to call RTX/RTSS IPC functions
 *      prototype - 12/10/97 - sas
 */

#ifndef _RTKAPI_H_
#define _RTKAPI_H_

#include "Licensing.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define RTKAPI __stdcall

#define LPSECURITY_ATTRIBUTES PVOID

//
// RtssInstance - returned from call to RtxRtssAttach and required for all
// calls to RTK functions
#define RTSSINST ULONG
//
// Before any of the calls listed below, the driver writer is required to call 
// RtkRtssAttach(Ex) to obtain the Rtss Instance.
// All RTKAPI calls require the instance identifier

  

// Requires the maximum WaitForSingleObjects value, must be > 0

// This function attaches a kernel device driver to RTSS.
RTSSINST RTKAPI RtkRtssAttachEx( 
                        LONG MaxWFSO, 
                        PULONG  pErrorCode,
                        VOID (RTKAPI *Routine)(PVOID context, LONG reason),
                        PVOID Context);

// This function attaches a kernel device driver to RTSS.
RTSSINST RTKAPI RtkRtssAttach( 
                        LONG MaxWFSO, 
                        PULONG  pErrorCode);

// This function detaches a kernel device driver from RTSS.
VOID  RTKAPI RtkRtssDetach( 
                        RTSSINST RtssInst,
                        PULONG  pErrorCode);

// This function waits until an object is signaled or the specified timeout period has elapsed.  If
// parameter Milliseconds is INFINITE, this will wait indefinitely.
ULONG RTKAPI RtkWaitForSingleObject(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        HANDLE hHandle,
                        ULONG Milliseconds);


// This function waits until at least one of a set of objects is signaled or until the specified
// timeout period elapses.  If parameter dwMilliseconds is INFINITE, this will wait indefinitely.
// In the RTSS environment, parameter bWaitAll must be FALSE!
ULONG RTKAPI RtkWaitForMultipleObjects( 
						RTSSINST RtssInst,
                        PULONG   pErrorCode,
						ULONG    dwCount,
                        CONST HANDLE *lpHandles,
	                    BOOLEAN	 bWaitAll,
                        ULONG	 dwMilliseconds);

// This function creates an RTSS mutex.
HANDLE RTKAPI RtkCreateMutex(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        LPSECURITY_ATTRIBUTES pSecurity,
                        BOOLEAN bInitialOwner,
                        PUNICODE_STRING lpName);

// This function opens an existing RTSS mutex.  The name is case-sensitive.
HANDLE RTKAPI RtkOpenMutex(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        ULONG   DesiredAccess,
                        BOOLEAN bInheritHandle,
                        PUNICODE_STRING lpName);

// This function relinquishes ownership of the specified RTSS mutex.
BOOLEAN RTKAPI RtkReleaseMutex(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        HANDLE hMutex);

// This function creates an RTSS semaphore.
HANDLE RTKAPI RtkCreateSemaphore(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        LPSECURITY_ATTRIBUTES pSecurity,
                        LONG lInitialCount,
                        LONG lMaximumCount,
                        PUNICODE_STRING lpName);

// This function opens an existing RTSS semaphore.  The name is case-sensitive.
HANDLE RTKAPI RtkOpenSemaphore(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        ULONG DesiredAccess,
                        BOOLEAN bInheritHandle,
                        PUNICODE_STRING lpName);

// This function increases the count of the specified semaphore object by a specified amount.
BOOLEAN RTKAPI RtkReleaseSemaphore(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        HANDLE hSemaphore,
                        LONG lReleaseCount,
                        PLONG lpPreviousCount);

// This function creates an RTSS event object.
HANDLE RTKAPI RtkCreateEvent(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        LPSECURITY_ATTRIBUTES pSecurity,
                        BOOLEAN bManualReset,
                        BOOLEAN bInitialState,
                        PUNICODE_STRING lpName);

// This function opens an existing RTSS event object.  The name is case-sensitive.
HANDLE RTKAPI RtkOpenEvent(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        ULONG   DesiredAccess,
                        BOOLEAN bInheritHandle,
                        PUNICODE_STRING lpName);

// This function sets the specified event to the signaled state, then resets the event to the
// non-signaled state after releasing the appropriate number of threads.  For a manual-reset event,
// all waiting threads that can be released are released.  For an auto-reset event, a single waiting
// thread is released.
BOOLEAN RTKAPI RtkPulseEvent(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        HANDLE hEvent);

// This function resets the specified RTSS event to the non-signaled state.
BOOLEAN RTKAPI RtkResetEvent(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        HANDLE hEvent);

// This function sets the specified RTSS event to the signaled state.
BOOLEAN RTKAPI RtkSetEvent(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        HANDLE hEvent);

// This function creates an RTSS shared memory object.
HANDLE RTKAPI RtkCreateSharedMemory(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        ULONG flProtect,
                        ULONG MaximumSizeHigh,
                        ULONG MaximumSizeLow,
                        PUNICODE_STRING lpName,
                        VOID ** location);

// This function opens an existing RTSS shared member object.  The name is case-sensitive.
HANDLE RTKAPI RtkOpenSharedMemory(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        ULONG   DesiredAccess,
                        BOOLEAN bInheritHandle,
                        PUNICODE_STRING lpName,
                        VOID ** location);

BOOLEAN RTKAPI RtkUnmapSharedMemory(
                        RTSSINST RtssInst,
                        PULONG pErrorCode,
                        VOID * location 
                        );

// This function closes an open object handle.
BOOLEAN RTKAPI RtkCloseHandle(
                        RTSSINST RtssInst,
                        PULONG  pErrorCode,
                        HANDLE hObject);

// This function retrieves information about all licenses available on the system
BOOLEAN RTKAPI RtkGetLicenses(
	RTSSINST RtssInst,
	PULONG pErrorCode,
	PRT_LICENSE_INFO_W pResult,
	PULONG pLength);

// This function verifies whether the specified version of the RTX TCP-IP stack is installed and has
// a valid license.
BOOLEAN RTKAPI RtkIsTcpStackLicensed(
	RTSSINST RtssInst, 
	PULONG pErrorCode,
	int majorVersion);

// This function verifies whether the specified version of the RTX runtime is installed and has a
// valid license.
BOOLEAN RTKAPI RtkIsRuntimeLicensed(
	RTSSINST RtssInst,
	PULONG pErrorCode,
	int majorVersion);

// This function returns whether the specified RTSS application binary can run.  This means it has
// been built to run with the provided license feature and that there is a valid license for the
// feature on the system.
BOOLEAN RTKAPI RtkIsAppRunnable(
	RTSSINST RtssInst,
	PULONG pErrorCode, 
	PUNICODE_STRING pApplicationPath,
	PUNICODE_STRING pFeatureName,
	unsigned int reserved );

#ifdef __cplusplus
}
#endif

#endif // _RTKAPI_H_
