//Camera.h v1.0.T12

#pragma once

#include "..\Library\Camera\MSZ\MSZAPI.h"
#include "HTimer.h"

void CALLBACK MSZRawCallBackCCD0(LPVOID lpParam, LPVOID lpUser);
DWORD WINAPI VideoThreadProc(LPVOID pParam);	//v1.0.T7 rename

class CDigitalCam
{
public:
	CDigitalCam(void);
	~CDigitalCam(void);
public:		//function
	bool IsImageReady();	//v1.0.T8 add
	void SetImageReady();	//v1.0.T8 add, v1.0.T10 edit
	int Init(void);			//v1.0.T9 rename
	int FreeResource(void);
	void StopVideo(void);	//v1.0.T7 rename
	void GrabImage(void);
	int SetExposureTime(short nTimeInUs);
	void StartVideo(void);	//v1.0.T7 rename
	void VideoThread(void);	//v1.0.T7 rename
	int SetImageParameter(BYTE iRedGain, int iContrast, int iBrightness, int iSaturation);
	bool IsStopVideo(void);	//v1.0.T7 rename
	int SetAgcAec(bool bAgc, bool bAec);
public:		//variable
	BYTE m_acImageBuf[752*480];	//v1.0.T8 rename
	HANDLE m_hVideoThread;		//v1.0.T7 rename
private:	//variable
	CHTimer m_GrabTimer;		//v1.8.T11 add
	double m_dGrabTime;			//v1.8.T11 add
	bool m_bImageReady;			//v1.0.T8 rename
	HANDLE m_hMSZCCD;
	CapInfoStruct m_CapInfo;
	ULONG m_nDeviceNum;
	USHORT m_nDeviceType;
	bool m_bStopVideo;	//v1.0.T7 rename
};
