//EfoControl.h v1.0.T37

#pragma once

#include "ThreadSerial.h"

#define EFO_VERSION				0x01
//#define TIME_FACTOR			0.04	//DAC/us (25us/DAC)		//v1.0.T20 delete
//#define CURRENT_FACTOR		3		//DAC/mA (0.33mA/DAC)	//v1.0.T35 delete

#define PACKET_DONE				0x00
#define PACKET_IN_PROGRESS		0x01

#define DLE						0xFA
//#define RID					0x10 
//#define TID					0x00
#define SYNC1					0xA5
#define SYNC2					0xAA

//real-time proc func
#define COMM_REV_PROFILE		0x01
#define COMM_SEL_MODE			0x02
#define COMM_TOGGLE_FAB			0x03
#define COMM_SEL_FAB			0x04
#define COMM_GAP_LARGE_DAC		0x05	//v1.0.T36 rename
#define COMM_GAP_LARGE_TIME		0x07	//v1.0.T37 add

//call back func
#define COMM_RET_REVISION		0x11
#define COMM_RET_PROFILE		0x12

enum FAB_TYPE
{ 
	FAB_TYPE_1,  //=0
	FAB_TYPE_2,
};

enum FAB_ERROR
{ 
	SPARK_OK,  //=0
	SPARK_OPEN,
	SPARK_GAP_LARGE,
	SPARK_UNKNOWN_ERROR,
	SPARK_MISSING_RESULT,
};

class CEfoControl : public CObject
{
public:
	CEfoControl();
	virtual ~CEfoControl();
public:	//variable
	long m_lRxCount;
	unsigned char m_acRxBuf[0xff];		//v1.0.T10 edit
public:	//function
	int SetGapLargeTime(BYTE cTime_x10us);	//v1.0.T37 add
	void InitConfig();	//v1.0.T22 add
	int Init();			//v1.0.T14 edit
	int SendProfile();
	int RequestProfile();
	int SetBurnInMode(BYTE cEnable);	//v1.0.T28 rename
	int SetFabType(BYTE cFabType);		//v1.0.T28 rename
	BYTE GetVersion();
	int SetGapLargeDac(BYTE cDac);	//v1.0.T28 edit
	bool IsSparkSuccess();		//v1.0.T12 add
	int GetErrorCode();
	void ClearRxCount(void);	//v1.0.T11 edit
	double GetTimeFactor();		//v1.0.T34 add
private:	//variable
	short m_nCurrentFactor, m_nCurrentOffset;	//v1.0.T35 add
	double m_dTimeFactor;	//DAC/us (default 25us/DAC)	//v1.0.T20 add
	CComPort m_ComPort;
	bool m_bPortOpen;	//v1.0.T25 edit
	int m_nPort;		//v1.0.T5 edit
	DWORD m_lBaud;		//v1.0.T28 edit
	BYTE m_cDataBits;	//v1.0.T28 edit
	CComPort::Parity m_Parity;
	CComPort::StopBits m_Stopbits;
};

//Event On Data Receive
void OnReceiveData(LPVOID pSender, void* pBuf, DWORD InBufferCount);
//Com Thread Open/close Event
//void OnComBreak(LPVOID pSender,DWORD dwMask,COMSTAT stat);

extern CEfoControl theEFO;
