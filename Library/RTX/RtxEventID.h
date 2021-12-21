/*++
 *
 * Copyright (c) 1996-2010 IntervalZero, Inc.  All rights reserved.
 *
 * Module:
 *
 *	RtxEventID.h
 *
 * Abstract:
 *
 *	Definition of event IDs used in RTX TimeView
 *
 * Environment:
 *
 *	RTX
 *
 -*/

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __RTX_EVENT_ID_H__
#define __RTX_EVENT_ID_H__


#define TOTAL_RTX_EVENT_GROUPS    22			//  from 1 to 22
#define EVENTS_IN_A_GROUP		1024			//  from 0 to 1023
#define RTX_EVENT_ID_BASE		0x00000200
#define	RTX_TE_REGISTER_PROCESS   1
#define	RTX_TE_REGISTER_THREAD    2
#define RTX_TE_LOG_PROCESS_NAME   3
#define	RTX_TE_TRIGGER_EVENT	  4 


typedef enum __RtxEventId {

//
// Group 1: Thread context switch
//
	RTX_TE_CONTEXT_SWITCH =	 (RTX_EVENT_ID_BASE << 1),		 // Old ThreadId, New ThreadId

//    
// Group 2: Timer interrupt 
//
	RTX_TE_INTERRUPT_TIMER = (RTX_EVENT_ID_BASE << 2),		 // None

//
// Group 3: Device, Exception and SRI interrupt events
//
	RTX_TE_INTERRUPT_DEVICE = (RTX_EVENT_ID_BASE << 3),						 // Vector #
	RTX_TE_INTERRUPT_EXCEPTION,					     // ExceptionType


//    
// Group 4: Thread management events
//
	RTX_TE_THREAD_QUEUE	=	 (RTX_EVENT_ID_BASE << 4),	     // pThread
	RTX_TE_THREAD_DEQUEUE,						     // pThread

//			
// Group 5: Object management events
//
    RTX_TE_OBJ_CREATE = (RTX_EVENT_ID_BASE << 5),	 // ObjectType, Pointer to name string
    RTX_TE_OBJ_OPEN,								 // ObjectType, Pointer to name string
    RTX_TE_OBJ_CLOSE,								 // ObjectType, Pointer to name string
    RTX_TE_OBJ_DESTROY,								 // ObjectType, Pointer to name string
    RTX_TE_OBJ_WFSO_WAIT,							 // ObjectType, ThreadId that waited
    RTX_TE_OBJ_WFSO_WAKEUP,							 // ObjectType, ThreadId that will wake
    RTX_TE_OBJ_WFSO_TIMEOUT,						 // ObjectType, ThreadId that will run
	RTX_TE_OBJ_WFO_WAKEUP,							 // Number of objects to wait, ThreadId that will wake
    RTX_TE_OBJ_WFO_TIMEOUT,							 // Number of objects to wait, ThreadId that will run


//
// Group 6: Semaphore events
//
    RTX_TE_RELEASE_SEMAPHORE =(RTX_EVENT_ID_BASE << 6),	 // Handle, Count
    RTX_TE_RELEASE_SEMAPHORE_RETURN,				 // ReturnValue, LastError
	RTX_TE_ND_CREATE_SEMAPHORE,						 // MaximumCount
    RTX_TE_ND_CREATE_SEMAPHORE_RETURN,			  	 // ReturnValue, LastError
	RTX_TE_ND_OPEN_SEMAPHORE,						 // None
	RTX_TE_ND_OPEN_SEMAPHORE_RETURN,				 // ReturnValue, LastError
			
//
// Group 7: Mutex events
//
    RTX_TE_RELEASE_MUTEX = (RTX_EVENT_ID_BASE << 7),		 // Handle
    RTX_TE_RELEASE_MUTEX_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_CREATE_MUTEX,							 // InitalOwner
    RTX_TE_ND_CREATE_MUTEX_RETURN,				  	 // ReturnValue, LastError
	RTX_TE_ND_OPEN_MUTEX,							 // None
	RTX_TE_ND_OPEN_MUTEX_RETURN,					 // ReturnValue, LastError
	RTX_TE_MUTEX_PRIORITY_THID,						 // Thread ID, None
	RTX_TE_MUTEX_PRIORITY_PROMOTE,					 // Current Priority, Promoted Priority
	RTX_TE_MUTEX_PRIORITY_DEMOTE,					 // Current Priority, Demoted Priority

//
// Group 8: IPC events
//
    RTX_TE_PULSE_EVENT = (RTX_EVENT_ID_BASE << 8),			 // Handle
    RTX_TE_PULSE_EVENT_RETURN,					  	 // ReturnValue, LastError
	RTX_TE_RESET_EVENT,								 // Handle
	RTX_TE_RESET_EVENT_RETURN,					 	 // ReturnValue, LastError
    RTX_TE_SET_EVENT,								 // Handle 
	RTX_TE_SET_EVENT_RETURN,						 // ReturnValue, LastError
    RTX_TE_ND_CREATE_EVENT,							 // ManualReset, InitialState
    RTX_TE_ND_CREATE_EVENT_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_OPEN_EVENT,							 // None
    RTX_TE_ND_OPEN_EVENT_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_CREATE_SHARED_MEM,					 // LowSize, HighSize
    RTX_TE_ND_CREATE_SHARED_MEM_RETURN,				 // ReturnValue, LastError
	RTX_TE_ND_OPEN_SHARED_MEM,						 // None
	RTX_TE_ND_OPEN_SHARED_MEM_RETURN,				 // ReturnValue, LastError


//
// Group 9: Timer events
//
    RTX_TE_CANCEL_TIMER	= (RTX_EVENT_ID_BASE << 9),		 // Handle
    RTX_TE_CANCEL_TIMER_RETURN,						 // ReturnValue, LastError
	RTX_TE_SET_TIMER,								 // Handle
    RTX_TE_SET_TIMER_RETURN,				  		 // ReturnValue, LastError
	RTX_TE_SET_TIMER_REL,							 // Handle
	RTX_TE_SET_TIMER_REL_RETURN,					 // ReturnValue, LastError
    RTX_TE_ND_CREATE_TIMER,							 // ClockType
	RTX_TE_ND_CREATE_TIMER_RETURN,					 // ReturnValue, LastError

//
// Group 10: Exception events
//
    RTX_TE_RAISE_EXCEPTION = (RTX_EVENT_ID_BASE << 10),	 // ExceptionCode, Flags
	RTX_TE_RAISE_EXCEPTION_RETURN,					 // ReturnValue, LastError
//
// Group 11: Device events
//
    RTX_TE_ND_GET_BUS_DATA = (RTX_EVENT_ID_BASE << 11),	 // SlotNumber
    RTX_TE_ND_GET_BUS_DATA_RETURN,				     // ReturnValue, LastError
	RTX_TE_ND_SET_BUS_DATA,							 // SlotNumber
    RTX_TE_ND_SET_BUS_DATA_RETURN,				 	 // ReturnValue, LastError
	RTX_TE_ND_TRANSLATE_BUS_ADDR,					 // LowBusAddress, LowTranslatedAddress 
	RTX_TE_ND_TRANSLATE_BUS_ADDR_RETURN,		     // ReturnValue, LastError

//
// Group 12: SRI activity events
//
    RTX_TE_SRI_NT_POST	= (RTX_EVENT_ID_BASE << 12),	// funcation pointer
	RTX_TE_SRI_RTSS_POST,							 // Funcation address
	RTX_TE_SRI_RTSS_PROXY_PRI,						 // Funcation address
	RTX_TE_SRI_RTSS_SERVER_1,						 // Funcation address
    RTX_TE_SRI_RTSS_SERVER_2,						 // Funcation address

//
// Group 13: Significant deterministic RTX API call events
//
    RTX_TE_ENTER_CRIT_SEC = (RTX_EVENT_ID_BASE << 13),		 // lpCriticalSection
    RTX_TE_ENTER_CRIT_SEC_RETURN,					 // ReturnValue, LastError
	RTX_TE_LEAVE_CRIT_SEC,							 // lpCriticalSection
	RTX_TE_LEAVE_CRIT_SEC_RETURN,					 // ReturnValue, LastError
 	RTX_TE_RESUME_THREAD,						     // hThread
	RTX_TE_RESUME_THREAD_RETURN,					 // ReturnValue, LastError
	RTX_TE_RT_SLEEP,								 // LowTime, HighTime
    RTX_TE_RT_SLEEP_RETURN,						 	 // ReturnValue, LastError
	RTX_TE_SET_THREAD_PRIORITY,						 // hThread, Priority
	RTX_TE_SET_THREAD_PRIORITY_RETURN,				 // ReturnValue, LastError
	RTX_TE_SLEEP,									 // TimeMS
    RTX_TE_SLEEP_RETURN,							 // ReturnValue, LastError	
	RTX_TE_SUSPEND_THREAD,							 // hThread
    RTX_TE_SUSPEND_THREAD_RETURN,					 // ReturnValue, LastError
	RTX_TE_WAIT_FOR_SINGLE_OBJECT,					 // Handle, Timeout
	RTX_TE_WAIT_FOR_SINGLE_OBJECT_RETURN,			 // ReturnValue, LastError
	RTX_TE_WAIT_FOR_MULTI_OBJECTS,					 // Number of objects, Timeout
	RTX_TE_WAIT_FOR_MULTI_OBJECTS_RETURN,			 // ReturnValue, LastError
	RTX_TE_GET_PROCESSAFF_MASK,	    				 // Get Process ID, Process Affinity Bit Mask
	RTX_TE_SET_PROCESSAFF_MASK,		            	 // Set Process ID, Process Affinity Bit Mask
	RTX_TE_SET_THREADAFF_MASK,			    		 // Set Thread  ID, Thread Affinity Bit Mask
	RTX_TE_SET_THREAD_IDEAL_PROC,		             // Set Thread ID, Ideal Processor
	RTX_TE_GET_THREAD_ID,							 // Get thread ID
	RTX_TE_GET_THREAD_EXEC_TIME,					 // Get Thread ID, Exec time
	RTX_TE_GET_THREAD_CREATE_TIME,					 // Get Thread ID, Create time
	RTX_TE_GET_THREAD_EXIT_TIME,					 // Get Thread ID, Exit time

//
// Group 14: Significant non-deterministic RTX API call events
//
    RTX_TE_ND_ALLOC_CONTIG_MEM = (RTX_EVENT_ID_BASE << 14), // Size
    RTX_TE_ND_ALLOC_CONTIG_MEM_RETURN,				 // ReturnValue, LastError
	RTX_TE_ND_ALLOC_LOCKED_MEM,						 // Size
	RTX_TE_ND_ALLOC_LOCKED_MEM_RETURN,				 // ReturnValue, LastError
	RTX_TE_ND_ATTACH_INTERRUPT,						 // ApplicationFunction, Priority
	RTX_TE_ND_ATTACH_INTERRUPT_RETURN,				 // ReturnValue, LastError
	RTX_TE_ND_ATTACH_SHUTDOWN,						 // ApplicationFunction, Priority
	RTX_TE_ND_ATTACH_SHUTDOWN_RETURN,				 // ReturnValue, LastError
    RTX_TE_ND_CLOSE_HANDLE,							 // Handle
    RTX_TE_ND_CLOSE_HANDLE_RETURN,					 // ReturnValue, LastError	
	RTX_TE_ND_CREATE_THREAD,						 // ApplicationFunction,
    RTX_TE_ND_CREATE_THREAD_RETURN,					 // ReturnValue, LastError 
	RTX_TE_ND_DELETE_CRIT_SEC,						 // lpSectionSection
    RTX_TE_ND_DELETE_CRIT_SEC_RETURN,				 // ReturnValue, LastError
	RTX_TE_ND_DEVICE_IOCONTROL,						 // Handle
	RTX_TE_ND_DEVICE_IOCONTROL_RETURN,				 // ReturnValue, LastError    
	RTX_TE_ND_EXIT_PROCESS,							 // ProcessID, ExitValue
    RTX_TE_ND_EXIT_PROCESS_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_EXIT_THREAD,							 // ExitValue
	RTX_TE_ND_EXIT_THREAD_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_FREE_CONTIG_MEM,						 // MemAddress, Size
    RTX_TE_ND_FREE_CONTIG_MEM_RETURN,				 // ReturnValue, LastError
	RTX_TE_ND_FREE_LOCKED_MEM,						 // MemAddress, Size
    RTX_TE_ND_FREE_LOCKED_MEM_RETURN,				 // ReturnValue, LastError
	RTX_TE_ND_HEAP_ALLOC,							 // Size
    RTX_TE_ND_HEAP_ALLOC_RETURN,    				 // ReturnValue, LastError
	RTX_TE_ND_HEAP_CREATE,							 // Initial Size
    RTX_TE_ND_HEAP_CREATE_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_HEAP_DESTROY,							 // Handle
    RTX_TE_ND_HEAP_DESTROY_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_HEAP_FREE,							 // MemAddress
    RTX_TE_ND_HEAP_FREE_RETURN,						 // ReturnValue, LastError
	RTX_TE_ND_HEAP_REALLOC,							 // Size
    RTX_TE_ND_HEAP_REALLOC_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_INIT_CRIT_SEC,						 // lpSectionSection
    RTX_TE_ND_INIT_CRIT_SEC_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_MAP_MEMORY,							 // LowPhysicalAddress, Length
    RTX_TE_ND_MAP_MEMORY_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_RT_PRINTF,							 // None
	RTX_TE_ND_RT_PRINTF_RETURN,						 // ReturnValue, LastError   
    RTX_TE_ND_TERMINATE_THREAD,						 // hThread, ExitCode 
	RTX_TE_ND_TERMINATE_THREAD_RETURN,				 // ReturnValue, LastError
	RTX_TE_ND_UNMAP_MEMORY,							 // VirtualAddress
	RTX_TE_ND_UNMAP_MEMORY_RETURN,					 // ReturnValue, LastError

//
// Group 15: File events
//
    RTX_TE_ND_CREATE_FILE = (RTX_EVENT_ID_BASE << 15),		 // None
    RTX_TE_ND_CREATE_FILE_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_DELETE_FILE,							 // None
    RTX_TE_ND_DELETE_FILE_RETURN,					 // ReturnValue, LastError
	RTX_TE_ND_READ_FILE,							 // Handle
    RTX_TE_ND_READ_FILE_RETURN,						 // ReturnValue, LastError
	RTX_TE_ND_REMOVE_DIR,							 // None
	RTX_TE_ND_REMOVE_DIR_RETURN,					 // ReturnValue, LastError
    RTX_TE_ND_SET_FILE_POINTER,						 // Handle
	RTX_TE_ND_SET_FILE_POINTER_RETURN,			  	 // ReturnValue, LastError
    RTX_TE_ND_WRITE_FILE,							 // Handle
	RTX_TE_ND_WRITE_FILE_RETURN,					 // ReturnValue, LastError

//
// Group 16: SRI activity events, internal
//
    RTX_TE_SRI_NT_ISR = (RTX_EVENT_ID_BASE << 16),	 // Funcation address							 // None
    RTX_TE_SRI_NT_DPC,								 // Funcation address
	RTX_TE_SRI_NT_SERVER,							 // Funcation address
    RTX_TE_SRI_RTSS_PROXY_SEC,						 // Funcation address
	RTX_TE_INTERRUPT_SRI,			   			     // Vector



//
// Groups 17 to 21: Reserved for future use
//

//
// Groups 22: User events 
//   
    RTX_TE_USER_EVENT	= (RTX_EVENT_ID_BASE << 22),	 // upto 1024 events


} RtxEventId;
    

#endif // __RTX_EVENT_ID_H__

#ifdef __cplusplus 
}
#endif
