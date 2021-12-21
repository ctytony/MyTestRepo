/* -*-C++-*-
 *
 *      Copyright (c) 1996-2010 IntervalZero, Inc.  All rights reserved.
 *
 *      Rtapi.h -- Real-time API data types and function prototypes.
 *
 */

#ifndef _RTAPI_H_
#define _RTAPI_H_

#include "Licensing.h"

#ifdef __cplusplus
extern "C" {
#endif

// Specify a RTAPI version unless an override is in effect.
#ifndef RTAPIVER
#define RTAPIVER 30
#endif 

// Define API decoration for direct importing of DLL references.
#if !defined(_RTBASEAPI_) && !defined(MIDL_PASS)
#define RTBASEAPI __declspec(dllimport)
#else
#define RTBASEAPI
#endif

// Define RTAPI function calling convention unless an override is in effect.
#ifndef RTAPI
#       if (defined(_MSC_VER)&&(_MSC_VER >= 800)) || defined(_STDCALL_SUPPORTED)
#              define RTAPI __stdcall
#       else
#              define RTAPI
#       endif
#endif

// Max length used for names by RtCreate[Objtype], RtOpen[Objtype]
#define RTX_MAX_OBJECT_NAME_LENGTH	128

// Define the declaration for application provided functions.
#define RTFCNDCL RTAPI

//
// Provide backward (source level) compatability with pre-release RTX 4.1 functions
// to their current equivalent Win32 functions.
//
// NOTE: These are obsolete and will be removed in a future version.
//
#define	RtGetLastError		GetLastError
#define	RtSetLastError		SetLastError
#define RtCreateThread		CreateThread
#define	RtExitThread		ExitThread
#define	RtGetCurrentThread	GetCurrentThread
#define	RtResumeThread		ResumeThread
#define	RtSuspendThread		SuspendThread
#define	RtTerminateThread	TerminateThread
#define	RtExitProcess		ExitProcess
#define	RtGetStdHandle		GetStdHandle
#define	RtSleep			Sleep
#define	RtSleepEx		SleepEx
#ifdef UNICODE
#define	RtWriteConsole		WriteConsoleW
#define	RtGetCommandLine	GetCommandLineW
#else
#define	RtWriteConsole		WriteConsoleA
#define	RtGetCommandLine	GetCommandLineA
#endif // !UNICODE


#if RTAPIVER >= 20

// This function returns TRUE if the calling code is running in RTSS, FALSE if it is running in Windows.
RTBASEAPI BOOL RTAPI RtIsInRtss(VOID);

// This function converts a single-byte string to an integer.
RTBASEAPI int _cdecl RtAtoi(LPCSTR string);

// This function converts a Unicode string to an integer.
RTBASEAPI int _cdecl RtWtoi(LPCWSTR string);

// This function provides a subset of printf's functionality without needing to link to the C
// runtime library.  The first parameter is a single-byte string.
RTBASEAPI int _cdecl RtPrintf(LPCSTR fmt, ...);

// This function provides a subset of printf's functionality without needing to link to the C
// runtime library.  The first parameter is a Unicode string.
RTBASEAPI int _cdecl RtWprintf(LPCWSTR fmt, ...);

//
// Real-time priority defines.
//
#define RT_PRIORITY_MAX 127
#define RT_PRIORITY_MIN 0

// This function returns the priority of the specified thread.
RTBASEAPI int RTAPI RtGetThreadPriority(HANDLE hThread);

// This function sets the priority of the specified thread.
RTBASEAPI BOOL RTAPI RtSetThreadPriority(HANDLE hThread, int nPriority);

// This function returns the time quantum of the specified thread.
RTBASEAPI DWORD RTAPI RtGetThreadTimeQuantum(HANDLE hThread);

// This function sets the time quantum (in milliseconds) of the specified thread.
RTBASEAPI BOOL RTAPI RtSetThreadTimeQuantum(
			HANDLE hThread,
			DWORD dwQuantumInMS);

// This function returns the time quantum (in milliseconds) of the specified thread.
RTBASEAPI BOOL RTAPI RtGetThreadTimes(
			HANDLE hThread,
			LPFILETIME lpCreationTime,
			LPFILETIME lpExitTime,
			LPFILETIME lpExecuteTime);

//
// Shutdown Handler reason defines.
//
#define RT_SHUTDOWN_OPERATOR_REQUEST    1
#define RT_SHUTDOWN_NT_SYSTEM_SHUTDOWN  2
#define RT_SHUTDOWN_NT_STOP             3

// This function registers a stop notification handler function with RTSS.  The handler function is
// called in its own thread when one of the system stop events occurs.
RTBASEAPI HANDLE RTAPI RtAttachShutdownHandler(
			PSECURITY_ATTRIBUTES  pThreadAttributes,
			ULONG StackSize,                        
			VOID (RTFCNDCL *Routine)(PVOID context, LONG reason),
			PVOID Context,
			ULONG Priority);

// This function destroys the shutdown handler object created by RtAttachShutdownHandler.
RTBASEAPI BOOL RTAPI RtReleaseShutdownHandler(HANDLE hShutdown);

// This function waits until an object is signaled or the specified timeout period has elapsed.  If
// the timeout period is INFINITE, this will wait indefinitely.
RTBASEAPI DWORD RTAPI RtWaitForSingleObject(
   			HANDLE hHandle,
			DWORD dwMilliseconds);

// This function waits until an object is signaled or the specified high-granularity timeout period
// has elapsed.  If both lpWaitTimeOut->LowPart and lpWaitTimeOut->HighPart are INFINITE, this will
// wait indefinitely.
RTBASEAPI DWORD RTAPI RtWaitForSingleObjectEx(
    			HANDLE hHandle,
	            PULARGE_INTEGER  lpWaitTimedOut);

//
// Data structures for threads waiting (WFSO/WFMO) on synchronization objects.
//
#define	MAX_WFMO	16				// Maximum number of objects per WFMO


// This function waits until at least one of a set of objects is signaled or until the specified
// timeout period elapses.  If parameter dwMilliseconds is INFINITE, this will wait indefinitely.
// In the RTSS environment, parameter bWaitAll must be FALSE!
RTBASEAPI DWORD RTAPI RtWaitForMultipleObjects(
	DWORD			dwCount,
    CONST HANDLE	*lpHandles,
	BOOL			bWaitAll,
    DWORD			dwMilliseconds);

// This function waits until at least one of a set of objects is signaled or until the specified
// high-gramularity timeout period elapses.  If both lpWaitTimeOut->LowPart and
// lpWaitTimeOut->HighPart are INFINITE, this will wait indefinitely.  In the RTSS environment,
// parameter bWaitAll must be FALSE!
RTBASEAPI DWORD RTAPI RtWaitForMultipleObjectsEx(
	DWORD			 dwCount,
    CONST HANDLE	 *lpHandles,
	BOOL			 bWaitAll,
    PULARGE_INTEGER  lpWaitTimedOut);

// This function closes an open object handle.
RTBASEAPI BOOL RTAPI RtCloseHandle(HANDLE hObject);

#ifdef UNICODE
// This function creates a named or unnamed region of physical memory that can be mapped by any process.
#define RtCreateSharedMemory RtCreateSharedMemoryW
#else
// This function creates a named or unnamed region of physical memory that can be mapped by any process.
#define RtCreateSharedMemory RtCreateSharedMemoryA
#endif // !UNICODE

// This function creates a named or unnamed region of physical memory that can be mapped by any process.
RTBASEAPI HANDLE RTAPI RtCreateSharedMemoryA(
			DWORD flProtect,
			DWORD dwMaximumSizeHigh,
			DWORD dwMaximumSizeLow,
			LPCSTR lpName,
			VOID ** location);

// This function creates a named or unnamed region of physical memory that can be mapped by any process.
RTBASEAPI HANDLE RTAPI RtCreateSharedMemoryW(
			DWORD flProtect,
			DWORD dwMaximumSizeHigh,
			DWORD dwMaximumSizeLow,
			LPCWSTR lpName,
			VOID ** location);

// Open shared memory flags

#define SHM_MAP_WRITE 2
#define SHM_MAP_READ 1
#define SHM_MAP_ALL_ACCESS (SHM_MAP_WRITE + SHM_MAP_READ)

#ifdef UNICODE
// This function opens a named shared memory object.  The name is case-sensitive.
#define RtOpenSharedMemory RtOpenSharedMemoryW
#else
// This function opens a named shared memory object.  The name is case-sensitive.
#define RtOpenSharedMemory RtOpenSharedMemoryA
#endif // !UNICODE

// This function opens a named shared memory object.  The name is case-sensitive.
RTBASEAPI HANDLE RTAPI RtOpenSharedMemoryA(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCSTR lpName,
			VOID ** location);

// This function opens a named shared memory object.  The name is case-sensitive.
RTBASEAPI HANDLE RTAPI RtOpenSharedMemoryW(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCWSTR lpName,
			VOID ** location);

// This function unmaps the shared memory at the specified memory location that was previous mapped
// by RtOpenSharedMemory or RtCreateSharedMemory.
RTBASEAPI BOOL RTAPI RtUnmapSharedMemory(VOID * location);

#ifdef UNICODE
// This function creates an RTSS mutex.
#define RtCreateMutex RtCreateMutexW
#else
// This function creates an RTSS mutex.
#define RtCreateMutex RtCreateMutexA
#endif // !UNICODE

// This function creates an RTSS mutex.
RTBASEAPI HANDLE RTAPI RtCreateMutexA(
			LPSECURITY_ATTRIBUTES lpMutexAttributes,
			BOOL bInitialOwner,
			LPCSTR lpName);

// This function creates an RTSS mutex.
RTBASEAPI HANDLE RTAPI RtCreateMutexW(
			LPSECURITY_ATTRIBUTES lpMutexAttributes,
			BOOL bInitialOwner,
			LPCWSTR lpName);

#ifdef UNICODE
// This function opens a named or unnamed mutex.  The name is case-sensitive.
#define RtOpenMutex RtOpenMutexW
#else
// This function opens a named or unnamed mutex.  The name is case-sensitive.
#define RtOpenMutex RtOpenMutexA
#endif // !UNICODE

// This function opens a named or unnamed mutex.  The name is case-sensitive.
RTBASEAPI HANDLE RTAPI RtOpenMutexA(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCSTR lpName);

// This function opens a named mutex.  The name is case-sensitive.
RTBASEAPI HANDLE RTAPI RtOpenMutexW(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCWSTR lpName);

// This function relinquishes ownership of an RTSS mutex.
RTBASEAPI BOOL RTAPI RtReleaseMutex(HANDLE hMutex);

#ifdef UNICODE
// This function creates a named or unnamed semaphore object.
#define RtCreateSemaphore RtCreateSemaphoreW
#else
// This function creates a named or unnamed semaphore object.
#define RtCreateSemaphore RtCreateSemaphoreA
#endif // !UNICODE

// This function creates a named or unnamed semaphore object.
RTBASEAPI HANDLE RTAPI RtCreateSemaphoreA(
			LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
			LONG lInitialCount,
			LONG lMaximumCount,
			LPCSTR lpName);

// This function creates a named or unnamed semaphore object.
RTBASEAPI HANDLE RTAPI RtCreateSemaphoreW(
			LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
			LONG lInitialCount,
			LONG lMaximumCount,
			LPCWSTR lpName);

#ifdef UNICODE
// This function opens a named semaphore object.  The name is case-sensitive.
#define RtOpenSemaphore RtOpenSemaphoreW
#else
// This function opens a named semaphore object.  The name is case-sensitive.
#define RtOpenSemaphore RtOpenSemaphoreA
#endif // !UNICODE

// This function opens a named semaphore object.  The name is case-sensitive.
RTBASEAPI HANDLE RTAPI RtOpenSemaphoreA(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCSTR lpName);

// This function opens a named semaphore object.  The name is case-sensitive.
RTBASEAPI HANDLE RTAPI RtOpenSemaphoreW(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCWSTR lpName);

// This function increases the count of the specified semaphore object by a specified amount.
RTBASEAPI BOOL RTAPI RtReleaseSemaphore(HANDLE hSemaphore,
			LONG lReleaseCount,
			LPLONG lpPreviousCount);

#ifdef UNICODE
// This function creates a named or unnamed event object.
#define RtCreateEvent RtCreateEventW
#else
// This function creates a named or unnamed event object.
#define RtCreateEvent RtCreateEventA
#endif // !UNICODE

// This function creates a named or unnamed event object.
RTBASEAPI HANDLE RTAPI RtCreateEventA(
			LPSECURITY_ATTRIBUTES lpEventAttributes,
			BOOL bManualReset,
			BOOL bInitialState,
			LPCSTR lpName);

// This function creates a named or unnamed event object.
RTBASEAPI HANDLE RTAPI RtCreateEventW(
			LPSECURITY_ATTRIBUTES lpEventAttributes,
			BOOL bManualReset,
			BOOL bInitialState,
			LPCWSTR lpName);

#ifdef UNICODE
// This function opens an existing named event object.  The name is case-sensitive.
#define RtOpenEvent RtOpenEventW
#else
// This function opens an existing named event object.  The name is case-sensitive.
#define RtOpenEvent RtOpenEventA
#endif // !UNICODE

// This function opens an existing named event object.  The name is case-sensitive.
RTBASEAPI HANDLE RTAPI RtOpenEventA(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCSTR lpName);

// This function opens an existing named event object.  The name is case-sensitive.
RTBASEAPI HANDLE RTAPI RtOpenEventW(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCWSTR lpName);

// This function sets the specified event object to the signaled state, releases one or more waiting
// thread, then resets the event object to the non-signaled state.  For manual reset events, all
// waiting threads are released.  For auto-reset events, a single thread is released, even if
// multiple threads are waiting.
RTBASEAPI BOOL RTAPI RtPulseEvent(HANDLE hEvent);

// This function sets the state of the specified event object to non-signaled.
RTBASEAPI BOOL RTAPI RtResetEvent(HANDLE hEvent);

// This function sets the state of the specified event object to signaled.
RTBASEAPI BOOL RTAPI RtSetEvent(HANDLE hEvent);

typedef
VOID
(RTFCNDCL *PTIMERAPCROUTINE)(
    LPVOID lpArgToCompletionRoutine,
    DWORD dwTimerLowValue,
    DWORD dwTimerHighValue
    );

#ifdef UNICODE
#define RtCreateWaitableTimer RtCreateWaitableTimerW
#else
#define RtCreateWaitableTimer RtCreateWaitableTimerA
#endif // !UNICODE

RTBASEAPI HANDLE RTAPI RtCreateWaitableTimerA(
			LPSECURITY_ATTRIBUTES lpTimerAttributes,
		    BOOL bManualReset,
			LPCSTR lpTimerName);

RTBASEAPI HANDLE RTAPI RtCreateWaitableTimerW(
			LPSECURITY_ATTRIBUTES lpTimerAttributes,
			BOOL bManualReset,
			LPCWSTR lpTimerName);

#ifdef UNICODE
#define RtOpenWaitableTimer RtOpenWaitableTimerW
#else
#define RtOpenWaitableTimer RtOpenWaitableTimerA
#endif // !UNICODE

RTBASEAPI HANDLE RTAPI RtOpenWaitableTimerA(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCSTR lpTimerName);

RTBASEAPI HANDLE RTAPI RtOpenWaitableTimerW(
			DWORD dwDesiredAccess,
			BOOL bInheritHandle,
			LPCWSTR lpTimerName);

RTBASEAPI BOOL RTAPI RtSetWaitableTimer(
			HANDLE hWaitableTimer,
			const LARGE_INTEGER *lpDueTime,
			LONG lPeriod,
			PTIMERAPCROUTINE pfnCompletionRoutine,
			LPVOID lpArgToCompletionRoutine,
			BOOL fResume);

RTBASEAPI BOOL RTAPI RtCancelWaitableTimer(HANDLE hWaitableTimer);

RTBASEAPI BOOL RTAPI RtSetWaitableTimerFt(
			HANDLE hWaitableTimer,
			const LARGE_INTEGER *lpDueTime,
			const LARGE_INTEGER *lpPeriod,
			PTIMERAPCROUTINE pfnCompletionRoutine,
			LPVOID lpArgToCompletionRoutine,
			BOOL fResume);

#endif // RTAPIVER >= 20

//
// Clocks and Timers
//
#define CLOCK_1		1	// System Clock
#define CLOCK_2		2	// Real-time HAL Clock
#define CLOCK_3		3
#define CLOCK_4		4
#define CLOCK_FASTEST	0xFFFF	// Fastest available clock & timer

#define CLOCK_SYSTEM	CLOCK_1

typedef unsigned long CLOCK, *PCLOCK;

// This function obtains the current value of the specified clock.
RTBASEAPI BOOL RTAPI RtGetClockTime(
			CLOCK Clock,
			PLARGE_INTEGER pTime);

// This function sets the current value of the specified clock.
RTBASEAPI BOOL RTAPI RtSetClockTime(
			CLOCK Clock,
			PLARGE_INTEGER pTime);

// This function obtains the resolution of the specified clock.
RTBASEAPI BOOL RTAPI RtGetClockResolution(
			CLOCK Clock,
		   	PLARGE_INTEGER pTime);

// This function obtains the minimum timer period of the specified clock in 100 ns units.
RTBASEAPI BOOL RTAPI RtGetClockTimerPeriod(
			CLOCK Clock,
			PLARGE_INTEGER pTime);

// This function creates a timer associated with the specified clock and returns a handle to the timer.
RTBASEAPI HANDLE RTAPI RtCreateTimer(
			PSECURITY_ATTRIBUTES pThreadAttributes,      
			ULONG StackSize,            
			VOID (RTFCNDCL *pRoutine)(PVOID context),
			PVOID Context,
			ULONG Priority,
			CLOCK Clock);

// Define maximum number of RTSS processors
#if (!defined(MAXNUM_RTPROCESSORS))
#define MAXNUM_RTPROCESSORS 0x1F
#endif

// Define maximum number of RTSS+Windows processors
#if (!defined(MAXNUM_TOTALPROCESSORS))
#define MAXNUM_TOTALPROCESSORS 0x20
#endif

#if ( !defined(_NTDDK_) )
#if !defined(VISUALSTUDIO) || (VISUALSTUDIO < 8)
typedef ULONG ULONG_PTR;
#endif
typedef ULONG_PTR KAFFINITY;
typedef KAFFINITY *PKAFFINITY;
#endif

// This function creates a timer associated with the specified clock and returns a handle to the
// timer.  Use this function instead of RtCreateTimer if you want to set the affinity for the timer
// handler.
RTBASEAPI HANDLE RTAPI RtCreateTimerEx(
			PSECURITY_ATTRIBUTES pThreadAttributes,      
			ULONG StackSize,            
			VOID (RTFCNDCL *pRoutine)(PVOID context),
			PVOID Context,
			ULONG Priority,
			CLOCK Clock,
			KAFFINITY ProcessorEnableMask);

// This function deletes the specified timer.
RTBASEAPI BOOL RTAPI RtDeleteTimer(HANDLE Handle);

// This function cancels the expiration of the specified timer.
RTBASEAPI BOOL RTAPI RtCancelTimer(
			HANDLE hTimer,
			PLARGE_INTEGER pRemain);

// This function returns the remaining relative time until the next expiration of the specified timer.
RTBASEAPI BOOL RTAPI RtGetTimer(
			HANDLE Handle,
			PLARGE_INTEGER pRemain);

// This function sets the expiration time and repeat interval on the specified timer.  To set the
// relative expiration time, use RtSetTimerRelative instead.
RTBASEAPI BOOL RTAPI RtSetTimer(
			HANDLE hTimer,
			PLARGE_INTEGER pExpiration,
			PLARGE_INTEGER pInterval);

// This function sets the relative expiration time and repeat interval on the specified timer.
RTBASEAPI BOOL RTAPI RtSetTimerRelative(
			HANDLE hTimer,
			PLARGE_INTEGER pExpiration,
			PLARGE_INTEGER pInterval);

// This function gets the current value for RTSS time, which is the number of 100 nanosecond ticks
// since 00:00 Jan 1, 1601 GMT.  This time is only updated every 10 milliseconds.  This function can
// be used only by RTSS processes; it cannot be used by Win32 processes.
RTBASEAPI void RTAPI RtGetRtssTimeAsFileTime(LPFILETIME pTime);

// This function sets the new value for RTSS time in units of 100 nanosecond ticks since 00:00 Jan
// 1, 1601 GMT.  This function can be used only by RTSS processes; it cannot be used by Win32
// processes.
RTBASEAPI void RTAPI RtSetRtssTimeAsFileTime(LPFILETIME pTime);

// This function suspends the current thread for a duration specified in 100 nanosecond units.  The
// specified duration must be less than or equal to one second and must be greater than or equal to
// the minimum timer period of the system.
RTBASEAPI VOID RTAPI RtSleepFt(PLARGE_INTEGER pSleepTime);

RTBASEAPI ULONG RTAPI RtSleepFtEx(
			PLARGE_INTEGER pSleepTime,
			BOOL bAlertable);

// This function enables direct user access to the specified range of I/O addresses.
RTBASEAPI BOOL RTAPI RtEnablePortIo(
			PUCHAR Port,
			ULONG Count);

// This function disables direct user access to the specified range of I/O addresses.
RTBASEAPI BOOL RTAPI RtDisablePortIo(
			PUCHAR Port,
			ULONG Count);

// This function reads one byte of data directly from an I/O port.
RTBASEAPI UCHAR RTAPI RtReadPortUchar(PUCHAR Port);

// This function reads two bytes of data directly from an I/O port.
RTBASEAPI USHORT RTAPI RtReadPortUshort(PUSHORT Port);

// This function reads four bytes of data directly from an I/O port.
RTBASEAPI ULONG RTAPI RtReadPortUlong(PULONG Port);

// This function copies data from an I/O port to a buffer (one byte at a time) until the buffer has
// been filled.
RTBASEAPI VOID RTAPI RtReadPortBufferUchar(
			PUCHAR Port,
			PUCHAR Buffer,
			ULONG Count);

// This function copies data from an I/O port to a buffer (two bytes at a time) until the buffer has
// been filled.
RTBASEAPI VOID RTAPI RtReadPortBufferUshort(
			PUSHORT Port,
			PUSHORT Buffer,
			ULONG Count);

// This function copies data from an I/O port to a buffer (four bytes at a time) until the buffer has
// been filled.
RTBASEAPI VOID RTAPI RtReadPortBufferUlong(
			PULONG Port,
			PULONG Buffer,
			ULONG Count);

// This function writes one byte of data to an I/O port.
RTBASEAPI VOID RTAPI RtWritePortUchar(
			PUCHAR Port,
			UCHAR Value);

// This function writes two bytes of data to an I/O port.
RTBASEAPI VOID RTAPI RtWritePortUshort(
			PUSHORT Port,
			USHORT Value);

// This function writes four bytes of data to an I/O port.
RTBASEAPI VOID RTAPI RtWritePortUlong(
			PULONG Port,
			ULONG Value);

// This function copies data to an I/O port from a buffer (one byte at a time) until the buffer has
// been filled.
RTBASEAPI VOID RTAPI RtWritePortBufferUchar(
			PUCHAR Port,
			PUCHAR Buffer,
			ULONG Count);

// This function copies data to an I/O port from a buffer (two bytes at a time) until the buffer has
// been filled.
RTBASEAPI VOID RTAPI RtWritePortBufferUshort(
			PUSHORT Port,
			PUSHORT Buffer,
			ULONG Count);

// This function copies data to an I/O port from a buffer (four bytes at a time) until the buffer
// has been filled.
RTBASEAPI VOID RTAPI RtWritePortBufferUlong(
			PULONG Port,
			PULONG Buffer,
			ULONG Count);

//#if ( !defined(_NTDDK_) && !defined(_NTHAL_H_) && !defined(__RTX_H__) )
#if (!defined(_NTDDK_)&& !defined(__RTX_H__))
// Defined to support different cache types
typedef enum _MEMORY_CACHING_TYPE {  
	MmNonCached,  
	MmCached,  
	MmWriteCombined
} MEMORY_CACHING_TYPE;

#endif

// This function maps a range of physical memory addresses into the user's virtual address space.
// NOTE: No access checks are performed on these addresses or the ranges requested. Use this
// function with care to avoid memory corruption.
RTBASEAPI PVOID RTAPI RtMapMemory(
			LARGE_INTEGER PhysAddr,
			ULONG Length,
			MEMORY_CACHING_TYPE CacheType);

// This function releases a mapping made by a previous call to RtMapMemory.
RTBASEAPI BOOL RTAPI RtUnmapMemory(PVOID VirtAddr);

// This function commits and locks the specified amount of memory to avoid page faults as the memory
// is used.  The committed locked memory will not incur page faults when the memory is used, nor
// will the system page the allocated memory out to secondary storage.
RTBASEAPI PVOID RTAPI RtAllocateLockedMemory(ULONG Size);

// This function frees memory previously committed and locked by a call to RtAllocateLockedMemory.
RTBASEAPI BOOL RTAPI RtFreeLockedMemory(PVOID pVirtualAddress);

// This function allocates physically contiguous non-paged memory.
RTBASEAPI PVOID RTAPI RtAllocateContiguousMemory(
			ULONG nNumberOfBytes,
			LARGE_INTEGER HighestAddress);

// This function allocates a range of physically contiguous, cache-aligned memory.  WARNING: The
// allocatesd memory is uninitialized.
RTBASEAPI PVOID RTAPI RtAllocateContiguousMemorySpecifyCache(
            ULONG  nNumberOfBytes,
            LARGE_INTEGER  LowestAddress,
            LARGE_INTEGER  HighestAddress,
            LARGE_INTEGER  BoundaryAddressMultiple,
            MEMORY_CACHING_TYPE  CacheType);

// This function frees previously allocated physically contiguous memory.
RTBASEAPI BOOL RTAPI RtFreeContiguousMemory(PVOID pVirtualAddress);

// This function returns the physical address for the virtual address of a contiguous physical
// memory buffer previously allocated by RtAllocateContiguousMemory.  In the RTSS environment, this
// function applies to all memory allocation.  In the Win32 environment, it applies to contiguous
// memory allocation only.
RTBASEAPI LARGE_INTEGER RTAPI RtGetPhysicalAddress(PVOID pVirtualAddress);

// This function allocates memory from local memory, a pre-allocated RTSS deterministic memory pool.
RTBASEAPI PVOID RTAPI RtAllocateLocalMemory(ULONG Size);

// This function frees memory previously allocated by a call to RtAllocateLocalMemory
RTBASEAPI BOOL RTAPI RtFreeLocalMemory(PVOID pVirtualAddress);

// This function returns RTSS deterministic memory pool information, including the pool size as a
// whole, the free memory size available, the maximum virtually contiguous free memory size.
// RtQueryLocalMemory is only supported in the RTSS environment.  It cannot be called from a Windows
// application.
RTBASEAPI BOOL RTAPI RtQueryLocalMemory(PULONG MemSize, PULONG MemAvail, PULONG MemContig);

// This function expands the local memory pool by the size specified.  This function cannot be
// called when a shutdown occurs.
RTBASEAPI BOOL RTAPI RtExpandLocalMemory(ULONG Size);

// MSVC++ 10.0 _MSC_VER = 1600
// MSVC++ 9.0  _MSC_VER = 1500
// MSVC++ 8.0  _MSC_VER = 1400
// MSVC++ 7.1  _MSC_VER = 1310
// MSVC++ 7.0  _MSC_VER = 1300
// MSVC++ 6.0  _MSC_VER = 1200
// MSVC++ 5.0  _MSC_VER = 1100

#if _MSC_VER == 1200
# pragma data_seg("rbldx")
#else
# pragma data_seg(push, "rbldx")
#endif

static unsigned char _rbldxdata_[1024] = { 0, 0, 0xba, 0xdb, 0xab, 0xe0, 0 };

#if _MSC_VER == 1200
# pragma data_seg(".data")
// This variable is here to force the compiler to resume normal section processing
// after the above switches between data segments.
static char __rtxdummy[] = "xyzzy";
#else
# pragma data_seg(pop)
#endif


//
// RtLockProcess Flags
//
#define RT_PLOCK_BASIC	0x0001
#define RT_PLOCK_ALL	0xFFFF

// This function locks certain sections of a process' virtual address space into physical memory.
RTBASEAPI BOOL RTAPI RtLockProcess(ULONG Sections);

// This function unlocks sections of the process' virtual address space previously locked into
// physical memory.  RtUnlockProcess has no effect within the RTSS environment.
RTBASEAPI BOOL RTAPI RtUnlockProcess(ULONG Sections);

//
// RtLockKernel Flags
//
#define RT_KLOCK_DATA		0x0001
#define RT_KLOCK_CODE		0x0002
#define RT_KLOCK_EXECUTIVE	(RT_KLOCK_DATA | RT_KLOCK_DATA)
#define RT_KLOCK_WIN32K		0x0004
#define RT_KLOCK_DRIVERS	0x0008
#define RT_KLOCK_ALL		0xFFFF

// This function locks certain sections of the Windows kernel's virtual address space into physical
// memory.  IMPORTANT: Using this function can have a significant negative impact on system
// performance.
RTBASEAPI BOOL RTAPI RtLockKernel(ULONG Sections);

// This function unlocks sections of the kernel's virtual address space previously locked into
// physical memory by RtLockKernel.
RTBASEAPI BOOL RTAPI RtUnlockKernel(ULONG Sections);

// This function commits and locks the specified stack to avoid page faults as the stack is used.
// The RTSS stack is always locked.  Any attempt to lock the stack beyond the stack size at thread
// creation fails; otherwise, this call has no effect in the RTSS environment.
RTBASEAPI BOOL RTAPI RtCommitLockStack(ULONG nNumberOfBytes);

// This function commits and locks the process heap to avoid page faults as the heap is used.  Since
// all RTSS heaps are always locked, this function has no effect in the RTSS environment.
RTBASEAPI BOOL RTAPI RtCommitLockProcessHeap(
			ULONG nNumberOfBytes,
			VOID (RTFCNDCL *pExceptionRoutine)(HANDLE));

// This function commits and locks the heap to avoid page faults as the heap is used.  Since all
// RTSS heaps are always locked, this function has no effect in the RTSS environment.
RTBASEAPI BOOL RTAPI RtCommitLockHeap(
			HANDLE hHeap,
			ULONG nNumberOfBytes,
			VOID (RTFCNDCL *pExceptionRoutine)(HANDLE));


#if ( !defined(_NTDDK_) && !defined(_NTHAL_H_) && !defined(__RTX_H__) )
//
// Define the I/O bus interface types.
//
typedef enum _INTERFACE_TYPE {
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    MaximumInterfaceType
}INTERFACE_TYPE, *PINTERFACE_TYPE;

//
// Interrupt modes.
//
typedef enum _KINTERRUPT_MODE {
    LevelSensitive,
    Latched
    } KINTERRUPT_MODE;

#endif 

#if ( !defined(_NTDDK_) )
//
// Define types of bus information.
//
typedef enum _BUS_DATA_TYPE {
    ConfigurationSpaceUndefined = -1,
    Cmos,
    EisaConfiguration,
    Pos,
    CbusConfiguration,
    PCIConfiguration,
    VMEConfiguration,
    NuBusConfiguration,
    PCMCIAConfiguration,
    MPIConfiguration,
    MPSAConfiguration,
    PNPISAConfiguration,
    MaximumBusDataType
} BUS_DATA_TYPE, *PBUS_DATA_TYPE;


//
// Define the format of the PCI Slot parameter.
//
typedef struct _PCI_SLOT_NUMBER {
    union {
        struct {
            ULONG   DeviceNumber:5;
            ULONG   FunctionNumber:3;
            ULONG   Reserved:24;
        } bits;
        ULONG   AsULONG;
    } u;
} PCI_SLOT_NUMBER, *PPCI_SLOT_NUMBER;

#define PCI_TYPE0_ADDRESSES             6
#define PCI_TYPE1_ADDRESSES             2

//
// Define the standard PCI configuration information.
//
typedef struct _PCI_COMMON_CONFIG {
    USHORT  VendorID;                   // (ro)
    USHORT  DeviceID;                   // (ro)
    USHORT  Command;                    // Device control
    USHORT  Status;
    UCHAR   RevisionID;                 // (ro)
    UCHAR   ProgIf;                     // (ro)
    UCHAR   SubClass;                   // (ro)
    UCHAR   BaseClass;                  // (ro)
    UCHAR   CacheLineSize;              // (ro+)
    UCHAR   LatencyTimer;               // (ro+)
    UCHAR   HeaderType;                 // (ro)
    UCHAR   BIST;                       // Built in self test

    union {
        struct _PCI_HEADER_TYPE_0 {
            ULONG   BaseAddresses[PCI_TYPE0_ADDRESSES];
            ULONG   CIS;
            USHORT  SubVendorID;
            USHORT  SubSystemID;
            ULONG   ROMBaseAddress;
            ULONG   Reserved2[2];

            UCHAR   InterruptLine;      //
            UCHAR   InterruptPin;       // (ro)
            UCHAR   MinimumGrant;       // (ro)
            UCHAR   MaximumLatency;     // (ro)
        } type0;


    } u;

    UCHAR   DeviceSpecific[192];

} PCI_COMMON_CONFIG, *PPCI_COMMON_CONFIG;


#define PCI_COMMON_HDR_LENGTH (FIELD_OFFSET (PCI_COMMON_CONFIG, DeviceSpecific))

#define PCI_MAX_DEVICES                     32
#define PCI_MAX_FUNCTION                    8

#define PCI_INVALID_VENDORID                0xFFFF

//
// Bit encodings for  PCI_COMMON_CONFIG.HeaderType
//
#define PCI_MULTIFUNCTION                   0x80
#define PCI_DEVICE_TYPE                     0x00
#define PCI_BRIDGE_TYPE                     0x01

//
// Bit encodings for PCI_COMMON_CONFIG.Command
//
#define PCI_ENABLE_IO_SPACE                 0x0001
#define PCI_ENABLE_MEMORY_SPACE             0x0002
#define PCI_ENABLE_BUS_MASTER               0x0004
#define PCI_ENABLE_SPECIAL_CYCLES           0x0008
#define PCI_ENABLE_WRITE_AND_INVALIDATE     0x0010
#define PCI_ENABLE_VGA_COMPATIBLE_PALETTE   0x0020
#define PCI_ENABLE_PARITY                   0x0040  // (ro+)
#define PCI_ENABLE_WAIT_CYCLE               0x0080  // (ro+)
#define PCI_ENABLE_SERR                     0x0100  // (ro+)
#define PCI_ENABLE_FAST_BACK_TO_BACK        0x0200  // (ro)

//
// Bit encodings for PCI_COMMON_CONFIG.Status
//
#define PCI_STATUS_FAST_BACK_TO_BACK        0x0080  // (ro)
#define PCI_STATUS_DATA_PARITY_DETECTED     0x0100
#define PCI_STATUS_DEVSEL                   0x0600  // 2 bits wide
#define PCI_STATUS_SIGNALED_TARGET_ABORT    0x0800
#define PCI_STATUS_RECEIVED_TARGET_ABORT    0x1000
#define PCI_STATUS_RECEIVED_MASTER_ABORT    0x2000
#define PCI_STATUS_SIGNALED_SYSTEM_ERROR    0x4000
#define PCI_STATUS_DETECTED_PARITY_ERROR    0x8000

//
// Bit encodes for PCI_COMMON_CONFIG.u.type0.BaseAddresses
//
#define PCI_ADDRESS_IO_SPACE                0x00000001  // (ro)
#define PCI_ADDRESS_MEMORY_TYPE_MASK        0x00000006  // (ro)
#define PCI_ADDRESS_MEMORY_PREFETCHABLE     0x00000008  // (ro)

#define PCI_TYPE_32BIT      0
#define PCI_TYPE_20BIT      2
#define PCI_TYPE_64BIT      4

//
// Bit encodes for PCI_COMMON_CONFIG.u.type0.ROMBaseAddresses
//
#define PCI_ROMADDRESS_ENABLED              0x00000001

//
// Reference notes for PCI configuration fields:
//
// ro   these field are read only.  changes to these fields are ignored
//
// ro+  these field are intended to be read only and should be initialized
//      by the system to their proper values.  However, driver may change
//      these settings.
//

#endif // _NTDDK_

/* wsa
/*
/* Define the CE-style PnP ISA data structures/defines
 */
 typedef struct _ISA_PNP_LOGICAL_DEVICE_INFO
          {
		   DWORD LogicalDeviceID;
		   DWORD CompatibleIDs[8];
		  } ISA_PNP_LOGICAL_DEVICE_INFO, *PISA_PNP_LOGICAL_DEVICE_INFO;

 typedef struct _ISA_PNP_CONFIG
          {
		   DWORD VendorID;
		   DWORD SerialNumber;
		   DWORD NumberLogicalDevices;
		   ISA_PNP_LOGICAL_DEVICE_INFO LogicalDeviceInfo[8];
		  } ISA_PNP_CONFIG, *PISA_PNP_CONFIG;

 typedef struct _ISA_PNP_RESOURCES
          {
		   USHORT Flags;
		   struct
		    {
			 USHORT MemoryBase;
			 USHORT MemoryUpperLimit;
			 UCHAR MemoryControl;
			} Memory24Descriptors[4];
		   struct
		    {
			 DWORD MemoryBase;
			 DWORD MemoryUpperLimit;
			 UCHAR MemoryControl;
			} Memory32Descriptors[4];
           USHORT IoPortDescriptors[8];
		   struct
		    {
			 UCHAR IRQLevel;
			 UCHAR IRQType;
			} IRQDescriptors[2];
		   UCHAR DMADescriptors[2];
		  } ISA_PNP_RESOURCES, *PISA_PNP_RESOURCES;

#define ISA_PNP_RESOURCE_FLAG_ACTIVE 0x00000001

#if ( !defined(__RTX_H__) )
//
// Interrupt states.
// Return types for interrupt level routine.
//
typedef enum _INTERRUPT_DISPOSITION {
    PassToNextDevice,
    Dismiss,
    CallInterruptThread
    } INTERRUPT_DISPOSITION, *PINTERRUPT_DISPOSITION;
#endif 

//
// Line based and message based interrupt attachment
// parameter structure definitions.
//
#pragma pack(push, 4)
typedef struct _ATTACH_INTERRUPT_LINE_BASED_PARAMETERS {
    PSECURITY_ATTRIBUTES pThreadAttributes;
    ULONG                StackSize;
    BOOLEAN              (RTFCNDCL * pRoutine)(PVOID context);
    PVOID                Context;
    ULONG                Priority;
    INTERFACE_TYPE       InterfaceType;
    ULONG                BusNumber;
    PCI_SLOT_NUMBER      SlotNumber;
    ULONG                BusInterruptLevel;
    ULONG                BusInterruptVector;
    BOOLEAN              Shared;
    KINTERRUPT_MODE      InterruptMode;
    INTERRUPT_DISPOSITION    (RTFCNDCL * MyInterrupt)(PVOID context);
    KAFFINITY            ProcessorEnableMask;
} ATTACH_INTERRUPT_LINE_BASED_PARAMETERS,
  *PATTACH_INTERRUPT_LINE_BASED_PARAMETERS;

typedef struct _ATTACH_INTERRUPT_MESSAGE_BASED_PARAMETERS {
    PSECURITY_ATTRIBUTES pThreadAttributes;
    ULONG                StackSize;
    BOOLEAN              (RTFCNDCL * pRoutine)(PVOID context);
    PVOID                Context;
    ULONG                Priority;
    ULONG                BusNumber;
    PCI_SLOT_NUMBER      SlotNumber;
    INTERRUPT_DISPOSITION    (RTFCNDCL * MyInterrupt)(PVOID context);
    KAFFINITY            ProcessorEnableMask;
} ATTACH_INTERRUPT_MESSAGE_BASED_PARAMETERS,
  *PATTACH_INTERRUPT_MESSAGE_BASED_PARAMETERS;
#pragma pack(pop)

//
// Top level interrupt attachment parameter definitions.
//
#define ATTACH_LINE_BASED                     0x1
#define ATTACH_MESSAGE_BASED                  0x2
#define ATTACH_MESSAGE_BASED_PREFER_MSI       0x3

typedef struct _ATTACH_INTERRUPT_PARAMETERS {
    ULONG AttachVersion;
    union {
        ATTACH_INTERRUPT_LINE_BASED_PARAMETERS LineBased;
        ATTACH_INTERRUPT_MESSAGE_BASED_PARAMETERS MessageBased;
    };
} ATTACH_INTERRUPT_PARAMETERS, *PATTACH_INTERRUPT_PARAMETERS;

#if ( !defined(_RTX_PROPERTIES_) )
typedef enum _MSI_CAPABILITY {
    NotMsiMsixCapable,
    MsiCapable,
    MsixCapable,
    MsiMsixCapable
} MSI_CAPABILITY, *PMSI_CAPABILITY;
#endif

// This function queries a PCI device and determines if the device has MSI or MSI-X capability.
// This is only supported in the RTSS environment.  It cannot be called from a Windows application.
RTBASEAPI MSI_CAPABILITY RTAPI RtQueryPciMsiCapability(
                        ULONG BusNumber,
                        PCI_SLOT_NUMBER SlotNumber);

// This function associates an Interrupt Service Thread (IST) and Interrupt Service Routine (ISR)
// with a line-based or message-based hardware interrupt.  This is only supported in the RTSS
// environment.  It cannot be called from a Windows application.  NOTE: This API call supersedes
// RtAttachInterruptVector and RtAttachInterruptVectorEx in RTSS.  Use RtAttachInterrupt instead of
// RtAttachInterruptVector or RtAttachInterruptVectorEx, which may be deprecated in the future.
RTBASEAPI HANDLE RTAPI RtAttachInterrupt(
                        PATTACH_INTERRUPT_PARAMETERS Parameters);

// This function releases an interrupt previously attached using RtAttachInterrupt.  NOTE: This API
// call supersedes RtReleaseInterruptVector in RTSS.  Use RtReleaseInterrupt instead of
// RtReleaseInterruptVector, which may be deprecated in the future.
RTBASEAPI BOOL RTAPI RtReleaseInterrupt(HANDLE hInterrupt);

// This function associates a handler routine in user space with a hardware interrupt.  This
// function does not permit shared interrupts.  NOTE: This API call has been superceded by
// RtAttachInterrupt, which permits shared interrupts.
RTBASEAPI HANDLE RTAPI RtAttachInterruptVector(
                        PSECURITY_ATTRIBUTES pThreadAttributes,
                        ULONG StackSize,                        
                        VOID (RTFCNDCL *pRoutine)(PVOID context),
                        PVOID Context,
                        ULONG Priority,
                        INTERFACE_TYPE InterfaceType,
                        ULONG BusNumber,
                        ULONG BusInterruptLevel,
                        ULONG BusInterruptVector);

// This function associates a handler routine in user space with a hardware interrupt.  NOTE: This
// API call has been superceded by RtAttachInterrupt.
RTBASEAPI HANDLE RTAPI RtAttachInterruptVectorEx(
                        PSECURITY_ATTRIBUTES pThreadAttributes,
                        ULONG StackSize,                        
                        BOOLEAN (RTFCNDCL *pRoutine)(PVOID context),
                        PVOID Context,
                        ULONG Priority,
                        INTERFACE_TYPE InterfaceType,
                        ULONG BusNumber,
                        ULONG BusInterruptLevel,
                        ULONG BusInterruptVector,
                        BOOLEAN Shared,
                        KINTERRUPT_MODE InterruptMode,
                        INTERRUPT_DISPOSITION (RTFCNDCL *MyInterrupt)(PVOID context));

// This function releases an interrupt previously attached by RtAttachInterruptVector or
// RtAttachInterruptVectorEx.  NOTE: This API call has been superceded by RtReleaseInterrupt.
RTBASEAPI BOOL RTAPI RtReleaseInterruptVector(HANDLE hInterrupt);

// In the Win32 environment, RtDisableInterrupts disables all user-level interrupt handling for all
// interrupts to which the Win32 process is attached.  In the RTSS environment, RtDisableInterrupts
// disables all interrupts at the processor level including timer interrupts.
RTBASEAPI BOOL RTAPI RtDisableInterrupts(VOID);

// In the Win32 environment, RtEnableInterrupts enables all user-level interrupt handling for all
// interrupts to which the Win32 process is attached.  In the RTSS environment, RtEnableInterrupts
// enables all interrupts at the processor level including timer interrupts.
RTBASEAPI VOID RTAPI RtEnableInterrupts(VOID);

// This function obtains details, starting at the given offset, about a given slot on an I/O bus.
RTBASEAPI ULONG RTAPI RtGetBusDataByOffset(
                        BUS_DATA_TYPE BusDataType,
                        ULONG BusNumber,
                        ULONG SlotNumber,
                        PVOID pBuffer,
                        ULONG Offset,
                        ULONG Length);

// This function sets bus-configuration data for a device on a dynamically configurable I/O bus with
// a published, standard interface.
RTBASEAPI ULONG RTAPI RtSetBusDataByOffset(
                        BUS_DATA_TYPE BusDataType,
                        ULONG BusNumber,
                        ULONG SlotNumber,
                        PVOID pBuffer,
                        ULONG Offset,
                        ULONG Length);

// This function translates a bus-specific address into the corresponding system logical address.
RTBASEAPI BOOL RTAPI RtTranslateBusAddress(
                        INTERFACE_TYPE InterfaceType,
                        ULONG BusNumber,
                        LARGE_INTEGER BusAddress,
                        PULONG pAddressSpace,
                        PLARGE_INTEGER pTranslatedAddress);

// This function returns a handle to an existing process object.
RTBASEAPI HANDLE RTAPI RtOpenProcess(
						DWORD dwAccess,			// ignored
						BOOL bInherit,			// ignored
						DWORD dwProcessId);

// This function retrieves the termination status of the specified process.
RTBASEAPI BOOL RTAPI RtGetExitCodeProcess(
						HANDLE hProcess,
						LPDWORD lpExitCode);

// This function terminates the specified process and all of its threads.  NOTE: Use this function
// only in extreme circumstances.  The state of global data maintained by dynamic-link libraries
// may be compromised if RtTerminateProcess is used rather than ExitProcess.
RTBASEAPI BOOL RTAPI RtTerminateProcess(
						HANDLE hProcess,
						UINT uExitCode);

// This function retrieves the process affinity mask for the specified RTSS process and the system
// affinity mask for RTSS.
RTBASEAPI BOOL RTAPI RtGetProcessAffinityMask(
						HANDLE hProcess,
						PDWORD lpProcessAffinityMask,
						PDWORD lpSystemAffinityMask);

// This function sets the processor affinity mask for the threads of the specified RTSS process.
RTBASEAPI BOOL RTAPI RtSetProcessAffinityMask(
						HANDLE hProcess,
						DWORD dwProcessAffinityMask);


#if !defined(_WINDEF_)
typedef struct _PROCESS_INFORMATION {
    HANDLE  hProcess;
    HANDLE  hThread;
    DWORD   dwProcessId;
    DWORD   dwThreadId;
} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;

typedef struct _STARTUPINFOA {
    DWORD   cb;
    LPSTR   lpReserved;
    LPSTR   lpDesktop;
    LPSTR   lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOA, *LPSTARTUPINFOA;

typedef struct _STARTUPINFOW {
    DWORD   cb;
    LPWSTR  lpReserved;
    LPWSTR  lpDesktop;
    LPWSTR  lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;

#ifdef UNICODE
typedef STARTUPINFOW STARTUPINFO;
typedef LPSTARTUPINFOW LPSTARTUPINFO;
#else
typedef STARTUPINFOA STARTUPINFO;
typedef LPSTARTUPINFOA LPSTARTUPINFO;
#endif // UNICODE

#endif //!defined(_WINDEF_)

//
// dwCreationFlag values
//
#define CREATE_USE_LOCAL_MEMORY               0x00004000
#define CREATE_2003NET_DEBUG                  0x00008000

#ifdef UNICODE
// This function creates and starts a new RTSS process.  The new RTSS process runs the specified
// RTSS executable file.
#define RtCreateProcess RtCreateProcessW
#else
// This function creates and starts a new RTSS process.  The new RTSS process runs the specified
// RTSS executable file.
#define RtCreateProcess RtCreateProcessA
#endif // !UNICODE

// This function creates and starts a new RTSS process.  The new RTSS process runs the specified
// RTSS executable file.
RTBASEAPI BOOL RTAPI RtCreateProcessA(
						LPCSTR lpApplicationName,
						LPSTR lpCommandLine,
						LPSECURITY_ATTRIBUTES lpProcessAttributes,	// ignored 
						LPSECURITY_ATTRIBUTES lpThreadAttributes,	// ignored 
						BOOL bInheritHandles,						// ignored 
						DWORD dwCreationFlags,						// support CREATE_SUSPENDED
						LPVOID lpEnvironment,						// ignored 
						LPCSTR lpCurrentDirectory,					// ignored
						LPSTARTUPINFO lpStartupInfo,				// ignored
						LPPROCESS_INFORMATION lpProcessInformation);

// This function creates and starts a new RTSS process.  The new RTSS process runs the specified
// RTSS executable file.
RTBASEAPI BOOL RTAPI RtCreateProcessW(
						LPCWSTR lpApplicationName,
						LPWSTR lpCommandLine,
						LPSECURITY_ATTRIBUTES lpProcessAttributes,	// ignored 
						LPSECURITY_ATTRIBUTES lpThreadAttributes,	// ignored 
						BOOL bInheritHandles,						// ignored 
						DWORD dwCreationFlags,						// support CREATE_SUSPENDED
						LPVOID lpEnvironment,						// ignored 
						LPCWSTR lpCurrentDirectory,					// ignored
						LPSTARTUPINFO lpStartupInfo,				// ignored
						LPPROCESS_INFORMATION lpProcessInformation);

// This function generates a trace event.
RTBASEAPI BOOL RTAPI RtTraceEvent(
			ULONG TraceEventID, 
			PVOID arg1,
			PVOID arg2);

//
// support RTX application level perf measurement
//
#define PERF_MEASURE_HALISR_ENTEREXIT     0x00000001
#define PERF_MEASURE_HAL_CLOCKTICK        0x00000002
#define PERF_MEASURE_HAL_INTERNAL         0x00000004
#define PERF_MEASURE_RTSS_TIMERTICK       0x00000008
#define PLATFORM_CONFIG_UP_PIC            0x00000001
#define PLATFORM_CONFIG_UP_APIC           0x00000002
#define PLATFORM_CONFIG_MP_DEDICATED      0x00000004
#define PLATFORM_CONFIG_MP_SHARED         0x00000008

// This function enables measuring HAL clock ticks and latencies for a specified processor.  NOTE:
// Only one application should enable performance measuring at a time.  You must call
// RtStopPerfMeasure to clean up the buffer allocated by RtStartPerfMeasure.
RTBASEAPI ULONG RTAPI RtStartPerfMeasure(
	ULONG perfMeasureType,  //control code of operation to perform (input)
	ULONG *ioBuffer,        //buffer to pass data (output)
	ULONG ioBufferSize,     //buffer size (input)
	ULONG *ioBufferWrite,   //pointer to write index (output)
	LONGLONG *lPerfCyclesPerSecond, //pointer to pass PerfCyclesPerSecond (output)
	ULONG *platformConfig,   //pointer to pass platform configuration (output)
	ULONG *rtssProcessorNumber // pointer to pass RTSS processor number (output)
  );

// This function stops measuring HAL clock ticks and latencies previously started with RtStartPerfMeasure.
RTBASEAPI ULONG RTAPI RtStopPerfMeasure(
	ULONG perfMeasureType,
	ULONG *ioBuffer
  );

// This function returns the system-assigned number of the processor on which the current thread is
// running.
RTBASEAPI ULONG RTAPI RtGetCurrentProcessorNumber(VOID);

RTBASEAPI LONG RTAPI __RtGetAllObjects(PUCHAR rawData, LONG size);

RTBASEAPI LONG RTAPI __RtGetAllHandles(PUCHAR rawData, LONG size);

RTBASEAPI BOOL RTAPI __RtRtssSetProcessIdealProcessor (HANDLE hProcess, DWORD dwIdealProcessor);

// RtxFeatureLicenseStatus
// This enumeration provides status values returned by RtGetLicenseFeatureStatus in parameter
// "status".
typedef enum _RtxFeatureLicenseStatus
{
	STATUS_LICENSE_ERROR	= -1,	//	error retrieving license info
	STATUS_LICENSE_VALID	= 0,			
	STATUS_LICENSE_VALID_EVALUATION = 1,
    STATUS_LICENSE_INVALID  = 2,
	STATUS_LICENSE_EXPIRED  = 3,
} RtxFeatureLicenseStatus;

#ifdef UNICODE
// This function obtains the status of the license for the specified product feature.
#define RtGetLicenseFeatureStatus RtGetLicenseFeatureStatusW
#else
// This function obtains the status of the license for the specified product feature.
#define RtGetLicenseFeatureStatus RtGetLicenseFeatureStatusA
#endif // !UNICODE

// This function obtains the status of the license for the specified product feature.
BOOL RTAPI RtGetLicenseFeatureStatusA(char * featureName, RtxFeatureLicenseStatus * status);

// This function obtains the status of the license for the specified product feature.
BOOL RTAPI RtGetLicenseFeatureStatusW(LPWSTR featureName, RtxFeatureLicenseStatus * status);

#ifdef UNICODE
// This function obtains the status of the license for the specified product feature.
#define RtGetLicenseFeatureStatusEx RtGetLicenseFeatureStatusExW
#else
// This function obtains the status of the license for the specified product feature.
#define RtGetLicenseFeatureStatusEx RtGetLicenseFeatureStatusExA
#endif

// This function obtains the status of the license for the specified product feature.
BOOL RTAPI RtGetLicenseFeatureStatusExA(char * featureName, RtxFeatureLicenseStatus * status, char * options);

// This function obtains the status of the license for the specified product feature.
BOOL RTAPI RtGetLicenseFeatureStatusExW(LPWSTR featureName, RtxFeatureLicenseStatus * status, LPWSTR options);


// Added RTX 10.0

typedef struct _RTSPIN_LOCK
{
	volatile ULONG	Lock;
	volatile ULONG	EFlags;
	PVOID			pOwner;
}RTSPIN_LOCK, *PRTSPIN_LOCK;

// This function initializes a spinlock.
RTBASEAPI BOOLEAN RTAPI RtInitializeSpinlock(PRTSPIN_LOCK pSpinlock);

// This function acquires a spinlock.
RTBASEAPI VOID RTAPI RtAcquireSpinlock(PRTSPIN_LOCK pSpinlock);

// This function releases a spinlock.
RTBASEAPI VOID RTAPI RtReleaseSpinlock(PRTSPIN_LOCK pSpinlock);

typedef struct _RTFAST_SEMAPHORE
{
	RTSPIN_LOCK		ListLock;
	LIST_ENTRY		List;
	volatile ULONG	Count;
	volatile ULONG	CountToWake;
	ULONG			CountAutoWake;
}RTFAST_SEMAPHORE, *PRTFAST_SEMAPHORE;

// This function initializes a fast semaphore object.
RTBASEAPI BOOLEAN RTAPI RtInitializeFastSemaphore(PRTFAST_SEMAPHORE pSemaphore, ULONG releaseCount);

// This function returns the number of threads waiting on the specified fast semaphore.
RTBASEAPI BOOLEAN RTAPI RtGetCountFastSemaphore(const PRTFAST_SEMAPHORE pSemaphore, PULONG pCount);

// This function places the current thread into a busy wait state until the fast semaphore is released.
RTBASEAPI BOOLEAN RTAPI RtAcquireFastSemaphore(PRTFAST_SEMAPHORE pSemaphore);

// This function releases all threads waiting on the fast semaphore, ensures that all waiting
// threads enter the ready-to-run state, and ensures that no new threads enter the busy wait state
// until all currently waiting threads have become runnable.
RTBASEAPI BOOLEAN RTAPI RtReleaseAllFastSemaphore(PRTFAST_SEMAPHORE pSemaphore);

// This function releases the specified count of threads waiting on the fast semaphore.  If the
// number of waiting threads is less than the specified count, RtReleaseFastSemaphore will store the
// remaining count value and continue to release new threads that call RtAcquireFastSemaphore on
// this fast semaphore.
RTBASEAPI BOOLEAN RTAPI RtReleaseFastSemaphore(PRTFAST_SEMAPHORE pSemaphore, ULONG count);

#ifdef UNICODE
// This function retrieves the RTX product type and version.
# define RtGetVersionEx RtGetVersionExW
#else
// This function retrieves the RTX product type and version.
# define RtGetVersionEx RtGetVersionExA
#endif

// This function retrieves the RTX product type and version.
RTBASEAPI BOOL RTAPI RtGetVersionExA(LPOSVERSIONINFOEXA lpVersionInfo);

// This function retrieves the RTX product type and version.
RTBASEAPI BOOL RTAPI RtGetVersionExW(LPOSVERSIONINFOEXW lpVersionInfo);


///////////////////////////////////////////////////////////////////////////////
//
// STARTUPINFOEX handling.
//
///////////////////////////////////////////////////////////////////////////////

#if !defined(_WINDEF_) || ( defined(_WINDEF_) && !(_WIN32_WINNT >= 0x0400) )

typedef struct _PROC_THREAD_ATTRIBUTE_LIST *PPROC_THREAD_ATTRIBUTE_LIST, *LPPROC_THREAD_ATTRIBUTE_LIST;

typedef struct _STARTUPINFOEXA {
    STARTUPINFOA StartupInfo;
    LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList;
} STARTUPINFOEXA, *LPSTARTUPINFOEXA;
typedef struct _STARTUPINFOEXW {
    STARTUPINFOW StartupInfo;
    LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList;
} STARTUPINFOEXW, *LPSTARTUPINFOEXW;

#ifdef UNICODE
typedef STARTUPINFOEXW STARTUPINFOEX;
typedef LPSTARTUPINFOEXW LPSTARTUPINFOEX;
#else
typedef STARTUPINFOEXA STARTUPINFOEX;
typedef LPSTARTUPINFOEXA LPSTARTUPINFOEX;
#endif // UNICODE

#define EXTENDED_STARTUPINFO_PRESENT		0x00080000

#define PROC_THREAD_ATTRIBUTE_NUMBER    0x0000FFFF
#define PROC_THREAD_ATTRIBUTE_THREAD    0x00010000  // Attribute may be used with thread creation
#define PROC_THREAD_ATTRIBUTE_INPUT     0x00020000  // Attribute is input only
#define PROC_THREAD_ATTRIBUTE_ADDITIVE  0x00040000  // Attribute may be "accumulated," e.g. bitmasks, counters, etc.

typedef enum _PROC_THREAD_ATTRIBUTE_NUM {
    ProcThreadAttributeParentProcess = 0,
    ProcThreadAttributeExtendedFlags,
    ProcThreadAttributeHandleList,
    ProcThreadAttributeGroupAffinity,
    ProcThreadAttributePreferredNode,
    ProcThreadAttributeIdealProcessor,
    ProcThreadAttributeUmsThread,
    ProcThreadAttributeMitigationPolicy,
    ProcThreadAttributeMax
} PROC_THREAD_ATTRIBUTE_NUM;

#define ProcThreadAttributeValue(Number, Thread, Input, Additive) \
    (((Number) & PROC_THREAD_ATTRIBUTE_NUMBER) | \
     ((Thread != FALSE) ? PROC_THREAD_ATTRIBUTE_THREAD : 0) | \
     ((Input != FALSE) ? PROC_THREAD_ATTRIBUTE_INPUT : 0) | \
     ((Additive != FALSE) ? PROC_THREAD_ATTRIBUTE_ADDITIVE : 0))

#define PROC_THREAD_ATTRIBUTE_PARENT_PROCESS \
    ProcThreadAttributeValue (ProcThreadAttributeParentProcess, FALSE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_EXTENDED_FLAGS \
    ProcThreadAttributeValue (ProcThreadAttributeExtendedFlags, FALSE, TRUE, TRUE)
#define PROC_THREAD_ATTRIBUTE_HANDLE_LIST \
    ProcThreadAttributeValue (ProcThreadAttributeHandleList, FALSE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_GROUP_AFFINITY \
    ProcThreadAttributeValue (ProcThreadAttributeGroupAffinity, TRUE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_PREFERRED_NODE \
    ProcThreadAttributeValue (ProcThreadAttributePreferredNode, FALSE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_IDEAL_PROCESSOR \
    ProcThreadAttributeValue (ProcThreadAttributeIdealProcessor, TRUE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_UMS_THREAD \
    ProcThreadAttributeValue (ProcThreadAttributeUmsThread, TRUE, TRUE, FALSE)
#define PROC_THREAD_ATTRIBUTE_MITIGATION_POLICY \
    ProcThreadAttributeValue (ProcThreadAttributeMitigationPolicy, FALSE, TRUE, FALSE)

#define PROCESS_CREATION_MITIGATION_POLICY_DEP_ENABLE            0x01
#define PROCESS_CREATION_MITIGATION_POLICY_DEP_ATL_THUNK_ENABLE  0x02
#define PROCESS_CREATION_MITIGATION_POLICY_SEHOP_ENABLE          0x04

#define PROC_THREAD_ATTRIBUTE_REPLACE_VALUE     0x00000001

#endif // (_WIN32_WINNT >= 0x0600)


typedef enum _RT_PROC_THREAD_ATTRIBUTE_NUM {
	RtProcThreadAttributeUseLocalMemory = 0x1000,
	RtProcThreadAttributeIdealProcessor,
	RtProcThreadAttributeProcessAffinity,
	RtProcThreadAttributeMax
} RT_PROC_THREAD_ATTRIBUTE_NUM;

#define RT_PROC_THREAD_ATTRIBUTE_USE_LOCAL_MEMORY \
	ProcThreadAttributeValue(RtProcThreadAttributeUseLocalMemory, FALSE, FALSE, FALSE)
#define RT_PROC_THREAD_ATTRIBUTE_IDEAL_PROCESSOR \
	ProcThreadAttributeValue(RtProcThreadAttributeIdealProcessor, FALSE, FALSE, FALSE)
#define RT_PROC_THREAD_ATTRIBUTE_PROCESS_AFFINITY \
	ProcThreadAttributeValue(RtProcThreadAttributeProcessAffinity, FALSE, FALSE, FALSE)


#if (_WIN32_WINNT >= 0x0501) || (WINVER >= 0x0501)
#if (WINVER == 0x0500) || (WINVER == 0x5000)
typedef struct _PROC_THREAD_ATTRIBUTE_LIST *PPROC_THREAD_ATTRIBUTE_LIST, *LPPROC_THREAD_ATTRIBUTE_LIST;
#endif

// This function initializes the specified list of attributes for process and thread creation.
BOOL RTAPI RtInitializeProcThreadAttributeList(LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,
										 DWORD dwAttributeCount,
										 DWORD dwFlags,
										 PSIZE_T lpSize);

// This function updates the specified attribute in a list of attributes for process and thread creation.
BOOL RTAPI RtUpdateProcThreadAttribute(LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList,
								 DWORD dwFlags,
								 DWORD_PTR Attribute,
								 PVOID lpValue,
								 SIZE_T cbSize,
								 PVOID lpPreviousValue,
								 PSIZE_T lpReturnSize);

// This function deletes the specified list of attributes for process and thread creation
// initialized by a previous call to RtInitializeProcThreadAttributeList.
VOID RTAPI RtDeleteProcThreadAttributeList(LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList);

#endif

RTBASEAPI BOOL RTAPI RtGetProcessFreeze(DWORD dwProcessId, BOOL *bFreeze);

#ifdef UNICODE
// This function retrieves a list of the IntervalZero-provided dongles attached to the machine.
#define RtGetDongles RtGetDonglesW
#else
// This function retrieves a list of the IntervalZero-provided dongles attached to the machine.
#define RtGetDongles RtGetDonglesA
#endif

// This function retrieves a list of the IntervalZero-provided dongles attached to the machine.
BOOL RTAPI RtGetDonglesA(
    char ** dongleData,
    DWORD * dongleCount);

// This function retrieves a list of the IntervalZero-provided dongles attached to the machine.
BOOL RTAPI RtGetDonglesW(
    wchar_t ** dongleData,
    DWORD * dongleCount);

#ifdef UNICODE
#define RT_LICENSE_INFO RT_LICENSE_INFO_W

// This function retrieves information about all licenses available on the system
#define RtGetLicenses RtGetLicensesW
#else
#define RT_LICENSE_INFO RT_LICENSE_INFO_A

// This function retrieves information about all licenses available on the system
#define RtGetLicenses RtGetLicensesA
#endif

// This function retrieves information about all licenses available on the system
BOOL RTAPI RtGetLicensesA(
	PRT_LICENSE_INFO_A result,
	PDWORD length);

// This function retrieves information about all licenses available on the system
BOOL RTAPI RtGetLicensesW(
	PRT_LICENSE_INFO_W result,
	PDWORD length);

// This function verifies whether the specified version of the RTX TCP-IP stack is installed and has
// a valid license.
BOOL RTAPI RtIsTcpStackLicensed(DWORD majorVersion);

// This function verifies whether the specified version of the RTX runtime is installed and has a
// valid license.
BOOL RTAPI RtIsRuntimeLicensed(DWORD majorVersion);

#ifdef UNICODE
// This function returns whether the specified RTSS application binary can run.  This means it has
// been built to run with the provided license feature and that there is a valid license for the
// feature on the system.
#define RtIsAppRunnable RtIsAppRunnableW
#else
// This function returns whether the specified RTSS application binary can run.  This means it has
// been built to run with the provided license feature and that there is a valid license for the
// feature on the system.
#define RtIsAppRunnable RtIsAppRunnableA
#endif

// This function returns whether the specified RTSS application binary can run.  This means it has
// been built to run with the provided license feature and that there is a valid license for the
// feature on the system.
BOOL RTAPI RtIsAppRunnableA(
	char * applicationPath,
	char * featureName,
	unsigned int reserved 
	);

// This function returns whether the specified RTSS application binary can run.  This means it has
// been built to run with the provided license feature and that there is a valid license for the
// feature on the system.
BOOL RTAPI RtIsAppRunnableW(
	wchar_t * applicationPath,
	wchar_t * featureName,
	unsigned int reserved
	);

#ifdef __cplusplus
}
#endif

#ifdef UNDER_RTSS
#ifdef UNDER_RTSS_UNSUPPORTED_CRT_APIS
#pragma deprecated(	_CreateFrameInfo,_CxxThrowException,_FPE_Raise,_FindAndUnlinkFrame,_Getdays,_Getmonths,_Gettnames,_HUGE,\
					_IsExceptionObjectToBeDestroyed,_Lock_shared_ptr_spin_lock,_Strftime,_Unlock_shared_ptr_spin_lock,_W_Getdays,\
					_W_Getmonths,_W_Gettnames,_Wcsftime,_XcptFilter,__AdjustPointer,__BuildCatchObject,__BuildCatchObjectHelper,\
					__C_specific_handler,__CppXcptFilter,__CxxDetectRethrow,__CxxExceptionFilter,__CxxFrameHandler,__CxxFrameHandler2,\
					__CxxFrameHandler3,__CxxQueryExceptionSize,__CxxRegisterExceptionObject,__CxxUnregisterExceptionObject,\
					__DestructExceptionObject,__FrameUnwindFilter,__NLG_Dispatch2,__NLG_Return2,__RTCastToVoid,__RTDynamicCast,\
					__RTtypeid,__STRINGTOLD,__STRINGTOLD_L,__TypeMatch,___lc_codepage_func,___lc_collate_cp_func,___lc_locale_name_func,\
					___mb_cur_max_func,___mb_cur_max_l_func,___setlc_active_func,___unguarded_readlc_active_add_func,\
					__badioinfo,__clean_type_info_names_internal,__create_locale,__crtCompareStringA,__crtCompareStringEx,\
					__crtCompareStringW,__crtCreateSemaphoreExW,__crtCreateSymbolicLinkW,__crtEnumSystemLocalesEx,__crtFlsAlloc,\
					__crtFlsFree,__crtFlsGetValue,__crtFlsSetValue,__crtGetDateFormatEx,__crtGetLocaleInfoEx,__crtGetShowWindowMode,\
					__crtGetTimeFormatEx,__crtGetUserDefaultLocaleName,__crtInitializeCriticalSectionEx,__crtIsPackagedApp,\
					__crtIsValidLocaleName,__crtLCMapStringA,__crtLCMapStringEx,__crtLCMapStringW,__crtSetThreadStackGuarantee,\
					__crtSetUnhandledExceptionFilter,__crt_debugger_hook,__daylight,__dllonexit,__doserrno,__dstbias,__fpecode,\
					__free_locale,__get_current_locale,__get_flsindex,__get_tlsindex,__getmainargs,__initenv,__iob_func,__isascii,\
					__iscsym,__iscsymf,__iswcsym,__iswcsymf,__jump_unwind,__lconv,__lconv_init,__p___argc,__p___argv,\
					__p___initenv,__p___mb_cur_max,__p___wargv,__p___winitenv,__p__acmdln,__p__commode,__p__daylight,__p__dstbias,\
					__p__environ,__p__fmode,__p__iob,__p__mbcasemap,__p__mbctype,__p__pctype,__p__pgmptr,__p__pwctype,__p__timezone,\
					__p__tzname,__p__wcmdln,__p__wenviron,__p__wpgmptr,__pctype_func,__pioinfo,__pwctype_func,__pxcptinfoptrs,\
					__report_gsfailure,__set_app_type,__setlc_active,__setusermatherr,__strncnt,__swprintf_l,__sys_errlist,\
					__sys_nerr,__threadhandle,__threadid,__timezone,__toascii,__tzname,__unDName,__unDNameEx,__unDNameHelper,\
					__uncaught_exception,__unguarded_readlc_active,__vswprintf_l,__wcserror,__wcserror_s,__wcsncnt,\
					__wgetmainargs,__winitenv,_abs64,_access,_access_s,_acmdln,_amsg_exit,_assert,_atodbl,_atodbl_l,_atof_l,\
					_atoflt,_atoflt_l,_atoi64,_atoi64_l,_atoi_l,_atol_l,_atoldbl,_atoldbl_l,_beep,_beginthread,_beginthreadex,\
					_byteswap_uint64,_byteswap_ulong,_byteswap_ushort,_c_exit,_cabs,_callnewh,_calloc_crt,_cexit,_cgets,_cgets_s,\
					_cgetws,_cgetws_s,_chdir,_chdrive,_chgsign,_chmod,_chsize,_chsize_s,_clearfp,_close,_commit,_commode,\
					_configthreadlocale,_control87 ,_controlfp_s,_copysign,_cprintf,_cprintf_l,_cprintf_p,_cprintf_p_l,_cprintf_s,\
					_cprintf_s_l,_cputs,_cputws,_creat,_create_locale,_cscanf,_cscanf_l,_cscanf_s,_cscanf_s_l,_ctime32,_ctime32_s,\
					_ctime64,_ctime64_s,_cwait,_cwprintf,_cwprintf_l,_cwprintf_p,_cwprintf_p_l,_cwprintf_s,_cwprintf_s_l,_cwscanf,\
					_cwscanf_l,_cwscanf_s,_cwscanf_s_l,_daylight,_difftime32,_difftime64,_dosmaperr,_dstbias,_dup,_dup2,_dupenv_s,\
					_ecvt,_ecvt_s,_endthread,_endthreadex,_eof,_errno,_execl,_execle,_execlp,_execlpe,_execv,_execve,_execvp,\
					_execvpe,_exit,_expand,_fclose_nolock,_fcloseall,_fcvt,_fcvt_s,_fdopen,_fflush_nolock,_fgetchar,_fgetwc_nolock,\
					_fgetwchar,_filbuf,_filelength,_filelengthi64,_fileno,_findclose,_findfirst32,_findfirst32i64,_findfirst64,\
					_findfirst64i32,_findnext32,_findnext32i64,_findnext64,_findnext64i32,_finite,_flsbuf,_flushall,_fpclass,\
					_fpieee_flt,_fprintf_l,_fprintf_p,_fprintf_p_l,_fprintf_s_l,_fputchar,_fputwc_nolock,_fputwchar,_fread_nolock,\
					_fread_nolock_s,_free_locale,_freea,_freea_s,_freefls,_fscanf_l,_fscanf_s_l,_fseek_nolock,_fseeki64,_fseeki64_nolock,\
					_fsopen,_fstat32,_fstat32i64,_fstat64,_fstat64i32,_ftell_nolock,_ftelli64,_ftelli64_nolock,_ftime32,_ftime32_s,\
					_ftime64,_ftime64_s,_fullpath,_futime32,_futime64,_fwprintf_l,_fwprintf_p,_fwprintf_p_l,_fwprintf_s_l,_fwrite_nolock,\
					_fwscanf_l,_fwscanf_s_l,_gcvt,_gcvt_s,_get_current_locale,_get_daylight,_get_doserrno,_get_dstbias,_get_errno,\
					_get_fmode,_get_heap_handle,_get_invalid_parameter_handler,_get_osfhandle,_get_output_format,_get_pgmptr,\
					_get_printf_count_output,_get_purecall_handler,_get_terminate,_get_timezone,_get_tzname,_get_unexpected,_get_wpgmptr,\
					_getc_nolock,_getch,_getch_nolock,_getche,_getche_nolock,_getcwd,_getdcwd,_getdiskfree,_getdllprocaddr,_getdrive,\
					_getdrives,_getmaxstdio,_getmbcp,_getpid,_getptd,_getsystime,_getw,_getwch,_getwch_nolock,_getwche,_getwche_nolock,\
					_getws,_getws_s,_gmtime32,_gmtime32_s,_gmtime64,_gmtime64_s,_heapadd,_heapchk,_heapmin,_heapset,_heapused,_heapwalk,\
					_hypot,_hypotf,_i64toa,_i64toa_s,_i64tow,_i64tow_s,_initptd,_initterm,_initterm_e,_invalid_parameter,\
					_invalid_parameter_noinfo,_invalid_parameter_noinfo_noreturn,_invoke_watson,_iob,_isalnum_l,_isalpha_l,_isatty,\
					_iscntrl_l,_isctype,_isctype_l,_isdigit_l,_isgraph_l,_isleadbyte_l,_islower_l,_ismbbalnum,_ismbbalnum_l,_ismbbalpha,\
					_ismbbalpha_l,_ismbbgraph,_ismbbgraph_l,_ismbbkalnum,_ismbbkalnum_l,_ismbbkana,_ismbbkana_l,_ismbbkprint,\
					_ismbbkprint_l,_ismbbkpunct,_ismbbkpunct_l,_ismbblead,_ismbblead_l,_ismbbprint,_ismbbprint_l,_ismbbpunct,\
					_ismbbpunct_l,_ismbbtrail,_ismbbtrail_l,_ismbcalnum,_ismbcalnum_l,_ismbcalpha,_ismbcalpha_l,_ismbcdigit,\
					_ismbcdigit_l,_ismbcgraph,_ismbcgraph_l,_ismbchira,_ismbchira_l,_ismbckata,_ismbckata_l,_ismbcl0,_ismbcl0_l,\
					_ismbcl1,_ismbcl1_l,_ismbcl2,_ismbcl2_l,_ismbclegal,_ismbclegal_l,_ismbclower,_ismbclower_l,_ismbcprint,_ismbcprint_l,\
					_ismbcpunct,_ismbcpunct_l,_ismbcspace,_ismbcspace_l,_ismbcsymbol,_ismbcsymbol_l,_ismbcupper,_ismbcupper_l,_ismbslead,\
					_ismbslead_l,_ismbstrail,_ismbstrail_l,_isnan,_isprint_l,_ispunct_l,_isspace_l,_isupper_l,_iswalnum_l,_iswalpha_l,\
					_iswcntrl_l,_iswcsym_l,_iswcsymf_l,_iswctype_l,_iswdigit_l,_iswgraph_l,_iswlower_l,_iswprint_l,_iswpunct_l,_iswspace_l,\
					_iswupper_l,_iswxdigit_l,_isxdigit_l,_itoa,_itoa_s,_itow,_itow_s,_j0,_j1,_jn,_kbhit,_lfind,_lfind_s,_loaddll,\
					_localtime32,_localtime32_s,_localtime64,_localtime64_s,_lock,_lock_file,_locking,_logb,_logbf,_lrotl,_lrotr,_lsearch,\
					_lsearch_s,_lseek,_lseeki64,_ltoa,_ltoa_s,_ltow,_ltow_s,_makepath,_makepath_s,_malloc_crt,_mbbtombc,_mbbtombc_l,\
					_mbbtype,_mbbtype_l,_mbcasemap,_mbccpy,_mbccpy_l,_mbccpy_s,_mbccpy_s_l,_mbcjistojms,_mbcjistojms_l,_mbcjmstojis,\
					_mbcjmstojis_l,_mbclen,_mbclen_l,_mbctohira,_mbctohira_l,_mbctokata,_mbctokata_l,_mbctolower,_mbctolower_l,_mbctombb,\
					_mbctombb_l,_mbctoupper,_mbctoupper_l,_mbctype,_mblen_l,_mbsbtype,_mbsbtype_l,_mbscat_s,_mbscat_s_l,_mbschr,_mbschr_l,\
					_mbscmp,_mbscmp_l,_mbscoll,_mbscoll_l,_mbscpy_s,_mbscpy_s_l,_mbscspn,_mbscspn_l,_mbsdec,_mbsdec_l,_mbsicmp,_mbsicmp_l,\
					_mbsicoll,_mbsicoll_l,_mbsinc,_mbsinc_l,_mbslen,_mbslen_l,_mbslwr,_mbslwr_l,_mbslwr_s,_mbslwr_s_l,_mbsnbcat,_mbsnbcat_l,\
					_mbsnbcat_s,_mbsnbcat_s_l,_mbsnbcmp,_mbsnbcmp_l,_mbsnbcnt,_mbsnbcnt_l,_mbsnbcoll,_mbsnbcoll_l,_mbsnbcpy,_mbsnbcpy_l,\
					_mbsnbcpy_s,_mbsnbcpy_s_l,_mbsnbicmp,_mbsnbicmp_l,_mbsnbicoll,_mbsnbicoll_l,_mbsnbset,_mbsnbset_l,_mbsnbset_s,\
					_mbsnbset_s_l,_mbsncat,_mbsncat_l,_mbsncat_s,_mbsncat_s_l,_mbsnccnt,_mbsnccnt_l,_mbsncmp,_mbsncmp_l,_mbsncoll,\
					_mbsncoll_l,_mbsncpy,_mbsncpy_l,_mbsncpy_s,_mbsncpy_s_l,_mbsnextc,_mbsnextc_l,_mbsnicmp,_mbsnicmp_l,_mbsnicoll,\
					_mbsnicoll_l,_mbsninc,_mbsninc_l,_mbsnlen,_mbsnlen_l,_mbsnset,_mbsnset_l,_mbsnset_s,_mbsnset_s_l,_mbspbrk,_mbspbrk_l,\
					_mbsrchr,_mbsrchr_l,_mbsrev,_mbsrev_l,_mbsset,_mbsset_l,_mbsset_s,_mbsset_s_l,_mbsspn,_mbsspn_l,_mbsspnp,_mbsspnp_l,\
					_mbsstr,_mbsstr_l,_mbstok,_mbstok_l,_mbstok_s,_mbstok_s_l,_mbstowcs_l,_mbstowcs_s_l,_mbstrlen,_mbstrlen_l,_mbstrnlen,\
					_mbstrnlen_l,_mbsupr,_mbsupr_l,_mbsupr_s,_mbsupr_s_l,_mbtowc_l,_memccpy,_memicmp,_memicmp_l,_mkdir,_mkgmtime32,\
					_mkgmtime64,_mktemp,_mktemp_s,_mktime32,_mktime64,_msize,_nextafter,_onexit,_open,_open_osfhandle,_pclose,\
					_pipe,_popen,_printf_l,_printf_p,_printf_p_l,_printf_s_l,_purecall,_putch,_putch_nolock,_putenv,_putenv_s,\
					_putw,_putwch,_putwch_nolock,_putws,_read,_realloc_crt,_recalloc,_recalloc_crt,_resetstkoflw,_rmdir,_rmtmp,\
					_rotl,_rotl64,_rotr,_rotr64,_scalb,_scanf_l,_scanf_s_l,_scprintf,_scprintf_l,_scprintf_p,_scprintf_p_l,_scwprintf,\
					_scwprintf_l,_scwprintf_p,_scwprintf_p_l,_searchenv,_searchenv_s,_set_abort_behavior,_set_controlfp,_set_doserrno,\
					_set_errno,_set_error_mode,_set_fmode,_set_invalid_parameter_handler,_set_malloc_crt_max_wait,_set_output_format,\
					_set_printf_count_output,_set_purecall_handler,_seterrormode,_setjmp,_setjmpex,_setmaxstdio,_setmbcp,_setmode,\
					_setsystime,_sleep,_snprintf,_snprintf_c,_snprintf_c_l,_snprintf_l,_snprintf_s,_snprintf_s_l,_snscanf,_snscanf_l,\
					_snscanf_s,_snscanf_s_l,_snwprintf,_snwprintf_l,_snwprintf_s,_snwprintf_s_l,_snwscanf,_snwscanf_l,_snwscanf_s,\
					_snwscanf_s_l,_sopen,_sopen_s,_spawnl,_spawnle,_spawnlp,_spawnlpe,_spawnv,_spawnve,_spawnvp,_spawnvpe,_splitpath,\
					_splitpath_s,_sprintf_l,_sprintf_p,_sprintf_p_l,_sprintf_s_l,_sscanf_l,_sscanf_s_l,_stat32,_stat32i64,_stat64,\
					_stat64i32,_statusfp,_strcoll_l,_strdate,_strdate_s,_strdup,_strerror,_strerror_s,_strftime_l,_stricmp,_stricmp_l,\
					_stricoll,_stricoll_l,_strlwr,_strlwr_l,_strlwr_s,_strlwr_s_l,_strncoll,_strncoll_l,_strnicmp,_strnicmp_l,_strnicoll,\
					_strnicoll_l,_strnset,_strnset_s,_strrev,_strset,_strset_s,_strtime,_strtime_s,_strtod_l,_strtoi64,_strtoi64_l,\
					_strtol_l,_strtoui64,_strtoui64_l,_strtoul_l,_strupr,_strupr_l,_strupr_s,_strupr_s_l,_strxfrm_l,_swab,_swprintf,\
					_swprintf_c,_swprintf_c_l,_swprintf_p,_swprintf_p_l,_swprintf_s_l,_swscanf_l,_swscanf_s_l,\
					_tell,_telli64,_tempnam,_time32,_time64,_timezone,_tolower,_tolower_l,_toupper,_toupper_l,_towlower_l,_towupper_l,\
					_tzname,_tzset,_ui64toa,_ui64toa_s,_ui64tow,_ui64tow_s,_ultoa,_ultoa_s,_ultow,_ultow_s,_umask,_umask_s,_ungetc_nolock,\
					_ungetch,_ungetch_nolock,_ungetwc_nolock,_ungetwch,_ungetwch_nolock,_unlink,_unloaddll,_unlock,_unlock_file,_utime32,\
					_utime64,_vcprintf,_vcprintf_l,_vcprintf_p,_vcprintf_p_l,_vcprintf_s,_vcprintf_s_l,_vcwprintf,_vcwprintf_l,_vcwprintf_p,\
					_vcwprintf_p_l,_vcwprintf_s,_vcwprintf_s_l,_vfprintf_l,_vfprintf_p,_vfprintf_p_l,_vfprintf_s_l,_vfwprintf_l,\
					_vfwprintf_p,_vfwprintf_p_l,_vfwprintf_s_l,_vprintf_l,_vprintf_p,_vprintf_p_l,_vprintf_s_l,_vscprintf,_vscprintf_l,\
					_vscprintf_p,_vscprintf_p_l,_vscwprintf,_vscwprintf_l,_vscwprintf_p,_vscwprintf_p_l,_vsnprintf,_vsnprintf_c,\
					_vsnprintf_c_l,_vsnprintf_l,_vsnprintf_s,_vsnprintf_s_l,_vsnwprintf,_vsnwprintf_l,_vsnwprintf_s,_vsnwprintf_s_l,\
					_vsprintf_l,_vsprintf_p,_vsprintf_p_l,_vsprintf_s_l,_vswprintf,_vswprintf_c,_vswprintf_c_l,_vswprintf_l,_vswprintf_p,\
					_vswprintf_p_l,_vswprintf_s_l,_vwprintf_l,_vwprintf_p,_vwprintf_p_l,_vwprintf_s_l,_waccess,_waccess_s,_wasctime,\
					_wasctime_s,_wassert,_wchdir,_wchmod,_wcmdln,_wcreat,_wcreate_locale,_wcscoll_l,_wcsdup,_wcserror,_wcserror_s,\
					_wcsftime_l,_wcsicmp,_wcsicmp_l,_wcsicoll,_wcsicoll_l,_wcslwr,_wcslwr_l,_wcslwr_s,_wcslwr_s_l,_wcsncoll,_wcsncoll_l,\
					_wcsnicmp,_wcsnicmp_l,_wcsnicoll,_wcsnicoll_l,_wcsnset,_wcsnset_s,_wcsrev,_wcsset,_wcsset_s,_wcstod_l,_wcstoi64,\
					_wcstoi64_l,_wcstol_l,_wcstombs_l,_wcstombs_s_l,_wcstoui64,_wcstoui64_l,_wcstoul_l,_wcsupr,_wcsupr_l,_wcsupr_s,\
					_wcsupr_s_l,_wcsxfrm_l,_wctime32,_wctime32_s,_wctime64,_wctime64_s,_wctomb_l,_wctomb_s_l,_wctype,_wdupenv_s,\
					_wexecl,_wexecle,_wexeclp,_wexeclpe,_wexecv,_wexecve,_wexecvp,_wexecvpe,_wfdopen,_wfindfirst32,_wfindfirst32i64,\
					_wfindfirst64,_wfindfirst64i32,_wfindnext32,_wfindnext32i64,_wfindnext64,_wfindnext64i32,_wfopen,_wfopen_s,_wfreopen,\
					_wfreopen_s,_wfsopen,_wfullpath,_wgetcwd,_wgetdcwd,_wgetenv,_wgetenv_s,_wmakepath,_wmakepath_s,_wmkdir,_wmktemp,\
					_wmktemp_s,_wopen,_wperror,_wpopen,_wprintf_l,_wprintf_p,_wprintf_p_l,_wprintf_s_l,_wputenv,_wputenv_s,\
					_wremove,_wrename,_write,_wrmdir,_wscanf_l,_wscanf_s_l,_wsearchenv,_wsearchenv_s,_wsetlocale,_wsopen,_wsopen_s,\
					_wspawnl,_wspawnle,_wspawnlp,_wspawnlpe,_wspawnv,_wspawnve,_wspawnvp,_wspawnvpe,_wsplitpath,_wsplitpath_s,_wstat32,\
					_wstat32i64,_wstat64,_wstat64i32,_wstrdate,_wstrdate_s,_wstrtime,_wstrtime_s,_wsystem,_wtempnam,_wtmpnam,_wtmpnam_s,\
					_wtof,_wtof_l,_wtoi,_wtoi64,_wtoi64_l,_wtoi_l,_wtol,_wtol_l,_wunlink,_wutime32,_wutime64,_y0,_y1,_yn,abort,acosf,\
					asctime,asctime_s,asinf,atan2f,atanf,atexit,bsearch_s,btowc,ceilf,clearerr,clearerr_s,clock,cosf,coshf,expf,fabsf,\
					feof,ferror,fflush,fgetc,fgetpos,fgetwc,fgetws,floorf,fmodf,fopen_s,fprintf_s,fputwc,fputws,fread_s,freopen,freopen_s,\
					fscanf,fscanf_s,fsetpos,fwprintf_s,fwrite,fwscanf,fwscanf_s,getchar,getenv,getenv_s,gets,gets_s,getwc,getwchar,\
					is_wctype,isleadbyte,llabs,lldiv,localeconv,log10f,logf,mblen,mbrlen,mbrtowc,mbsrtowcs,mbsrtowcs_s,mbstowcs,mbstowcs_s,\
					mbtowc,memcpy_s,memmove_s,modff,powf,printf_s,puts,putwc,putwchar,qsort_s,raise,rand_s,remove,rename,scanf,scanf_s,\
					setbuf,setvbuf,sinf,sinhf,sprintf_s,sqrtf,sscanf_s,strcat_s,strcoll,strcpy_s,strerror_s,strftime,strncat_s,\
					strncpy_s,strnlen,strtok_s,strxfrm,swprintf_s,swscanf,swscanf_s,system,tanf,tanhf,tmpfile,tmpfile_s,tmpnam,tmpnam_s,\
					ungetc,ungetwc,vfprintf,vfprintf_s,vfwprintf,vfwprintf_s,vprintf,vprintf_s,vsprintf_s,vswprintf_s,vwprintf,vwprintf_s,\
					wcrtomb,wcrtomb_s,wcscat_s,wcscoll,wcscpy_s,wcsncat_s,wcsncpy_s,wcsnlen,wcspbrk,wcsrchr,wcsrtombs,wcsrtombs_s,wcsspn,\
					wcstok_s,wcstombs,wcstombs_s,wcstoul,wcsxfrm,wctob,wctomb,wctomb_s,wmemcpy_s,wmemmove_s,wprintf_s,wscanf,wscanf_s)
#ifndef _wpgmptr
#pragma deprecated(_pgmptr, _wpgmptr, _wenviron, _pwctype, _pctype, _fmode, _environ,__wargv, __mb_cur_max, __argc, __argv)
#endif
#endif
#endif

#endif // _RTAPI_H_
