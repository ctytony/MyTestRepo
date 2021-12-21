//Vision.h v1.9.T3

#pragma once

#include "DataStruct.h"
#include "..\Library\Camera\Camera.h"
#include "Client.h"
#include "Mil.h"
#include "..\Library\Camera\Dahua13MG.h"
#include "..\Library\Camera\DHUsb3Cam.h"	//v1.8.L56 add

class CVision
{
public:
    CVision(void);
    ~CVision(void);
public:	//variables
    CDigitalCam m_DigitalCam;	//v1.7.T16 edit
	CDHUsb3Cam m_DHUsb3Cam;		//v1.8.L56 add
    CClient m_Client;
	short m_nImageProcess;		//v1.8.T19 rename
public:	//functions
	short SaveImage();			//v1.8.T46 add, v1.8.T48 edit
	bool IsShowResult();		//v1.8.T43 add
	void ToggleShowResult();	//v1.8.T42 add
	void SetShowResult(bool bShow);	//v1.8.T40 add
	CString GetVersion(void);	//v1.7.T24 add
	int InitUsb(HWND hDispWnd);	//v1.7.T17 add
	int InitDH13MG(HWND hDispWnd);	//v1.7.T28 add
	int InitUsb3(HWND hDispWnd);	//v1.8.L56 add
	static void UpdateBuffer(CONST VOID *pVoid);	//v1.7.T28 add
	void FreeVisionResource(void);
	void StartLiveVideo();
	void StopLiveVideo();
	void GrabImage();	//v1.8.T53 rename
	void DrawRectangle(short nWidth, short nHeight);
	void DrawRect(int iStartX, int iStartY, int iEndX, int iEndY, short nColor, short nWireNum = 0);
	void ClearOverlay();
	void ShowOverlay(bool bEnable);
	void BinarizeImage(short nThreshold);
	int LearnPattern(bool bRotateEnable, short nPatWidth, short nPatHt, short *pnPatRcd, bool bIndexPR);	//v1.8.T12 edit
	int SearchPattern(short nPatRcd, short nSrchWidth, short nSrchHt, ST_SEARCH_PATTERN_RESULT* pstSrchRes, short nBackUpID);	//v1.8.T13 rename, v1.8.T16 edit
	//v1.9.T1 add
	int SearchPattern(short nPatRcd,  short nSrchStartX, short nSrchStartY, short nSrchWidth,
		short nSrchHt, ST_SEARCH_PATTERN_RESULT* pstSrchRes, short nBackUpID, int showNum = -1);
	int SearchMultiPattern(short nPatNum, short nPatRcd, short* nSrchStartX, short* nSrchStartY, short* nSrchWidth,
		short* nSrchHt, ST_SEARCH_PATTERN_RESULT* pstSrchRes, short nBackUpID);
	int SearchMultiPattern2(short nPatNum, short* nPatRcd, short nSrchStartX, short nSrchStartY, short nSrchWidth,
		short nSrchHt, ST_SEARCH_PATTERN_RESULT* pstSrchRes, short nBackUpID);
	
	void ClearPatternRcd(short nPatRcd);	//v1.8.T15 edit
	int SavePatternRcd(void);	//v1.8.T9 edit
	int LoadPatternRcd(void);	//v1.8.T9 edit
	void ClearAllPatternRcd(short bClearType);
	int DrawPatternOnOverlay(short nPatRcd);	//v1.8.T16 edit
	void SetPassScore(short nPatRcd, short nPassScore);	//v1.8.T16 edit, v1.8.T17 rename
	void GetPatternRcd(short nPatRcd, ST_PATTERN_RECORD *pstPatRcd);	//v1.8.T17 rename
	void SetDisplayImage(bool bEnable);
	int SetCameraExposureTime(short nExposureTime);
	void UpdateWireLayout(int iStartX, int iStartY, int iEndX, int iEndY, short nWireNum,  short nColor);
	void UpdateBondPtLayout(int iStartX, int iStartY, short nBondPt, short nWireNum, short nColor);
	void DrawCircle(long lXCenter, long lYCenter, long lRadius);
	bool IsHookEnable(void);
	int AllocMaskBuffer(short nWidth, short nHeight);
	bool IsMaskBufferValid();
	int MaskBufferPoint(short nPixelX, short nPixelY);
	int DisplayMask(short nPixelX, short nPixelY);
	int SetRotateAngle(short nAngle);
	int SetImageParameter(BYTE iRedGain, int iContrast, int iBrightness, int iSaturation);
	void SetImageProcess(short nProcess);	//v1.8.T19 rename
	void WaitImageTransfer();
	void BinaryAutoThreshold(short* pnThreshold);
	int LocateLeadCenter(short nOrgX, short nOrgY, double dOrgAngle, 
						 short nThreshold, short nTipOffset, 
						 double* pdCenterX, double* pdCenterY);
	int SetAgcAec(bool bAgc, bool bAec);	//do not change aec, will affect fps 
	BYTE GetCentrePos(bool bGrabImage, BYTE nWidth, BYTE nHeight, BYTE bDrawBlob, TBL_POS* lpstOffset);	//V6.5 edit
	BYTE GetCentrePos(bool bGrabImage, BYTE nWidth, BYTE nHeight, BYTE nGapFill, BYTE bBinarize, BYTE bCircleFit, BYTE bDrawEdge, TBL_POS* lpstOffset);	//V6.5 edit
	double GetImageQuality(short nWidth, short nHeight);	//V6.2 add
	int IsDieInverted(short nPatRcd, ST_SEARCH_PATTERN_RESULT* pstSrchRes);  //v1.8.L59 add, v1.8.T60 rename
private:	//variables
	CDH13MG m_DH13MG;	//v1.7.T28 add
	bool bHookEnable;
private:	//functions
	int SaveVisionDataToFile(char* pchFileName);	//v1.8.T18 add
	int LoadVisionDataFromFile(char* pchFileName);	//v1.8.T18 add
	void DrawCursor();	//v1.8.T5 edit
	double GetImageData(short nWidth, short nHeight, long lStatType);	//V6.2 add
	BYTE Binarize(MIL_ID midImage, BYTE nWidth, BYTE nHeight);	//V6.4 add
	BYTE FindSingleBlob(BYTE nWidth, BYTE nHeight, BYTE bDrawBlob, TBL_POS* lpstOffset);	//V6.5 edit
	BYTE FindSingleEdge(BYTE nWidth, BYTE nHeight, BYTE nDenoise, BYTE bClosure, BYTE nGapFill, BYTE bBinarize, BYTE bCircleFit, BYTE bDrawEdge, TBL_POS* lpstOffset);	//V6.4 add
	BYTE FindSingleEdge(BYTE nWidth, BYTE nHeight, BYTE nGapFill, BYTE bBinarize, BYTE bCircleFit, BYTE bDrawEdge, TBL_POS* lpstOffset);								//V6.4 add
};

extern CVision theVision;