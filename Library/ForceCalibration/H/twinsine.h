
#ifndef __TWIN_SINE_H__
#define __TWIN_SINE_H__

#include "mtncalc.h"

//Structure Define for function prototype.
//Input
typedef struct
{
MOT_ALGO_DBL fDist;
MOT_ALGO_DBL fAccMax; // fAVmd_time;
MOT_ALGO_DBL fVelMax;
MOT_ALGO_DBL fJmax;
MOT_ALGO_DBL fT1;
MOT_ALGO_DBL fT2;
int   iTimeMoveFlag;

}CALC_TWINSINE_PRECAL_INPUT; 
//Structure for output
typedef struct
{
MOT_ALGO_DBL fT1;
MOT_ALGO_DBL fT2;
MOT_ALGO_DBL fC1;
MOT_ALGO_DBL fC2;
MOT_ALGO_DBL fC3;
MOT_ALGO_DBL fC4;
MOT_ALGO_DBL fC5;
MOT_ALGO_DBL fC2C4;
MOT_ALGO_DBL fC2C2C4;
MOT_ALGO_DBL fC5T1T1;
MOT_ALGO_DBL fDConstAtT1T2;
MOT_ALGO_DBL fAmax_real;
MOT_ALGO_DBL fVmax_real;
MOT_ALGO_DBL fJmax_real;
short sErrorCode;

}CALC_TWINSINE_PRECAL_OUTPUT; 

extern short twinsine_precal(
        CALC_TWINSINE_PRECAL_INPUT  *stpInput,
        CALC_TWINSINE_PRECAL_OUTPUT  *stpOutput);

//Structure Define for function prototype.
//Input
typedef struct
{
MOT_ALGO_DBL fT1;
MOT_ALGO_DBL fT2;
MOT_ALGO_DBL fC1;
MOT_ALGO_DBL fC2;
MOT_ALGO_DBL fC3;
MOT_ALGO_DBL fC4;
MOT_ALGO_DBL fC5;
MOT_ALGO_DBL fC2C4;
MOT_ALGO_DBL fC2C2C4;
MOT_ALGO_DBL fC5T1T1;
MOT_ALGO_DBL fDConstAtT1T2;
MOT_ALGO_DBL fAmax_real;
MOT_ALGO_DBL fVmax_real;
MOT_ALGO_DBL fJmax_real;
short sErrorCode;

}CALC_TWINSINE_PROFGEN_INPUT;

//Structure for output
typedef struct
{
MOT_ALGO_DBL *pfdist_prof;
MOT_ALGO_DBL *pfvel_prof;
MOT_ALGO_DBL *pfacc_prof;
MOT_ALGO_DBL *pfjerk_prof;

}CALC_TWINSINE_PROFGEN_OUTPUT; 

short twinsine_profgen(
        CALC_TWINSINE_PROFGEN_INPUT  *stpInput,
        CALC_TWINSINE_PROFGEN_OUTPUT  *stpOutput);


#endif // __TWIN_SINE_H__