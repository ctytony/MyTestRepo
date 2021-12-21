/*
 *
 * Copyright (c) 1996-2010 IntervalZero, Inc.  All rights reserved.
 *
 * Module Name:
 *
 *	rtx_dbgext_data.h
 *
 * Abstract:
 *
 * Defines RTX debugging data structures.  Debugger extensions or any other tool
 * subscribing for RTX debugging data refer to this header.
 *
 * Environment:
 *
 *	User and Kernel space
 * 
--*/


#pragma once 

#include "rtdef.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define RTDDE_DBGEXT_PROC_EXITED 0x00000001

//typedef OSVERSIONINFO RTXVERSIONINFO, *PRTXVERSIONINFO;
#define rtdde_system_process L"RTSS System Process"
#define rtdde_proxy_process L"RTSS Proxy Process"
#define rtdde_c_runtime_process L"W32_dll.rtss - Supplemental Win32 for C runtime library"

typedef enum _RTDDE_DBGEXT_INFO_TYPE
{
	RTDDE_basic_info = 0,
	RTDDE_summary_info,
	RTDDE_extended_info,
	RTDDE_local_mem_list,	
	RTDDE_mem_list,
	RTDDE_stack_list,
	RTDDE_contiguous_mem_list,
	RTDDE_mapped_mem_list,
	RTDDE_thread_list,
	RTDDE_handle_array
} RTDDE_DBGEXT_INFO_TYPE;

typedef struct _RTDDE_PROC_BASIC_INFO
{
	ULONG ProcID;
	WCHAR ProcName[RTX_PATH_MAX];
	ULONG_PTR ProcObj;
}RTDDE_PROC_BASIC_INFO, *PRTDDE_PROC_BASIC_INFO;

typedef struct _RTDDE_PROC_SUMMARY_INFO
{
	RTDDE_PROC_BASIC_INFO proc_basic_info;
	WCHAR CmdLine[RTX_PATH_MAX];
	BOOL IsDebuggerAttached;
	ULONG ProcFlags;
	ULONG ProcRefCount;
	ULONG ExitCode;
	ULONG_PTR ProcHeap;
	ULONG_PTR DriverObj;
	ULONG_PTR ShutdownObj;
	ULONG   ProcessorAffinityMask;  // A bit vector, each bit represents an allowed processor
	ULONG   AffinedProcessorNumber; // The processor number on which the process is bound
	ULONG   LocalMemoryUsage;   // Local memory used in bytes
}RTDDE_PROC_SUMMARY_INFO, *PRTDDE_PROC_SUMMARY_INFO;

typedef struct _RTDDE_PROC_EXTENDED_INFO
{
	RTDDE_PROC_SUMMARY_INFO proc_summary_info;
	ULONG LocalMemFlags;
	ULONG_PTR ProcEntry;
	//ULONG_PTR LocalMemList;
	ULONG_PTR MemList;
	ULONG_PTR StackList;
	ULONG_PTR ContiguousMemList;
	ULONG_PTR MappedMemList;
	ULONG_PTR ResourceList;
	ULONG_PTR HandleArray;
	ULONG_PTR ThreadList;
	ULONG_PTR InitThreadObj;
	ULONG_PTR InitThreadStack;
	ULONG_PTR InitThreadStackSize;
}RTDDE_PROC_EXTENDED_INFO, *PRTDDE_PROC_EXTENDED_INFO;

typedef struct _RTDDE_THREAD_BASIC_INFO
{
	ULONG ThreadID;
	ULONG State;
	ULONG_PTR ThreadObj;
}RTDDE_THREAD_BASIC_INFO, *PRTDDE_THREAD_BASIC_INFO;

typedef struct _RTDDE_THREAD_SUMMARY_INFO
{
	RTDDE_THREAD_BASIC_INFO thread_basic_info;
	ULONG ProcID;
	WCHAR ProcName[RTX_PATH_MAX];
	ULONG_PTR ProcObj;
	ULONG Flags;
	ULONG NativePriority;
	ULONG CurrentPriority;
	ULONG   ProcessorAffinityMask;  // A bit vector, each bit represents an allowed processor
	ULONG   AffinedProcessorNumber; // The processor number on which the process is bound
}RTDDE_THREAD_SUMMARY_INFO, *PRTDDE_THREAD_SUMMARY_INFO;

typedef struct _RTDDE_THREAD_EXTENDED_INFO
{
	RTDDE_THREAD_SUMMARY_INFO thread_summary_info;
	ULONG LastErr;
	ULONG_PTR StackAddr;
	ULONG_PTR StackEnd;
	ULONG	StackSize;
	ULONG_PTR EntryPoint;
	ULONG_PTR EntryArg;
	ULONG_PTR Timer;
	WCHAR Frames[25] [256]; //We could make this customizable if required.
}RTDDE_THREAD_EXTENDED_INFO, *PRTDDE_THREAD_EXTENDED_INFO;

typedef struct _RTDDE_MEMORY_INFO
{
	ULONG_PTR MemAddress;
	ULONG Size;
}RTDDE_MEMORY_INFO, *PRTDDE_MEMORY_INFO;

typedef struct _RTDDE_TIMER_OBJ
{
	ULONG		Period; 			
	ULONG		Clock;				
    LONG		EventState;			
	ULONG_PTR	EventWaitList;			
	ULONG_PTR	ThreadAddr;				
    ULONG_PTR	FuncAddr;	
    PVOID		Context;			
}RTDDE_TIMER_OBJ, *PRTDDE_TIMER_OBJ;

