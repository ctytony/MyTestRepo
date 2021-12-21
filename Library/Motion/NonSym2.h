///NonSym2.h v1.0.T11

// History
// YYYYMMDD Author      Notes
// 20081205 JohnZHAO    Release micro definition

#pragma once

#define PI							3.14159265358979
#define NON_SYM_LOCAL_VAR_LENGTH	1024			// 20081021
#define NON_SYM_CURVE_CASE_NUM		1024 * 5		// 20081021

#define NON_SYM_RULE_WEIGHTED_RMS_ERR		0
#define NON_SYM_RULE_RMS_ERR				1
#define NON_SYM_RULE_WEIGHTED_INV_RMS_ERR	2

#define NON_SYM_CURVE_LOOP_ERR_INV_DIST_VEND	1
#define NON_SYM_CURVE_LOOP_ERR_T3_LOWER_BOUND	2
#define NON_SYM_CURVE_DEBUG_FILE  "NonSymCycloidalFitCurveDebug.txt"

struct NON_SYM_PROFILE_INPUT
{
	unsigned int uiT1;
	unsigned int uiT2;
	unsigned int uiT3;
	double dActualMaxVel;
	double dActualMaxAccT1;
	double dActualMaxAccT3;
	double dDist;
	double dVend;
	double dVelFactorT1;
	double dJerkFactorT1;
	double dDistFactorT1;
	double dOmegaFactorT1;
	double dDistFactorT3;
	double dVelFactorT3;
	double dJerkFactorT3;
	double dOmegaFactorT3;
	int	iPlotFlag;
};

struct NON_SYM_PROFILE_OUTPUT
{
	double *pdDistProf;
	double *pdVelProf;
	double *pdAccProf;
	double *pdJerkProf;
};

struct NON_SYM_CURVE_INPUT
{
	double dDist;
	double dMaxVel;
	double dMaxAcc;
	double dVend;
	double *pdTargetCurve;
	unsigned int nTargetLen;
	unsigned int nFitLen;
	int iFlagObjRule;
	int iPlotFlag;			//if >= 3, generate NON_SYM_CURVE_DEBUG_FILE for debugging
}; 

struct NON_SYM_CURVE_OUTPUT
{
	double dObjValue;
	NON_SYM_PROFILE_INPUT stNonSymProfInput;
};

struct NON_SYM_CURVE_CASE
{
	NON_SYM_PROFILE_INPUT stNonSymProfInput;
	double dObjValueCases;
};

extern int CalcNonSymCurve(NON_SYM_CURVE_INPUT *pstInput, NON_SYM_CURVE_OUTPUT *pstOutput);
extern int CalcNonSymProfile(NON_SYM_PROFILE_INPUT *pstInput, NON_SYM_PROFILE_OUTPUT *pstOutput);
