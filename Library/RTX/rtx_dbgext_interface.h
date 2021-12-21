/*
 *
 * Copyright (c) 1996-2010 IntervalZero, Inc.  All rights reserved.
 *
 * Module Name:
 *
 *	rtx_dbgext_interface.h
 *
 * Abstract:
 *
 * Defines RTX debugging data interface.  Debugger extensions or any other tool
 * subscribing for RTX debugging data will use the methods defined in this interface
 * to fetch the data.
 *
 * Environment:
 *
 *	Win32
 * 
--*/

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef RTXDBGEXT_EXPORTS
#define RTXDBGEXT_API __declspec(dllexport)
#else
#define RTXDBGEXT_API __declspec(dllimport)
#endif

#include "rtx_dbgext_data.h"
#include <dbgeng.h>

__interface IRtxDbgDataExt
{
	ULONG AddRef();
	ULONG Release();

	HRESULT RTDDEGetProcessList(IN IDebugClient *DebugClient, OUT PULONG *ProcAddr, IN OUT PULONG Size);
	HRESULT RTDDEGetProcessInfo(IN IDebugClient *DebugClient, IN PULONG ProcAddr, OUT PVOID *ProcInfo, IN ULONG Size);
	HRESULT RTDDEGetCurrentProcess(IN IDebugClient *DebugClient, IN ULONG nRtssProcessors, IN ULONG RtssP0Num, OUT PRTDDE_PROC_SUMMARY_INFO CurrentProc);
	HRESULT RTDDEGetCurrentThread(IN IDebugClient *DebugClient, IN ULONG nRtssProcessors, IN ULONG RtssP0Num, OUT PRTDDE_THREAD_SUMMARY_INFO ActiveThread);
	HRESULT RTDDEGetProcessAddrFromID(IN IDebugClient *DebugClient, IN UINT ProcId, PULONG ProcAddr);
	HRESULT RTDDEGetObjectList(IN IDebugClient *DebugClient, OUT PULONG *ObjAddr, IN OUT PULONG Size);
	HRESULT RTDDEGetThreadList(IN IDebugClient *DebugClient, IN PULONG ThreadListAddr, OUT PULONG *ThreadList, IN OUT PULONG Size);
	HRESULT RTDDEGetHandleList(IN IDebugClient *DebugClient, IN PULONG HandleListAddr, OUT PULONG *HandleList,  IN OUT PULONG Size);
		
	HRESULT RTDDEGetThreadInfo(IN IDebugClient *DebugClient, IN PULONG ThreadAddr, OUT PVOID *ThreadInfo, IN ULONG Size);
	HRESULT RTDDEGetMemoryInfo(IN IDebugClient *DebugClient, IN PULONG MemList, OUT PRTDDE_MEMORY_INFO *MemInfo, IN OUT PULONG Size);
	HRESULT RTDDEGetHandleInfo(IN IDebugClient *DebugClient, IN ULONG Handle, OUT PVOID *HandleInfo, IN ULONG Size);
	HRESULT RTDDEGetObjectInfo(IN IDebugClient *DebugClient, IN PULONG ObjAddr, OUT PVOID *ObjectInfo, IN ULONG Size);
	HRESULT RTDDEGetSubsystemInfo(IN IDebugClient *DebugClient, PRTDDE_SUBSYSTEM_INFO SubsystemInfo);
	HRESULT RTDDEGetRtxVersionInfo(IN IDebugClient *DebugClient, PULONG MajorVersion, PULONG MinorVersion, PULONG FixNo);
	HRESULT RTDDEGetRtxMinidumpInfo(IN IDebugClient *DebugClient, OUT PBYTE MiniDumpBuffer, IN OUT PULONG MiniDumpBufferSize);
};

extern "C" RTXDBGEXT_API HRESULT __stdcall InitializeRTXDbgDataExt(IN IDebugClient *DebugClient, IN bool bUseMsDbgeng, OUT IRtxDbgDataExt **pRtxDbgExt);

#ifdef __cplusplus 
}
#endif