typedef struct _RTDDE_INTR_OBJ
{
	LONG        Vector;         
    DWORD       Mask;           
    ULONG_PTR   ProcAddr;        
    ULONG_PTR  Attachments;    
    BOOLEAN     Shared;         
}RTDDE_INTR_OBJ, *PRTDDE_INTR_OBJ;

typedef struct _RTDDE_ATTACH_OBJ
{
	ULONG_PTR		List;                       
    LONG			EventState;			
	ULONG_PTR		EventWaitList;			
    PRTDDE_INTR_OBJ  Interrupt;                  
    ULONG_PTR		ThreadAddr;                     
    ULONG_PTR		FuncAddr;
    PVOID			Context;                    
}RTDDE_ATTACH_OBJ, *PRTDDE_ATTACH_OBJ;

typedef struct _RTDDE_SHUTDOWN_OBJ
{
	LONG		EventState;			
	ULONG_PTR	EventWaitList;			
	ULONG_PTR	FuncAddr;
	ULONG_PTR	ThreadAddr;			
	PVOID		Context;
}RTDDE_SHUTDOWN_OBJ, *PRTDDE_SHUTDOWN_OBJ;

typedef struct _RTDDE_EVENT
{
	BOOLEAN		ManualReset;
    BOOLEAN		State;
}RTDDE_EVENT_OBJ, *PRTDDE_EVENT_OBJ;

typedef struct _RTDDE_MUTEX
{
	LONG		    Count;			
    BOOLEAN		    Abandoned;		
    ULONG_PTR		OwningThreadAddr;
}RTDDE_MUTEX, *PRTDDE_MUTEX;

typedef struct _RTDDE_SEMAPHORE
{
	LONG			Count;			
    LONG			MaximumCount;	
}RTDDE_SEMAPHORE, *PRTDDE_SEMAPHORE;

typedef struct _RTDDE_OBJECT_BASIC_INFO
{
	ULONG_PTR ObjAddr;
	WCHAR Type[32]; 
	ULONG Count;
	WCHAR ObjectName[RTX_PATH_MAX];	
}RTDDE_OBJECT_BASIC_INFO, *PRTDDE_OBJECT_BASIC_INFO;

typedef struct _RTDDE_OBJECT_EXTENDED_INFO
{
	RTDDE_OBJECT_BASIC_INFO RTDDE_obj_basic_info;
	ULONG ObjAttrib;
	ULONG Objsize;
	ULONG_PTR Dispfunc;
	union _RTDDE_OBJ
	{
		RTDDE_TIMER_OBJ		TimerObj;
		RTDDE_INTR_OBJ		InterruptObj;
		RTDDE_ATTACH_OBJ	AttachObj;
		RTDDE_SHUTDOWN_OBJ	ShutdownObj;
		RTDDE_EVENT_OBJ		EventObj;
		RTDDE_MUTEX			MutexObj;
		RTDDE_SEMAPHORE		SemaphoreObj;
	}RTDDE_OBJ;
}RTDDE_OBJECT_EXTENDED_INFO, *PRTDDE_OBJECT_EXTENDED_INFO;

typedef struct _RTDDE_HANDLE_BASIC_INFO
{
	RTDDE_OBJECT_BASIC_INFO RTDDE_obj_basic_info;
	ULONG RTWin32Handle;
}RTDDE_HANDLE_BASIC_INFO, *PRTDDE_HANDLE_BASIC_INFO;

typedef struct _RTDDE_HANDLE_EXTENDED_INFO
{
	RTDDE_OBJECT_EXTENDED_INFO RTDDE_obj_ext_info;
	ULONG RTWin32Handle;
}RTDDE_HANDLE_EXTENDED_INFO, *PRTDDE_HANDLE_EXTENDED_INFO;

typedef struct _RTDDE_SUBSYSTEM_INFO
{
	ULONG NumStartedRtssProcessors;
	ULONG RtssProcessorNumberP0;
	ULONG DefaultTimeQuantum;
	BOOL EnterDbgOnException;
	ULONG StopAtFirstException;
	ULONG ExceptionDispositionFlag;
	ULONG DebugBreakAtFirstException;
	BOOL FreeStackOnThreadTerminate;
	ULONG HalTimerPeriod;
	ULONG LocalMemInitSize;
	ULONG MapMemoryMaxSize;
	BOOL ShutdownPrintHandlerPresent;
	ULONG StarvationTimeout;
	ULONG StarvationTimeOutBehavior;
	ULONG CrashDumpType;
	ULONG FullCacheFlushDelay;
}RTDDE_SUBSYSTEM_INFO, *PRTDDE_SUBSYSTEM_INFO;

typedef struct _RTDDE_MINI_DUMP_INFO
{
	ULONG Signature;
	ULONG VersionInfo;
	ULONG ExceptionData[8];
	char ExceptionDesc[32];
	CONTEXT ExceptionContext;
	RTDDE_PROC_SUMMARY_INFO CurProcInfo;
	RTDDE_THREAD_SUMMARY_INFO CurThreadInfo;
	RTDDE_SUBSYSTEM_INFO SubsystemInfo;
	ULONG Frames[3][5]; //5 because we copy child frame, return address and 3 params following it.
	ULONG ActiveProcCount;
	//what follows is a list of active rtss processes basic info.
	//PRTDDE_PROC_BASIC_INFO ActiveProcArray;	
}RTDDE_MINI_DUMP_INFO, *PRTDDE_MINI_DUMP_INFO;

#ifdef __cplusplus 
}
#endif
