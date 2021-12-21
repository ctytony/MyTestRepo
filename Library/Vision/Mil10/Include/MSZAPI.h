 #ifndef _MVC1000APIUSER_H
#define _MVC1000APIUSER_H

#include "CommonDataType.h"

#ifdef MVC1000API_EXPORTS
#define MSZ_API extern __declspec(dllexport) MV_RETURN_CODE WINAPI
#else
#define MSZ_API extern __declspec(dllimport) MV_RETURN_CODE WINAPI
#endif

#ifdef __cplusplus
extern "C"{
#else
#endif


/*--------------------------------------------------------------
	initial device
  --------------------------------------------------------------*/
MSZ_API MSZ_Usb2Init(	IN LPCSTR pFilterName, IN int*  index,	
				    IN struct CapInfoStruct *pCapInfo, OUT HANDLE* hImager);

/*--------------------------------------------------------------
	uninitial device
  --------------------------------------------------------------*/
MSZ_API MSZ_Usb2Uninit(IN HANDLE* hImager);


/*--------------------------------------------------------------
	get frame rate (frames/second)
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2GetNumberDevices(IN HANDLE hImager,	
						OUT PU32 pNumberDevices);


/*--------------------------------------------------------------
	start preview
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2StartCapture(IN HANDLE hImager, BOOL bCapture);

MSZ_API 
MSZ_Usb2Start(IN HANDLE hImager,	
			 LPCTSTR   title,								//="My View"
			 ULONG     style,								//=WS_OVERLAPPEDWINDOW|WS_VISIBLE
			 ULONG     x,									//=0
			 ULONG     y,									//=0
			 ULONG     width,								//=-1
			 ULONG     height,								//=-1
			 HWND      parent,								//=NULL
			 ULONG     nId,									//=0
			 IN int    ViewDataThreadPriority,				//=THREAD_PRIORITY_NORMAL
			 IN int    ViewDrawThreadPriority);				 //=THREAD_PRIORITY_NORMAL


/*--------------------------------------------------------------
	pause preview
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2PausePreview(IN HANDLE hImager,	IN ULONG pause);

MSZ_API
MSZ_Usb2Pause(IN HANDLE hImager,	IN ULONG pause);

/*--------------------------------------------------------------
	stop preview
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2Stop(IN HANDLE hImager);			// Imager object handle


/*--------------------------------------------------------------
	get frame rate
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2GetFrameRate(IN HANDLE hImager,	
					OUT float *pfFrameRate);

/*--------------------------------------------------------------
	set preivew window's size
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetViewWin(IN HANDLE hImager, 
				  IN PMVRECT pRect);	


/*--------------------------------------------------------------
	get preview window's size
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2GetViewWin( IN  HANDLE   hImager,  
				   OUT PMVRECT pRect);

/*--------------------------------------------------------------
	set preview window same to capture window size
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2ResetViewWin(IN HANDLE hImager);

/*--------------------------------------------------------------
	set capture and preview property
  --------------------------------------------------------------*/
MSZ_API  
MSZ_Usb2SetCapInfo(IN HANDLE hImager, 
				  struct CapInfoStruct *pCapInfo);

/*--------------------------------------------------------------
	set capture and preview property
  --------------------------------------------------------------*/
MSZ_API 
MSZ_Usb2SetPartOfCapInfo(IN HANDLE hImager, 
						IN struct CapInfoStruct *pCapInfo);

/*--------------------------------------------------------------
	get 24bit rgb data of frame
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2GetRgbData(IN HANDLE hImager, 
				  IN struct CapInfoStruct *pCapInfo, ///////change
				  OUT BYTE* pDest);

/*--------------------------------------------------------------
	get the raw data of one frame
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2GetRawData(IN HANDLE hImager, IN struct CapInfoStruct *pCapInfo);

/*--------------------------------------------------------------
	convert 8bit raw to 24bit rgb 
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2ConvertRawToRgb(IN HANDLE   hImager,
					   IN BYTE*    pSrc,		// Ptr to the source buffer
					   IN ULONG    width,		//image width
					   IN ULONG    height,		//image height
					   OUT BYTE*   pDest);

/*--------------------------------------------------------------
	capture one frame and save to bmp file
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SaveFrameAsBmp(IN HANDLE   hImager, 
					  IN struct   CapInfoStruct *pCapInfo, 
					  OUT BYTE    *pDest, 
					  IN LPCTSTR  FileName);

/*--------------------------------------------------------------
	auto exposure
  --------------------------------------------------------------*/
MSZ_API 
MSZ_Usb2AutoExposure(IN HANDLE  hImager, 
					IN BOOL    bAdjustExp, 
					IN BYTE    btTarget, 
					IN LPVOID  lpFunc, 
					IN LONG    *pParam);

/*--------------------------------------------------------------
	auto white balance
  --------------------------------------------------------------*/
MSZ_API 
MSZ_Usb2AWB(IN HANDLE   hImager,	
		   IN BOOL     bDoAgc, 
		   IN BYTE     btRTarget, 
		   IN BYTE     btGTarget, 
		   IN BYTE     btBTarget, 
		   IN LPVOID   lpFunc, 
		   IN LONG     *pParam);

/*--------------------------------------------------------------
	set selectable region AE'S callback function
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetAeCallBackFunction(IN HANDLE hImager,
							 IN BYTE   btAeTarget, 
							 IN LPVOID lpFunc, 
							 IN LONG * pParam);

/*--------------------------------------------------------------
	set selectable region AWB'S callback function
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetAwbCallBackFunction(IN HANDLE hImager,
							  IN BYTE   btRTarget,
							  IN BYTE   btGTarget,
							  IN BYTE   btBTarget, 
							  IN LPVOID lpFunc, 
							  IN LONG   *pParam);

/*--------------------------------------------------------------
	set GDI MASK'S callback function
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetMaskCallBack(IN HANDLE hImager, LPMV_CALLBACK2 MaskCallBackFunction, LPVOID lpUser);

/*--------------------------------------------------------------
	if GDI MASK
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetMask(IN HANDLE hImager,BOOL bMask);

/*--------------------------------------------------------------
	load gammatable
  --------------------------------------------------------------*/
MSZ_API 
MSZ_Usb2SetGamma(IN HANDLE hImager, 
				IN BYTE   *pGamma, 
				IN BOOL   bGamma);

/*--------------------------------------------------------------
	set gamma value to correlation, (0~2)
  --------------------------------------------------------------*/
MSZ_API 
MSZ_Usb2SetGammaValue(HANDLE hImager, 
					 float  GammaValue,  
					 BOOL   bGammaOn);

/*--------------------------------------------------------------
	B/W preview
  --------------------------------------------------------------*/
MSZ_API 
MSZ_Usb2SetBw(IN HANDLE hImager,	
			 IN BOOL   bBw);

/*--------------------------------------------------------------
	clear view function
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2ClearView(IN HANDLE hImager,IN BOOL bClearView);

MSZ_API
/*--------------------------------------------------------------
	set image mirror mode
  --------------------------------------------------------------*/
MSZ_Usb2SetMirrorMode( IN HANDLE hImager, BYTE nMode );

/*--------------------------------------------------------------------------
	detect Noise Pixel.
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2DetectNoisePixel( IN HANDLE hImager );


/*--------------------------------------------------------------------------
	removal Noise Pixel. first, you should do detect noise pixels
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2RemoveNoisePixel( IN HANDLE hImager, BOOL bRemoveNoise );

MSZ_API
MSZ_Usb2Enhanced( IN HANDLE hImager, BOOL bEnhance );

/*--------------------------------------------------------------
	set frame (frame arrive) callback function. when one frame finished transfer,
	callback function will be called
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetFrameCallBack( IN HANDLE hImager, LPMV_CALLBACK2 FrameCallBackFunction, LPVOID lpUser);

MSZ_API
MSZ_Usb2SetRawCallBack( IN HANDLE hImager, LPMV_CALLBACK2 RawCallBackFunction, LPVOID lpUser);

MSZ_API
MSZ_Usb2SetFrameArriveCallBack(
							  IN HANDLE hImager, 
							  LPMV_CALLBACK fpFrameArriveCallBackFunction, 
							  LPVOID lpUser
							  );

/*--------------------------------------------------------------------------
	real time image process.
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SharpPreview( IN HANDLE hImager, BOOL bSharp );


/*--------------------------------------------------------------------------
	real time image process.
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2MeanPreview( IN HANDLE hImager, BOOL bMean );

/*--------------------------------------------------------------------------
	real time image process.
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2EmbossPreview(IN HANDLE hImager, BOOL bEmboss);


/*--------------------------------------------------------------------------
	real time image process.
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2EdgeDetectPreview(IN HANDLE hImager, BOOL bEdgeDetect);

/*--------------------------------------------------------------------------
	real time image process.
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2GaussBlurPreview(IN HANDLE hImager, BOOL bGaussBlur);

/*--------------------------------------------------------------
	get serial no. no need call for now.
  --------------------------------------------------------------*/
MSZ_API	
MSZ_Usb2GetSerial(IN HANDLE hImager, OUT LPTSTR szSerial);

/*--------------------------------------------------------------
	get verification key. no need call for now.
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2ReadKey( IN HANDLE hImager );

/*--------------------------------------------------------------
	get device type. the definition of device type refers to "commondatatype.h".
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2GetDeviceType( IN HANDLE hImager, OUT USHORT *pSubNum );

/*---------------------------------------------------------------------------------
	Set device operation mode, continue mode or trigger mode.
  --------------------------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetOpMode( IN HANDLE hImager, BYTE nMode, BOOL bStrobe );

/*---------------------------------------------------------------------------------
	For trigger mode, set the trigger pulse width. 
  --------------------------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetTriggerPulse( IN HANDLE hImager, WORD nPulseWidth );

/*---------------------------------------------------------------------------------
	For trigger mode, set the trigger delay. 
  --------------------------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetTriggerDelay( IN HANDLE hImager,WORD nTrgDelay);

/*---------------------------------------------------------------------------------
	For trigger mode, set the trigger delay. 
  --------------------------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetTriggerInterval( IN HANDLE hImager,WORD nTrgInterval);

/*---------------------------------------------------------------------------------
	For continue mode, set the led width. 
  --------------------------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetLedWidth( IN HANDLE hImager,WORD nLedWidth);

/*---------------------------------------------------------------------------------
	For continue mode, set the led delay. 
  --------------------------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetLedDelay( IN HANDLE hImager,WORD nLedDelay);

/*---------------------------------------------------------------------------------
	For trigger mode, send a trigger to camera by software to capture one frame. 
  --------------------------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SendUserTrigger( IN HANDLE hImager );

/*---------------------------------------------------------------------------------
	For trigger mode, set the polarity of trigger and strobe signal. 
  --------------------------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetSignalPolarity( IN HANDLE hImager, BYTE nTrigglePolarity, BYTE nStrobePolarity);

/*---------------------------------------------------------------------------------
	Set A/D mode of the camera.         
	//0 - 5M - [11:4], 1M/3M - [9:2]
    //1 - 5M - [10:3], 1M/3M - [8:1]
    //2 - 5M - [9:2],  1M/3M - [7:0]
    //3 - 5M - [8:1],  1M/3M - [9:2]
    //4 - 5M - [7:0],  1M/3M - [9:2]
    //else - 5M - [11:4], 1M/3M - [9:2]
  --------------------------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetADCMode( IN HANDLE hImager, BYTE nMode );

MSZ_API
MSZ_Usb2SetTestPattern(IN HANDLE hImager, BYTE nMode);

MSZ_API
MSZ_Usb2SetAgcAec( IN HANDLE hImager, BYTE nMode );

MSZ_API
MSZ_Usb2SetAWBProcess(IN HANDLE hImager, BOOL bAWBProcess, float *pRRatio, float *pBRatio);

MSZ_API
MSZ_Usb2SetRadio( IN HANDLE hImager, float fRate, BYTE nTable );

MSZ_API
MSZ_Usb2GetDeviceSubNum( IN HANDLE hImager, OUT USHORT *pSubNum );

/*--------------------------------------------------------------
	Reserved,Can not used.
	Color enhance
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetImageProcess(IN HANDLE hImager, int nBrightness, int nContrast, int nHue, int nSaturation, BOOL bProcess);

/*--------------------------------------------------------------
	Save user data to eeprom
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SaveUserData( IN HANDLE hImager, MVUserData * pUserData );

/*--------------------------------------------------------------
	Load user data from eeprom
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2LoadUserData( IN HANDLE hImager, MVUserData * pUserData );

/*-------------------------------------------------------------------------------------------
	Trigger mode, the number of frame generated by one trigger.  For CCD Camera.
  -------------------------------------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetTriggerMode( IN HANDLE hImager, BYTE nMode );

/*--------------------------------------------------------------
	Flash mode. For CCD Camera.
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetFlashMode( IN HANDLE hImager, BYTE nMode );

/*--------------------------------------------------------------
	long time integration. For CCD Camera.
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2LongTimeIntegration( IN HANDLE hImager, DWORD nIntTimeL, DWORD nIntTimeH );

/*--------------------------------------------------------------
	Set Display Threshold. For 14bit CCD Camera MVC1450DMF14.
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetDataThreshold( IN HANDLE hImager, UINT nMaxValue, UINT nMinValue );

/*--------------------------------------------------------------
	Get or set temperature for 14bit refraction CCD.
  --------------------------------------------------------------*/
MSZ_API
MSZ_Usb2GetTemperature( IN HANDLE hImager, float *pTemperature );

MSZ_API
MSZ_Usb2SetTemperature( IN HANDLE hImager, BOOL bTemperature, INT nTemperature );

/*--------------------------------------------------------------
Set AD CDS Gain, VGA Gain, Clamp for 14bit CCD.
--------------------------------------------------------------*/
MSZ_API
MSZ_Usb2SetCDSGain( IN HANDLE hImager, UINT nCDSGian );

MSZ_API
MSZ_Usb2SetADClamp( IN HANDLE hImager, UINT nClamp );

MSZ_API
MSZ_Usb2SetVGAGain( IN HANDLE hImager, UINT nVGAGian );

MSZ_API
MSZ_Usb2SetFilterProcess( IN HANDLE hImager, BOOL bFilter, UINT nFNum );

MSZ_API
MSZ_Usb2SetThreadAffinityMask( IN HANDLE hImager, DWORD nThreadAffinityMask );

MSZ_API
MSZ_Usb2SetTimingIntegration( IN HANDLE hImager, BYTE nFrameNo, DWORD nIntTimeL, DWORD nIntTimeH );

#ifdef __cplusplus
}
#endif

#endif //_MVC1000APIUSER_H

