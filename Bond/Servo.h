//Servo.h v3.1.T478

#pragma once

#include "DataStruct.h"
#include "Motion.h"
#include "ServoController.h"
#include "ServoETEL.h"

class CServo
#if _SERVO_TYPE == 2
: public CServoETEL
#else
: public CServoController
#endif
{
public:
	CServo(void);
	~CServo(void);
private:	//variables
	short m_nMsgType;
	short m_nMsgNum;
	short m_nMsgIndex;
	bool m_bStopValid;
	bool m_bSkipValid;
	CStringList m_sList;
public:	//functions
	int TriggerRestartWire(bool bParam);	//v3.1.T134 add
	int DisableServoController(void);
	int RunBuffer(short nBuffer);
    int ResetBondProgress(void);
    int GetTablePosition(TBL_POS *pstTblPos, byte nWait = MOTION_WITH_WAIT);
	void WaitTableStop();
	int MoveTable2(TBL_POS stPos, bool bAbsolute = true, bool bWait = true, bool bSlowSpeed = true);	//v3.1.T24 add, v3.1.T43 edit
	int MoveTable(TBL_POS *pstTblPos, short nMotionMode, short nWait, bool bSlowSpeed = true);
	int MoveAxisZ(long lDistance, short nMotionMode, short nWait);
	int StartJogX(short nJogDirection);
	int StartJogY(short nJogDirection);
	int StartJogZ(short nJogDirection, short nJogSpeed);
    int CheckServoError(short nCheckBufMode);
	int SendIntDataToServo(int *pnData, short nDataLength, short nBuffer);
	int SendRealDataToServo(double *pdData, short nDataLength, short nBuffer, short nServoVar);
private:	//functions
	int SyncMotor(short nMotor);
};

extern CServo theServo;
