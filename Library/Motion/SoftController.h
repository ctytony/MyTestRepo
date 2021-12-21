//SoftController.h v1.0.T119

#pragma once

#include "..\Library\ACS\ACS_SC\acsc.h"
#include "Motion.h"

class CSoftController
{
public:
	CSoftController(void);
	~CSoftController(void);
public:	//functions
	void DownloadEric2Buffer0(void);	//v1.0.T30 add
	void DownloadEric2Buffer1(void);	//v1.0.T30 add
	void DownloadEric2Buffer9(void);	//v1.0.T30 add
	void DownloadNewBuffer0(void);		//v1.0.T17 add
	void DownloadNewBuffer1(void);		//v1.0.T17 add
	void DownloadNewBuffer2(void);		//v1.0.T17 add
	void DownloadNewBuffer7(void);		//v1.0.T17 add
	void DownloadNewBuffer8(void);		//v1.0.T17 add
	void DownloadNewBuffer9(void);		//v1.0.T17 add
	int Reboot(void);					//v1.0.T8 add
	int RebootFactoryDefault(void);		//v1.0.T8 add
	void ResetCtactDetect(void);		//v1.0.T7 add
	void SetMotorZJerkFFC(int iGain, int iFactor);	//v1.0.T7 add
	void SetPosCompFactor(double dPosCompKp, double dPosCompOs);	//v1.0.T7 add
	HANDLE GetCommHandle(void);			//v1.0.T6 add
	int DownloadBufferToServo(short nBuffer, char* pcProgram, short nSize);	//v1.0.T5 add
	void DownloadBuffer0(void);		//v1.0.T16 edit
	void DownloadBuffer1(void);		//v1.0.T16 edit
	void DownloadBuffer2(void);		//v1.0.T16 edit
	void DownloadWclBuffer2(void);	//v1.0.L35 add
	void DownloadBuffer3(void);		//v1.0.T16 edit
	void DownloadBuffer4(void);		//v1.0.T16 edit
	int Init(void);
	int ReadInteger(EN_MEMORY_VAR nMemVar, int *pnValue);
	int WriteInteger(EN_MEMORY_VAR nMemVar, int nValue);
	int ReadReal(EN_MEMORY_VAR nMemVar, double *pdValue);
	int WriteReal(EN_MEMORY_VAR nMemVar, double dValue);
    int EnableMotor(int iMotor1 = -1, int iMotor2 = -1, int iMotor3 = -1, int iMotor4 = -1,
					int iMotor5 = -1, int iMotor6 = -1, int iMotor7 = -1, int iMotor8 = -1);
	int GetMotorAxis(short nMotor);
private:	//variables
	short m_nAcsTimeout;			//v1.0.T5 add
	double m_dWaveformSampleTime;	//v1.0.T5 add
	double m_dServoSampleTime;		//v1.0.T5 add
	HANDLE m_hComm;
	int m_anMemoryAddr[20];
private:	//functions
	void GetVarName(int nVarID, char* pcVarName);
	void UploadAllMemoryAddr();
public:
	int GetMemoryAddr(EN_MEMORY_VAR nMemVar);
};

extern CSoftController theSoftController;