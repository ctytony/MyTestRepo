// Copyright (c) 2015 IntervalZero, Inc.  All rights reserved.
//
// This file defines macros that represent custom Win32 error codes that RTX real-time APIs can
// return.  Custom error codes all have bit 29 set.  See:
//
//    https://msdn.microsoft.com/en-us/library/ms680627.aspx
//
// which says:
//
//    Bit 29 is reserved for application-defined error codes; no system error code has this bit set.
//    If you are defining an error code for your application, set this bit to indicate that the
//    error code has been defined by your application and to ensure that your error code does not
//    conflict with any system-defined error codes.
//
// RTX real-time APIs can still return normal Win32 error codes, such as ERROR_FILE_NOT_FOUND (2).
// These error codes extend the range of possible error codes to include codes specific to RTX.
//
// Policy: Use Windows errors where possible and please validate before creating additional
// non-windows errors.

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// Custom error codes must have bit 29 set.  Microsoft guarantees that no regular Win32 error code
// will ever have bin 29 set.  IMPORTANT: Do not change the name of this macro!
#define RT_CUSTOM_ERROR(VALUE)  ((VALUE) | (1 << 29))


// IMPORTANT: Do not change or move the comment on the next line.
// ERROR-CODES-START-HERE

// Description: No RTX runtime license is available. 
#define RT_ERROR_NO_RUNTIME_LICENSE                     RT_CUSTOM_ERROR(1)

// Description: The RTX runtime license has expired.
#define RT_ERROR_EXPIRED_RUNTIME_LICENSE                RT_CUSTOM_ERROR(2)

// Description: No valid license was found for the given product feature code.
#define RT_ERROR_NO_LICENSE                             RT_CUSTOM_ERROR(3)

// Description: No RTX SDK license was available when this application (or an RTDLL on which it
// depends) was built. You may be able to resolve this error by rebuilding the application.
#define RT_ERROR_MODULE_NO_SDK_LICENSE                  RT_CUSTOM_ERROR(4)

// Description: This application (or an RTDLL on which it depends) was built with an evaluation
// RTX SDK license. It cannot be executed when the RTX runtime license is a retail license. 
// You may be able to resolve this error by rebuilding the application.
#define RT_ERROR_MODULE_EVAL_SDK_LICENSE                RT_CUSTOM_ERROR(5)

// Description: This application (or an RTDLL on which it depends) is missing important RTX
// license information. You may be able to resolve this error by rebuilding the application.
#define RT_ERROR_MODULE_LICENSE_INFO_MISSING            RT_CUSTOM_ERROR(6)

// Description: This application (or an RTDLL on which it depends) contains corrupted license
// information. You may be able to resolve this error by rebuilding the application.
#define RT_ERROR_MODULE_LICENSE_INFO_CORRUPT            RT_CUSTOM_ERROR(7)

// Description: One or more object files in this application (or an RTDLL on which it depends) were
// built using an RTX SDK version that is incompatible with the current RTX runtime.
// You may be able to resolve this error by rebuilding the application.
#define RT_ERROR_MODULE_UNSUPPORTED_SDK                 RT_CUSTOM_ERROR(8)

// Description: License data is corrupt.
#define RT_ERROR_CORRUPT_LICENSE_DATA                   RT_CUSTOM_ERROR(9)

// Description: RTX Subsystem was unable to communicate with required services.
#define RT_ERROR_SERVICE_ACCESS_FAILURE                 RT_CUSTOM_ERROR(10)

// Description: Internal error 1.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_1                             RT_CUSTOM_ERROR(11)

// Description: Internal error 2.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_2                             RT_CUSTOM_ERROR(12)

// Description: Internal error 3.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_3                             RT_CUSTOM_ERROR(13)

// Description: Internal error 4.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_4                             RT_CUSTOM_ERROR(14)

// Description: Internal error 5.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_5                             RT_CUSTOM_ERROR(15)

// Description: Internal error 6.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_6                             RT_CUSTOM_ERROR(16)

// Description: Internal error 7.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_7                             RT_CUSTOM_ERROR(17)

// Description: Internal error 8.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_8                             RT_CUSTOM_ERROR(18)

// Description: Internal error 9.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_9                             RT_CUSTOM_ERROR(19)

// Description: Internal error 10.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_10                            RT_CUSTOM_ERROR(20)

// Description: Internal error 11.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_11                            RT_CUSTOM_ERROR(21)

// Description: Internal error 12.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_12                            RT_CUSTOM_ERROR(22)

// Description: Internal error 13.  Please contact IntervalZero support for more information. 
#define RT_ERROR_INTERNAL_13                            RT_CUSTOM_ERROR(23)

#ifdef __cplusplus 
}
#endif
