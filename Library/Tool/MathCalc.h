//MathCalc.h v1.0.T18sc

#pragma once

#include "DataStruct.h"

#define _SERVO 1	//v1.0.T14 add
#if _SERVO == 1
	#define WCL_CALIB_CURRENT1			-0.7
	#define WCL_CALIB_CURRENT2			-2.2
#else
	#define WCL_CALIB_CURRENT1			-2.8
	#define WCL_CALIB_CURRENT2			-8.75
#endif

#define BH_FORCE_KF_LOW_LMT			0.01
#define BH_FORCE_KF_UP_LMT			0.2
//v1.0.T9 delete
//#define BH_FORCE_KA_LOW_LMT			-0.002
//#define BH_FORCE_KA_UP_LMT			0.002
#define BH_FORCE_IO_LOW_LMT			-3
#define BH_FORCE_IO_UP_LMT			3

#define WCL_FACTOR_KF_LOW_LMT		0.01
#define WCL_FACTOR_KF_UP_LMT		0.4
#define WCL_FACTOR_IO_LOW_LMT		-10.0
#define WCL_FACTOR_IO_UP_LMT		15.0

//Round double or float to integer
extern int round(double x);
extern int round(float x);
//Calculate distance between two points
extern double Calc2PtDist(TBL_POS stPos1, TBL_POS stPos2);	//v1.0.T7 add
//Calculate position with radius from 1st point along a wire from 1st point to 2nd point
extern TBL_POS CalcPosAlongWire(TBL_POS stPos1, TBL_POS stPos2, double dRadius, short nAngle = 0);	//v1.0.T17 add, v1.0.T18 rename
extern TBL_POS CalcDistAlongWire(TBL_POS stPos1, TBL_POS stPos2, double dRadius, short nAngle = 0);	//v1.0.T17 add
//Round the position to integer.
extern void RoundPosition(TBL_POS* pstPoint);
extern void CurveFit(short nTotal, double *pdX, double *pdY, double *pdK, double *pdB);
extern double CutTail(double dTarget);
//v1.0.T2 add
extern int Calc3rdOrderMotionProfile(double dJerk, double dMaxAcc, double dMaxVel, double dDist, short* pnMoveTime, double* padMotionProfile);
extern int Calc3rdOrderPartialMotionTime(double dJerk, double dMaxAcc, double dMaxVel, double dDist, double dPartDist, short* pnMoveTime);
//v1.0.T3 add
extern short CalcForceCalibFactor(double dForceKa, long lCtactLvl, double* pdOutputDac, double* pdMeasureForce);
extern short CalcForceCalibFactor2(double dForceKa, long lCtactLvl, double* pdOutputDac, double* pdMeasureForce);
extern int CalcWclCalibFactor(double dMeasureForce1, double dMeasureForce2, double* pdKf, double* pdIo);

extern short GetTableAutoSpeedProfile(long lMoveDist_um);	//v1.0.T4 add
extern short Calc3rdOrderMotionTime(double dJerk, double dMaxAcc, double dMaxVel, double dDist);	//v1.0.T11 add
