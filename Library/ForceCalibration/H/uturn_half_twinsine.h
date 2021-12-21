
#ifndef __U_TURN_HALF_TWIN_SINE__
#define __U_TURN_HALF_TWIN_SINE__

// U-turn half twin sine module
// 
// It meets strict condition s.t. Vend = -Vini, Dist = 0, and Dist overshoot is in the same direction of Vini;


typedef struct
{
double dDist_end;
double dVini;
double dVend;
double dAmax;
double dJmax;
}U_TURN_HALF_TWIN_SINE_PRECAL_INPUT;

typedef struct
{
double T_total;
double T_acc;
double T_dec;
double T_cnst;
double ActualJerkMax;
double ActualAccMax;
double ActualVelMax;
double ActualJerkDecMax;
double ActualAccDecMax;
double ActAcc;
double AdjVmax;
double ActVmax;
double Dramp;
double Dconst;
double AbsDist;
double Dist;
double Vend;
double Vini;
}U_TURN_HALF_TWIN_SINE_PRECAL_OUTPUT;

int axis_u_turn_half_ts_precal(U_TURN_HALF_TWIN_SINE_PRECAL_INPUT *stpInput, U_TURN_HALF_TWIN_SINE_PRECAL_OUTPUT *stpOutput);

typedef U_TURN_HALF_TWIN_SINE_PRECAL_OUTPUT U_TURN_HALF_TWIN_SINE_PROFGEN_INPUT;

typedef struct
{
	double *pdDist;
	double *pdVel;
	double *pdAcc;
	double *pdJerk;
}U_TURN_HALF_TWIN_SINE_PROFGEN_OUTPUT;
int axis_u_turn_half_ts_profgen(U_TURN_HALF_TWIN_SINE_PROFGEN_INPUT *stpInput, U_TURN_HALF_TWIN_SINE_PROFGEN_OUTPUT *stpOutput);

#endif // __U_TURN_HALF_TWIN_SINE__