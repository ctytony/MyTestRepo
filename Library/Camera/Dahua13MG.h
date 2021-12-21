//Dahua13MG v1.0.T1

#pragma once

#pragma warning(disable : 4819)

#include "..\Library\Camera\Dahua13MG\SDK.h"

#define	DAHUA_IMG_WIDTH        872
#define DAHUA_IMG_HEIGHT       654

typedef int (__stdcall *LPFACQUISITIONCT)(GENICAM_AcquisitionControlInfo*pAcquisitionControlInfo,GENICAM_AcquisitionControl** ppAcquisitionControl);

typedef int (__stdcall *LPFNGETSYSTEMINSTANCE)(GENICAM_System** ppSystem);
typedef int (__stdcall *LPFNCREATEGIGECAMERA)(GENICAM_GigECameraInfo* pGigECameraInfo,GENICAM_GigECamera** ppGigECamera);
typedef int (__stdcall *LPFNCREATEIMAGEFORMATCONTROL)(GENICAM_ImageFormatControlInfo* pImageFormatControlInfo,GENICAM_ImageFormatControl** ppImageFormatControl);
typedef int (__stdcall *LPFNCREATESTREAMSOURCE)(const GENICAM_StreamSourceInfo* pStreamSourceInfo,GENICAM_StreamSource** ppStreamSource);

typedef void (*LPUPDATE_BUFFER_ROUTINE)(CONST VOID* lpVoid);

class CDH13MG
{
public:
	CDH13MG(void);
	~CDH13MG(void);
public:		//function
	BYTE Init(LPUPDATE_BUFFER_ROUTINE pUpdateBuffer);
	void UnInit();
	void StartVideo();
	void StopVideo();
	void GrabImage();
protected:
	BYTE m_bVideo;
	HINSTANCE m_hLib;
	HANDLE m_hThread;
	GENICAM_System *m_pSystem;
	GENICAM_Camera *m_pCamera;
	GENICAM_StreamSource *m_pStream;
	//v1.7 13.X4
	GENICAM_CmdNode m_trigPtr;//triggersoftware

	LPUPDATE_BUFFER_ROUTINE m_pUpdateBuffer;
	BYTE SetIpAddress(LPSTR psIpAddress);
	BYTE SetImageFormat(short nWidth, short nHeight);
	static DWORD WINAPI ThreadProc(LPVOID pVoid);
};
