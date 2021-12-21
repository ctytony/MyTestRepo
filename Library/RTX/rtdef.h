///////////////////////////////////////////////////////////////////////////////
//
//
//     Copyright (c) 1996-2012 IntervalZero, Inc.  All rights reserved.
//
//		rtdef.h -- typedefs and other preprocessor definitions common to the subsystem, RTX
//			aware win32 layer and third party extension headers (e.g., third party debuggers,
//			third party and our debugger data extension etc).
//
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __RTDEF__
#define __RTDEF__
#ifdef __cplusplus
extern "C"
{
#endif

//
// Max path length used for names by RtCreate, RtOpenSharedMemory etc.
//
#define RTX_PATH_MAX	(128+1)		// N.B.: MUST match RTX_MAX_OBJECT_NAME_LENGTH
									// in rtapi.h + 1 for trailing NULL!!

#define FlagOn(X, Y)		(((X) & (Y)) != 0)
#define AllFlagsOn(X, Y)	(((X) & (Y)) == (Y))


#endif /*__RTDEF__*/
#ifdef __cplusplus 
}
#endif