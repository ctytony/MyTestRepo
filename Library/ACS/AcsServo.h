//AcsServo.h v1.0.T13

#pragma once

typedef enum OPERATE_MODE {ONLINE_MODE, OFFLINE_MODE};	//v1.0.T7 add

class CAcsServo
{
public:
    CAcsServo(void);
    ~CAcsServo(void);
public:	//functions
	int EnableMotors(int iMotor1 = -1, int iMotor2 = -1, int iMotor3 = -1, int iMotor4 = -1,
					int iMotor5 = -1, int iMotor6 = -1, int iMotor7 = -1, int iMotor8 = -1);
	HANDLE GetServoHandle(void);
	int ReadDpramInteger(short nDpramAddr, int *pnValue);
	int WriteDpramInteger(short nDpramAddr, int nValue);
	int Init();
    int Initialization(bool bOperationMode);	//v1.0.T7 add
private:	//variables
    bool m_bOperationMode;	//v1.0.T7 add
	HANDLE m_hComm;
};

extern CAcsServo theAcsServo;