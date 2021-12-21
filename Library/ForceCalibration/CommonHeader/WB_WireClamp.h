

#ifndef __WB_WIRE_CLAMP__
#define __WB_WIRE_CLAMP__


typedef struct
{
	double dVoltageCmd_Volt;
	double dWireClampingForce_Gram;
	double dDOUT_ACS;
	double dDrvCOM_ACS;
}WB_WIRE_CLAMP_CALIBRATION_POINT;

#define MAX_NUM_POINT_CALIBRATION  10
typedef struct
{
WB_WIRE_CLAMP_CALIBRATION_POINT  astCalibrationPoints[MAX_NUM_POINT_CALIBRATION];

}WB_WIRE_CLAMP_CALIBRATION;

#endif // __WB_WIRE_CLAMP__