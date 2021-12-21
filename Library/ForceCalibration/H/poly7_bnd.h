
#pragma once


#define POLY_7(k0, k1, k2, k3, k4, k5, k6, k7, x) (k0 + x*(k1 + x*(k2 + x*(k3 + x*(k4 + x* (k5 + x* (k6 + x * k7)))))))

// 20081013
typedef struct
{
	unsigned int TimeT_ini;
	double dDist;   
	double dVelIni; 
	double dVelEnd; 
	double dAccIni; 
	double dAccEnd; 
	double dJerkIni;
	double dJerkEnd;
}POLY7_CALC_K_BY_TIME_INPUT;

typedef struct
{
	unsigned int TimeT_ini;
	double dDist;   
	double dVelIni; 
	double dVelEnd; 
	double dAccIni; 
	double dAccEnd; 
	double dJerkIni;
	double dJerkEnd;
	double dK4;
	double dK5;
	double dK6;
	double dK7;
}POLY7_CALC_K_BY_TIME_OUTPUT;

#define NUM_ROW_MATRIX  4
#define NUM_COL_MATRIX  4
typedef struct
{
	double aMatrix[NUM_ROW_MATRIX * NUM_COL_MATRIX];
	double aInvMatrix[NUM_ROW_MATRIX * NUM_COL_MATRIX];
	double aRHS[NUM_ROW_MATRIX];
	double aSolution[NUM_ROW_MATRIX];
}POLY7_CALC_K_BY_TIME_DEBUG;

int poly7_calc_k_by_time(POLY7_CALC_K_BY_TIME_INPUT *stpInput, POLY7_CALC_K_BY_TIME_OUTPUT *stpOutput, POLY7_CALC_K_BY_TIME_DEBUG *stpDebug);

typedef struct
{
	unsigned int TimeT_ini;
	double dDist;   
	double dVelIni; 
	double dVelEnd; 
	double dAccIni; 
	double dAccEnd; 
	double dJerkIni;
	double dJerkEnd;
	double dK4;
	double dK5;
	double dK6;
	double dK7;
}POLY7_GEN_PROF_INPUT;

typedef struct
{
	double *pdDistProf;
	double *pdVelProf;
	double *pdAccProf;
	double *pdJerkProf;
}POLY7_GEN_PROF_OUTPUT;
void poly7_generate_prof(POLY7_GEN_PROF_INPUT *stpInput, POLY7_GEN_PROF_OUTPUT *stpOutput);
void poly7_generate_vel_prof(POLY7_GEN_PROF_INPUT *stpInput, POLY7_GEN_PROF_OUTPUT *stpOutput);
unsigned int get_time_prof7_verify_looping_vel_not_change_sign(POLY7_CALC_K_BY_TIME_INPUT *stpInput, unsigned int uiTimeT_min, unsigned int uiTimeT_max);

