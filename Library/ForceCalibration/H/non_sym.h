// History
// YYYYMMDD Author      Notes
// 20081205 JohnZHAO    Release micro definition

#pragma once

#define PI   (3.14159265358979)
#define NON_SYM_LOCAL_VAR_LENGTH   1024                // 20081021
#define NON_SYM_FITTING_DEF_CASES  (1024 * 5)          // 20081021

typedef struct
{
	unsigned int uiT1;
	unsigned int uiT2;
	unsigned int uiT3;
	double dActualMaxVel;
	double dActualMaxAccT1;
	double dActualMaxAccT3;
	double dDist;
	double dVend;
	double dVelFactor1_T1;
	double dJerkFactor_T1;
	double dDistFactor_T1;
	double dOmegaFactor_T1;
	double dDistFactor_T3;
	double dVelFactor1_T3;
	double dJerkFactor_T3;
	double dOmegaFactor_T3;
	int	iPlotFlag;
}NON_SYM_GEN_PROF_INPUT;

typedef struct
{
	double *pdDistProf;
	double *pdVelProf;
	double *pdAccProf;
	double *pdJerkProf;
}NON_SYM_GEN_PROF_OUTPUT;
int non_sym_gen_prof(NON_SYM_GEN_PROF_INPUT *stpInput, NON_SYM_GEN_PROF_OUTPUT *stpOutput);

#define NON_SYM_FIT_RULE_WEIGHTED_RMS_ERR      0
#define NON_SYM_FIT_RULE_RMS_ERR               1
#define NON_SYM_FIT_RULE_WEIGHTED_INV_RMS_ERR  2

#define NON_SYM_FIT_CURVE_LOOPING_ERROR_INVERSE_DIST_VEND  1
#define NON_SYM_FIT_CURVE_LOOPING_ERROR_T3_LOWER_BOUND     2
#define NON_SYM_CYCLOIDAL_FIT_CURVE_DEBUG_FILE  "NonSymCycloidalFitCurveDebug.txt"
typedef struct
{
double dDist        ;
double dMaxVel      ;
double dMaxAcc      ;
double dVend        ;
double *pdTargetCurve ;
unsigned int nTargetLen;
unsigned int nFitLen;
int iFlagObjRule;
int iPlotFlag;   // if >= 3 , generate file NON_SYM_CYCLOIDAL_FIT_CURVE_DEBUG_FILE, for debugging
}NON_SYM_CYCLOIDAL_CALC_T_FIT_CURVE_INPUT; 

typedef struct
{
double dObjValue;
NON_SYM_GEN_PROF_INPUT stNonSymGenProfIn;
}NON_SYM_CYCLOIDAL_CALC_T_FIT_CURVE_OUTPUT;

typedef struct
{
	NON_SYM_GEN_PROF_INPUT stNonSymGenProfInputCases;
	double pdObjValueCases;
}NON_SYM_CYCLOIDAL_CALC_T_FIT_CURVE_CASE;

extern int non_sym_cycloidal_calc_t_fit_curve(NON_SYM_CYCLOIDAL_CALC_T_FIT_CURVE_INPUT * stpInput, 
									   NON_SYM_CYCLOIDAL_CALC_T_FIT_CURVE_OUTPUT *stpOutput);
