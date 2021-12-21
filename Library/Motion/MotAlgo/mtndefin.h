

#ifndef __MTN_DEFINE__
#define __MTN_DEFINE__

//#ifdef __cplusplus      /* For the mentally challenged */
//extern "C" {
//#endif


typedef unsigned char 	MOT_ALGO_BYTE;
typedef unsigned char   MOT_ALGO_UCHAR;
typedef unsigned short 	MOT_ALGO_WORD;
typedef unsigned long 	MOT_ALGO_DWORD;
typedef unsigned int    MOT_ALGO_UINT;
typedef unsigned long   MOT_ALGO_ULONG;
typedef unsigned short  MOT_ALGO_USHORT;

typedef signed short    MOT_ALGO_BOOL;
typedef signed char     MOT_ALGO_SBYTE;
typedef signed short    MOT_ALGO_SHORT;
typedef signed int      MOT_ALGO_INT;
typedef signed long     MOT_ALGO_LONG;
typedef char            MOT_ALGO_CHAR;
typedef float           MOT_ALGO_FLOAT;
typedef double          MOT_ALGO_DBL;

// For module mtncalc
#define MTN_CALC_SUCCESS        0
#define MTN_CALC_ERROR          1

// for mtn aft and mtn_api
#define MTN_API_OK_ZERO 0
#define MTN_API_ERROR_UPLOAD_DATA   -1
#define MTN_API_ERROR_OPEN_FILE		-2
#define MTN_API_ERROR_READ_FILE     -3
#define MTN_API_ERROR_COUNT_NULL_STRING -4
#define MTN_API_ERROR_MEMORY		-5
#define MTN_API_ERR_FILE_PTR            -6
#define MTN_API_ERR_RENAME_FILE		-7
#define MTN_API_ERR_MODIFY_STRING   -8
#define MTN_API_ERR_INSERT_STRING   -9

#define MTN_API_ERROR				1   // General error, check corresponding strText for error message
#define MTN_API_ERROR_NOT_SUPPORT_VENDOR 2

// For all the feature name, maximum string length
// SERVO_AXIS_BLK: <strApplicationNameAxis, strAxisNameAcs>
// SERVO_ACS: strAxisNameAcs
// All debug strings, in mtnconfig.cpp, 
//                    in mtnconfig.h, 
#define MTN_API_MAX_STRLEN_FEATURENAME   128
// locally used in MtnConfig.cpp
#define OPENERR         1
#define OPENOK          0

#define READERR 		2
#define READOK          0

#define __MAX_FILE_NAME__  128
#define CFG_MAX_NUM_BLOCKS 128

//#ifdef __cplusplus
//}
//#endif

 #endif //__MTN_DEFINE__