

#ifndef  __MTN_AFT_H__
#define  __MTN_AFT_H__

#define MAX_BYTE_NAME_STRING      128
typedef struct
{
	double dDistLimitToLimit_mm;
	double dDistUppLimitToIndex_mm;
	double dDistLowLimitToIndex_mm;
	double dDistIndexUppLow_mm;
	int iDriverPolarity;
	
	double dExpectMaxAccPosDir_m_s;
	double dExpMaxAccMeanPosDir;
	double dExpMaxAccStdPosDir;

	double dExpectMaxAccNegDir_m_s;
	double dExpMaxAccMeanNegDir;
	double dExpMaxAccStdNegDir;

}AFT_OUTPUT_AXIS;

typedef struct
{
	double dDistLimitToLimit_cnt;
	double dDistUppLimitToIndex_cnt;
	double dDistLowLimitToIndex_cnt;
	double dDistIndexUppLow_cnt;
}AFT_CHECKING_AXIS;

// 0: pass
// 1: fail, > upp limit
// -1: fail, < low limit
typedef struct
{
	int iFlagFailLimitToLimit;
	int iFlagFailUppLimitToIndex;
	int iFlagFailLowLimitToIndex;
	int iFlagFailIndexUppLow;

	int iFlagExpMaxAccNegDir;
	int iFlagExpMaxAccPosDir;
}AFT_RESULT_AXIS;

typedef struct
{
	char strAxisName_cn[MAX_BYTE_NAME_STRING];
	char strAxisName_en[MAX_BYTE_NAME_STRING];
	double dDistLimitToLimit_mm_U;
	double dDistLimitToLimit_mm_L;
	double dDistUppLimitToIndex_mm_U;
	double dDistUppLimitToIndex_mm_L;
	double dDistLowLimitToIndex_mm_U;
	double dDistLowLimitToIndex_mm_L;
	double dDistIndexUppLow_mm_U;
	double dDistIndexUppLow_mm_L;

	double dExpectMaxAccPosDir_m_s_U;
	double dExpectMaxAccPosDir_m_s_L;

	double dExpectMaxAccNegDir_m_s_U;
	double dExpectMaxAccNegDir_m_s_L;

}AFT_SPEC_AXIS;

extern void aft_init_spec();
void aft_axis_calc_output_from_checking(int iAxisWb, AFT_CHECKING_AXIS *stpAFT_AxisChecking);
void aft_axis_verify_output_by_spec(int iAxisWb);

void mtn_aft_bakup_speed_profile(HANDLE hHandleACS);
void mtn_aft_restore_speed_profile(HANDLE hHandleACS);
void mtn_aft_init_random_burn_in_condition(HANDLE hHandleACS);

#include "AftVelLoopTest.h"
void aft_ana_vel_step_group_test(AFT_VEL_LOOP_TEST_INPUT *stpVelLoopTestInput, AFT_VEL_LOOP_TEST_OUTPUT *stpVelLoopTestOutput, unsigned int nTotalCase);

#endif   // __MTN_AFT_H__