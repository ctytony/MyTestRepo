//BsdControl.h v1.0.T15

#pragma once

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct BSD_SETTING   //USB BSD SET
{
	BYTE  NsopDac;
	BYTE  NsolDac;
	WORD  NsopSample;
	WORD  TailShortSample;
	WORD  NsolSample; 

    //must init before use, or by default as 0
	BSD_SETTING():
		NsopDac(),
		NsolDac(),
		NsopSample(),
		TailShortSample(),
		NsolSample(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

//Result Bit Definition	-- g_u8Result
#define RESULT_NO_ERROR	       0x00
#define RESULT_NSOP			   0x01
#define RESULT_NSOL			   0x02
#define RESULT_SHORT_TAIL	   0x04
#define RESULT_NSOP_CNT 	   0x05
#define RESULT_NSOL_CNT 	   0x06

enum enBsdCommError
{
    BSD_COMM_OK,
    BSD_COMM_NETWORK_ERROR,
    BSD_COMM_SOCKET_ERROR,
    BSD_COMM_CONNECT_ERROR,
    BSD_COMM_VERSION_ERROR,
    BSD_COMM_EFO_INIT_ERROR,
};

//#define WIRE_FEED_DETECT_TIMES    40	//v1.0.T9 delete

/*Protocol: 
Max Data Length:  10 bytes
Format: 
/Start0, /Start1, Command,DataLen,Data,/End
Interrupt Transfer
*/

class CBsdControl : public CObject
{
public:
	CBsdControl();
	virtual ~CBsdControl();
public:	//function
    int Init(void);
    void StartStickDetect(short nStickDetectBlock);
    short CheckStickResult(BOOL bNsopEnable, BOOL bNsolEnable, BOOL bTailBreak, short nNonStickSample, short nTailBreakNonStickSample, short nTailShortSample, bool bStickCoarseDetect, short nStickCoarseDetectSample, short* pnNsolSample);
    int SetStickPara(BYTE nDieShortThreshold, BYTE nLeadShortThreshold, WORD nNonStickSample, WORD nTailShortSample);	//v1.0.T8 edit
    //void InitWireFeedCounter(void);	//v1.0.T10 delete
    void ClearWireFeedCount(void);		//v1.0.T10 add
    short CheckWireFeedStatus(int nWireCount);
	int OpenUsbDevice(void);
	void SetPara(BSD_SETTING* stBsdPara);	//v1.0.T8 edit
	//short UsbBsdDebugCheckResult(BYTE* u8Result);	//v1.0.T12 delete
	short GetStickResult(BYTE* pnResult);	//v1.0.T12 add
	int GetStickCount(WORD* pnNsopCount, WORD* pnNsolCount);	//v1.0.T11 edit
	void SetDualDacValue(byte u8Chn1, byte u8Chn2);
	void ResetBsd(void);
	void CloseUsbDevice(void);
	void GetVersion(BYTE* bVer);
	void GetPara(BSD_SETTING* stBsdPara);	//v1.0.T8 edit
	//short UsbGetWireFeedCounter(WORD* wCtns);	//v1.0.T10 delete
	short GetWireFeedCount(WORD* wCtns);		//v1.0.T10 add
private:	//variable
    short m_nBondedWire;	//no. of bonded wire
    short m_nStickErrorCounter;
private:	//function
	void UsbClearWireFeedCount(void);	//v1.0.T9 add
};

extern CBsdControl theBSD;
