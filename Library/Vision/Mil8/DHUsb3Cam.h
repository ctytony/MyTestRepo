//DHUsb3Cam.h v1.8.T64

#pragma once

#include "..\Library\Camera\Daheng\DxImageProc.h"
#include "..\Library\Camera\Daheng\GxIAPI.h"
#include "HTimer.h"

void CALLBACK OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);
DWORD WINAPI DHThreadProc(LPVOID pParam);

class CDHUsb3Cam
{
public:
	CDHUsb3Cam(void);
	~CDHUsb3Cam(void);
public:
	int Init();		//v1.8.T64 rename
	int UnInit(void);
	bool IsImageReady();
	bool IsStopVideo(void);
	void OpenDevice();
	void CloseDevice();
	void StartVideo();
	void StopVideo();
	void GrabImage();
	void SetImageReady();
	void DHVideoThread(void);
	GX_STATUS InitDevice();
	GX_STATUS SetPixelFormat8bit();
	GX_STATUS GetDeviceParam();
	int SetExposureTime(short nTimeInUs);	//v1.8.L58 add
	int SetGain(short nGain);				//v1.8.L58 add
public:
	GX_DEV_HANDLE m_hDevice;
	int64_t m_nPayLoadSize;
	int64_t m_nImageHeight;
	int64_t m_nImageWidth;
	bool m_bStopVideo;
	bool m_bImageReady;
	CHTimer m_GrabTimer;
	double m_dGrabTime;
	HANDLE m_hVideoThread;
};
