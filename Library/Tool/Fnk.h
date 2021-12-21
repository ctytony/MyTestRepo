//fnk.h v1.0.T4

#pragma once

#include "ftd2xx.h"

class CFnk
{
public:
	CFnk();
public:		//function
	void Init();
	DWORD GetFreq();
	double GetImp();
	double GetPower();
	void GetPower(double *pdPower, double *pdVoltage, double *pdCurrent, int *pnPhase);
	void OutPower(int nPower, int nDuration);
	void SetConstMode(BYTE nMode);	//v1.0.T3 add
	void SetScaleInput(int nInput);	//v1.0.T3 add
private:	//variable
	FT_HANDLE m_hFT;
	BYTE m_acRxBuf[50];
private:	//function
	DWORD CFnk::GetRxBytes();
	void ReadData(CString sMsg = _T(""));
	void WriteData(BYTE *pcDataBuf, bool bPurgeRxBuf = true);
	DWORD HextoDec(const BYTE *pcHex, int nLength);
	void DectoHex(int nDec, BYTE *pcHex, int nLength);
	BYTE CountBit(BYTE cValue);
	void SetStartStopFreq(int nStartFreq, int nStopFreq);
	void SetFreqImpPreset(int nFreqPreset, int nImpPreset);
	void SetPhasePreset(int nPhaseAngle, int nPhaseRange);
	void SetFreqRange(int nRange);
	void SetLockRange(int nRange);
};

extern CFnk theFnk;
