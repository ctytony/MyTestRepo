//UsgControl.h v1.0.T28

#pragma once

#include "..\Library\ThreadSerial\ThreadSerial.h"

// CUsgControl command target

struct ST_USG_INFO	//v1.0.T21 rename
{
	short nTailTime;
	short nTailPower;
	short nBondingSitTime;
	short nBondingSitPower;
	short nFeedTime;
	short nFeedPower;
};

struct ST_DYNAMIC_USG_INFO	//v1.0.T21 rename
{
	// Standby power
	short nStandbyTime[2];
	short nStandbyPower[2];

	// 1st bond contact boost power
	short nCtactBoostTime1;
	short nCtactBoostPower1;

	// Contact power
	short nCtactRampTime[2];
	short nCtactTime[2];
	short nCtactPower[2];

	// 2nd bond scrub offset time;
	short nScrubOffsetTime2;

	// Scrub power
	short nScrubTime[2];
	short nScrubPower[2];

	// Power delay
	short nPowerDelay[2];

	// 1st bond base boost power
	short nBaseBoostTime1;
	short nBaseBoostPower1;

	// Base power
	short nBaseRampTime[2];
	short nBaseTime[2];
	short nBasePower[2];

	// Release power
	short nRlsRampTime[2];
	short nReleaseTime[2];
	short nReleasePower[2];
};

struct ST_USG_VERTEX
{
	WORD nTime;		//v1.0.T22 rename	//LSB
	BYTE cLevel;	//v1.0.T22 rename	//Command format: 0xTimeLevel 16bits; 0xPwrLevel 8 bit
};

struct ST_USG_PROFILE	//v1.0.T12 rename
{
	BYTE cIndex;		//v1.0.T22 rename
	ST_USG_VERTEX stVertex1;	//v1.0.T12 rename
	ST_USG_VERTEX stVertex2;	//v1.0.T12 rename
	ST_USG_VERTEX stVertex3;	//v1.0.T12 rename
	ST_USG_VERTEX stVertex4;	//v1.0.T12 rename
};

#define MAX_VERTEX_NUM			20
#define USG_BUFFER_LEN			80

#define EMPTY					0x00
#define DLE						0xFA                      
#define DLE01					0x01
#define DLE02					0x02
#define RID						0x10 
#define TID						0x00
#define SYNC1					0xA5
#define SYNC2					0xAA

#define PKTYPE_EXE				0x01
#define PKTYPE_CALL_BACK		0x02
#define PKTYPE_EXE_WITH_PARA	0x03

//Command
#define GETVERSION				0x01
#define SOFTBOOT				0x02
#define BOND					0x03
#define SET_PROFILE				0x06
#define GET_PROFILE				0x07
#define STANDBY					0x08
#define STOP_BOND				0x09
#define USG_CALIBRATE			0x0A

//v1.0.T20 delete
//#define USG_ME240_FIRST_BOND_SEG_NUM  20	//16
//#define USG_ME240_SECOND_BOND_SEG_NUM 14
#define USG_MIN_RAMP_TIME	1	//0.1ms	//v1.0.T20 rename

class CUsgControl : public CObject
{
public:
	CUsgControl();
	virtual ~CUsgControl();
public:
	int Init();
	int GetImpedance(double *pdImp);
	int GetFrequency(double *pdFreq);
    int CalibrateUsg(char cUsgLevel, double *pdImp, double *pdFreq);
	void ResetProfileBuffer();
	void SetProfile(WORD *pnVertexTime, BYTE *pcVertexLevel, BYTE cIndex, int nSegment);	//v1.0.T26 edit
	void SetSmallProfile(ST_USG_PROFILE stUsgProf);	//v1.0.T13 edit
	short GetVersion();
	void Standby();
	void RequestGetProfile(BYTE cProfileID);
	short CheckWrtStatus();
private:
	void CopyBuffer(BYTE *pcSource, BYTE *pcTarget, int nLen);		//v1.0.T24 edit
	bool IsBufferSame(BYTE *pcSource, BYTE *pcTarget, int nLen);	//v1.0.T26 edit
	unsigned short CalcCRC(BYTE *pcNonCRC, short nByteLen);			//v1.0.T23 edit
public:
	long m_lRxCount;	//v1.0.T19 rename
	BYTE m_acRxBuf[0xffff];	//v1.0.T19 rename
private:
	CComPort m_ComPort;
	BOOL m_bPortOpen;
	BYTE m_acBuf0[USG_BUFFER_LEN];
	BYTE m_acBuf4[USG_BUFFER_LEN];
	BYTE m_acBuf2[USG_BUFFER_LEN];
	BYTE m_acBuf6[USG_BUFFER_LEN];
	BYTE m_cVersion;	//v1.0.T28 add
};

//Event On Data Receive
void OnReceiveInfo(LPVOID pSender, void *pBuf, DWORD lRxCount);	//v1.0.T19 edit
//Com Thread Open/close Event
//void OnComBreak(LPVOID pSender,DWORD dwMask,COMSTAT stat);

extern CUsgControl theUSG;
