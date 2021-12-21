//CanController.h v1.1.T11

#pragma once

#include "..\Library\CanBus\ControlCAN\ControlCAN.h"
#include "..\Library\CanBus\CriticalSection.h"

// CCanController

enum CAN_STATUS
{
	CAN_OK,
	CAN_OPEN_ERROR,
	CAN_INIT_ERROR,
	CAN_START_ERROR,
	CAN_SEND_ERROR,
	CAN_RECEIVE_ERROR,
	CAN_VERIFY_DATA_ERROR,
};

class CCanController : public CObject
{
public:
	CCanController();
	virtual ~CCanController();
public:	//functions
	void SetLight(char cCoaxRed, char cSideRed, char cCoaxBlue, char cSideBlue);
	CAN_STATUS Init(short nCanPort);
	VCI_CAN_OBJ m_astRxBuf[50];		//v1.1.T10 add
private:	//variables
	CCriticalSection m_CS;	//v1.1.T6 add
	DWORD m_lDeviceType;	//v1.0.T7 add
	HANDLE m_hDevice;		//v1.0.T4 add, v1.1.T3 edit
	DWORD m_lCanPort;
private:	//functions
	CAN_STATUS SendData(char cAddr, char cData0, char cData1, char cData2, char cData3, char cData4, char cData5, char cData6, char cData7, bool bReceive = true);
	CAN_STATUS SendDataWH(char cCardAddr, char cCardAxis, char cCommand, long lData, bool bShowErr);	//v1.1.T11 add
};

extern CCanController theCanController;
