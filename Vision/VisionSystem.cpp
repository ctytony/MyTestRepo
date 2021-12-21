/***************************************************************
VisionSystem.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "DataStruct.h"
#include "MenuConfig.h"
#include "macdef.h"
#include "msgdef.h"
#include "MsgManager.h"
#include "DataManager.h"
#include "MathCalc.h"
#include "SerialComIO.h"
#include "Program.h"
#include "LogTrace.h"
#include "Wire.h"  
#include "AutoPadMgr.h"
#include "MachineConfig.h"
#include "MachineData.h"
#include "CanController.h"
#include "VisionSystem.h"
#include "servo.h"
#include "Debug.h"
#include "Bond.h"
#include "memory"

CVisionSystem theVisionSys;

// CVisionSystem

CVisionSystem::CVisionSystem()
	: m_dHighMagFactorX(1.01)
	, m_dHighMagFactorY(1.01)
	, m_dLowMagFactorX(1.01)
	, m_dLowMagFactorY(1.01)
	, m_iCalibMag(0)
    , m_nCurCoaxRedLight(0)
    , m_nCurCoaxBlueLight(0)
    , m_nCurSideRedLight(0)
    , m_nCurSideBlueLight(0)
    , m_nBinaryThreshold(100)
    , m_nPatSizeX(0)
    , m_nPatSizeY(0)
    , m_bVideoState(false)
	, m_bDrawWire(false)
    , m_bEditPatternMask(false)
    , m_dVisionFactorCos(0)
    , m_dVisionFactorSin(0)
{
}

CVisionSystem::~CVisionSystem()
{
}

// CVisionSystem member functions

int CVisionSystem::Init(HWND hDispWnd)	//v1.6.T339 edit
{
	if (theMcConfig.IsBypassCamera())
	{
		theVision.InitUsb(hDispWnd);	//v3.0.T170 add
		return FOK;
	}

	int iErr;
	if (theMcConfig.GetCameraType() == CAMERA_DAHUA_GIGE)
		iErr = theVision.InitDH13MG(hDispWnd);	//v1.6.T356 add
	else if (theMcConfig.GetCameraType() == CAMERA_DAHENG_USB3)	//v3.0.L169 add
		iErr = theVision.InitUsb3(hDispWnd);
	else
		iErr = theVision.InitUsb(hDispWnd);		//v1.6.T340 edit
	if (iErr != VISION_OK) return iErr;
	Sleep(30);

	MACHINE_DATA_LIGHT* pstLight = theMcData.GetLightRcd();
	UpdateLightingParameter(pstLight->stIdleLight.nCoaxRedLight,
							pstLight->stIdleLight.bSideRedLight, 
							pstLight->stIdleLight.nCoaxBlueLight,
							pstLight->stIdleLight.bSideBlueLight, true);

	//v2.9.0.w1 add
	PGM_RECORD *pstPgm = thePgm.GetRcd();
	//SetAgcAec(false, false);	//v2.0.T106 delete
	SetAgcAec((pstPgm->nAGC == 1), false);
	SetCameraParameters((byte)pstPgm->nRedGain,
					pstPgm->nContrast, pstPgm->nBrightness, 0);

	EnableVideo(true);
	return VISION_OK;
}

void CVisionSystem::GetVisionFactor(double *pdHighFactorX, double *pdHighFactorY, double *pdLowFactorX, double *pdLowFactorY, double *pdFactorCos, double *pdFactorSin)
{
	*pdHighFactorX = m_dHighMagFactorX;
	*pdHighFactorY = m_dHighMagFactorY;
	*pdLowFactorX  = m_dLowMagFactorX;
	*pdLowFactorY  = m_dLowMagFactorY;

    *pdFactorCos = m_dVisionFactorCos;
    *pdFactorSin = m_dVisionFactorSin;
}

// Vision factor calibration
void CVisionSystem::UpdateVisionFactor(int iCalibMag, double dMagFactX, double dMagFactY, double dFactCos, double dFactSin)
{
	if (iCalibMag == 0)
	{
		m_dLowMagFactorX = dMagFactX;
		m_dLowMagFactorY = dMagFactY;
	}
	else
	{
		m_dHighMagFactorX = dMagFactX;
		m_dHighMagFactorY = dMagFactY;
	}

    m_dVisionFactorCos = dFactCos;
    m_dVisionFactorSin = dFactSin;
}

void CVisionSystem::FreeVisionResource(void)
{
    if (theMcConfig.IsBypassCamera()) return;

	UpdateLightingParameter(0, 0, 0, 0);
	theVision.FreeVisionResource();
}

void CVisionSystem::GetLightingParameter(short *pnCoaxRedLight, short *pnSideRedLight, short *pnCoaxBlueLight, short *pnSideBlueLight)
{
    *pnCoaxRedLight  = m_nCurCoaxRedLight;
    *pnSideRedLight  = m_nCurSideRedLight;
    *pnCoaxBlueLight = m_nCurCoaxBlueLight;
    *pnSideBlueLight = m_nCurSideBlueLight;
}

int CVisionSystem::UpdateLightingParameter(short nCoaxRedLight, short nSideRedLight, short nCoaxBlueLight, short nSideBlueLight, bool bPowerOn)
{
    if (theMcConfig.IsBypassCamera()) return FOK;

	int lErr = 0;

	if (bPowerOn == true)
	{
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
		{
			m_nCurCoaxRedLight = nCoaxRedLight;
			m_nCurSideRedLight = nSideRedLight;
			m_nCurCoaxBlueLight = nCoaxBlueLight;
			m_nCurSideBlueLight = nSideBlueLight;
			theCanController.SetLight(
						(char)nCoaxRedLight, (char)nSideRedLight,
						(char)nCoaxBlueLight, (char)nSideBlueLight);
		}
		else
		{
			m_nCurCoaxRedLight = nCoaxRedLight;
			lErr = theSerialCommIO.UpdateLightingParameter(CHN_COAX_RED_LIGHT, nCoaxRedLight);

			m_nCurSideRedLight = nSideRedLight;
			lErr = theSerialCommIO.UpdateLightingParameter(CHN_SIDE_RED_LIGHT, nSideRedLight);

			m_nCurCoaxBlueLight = nCoaxBlueLight;
			lErr = theSerialCommIO.UpdateLightingParameter(CHN_COAX_BLUE_LIGHT, nCoaxBlueLight);

			m_nCurSideBlueLight = nSideBlueLight;
			lErr = theSerialCommIO.UpdateLightingParameter(CHN_SIDE_BLUE_LIGHT, nSideBlueLight);
		}
	}
	else
	{
		if (theMcConfig.GetStepperType() == STEP_TYPE_STEPPERIO)
		{
			if ((m_nCurCoaxRedLight != nCoaxRedLight)
				|| (m_nCurSideRedLight != nSideRedLight)
				|| (m_nCurCoaxBlueLight != nCoaxBlueLight)
				|| (m_nCurSideBlueLight != nSideBlueLight))
			{
				m_nCurCoaxRedLight = nCoaxRedLight;
				m_nCurSideRedLight = nSideRedLight;
				m_nCurCoaxBlueLight = nCoaxBlueLight;
				m_nCurSideBlueLight = nSideBlueLight;
				theCanController.SetLight(
						(char)nCoaxRedLight, (char)nSideRedLight,
						(char)nCoaxBlueLight, (char)nSideBlueLight);
			}
		}
		else
		{
			if (m_nCurCoaxRedLight != nCoaxRedLight)
			{
				m_nCurCoaxRedLight = nCoaxRedLight;
				lErr = theSerialCommIO.UpdateLightingParameter(CHN_COAX_RED_LIGHT, nCoaxRedLight);
			}

			if (m_nCurSideRedLight != nSideRedLight)
			{
				m_nCurSideRedLight = nSideRedLight;
				lErr = theSerialCommIO.UpdateLightingParameter(CHN_SIDE_RED_LIGHT, nSideRedLight);
			}

			if (m_nCurCoaxBlueLight != nCoaxBlueLight)
			{
				m_nCurCoaxBlueLight = nCoaxBlueLight;
				lErr = theSerialCommIO.UpdateLightingParameter(CHN_COAX_BLUE_LIGHT, nCoaxBlueLight);
			}

			if (m_nCurSideBlueLight != nSideBlueLight)
			{
				m_nCurSideBlueLight = nSideBlueLight;
				lErr = theSerialCommIO.UpdateLightingParameter(CHN_SIDE_BLUE_LIGHT, nSideBlueLight);
			}
		}
	}

	return lErr;
}

void CVisionSystem::GetBinaryThreshold(short *pnThreshold)
{
    *pnThreshold = m_nBinaryThreshold;
}

void CVisionSystem::UpdateBinaryThreshold(short nThreshold)
{
    if (theMcConfig.IsBypassCamera()) return;

	m_nBinaryThreshold = nThreshold;
	theVision.WaitImageTransfer();
	theVision.BinarizeImage(m_nBinaryThreshold);
}

void CVisionSystem::ChangeLensMag(int nLensMag)
{
}

int CVisionSystem::LearnVisionPattern(bool bRotate, short nPatWidth, short nPatHeight, short *pnPatRcd, bool bIndexPR)
{
    if (theMcConfig.IsBypassCamera()) return FOK;
	m_bEditPatternMask = false;
	return theVision.LearnPattern(bRotate, nPatWidth, nPatHeight, pnPatRcd, bIndexPR);
}

// Set size of search pattern
void CVisionSystem::SetSearchSize(short nSrchSizeX, short nSrchSizeY)
{
    if (theMcConfig.IsBypassCamera())
	{
		m_nPatSizeX = nSrchSizeX;
		m_nPatSizeY = nSrchSizeY;
		return;
	}

	theVision.ClearOverlay();
	theVision.DrawRectangle(nSrchSizeX, nSrchSizeY);
	m_nPatSizeX = nSrchSizeX;
	m_nPatSizeY = nSrchSizeY;
}

void CVisionSystem::GetSearchSize(short *nSrchSizeX, short *nSrchSizeY)
{
	*nSrchSizeX = m_nPatSizeX;
	*nSrchSizeY = m_nPatSizeY;
}

int CVisionSystem::SearchVisionPattern(short nPatRcd, short nSrchRange, short nPassScore, PR_SRCH_RESULT *pstSrchRes, short nBackUpID)
{
	int nResult = SearchVisionPattern(nPatRcd,
						VISION_SEARCH_BASE_WIDTH * nSrchRange,
						VISION_SEARCH_BASE_HEIGHT * nSrchRange,
						nPassScore, pstSrchRes, nBackUpID);
	if ((nResult == FOK)
		|| (nSrchRange >= VISION_MAX_SEARCH_RANGE)) return nResult;

	if (thePgm.GetRcd()->bTry2xSearchRange == FALSE) return nResult;

	theBond.ShowDebugMsg(DEBUG_VISION_TIMING,
			_T("PrFail-Range/Pass/Score"),
			nSrchRange, nPassScore, pstSrchRes->dScore);

	nSrchRange *= 2;
	if (nSrchRange > VISION_MAX_SEARCH_RANGE)
		nSrchRange = VISION_MAX_SEARCH_RANGE;

	nResult = SearchVisionPattern(nPatRcd,
					VISION_SEARCH_BASE_WIDTH * nSrchRange,
					VISION_SEARCH_BASE_HEIGHT * nSrchRange,
					nPassScore, pstSrchRes, nBackUpID);

	theBond.ShowDebugMsg(DEBUG_VISION_TIMING,
				_T("PrFail2-Range/Pass/Score"),
				nSrchRange, nPassScore, pstSrchRes->dScore);

	return nResult;
}

int CVisionSystem::SearchVisionPattern(short nPatRcd, short nSrchRangeX, short nSrchRangeY, short nPassScore, PR_SRCH_RESULT *pstSrchRes, short nBackUpID)
{
    if (theMcConfig.IsBypassCamera())
	{
        pstSrchRes->dOffsetX = 0.0;
        pstSrchRes->dOffsetY = 0.0;
        pstSrchRes->dScore = 90.0;
        pstSrchRes->dAngle = 0.0;
		return FOK;
	}

	int iErr = FOK;
	ST_SEARCH_PATTERN_RESULT stSrchRes;

	//send to vision side with specified search ID
	theVision.WaitImageTransfer();

	iErr = theVision.SearchPattern(nPatRcd, nSrchRangeX, nSrchRangeY, &stSrchRes, nBackUpID);

	if (theMcConfig.IsCheckInvertDie())	//v3.0.T287 add
	{
		//v3.0.L284 add
		if (theVision.IsDieInverted(nPatRcd, &stSrchRes) == 1)
		{
			short nMsg = MSG_CHIP_REVERSE;
			if (theMsg.ShowMsg(ERROR_MSG, 1, &nMsg, true, false, true) == SHOW_MSG_RESULT_STOP)
				theBond.InformAutoBondStop();
		}
	}

	if (theDebug.IsDebugVisionTiming())
		pstSrchRes->dScore = stSrchRes.dScore;

	if (iErr == VISION_ERROR_NONE)
	{
		pstSrchRes->dOffsetX = stSrchRes.dOffsetX;
		pstSrchRes->dOffsetY = stSrchRes.dOffsetY;
		pstSrchRes->dScore = stSrchRes.dScore;
		pstSrchRes->dAngle = -stSrchRes.dRotateAngle * PI / 180.0;

		//v3.0.T129 add
		if (theDebug.IsDebugLogImage())
		{
			double dAngle = stSrchRes.dRotateAngle;
			if (dAngle > 180) dAngle -= 360;
			else if (dAngle < -180) dAngle += 360;

			if ((pstSrchRes->dScore < theMcConfig.GetLogImageScore())
				|| (abs(dAngle) > theMcConfig.GetLogImageAngle()))
			{
				//v3.0.T130 edit
				short nImageID = theVision.SaveImage();
				CString str;
				str.Format(_T("Img%d_S%.2f_A%.2f"), nImageID,
						stSrchRes.dScore, stSrchRes.dRotateAngle);
				theLog.WriteLine(str, true);
				theDebug.AddInfo(str);
			}
		}

		if (pstSrchRes->dScore < nPassScore)
		{
			if (pstSrchRes->dScore > thePgm.GetAutoSkipScore())
				iErr = VISION_ERROR_QUALITY_REJECT;
			else
			{
				iErr = VISION_ERROR_NO_PATTERN;
				return iErr;
			}
		}

		//v3.0.T116 delete
		//double fAngle;
		//if (stSrchRes.dRotateAngle >= 180)
		//	fAngle = stSrchRes.dRotateAngle - 360;
		//else if (stSrchRes.dRotateAngle <= -180)
		//	fAngle = stSrchRes.dRotateAngle + 360;
		//else
		//	fAngle = stSrchRes.dRotateAngle;
		//if (abs(fAngle) > 60)
		//	iErr = VISION_ERROR_QUALITY_REJECT;

		if ((abs(round(pstSrchRes->dOffsetX * 2.0)) >  nSrchRangeX)
			|| (abs(round(pstSrchRes->dOffsetY * 2.0)) > nSrchRangeY))
			iErr = VISION_ERROR_QUALITY_REJECT;
	}
	else if (iErr < 0)
	{
		CString str;
		CStringList sList;
		sList.RemoveAll();

		if (iErr == -99)
		{
			str.Format(_T("Pattern validation expired"));
			sList.AddTail(str);
		}
		else
		{
			str.Format(_T("Pattern lost! Error = %d"), iErr);
			sList.AddTail(str);
		}
		theMsg.ShowMsg(HELP_MSG, sList);
		return VISION_ERROR_NO_PATTERN;
	}
	else
		iErr = VISION_ERROR_NO_PATTERN;

	return iErr;
}

//v3.1.T489 add
int CVisionSystem::SearchVisionPattern(short nPatRcd, short nSrchStartX,short nSrchStartY,
		short nSrchWidth,short nSrchHt, short nPassScore, PR_SRCH_RESULT *pstSrchRes, short nBackUpID, int showNum)
{
	if (theMcConfig.IsBypassCamera())
	{
        pstSrchRes->dOffsetX = 0.0;
        pstSrchRes->dOffsetY = 0.0;
        pstSrchRes->dScore = 90.0;
        pstSrchRes->dAngle = 0.0;
		return FOK;
	}

	int iErr = FOK;
	ST_SEARCH_PATTERN_RESULT stSrchRes;

	//send to vision side with specified search ID
	theVision.WaitImageTransfer();

	iErr = theVision.SearchPattern(nPatRcd, nSrchStartX,nSrchStartY, nSrchWidth,nSrchHt, &stSrchRes, nBackUpID, showNum);

	if (theMcConfig.IsCheckInvertDie())	//v3.0.T287 add
	{
		//v3.0.L284 add
		if (theVision.IsDieInverted(nPatRcd, &stSrchRes) == 1)
		{
			short nMsg = MSG_CHIP_REVERSE;
			if (theMsg.ShowMsg(ERROR_MSG, 1, &nMsg, true, false, true) == SHOW_MSG_RESULT_STOP)
				theBond.InformAutoBondStop();
		}
	}

	if (theDebug.IsDebugVisionTiming())
		pstSrchRes->dScore = stSrchRes.dScore;

	if (iErr == VISION_ERROR_NONE)
	{
		pstSrchRes->dOffsetX = stSrchRes.dOffsetX;
		pstSrchRes->dOffsetY = stSrchRes.dOffsetY;
		pstSrchRes->dScore = stSrchRes.dScore;
		pstSrchRes->dAngle = -stSrchRes.dRotateAngle * PI / 180.0;

		//v3.0.T129 add
		if (theDebug.IsDebugLogImage())
		{
			double dAngle = stSrchRes.dRotateAngle;
			if (dAngle > 180) dAngle -= 360;
			else if (dAngle < -180) dAngle += 360;

			if ((pstSrchRes->dScore < theMcConfig.GetLogImageScore())
				|| (abs(dAngle) > theMcConfig.GetLogImageAngle()))
			{
				//v3.0.T130 edit
				short nImageID = theVision.SaveImage();
				CString str;
				str.Format(_T("Img%d_S%.2f_A%.2f"), nImageID,
						stSrchRes.dScore, stSrchRes.dRotateAngle);
				theLog.WriteLine(str, true);
				theDebug.AddInfo(str);
			}
		}

		if (pstSrchRes->dScore < nPassScore)
		{
			if (pstSrchRes->dScore > thePgm.GetAutoSkipScore())
				iErr = VISION_ERROR_QUALITY_REJECT;
			else
			{
				iErr = VISION_ERROR_NO_PATTERN;
				return iErr;
			}
		}
		/*if ((abs(round(pstSrchRes->dOffsetX * 2.0)) >  nSrchRangeX)
			|| (abs(round(pstSrchRes->dOffsetY * 2.0)) > nSrchRangeY))
			iErr = VISION_ERROR_QUALITY_REJECT;*/
	}
	else if (iErr < 0)
	{
		CString str;
		CStringList sList;
		sList.RemoveAll();

		if (iErr == -99)
		{
			str.Format(_T("Pattern validation expired"));
			sList.AddTail(str);
		}
		else
		{
			str.Format(_T("Pattern lost! Error = %d"), iErr);
			sList.AddTail(str);
		}
		theMsg.ShowMsg(HELP_MSG, sList);
		return VISION_ERROR_NO_PATTERN;
	}
	else
		iErr = VISION_ERROR_NO_PATTERN;

	return iErr;
}

//v3.1.T489 add
int CVisionSystem::SearchMultiVisionPattern(short &nPatNum, short* nPatRcd, short* nSrchStartX, short* nSrchStartY, short* nSrchWidth, short* nSrchHt, 
        short* nPassScore, PR_SRCH_RESULT* pstSrchRes, short nBackUpID)
{
	if (theMcConfig.IsBypassCamera() || nPatNum < 1)
	{
		pstSrchRes->dOffsetX = 0.0;
		pstSrchRes->dOffsetY = 0.0;
		pstSrchRes->dScore = 90.0;
		pstSrchRes->dAngle = 0.0;
		nPatNum = 0;
		return FOK;
	}
	pstSrchRes = new PR_SRCH_RESULT[nPatNum];
	int iErr = FOK;
	ST_SEARCH_PATTERN_RESULT stSrchRes;
	//send to vision side with specified search ID
	theVision.WaitImageTransfer();
	for (int i = 0; i < nPatNum; i++)
	{
		PR_SRCH_RESULT stTempSrchRes;
		iErr = theVision.SearchPattern(nPatRcd[i], nSrchStartX[i], nSrchStartY[i],nSrchWidth[i],nSrchHt[i], &stSrchRes, nBackUpID);
		if (theMcConfig.IsCheckInvertDie())	//v3.0.T287 add
		{
			//v3.0.L284 add
			if (theVision.IsDieInverted(nPatRcd[i], &stSrchRes) == 1)
			{
				short nMsg = MSG_CHIP_REVERSE;
				if (theMsg.ShowMsg(ERROR_MSG, 1, &nMsg, true, false, true) == SHOW_MSG_RESULT_STOP)
					theBond.InformAutoBondStop();
			}
		}
		if (theDebug.IsDebugVisionTiming())
			stTempSrchRes.dScore = stSrchRes.dScore;

		if (iErr == VISION_ERROR_NONE)
		{
			stTempSrchRes.dOffsetX = stSrchRes.dOffsetX;
			stTempSrchRes.dOffsetY = stSrchRes.dOffsetY;
			stTempSrchRes.dScore = stSrchRes.dScore;
			stTempSrchRes.dAngle = -stSrchRes.dRotateAngle * PI / 180.0;

			//v3.0.T129 add
			if (theDebug.IsDebugLogImage())
			{
				double dAngle = stSrchRes.dRotateAngle;
				if (dAngle > 180) dAngle -= 360;
				else if (dAngle < -180) dAngle += 360;

				if ((stTempSrchRes.dScore < theMcConfig.GetLogImageScore())
					|| (abs(dAngle) > theMcConfig.GetLogImageAngle()))
				{
					//v3.0.T130 edit
					short nImageID = theVision.SaveImage();
					CString str;
					str.Format(_T("Img%d_S%.2f_A%.2f"), nImageID,
						stSrchRes.dScore, stSrchRes.dRotateAngle);
					theLog.WriteLine(str, true);
					theDebug.AddInfo(str);
				}
			}

			if (stTempSrchRes.dScore < nPassScore[i])
			{
				if (stTempSrchRes.dScore > thePgm.GetAutoSkipScore())
					iErr = VISION_ERROR_QUALITY_REJECT;
				else
				{
					iErr = VISION_ERROR_NO_PATTERN;
					return iErr;
				}
			}
			/*if ((abs(round(stTempSrchRes.dOffsetX * 2.0)) > nSrchRangeX[i])
				|| (abs(round(stTempSrchRes.dOffsetY * 2.0)) > nSrchRangeY[i]))
				iErr = VISION_ERROR_QUALITY_REJECT;*/
		}
		else if (iErr < 0)
		{
			CString str;
			CStringList sList;
			sList.RemoveAll();

			if (iErr == -99)
			{
				str.Format(_T("Pattern validation expired"));
				sList.AddTail(str);
			}
			else
			{
				str.Format(_T("Pattern lost! Error = %d"), iErr);
				sList.AddTail(str);
			}
			theMsg.ShowMsg(HELP_MSG, sList);
			return VISION_ERROR_NO_PATTERN;
		}
		else
			iErr = VISION_ERROR_NO_PATTERN;
		pstSrchRes[i] = stTempSrchRes;
		if (iErr != VISION_ERROR_NONE)
		{
			nPatNum = i + 1;
			return iErr;
		}
	}
	
	return iErr;
}

//v3.1.T489 add
int CVisionSystem::SearchMultiVisionPattern2(short& nPatNum, short nPatRcd, short* nSrchStartX, short* nSrchStartY,
	short* nSrchWidth, short* nSrchHt, short* nPassScore, PR_SRCH_RESULT* pstSrchRes, short nBackUpID)
{
	if (theMcConfig.IsBypassCamera() || nPatNum < 1)
	{
		pstSrchRes->dOffsetX = 0.0;
		pstSrchRes->dOffsetY = 0.0;
		pstSrchRes->dScore = 90.0;
		pstSrchRes->dAngle = 0.0;
		nPatNum = 0;
		return FOK;
	}
	pstSrchRes = new PR_SRCH_RESULT[nPatNum];
	int iErr = FOK;
	ST_SEARCH_PATTERN_RESULT* stSrchRes;
	//send to vision side with specified search ID
	theVision.WaitImageTransfer();
	iErr = theVision.SearchMultiPattern(nPatNum, nPatRcd, nSrchStartX, nSrchStartY, nSrchWidth, nSrchHt, stSrchRes, nBackUpID);
	for (int i = 0; i < nPatNum; i++)
	{
		PR_SRCH_RESULT stTempSrchRes;
		if (theMcConfig.IsCheckInvertDie())	//v3.0.T287 add
		{
			//v3.0.L284 add
			if (theVision.IsDieInverted(nPatRcd, &stSrchRes[i]) == 1)
			{
				short nMsg = MSG_CHIP_REVERSE;
				if (theMsg.ShowMsg(ERROR_MSG, 1, &nMsg, true, false, true) == SHOW_MSG_RESULT_STOP)
					theBond.InformAutoBondStop();
			}
		}
		if (!theDebug.IsDebugVisionTiming())
			stTempSrchRes.dScore = 0;

		if (iErr == VISION_ERROR_NONE)
		{
			stTempSrchRes.dOffsetX = stSrchRes[i].dOffsetX;
			stTempSrchRes.dOffsetY = stSrchRes[i].dOffsetY;
			stTempSrchRes.dScore = stSrchRes[i].dScore;
			stTempSrchRes.dAngle = -stSrchRes[i].dRotateAngle * PI / 180.0;

			//v3.0.T129 add
			if (theDebug.IsDebugLogImage())
			{
				double dAngle = stSrchRes[i].dRotateAngle;
				if (dAngle > 180) dAngle -= 360;
				else if (dAngle < -180) dAngle += 360;

				if ((stTempSrchRes.dScore < theMcConfig.GetLogImageScore())
					|| (abs(dAngle) > theMcConfig.GetLogImageAngle()))
				{
					//v3.0.T130 edit
					short nImageID = theVision.SaveImage();
					CString str;
					str.Format(_T("Img%d_S%.2f_A%.2f"), nImageID,
						stSrchRes[i].dScore, stSrchRes[i].dRotateAngle);
					theLog.WriteLine(str, true);
					theDebug.AddInfo(str);
				}
			}
			if (stTempSrchRes.dScore < nPassScore[i])
			{
				if (stTempSrchRes.dScore > thePgm.GetAutoSkipScore())
					iErr = VISION_ERROR_QUALITY_REJECT;
				else
				{
					iErr = VISION_ERROR_NO_PATTERN;
					return iErr;
				}
			}
			/*if ((abs(round(stTempSrchRes.dOffsetX * 2.0)) > nSrchRangeX)
				|| (abs(round(stTempSrchRes.dOffsetY * 2.0)) > nSrchRangeY))
				iErr = VISION_ERROR_QUALITY_REJECT;*/
		}
		else if (iErr < 0)
		{
			CString str;
			CStringList sList;
			sList.RemoveAll();

			if (iErr == -99)
			{
				str.Format(_T("Pattern validation expired"));
				sList.AddTail(str);
			}
			else
			{
				str.Format(_T("Pattern lost! Error = %d"), iErr);
				sList.AddTail(str);
			}
			theMsg.ShowMsg(HELP_MSG, sList);
			return VISION_ERROR_NO_PATTERN;
		}
		else
			iErr = VISION_ERROR_NO_PATTERN;
		pstSrchRes[i] = stTempSrchRes;
		if (iErr != VISION_ERROR_NONE)
		{
			nPatNum = i + 1;
			return iErr;
		}
	}

	return iErr;
}

int CVisionSystem::SearchMultiVisionPattern3(short& nPatNum, short* nPatRcd, short nSrchStartX, short nSrchStartY,
	short nSrchWidth, short nSrchHt, short nPassScore, PR_SRCH_RESULT* pstSrchRes, short nBackUpID)
{
	if (theMcConfig.IsBypassCamera() || nPatNum < 1)
	{
		pstSrchRes->dOffsetX = 0.0;
		pstSrchRes->dOffsetY = 0.0;
		pstSrchRes->dScore = 90.0;
		pstSrchRes->dAngle = 0.0;
		nPatNum = 0;
		return FOK;
	}
	pstSrchRes = new PR_SRCH_RESULT[nPatNum];
	int iErr = FOK;
	ST_SEARCH_PATTERN_RESULT* stSrchRes;
	//send to vision side with specified search ID
	theVision.WaitImageTransfer();
	iErr = theVision.SearchMultiPattern2(nPatNum, nPatRcd, nSrchStartX, nSrchStartY, nSrchWidth, nSrchHt, stSrchRes, nBackUpID);
	for (int i = 0; i < nPatNum; i++)
	{
		PR_SRCH_RESULT stTempSrchRes;
		if (theMcConfig.IsCheckInvertDie())	//v3.0.T287 add
		{
			//v3.0.L284 add
			if (theVision.IsDieInverted(nPatRcd[i], &stSrchRes[i]) == 1)
			{
				short nMsg = MSG_CHIP_REVERSE;
				if (theMsg.ShowMsg(ERROR_MSG, 1, &nMsg, true, false, true) == SHOW_MSG_RESULT_STOP)
					theBond.InformAutoBondStop();
			}
		}
		if (!theDebug.IsDebugVisionTiming())
			stTempSrchRes.dScore = 0;

		if (iErr == VISION_ERROR_NONE)
		{
			stTempSrchRes.dOffsetX = stSrchRes[i].dOffsetX;
			stTempSrchRes.dOffsetY = stSrchRes[i].dOffsetY;
			stTempSrchRes.dScore = stSrchRes[i].dScore;
			stTempSrchRes.dAngle = -stSrchRes[i].dRotateAngle * PI / 180.0;

			//v3.0.T129 add
			if (theDebug.IsDebugLogImage())
			{
				double dAngle = stSrchRes[i].dRotateAngle;
				if (dAngle > 180) dAngle -= 360;
				else if (dAngle < -180) dAngle += 360;

				if ((stTempSrchRes.dScore < theMcConfig.GetLogImageScore())
					|| (abs(dAngle) > theMcConfig.GetLogImageAngle()))
				{
					//v3.0.T130 edit
					short nImageID = theVision.SaveImage();
					CString str;
					str.Format(_T("Img%d_S%.2f_A%.2f"), nImageID,
						stSrchRes[i].dScore, stSrchRes[i].dRotateAngle);
					theLog.WriteLine(str, true);
					theDebug.AddInfo(str);
				}
			}
			if (stTempSrchRes.dScore < nPassScore)
			{
				if (stTempSrchRes.dScore > thePgm.GetAutoSkipScore())
					iErr = VISION_ERROR_QUALITY_REJECT;
				else
				{
					iErr = VISION_ERROR_NO_PATTERN;
					return iErr;
				}
			}
			/*if ((abs(round(stTempSrchRes.dOffsetX * 2.0)) > nSrchRangeX)
				|| (abs(round(stTempSrchRes.dOffsetY * 2.0)) > nSrchRangeY))
				iErr = VISION_ERROR_QUALITY_REJECT;*/
		}
		else if (iErr < 0)
		{
			CString str;
			CStringList sList;
			sList.RemoveAll();

			if (iErr == -99)
			{
				str.Format(_T("Pattern validation expired"));
				sList.AddTail(str);
			}
			else
			{
				str.Format(_T("Pattern lost! Error = %d"), iErr);
				sList.AddTail(str);
			}
			theMsg.ShowMsg(HELP_MSG, sList);
			return VISION_ERROR_NO_PATTERN;
		}
		else
			iErr = VISION_ERROR_NO_PATTERN;
		pstSrchRes[i] = stTempSrchRes;
		if (iErr != VISION_ERROR_NONE)
		{
			nPatNum = i + 1;
			return iErr;
		}
	}
	return iErr;
}

//v3.1.T489 add
int CVisionSystem::GetSrchRegion(short* nSrchStartX, short* nSrchStartY, short* nSrchWidth,
	short* nSrchHt, short nNumOfCol, short nNumOfLine)
{
	short imageHeight = ZOOMED_IMG_HEIGHT;
	short imageWidth = ZOOMED_IMG_WIDTH;
	short baseHt = imageHeight / nNumOfLine;
	short baseWidth = imageWidth / nNumOfCol;
	short totalNum = nNumOfCol * nNumOfLine;
	nSrchStartX = new short[totalNum];
	nSrchStartY = new short[totalNum];
	nSrchWidth = new short[totalNum];
	nSrchHt = new short[totalNum];
	
	for (int i = 0; i < totalNum; i++)
	{
		short colIndex = i % nNumOfCol;
		short rowIndex = i / nNumOfCol;
		nSrchStartX[i] = colIndex * baseWidth;
		nSrchStartY[i] = rowIndex * baseHt;
		nSrchWidth[i] = baseWidth;
		nSrchHt[i] = baseHt;
	}
	return 0;
}

//v3.1.T489 add
int CVisionSystem::GetSrchRegion(short& nSrchStartX, short& nSrchStartY, short& nSrchWidth, short& nSrchHt,int index)
{
	nSrchStartX = 2;
	nSrchHt = ZOOMED_IMG_HEIGHT / 3;
	nSrchWidth = ZOOMED_IMG_WIDTH - 6;
	nSrchStartY = index * (nSrchHt);
	return 0;
}

void CVisionSystem::EnableVideo(bool bEnable)
{
    if (theMcConfig.IsBypassCamera()) return;

	if (m_bVideoState != bEnable)	//toggle implement
	{
		if (bEnable)
			theVision.StartLiveVideo();
		else
			theVision.StopLiveVideo();

		m_bVideoState = bEnable;
	}
}

void CVisionSystem::GrabImage(void)
{
	//if (theMcConfig.IsBypassCamera()) return;	//v3.0.T170 delete
    theVision.GrabImage();
}

void CVisionSystem::VisionOffsetToTablePos(int nLensMag, PR_SRCH_RESULT *pstSrchResult, TBL_POS *pstVisionOffset)
{
    double dx, dy;

    if (nLensMag == VISION_LOW_MAG)
    {
        dx = -pstSrchResult->dOffsetX * m_dLowMagFactorX;
        dy = -pstSrchResult->dOffsetY * m_dLowMagFactorY;
    }
    else
    {
        dx = -pstSrchResult->dOffsetX * m_dHighMagFactorX;
        dy = -pstSrchResult->dOffsetY * m_dHighMagFactorY;
    }

    pstVisionOffset->dx = dx * m_dVisionFactorCos - dy * m_dVisionFactorSin;
    pstVisionOffset->dy = dx * m_dVisionFactorSin + dy * m_dVisionFactorCos;
}

void CVisionSystem::TablePosToVisionOffset(int nLensMag, TBL_POS* pstTblPos, TBL_POS* pstVisionOffset)
{
    double dx, dy;
    dx = pstTblPos->dx * m_dVisionFactorCos + pstTblPos->dy * m_dVisionFactorSin;
    dy = pstTblPos->dy * m_dVisionFactorCos - pstTblPos->dx * m_dVisionFactorSin;

    if (nLensMag == VISION_LOW_MAG)
    {
	    if ((m_dLowMagFactorX == 0) || (m_dLowMagFactorY == 0))
		{
			AfxMessageBox(_T("Low mag factor error!"));
			return;
		}
        pstVisionOffset->dx = dx / m_dLowMagFactorX;
        pstVisionOffset->dy = dy / m_dLowMagFactorY;
    }
    else
    {
	    if ((m_dHighMagFactorX == 0) || (m_dHighMagFactorY == 0))
		{
			AfxMessageBox(_T("High mag factor error!"));
			return;
		}
        pstVisionOffset->dx = dx / m_dHighMagFactorX;
        pstVisionOffset->dy = dy / m_dHighMagFactorY;
    }
}

int CVisionSystem::ShowVisionPattern(short nPatRcd)
{
    if (theMcConfig.IsBypassCamera()) return FOK;
	return theVision.DrawPatternOnOverlay(nPatRcd);
}

void CVisionSystem::ClearDrawing(void)
{
	if (theMcConfig.IsBypassCamera()) return;
	theVision.ClearOverlay();
}

void CVisionSystem::SetPassScore(short nPatRcd, short nPassScore)
{
    if (theMcConfig.IsBypassCamera()) return;
	theVision.SetPassScore(nPatRcd, nPassScore);
}

void CVisionSystem::ClearBondVisionPattern(void)
{
    if (theMcConfig.IsBypassCamera()) return;
	theVision.ClearAllPatternRcd(VISION_CLEAR_BOND_PR);
}

void CVisionSystem::ClearAllPatternRcd()
{
    if (theMcConfig.IsBypassCamera()) return;
	theVision.ClearAllPatternRcd(VISION_CLEAR_ALL);
}

void CVisionSystem::ClearAllIndexPrPattern(void)
{
    if (theMcConfig.IsBypassCamera()) return;
	theVision.ClearAllPatternRcd(VISION_CLEAR_INDEX_PR);
}

void CVisionSystem::ClearPatternRcd(short nPatRcd)
{
    if (theMcConfig.IsBypassCamera()) return;
	theVision.ClearPatternRcd(nPatRcd);
}

int CVisionSystem::SavePatternRcd(void)	//v3.0.T70 edit
{
    if (theMcConfig.IsBypassCamera()) return FOK;

	int iErr = theVision.SavePatternRcd();
	if (iErr != FOK)
	{
		CString szString;
		szString.Format(_T("SavePatternRcd error: %d"), iErr);
		::AfxMessageBox(szString);
	}

	return iErr;
}

int CVisionSystem::LoadPatternRcd(void)	//v3.0.T70 edit
{
    if (theMcConfig.IsBypassCamera()) return FOK;

	int iStatus = theVision.LoadPatternRcd();
	if (iStatus != FOK)
	{
		CString sMsg;
		sMsg.Format(_T("LoadPatternRcd error: %d"), iStatus);
		::AfxMessageBox(sMsg);
	}

	const PGM_RECORD *pstPgm = thePgm.GetRcd();
	SetCameraExposureTime(pstPgm->nExposureTime);
	SetRotateAngle(pstPgm->nPrRotateAngle);
	SetCameraParameters((BYTE)pstPgm->nRedGain,
					pstPgm->nContrast, pstPgm->nBrightness, 0);
	SetImageProcess(pstPgm->nPrFilterId);

	return FOK;
}

void CVisionSystem::GetPatternRcd(short nPatRcd, ST_PATTERN_RECORD *pstPatRcd)
{
    theVision.GetPatternRcd(nPatRcd, pstPatRcd);
}

CString CVisionSystem::GetVersion(void)
{
    return (theVision.GetVersion());
}

int CVisionSystem::SetCameraExposureTime(short nTime_x1us)
{
    if (theMcConfig.IsBypassCamera()) return FOK;
	return theVision.SetCameraExposureTime(nTime_x1us);
}

int CVisionSystem::SetCameraParameters(byte nRedGain, short nContrast, short nBrightness, short nkeepPos) 
{
    if (theMcConfig.IsBypassCamera()) return FOK;

	int nErr = FOK;
	if (theMcConfig.GetCameraType() == CAMERA_MSZ300_USB)
		nErr = theVision.SetImageParameter(nRedGain, nContrast, nBrightness, 0);
	return nErr;
}

void CVisionSystem::DrawLine( TBL_POS *pCurTablePos, short nCurWire)
{
    TBL_POS stStartPos, stEndPos, st1stPadPos, stPadOffset;
    double dFactorX, dFactorY;
    TBL_POS stLeftTopWnd, stBottomRightWnd, stOffset, stPadSize;
    TBL_POS stTrackOffset, st1stBondPt, st2ndBondPt, stVisionOffset;
    PR_SRCH_RESULT stSrchResult;
    WIRE_RECORD *pstWire;
    short i, j, nTrackNum, nTotalWire, nTaught1stBondPt, nTaught2ndBondPt;
    AUTO_PAD_TYPE stPadRcd;

    if (m_bDrawWire)
    {
        stSrchResult.dAngle = 0;
        stSrchResult.dOffsetX = -VISION_VIEW_WIDTH / 2;
        stSrchResult.dOffsetY = -VISION_VIEW_HEIGHT / 2;
        stSrchResult.dScale   = 1;
        stSrchResult.dScore   = 1;
        VisionOffsetToTablePos(VISION_LOW_MAG, &stSrchResult, &stOffset);

        // transfer pixal to xy 
        stLeftTopWnd.dx     = -stOffset.dx;
        stLeftTopWnd.dy     = -stOffset.dy;
        stBottomRightWnd.dx = stOffset.dx;
        stBottomRightWnd.dy = stOffset.dy;

        theVisionSys.ClearDrawing();

        if (thePgm.IsProgramValid() == true)
        {
		    PGM_RECORD *pstPgm = thePgm.GetRcd();

            if (pstPgm->nTrackType == DUAL_TRACK)
                nTrackNum = 2;
            else
                nTrackNum = 1;

            nTotalWire = theWire.GetTotalWireNum();
            nTaught1stBondPt = theWire.Get1stBondTaughtPts();
            nTaught2ndBondPt = theWire.Get2ndBondTaughtPts();

            nTotalWire = min(nTaught1stBondPt, nTaught2ndBondPt);

            for (j = 0; j < nTrackNum; j++)
            {
                stTrackOffset.dx = pstPgm->stTrackOffset.dx * j;
                stTrackOffset.dy = pstPgm->stTrackOffset.dy * j;

				bool bPointValid;
                for (i = 1; i <= nTotalWire; i++)
                {
					bPointValid = false;

                    pstWire = theWire.GetRcd(i);
                    theWire.CalcBondPoint(i, &st1stBondPt, &st2ndBondPt);

                    st1stBondPt.dx -= pCurTablePos->dx;
                    st1stBondPt.dy -= pCurTablePos->dy;
                    st2ndBondPt.dx -= pCurTablePos->dx;
                    st2ndBondPt.dy -= pCurTablePos->dy;

                    if ((st1stBondPt.dx >= stLeftTopWnd.dx)
						&& (st1stBondPt.dx <= stBottomRightWnd.dx)
						&& (st1stBondPt.dy >= stLeftTopWnd.dy)
						&& (st1stBondPt.dy <= stBottomRightWnd.dy))
                    {
                        bPointValid = true;
                    }

                    if ((st2ndBondPt.dx >= stLeftTopWnd.dx)
						&& (st2ndBondPt.dx <= stBottomRightWnd.dx)
						&& (st2ndBondPt.dy >= stLeftTopWnd.dy)
						&& (st2ndBondPt.dy <= stBottomRightWnd.dy))
                    {
                        bPointValid = true;
                    }

                    if (bPointValid)
                    {
                        /* if (lLensMag == VISION_LOW_MAG)
                        {*/
                        dFactorX = m_dLowMagFactorX;
                        dFactorY = m_dLowMagFactorY;
                        /*}*/
                        /*	else
                        {
                        dFactorX = m_dHighMagFactorX;
                        dFactorY = m_dHighMagFactorY;
                        }*/

                        //TBL_POS stVisionOffset;

                        //stStartPos.dx = -round(st1stBondPt.dx / dFactorX)+VISION_VIEW_WIDTH/2;
                        //stStartPos.dy = -round(st1stBondPt.dy / dFactorY)+VISION_VIEW_HEIGHT/2;
                        TablePosToVisionOffset(VISION_LOW_MAG, &st1stBondPt, &stVisionOffset);
                        stStartPos.dx = -round(stVisionOffset.dx) + VISION_VIEW_WIDTH/2;
                        stStartPos.dy = -round(stVisionOffset.dy) + VISION_VIEW_HEIGHT/2;

                        //stEndPos.dx = -round(st2ndBondPt.dx / dFactorX)+VISION_VIEW_WIDTH/2;
                        //stEndPos.dy = -round(st2ndBondPt.dy / dFactorY)+VISION_VIEW_HEIGHT/2;
                        TablePosToVisionOffset(VISION_LOW_MAG, &st2ndBondPt, &stVisionOffset);
                        stEndPos.dx = -round(stVisionOffset.dx) + VISION_VIEW_WIDTH/2;
                        stEndPos.dy = -round(stVisionOffset.dy) + VISION_VIEW_HEIGHT/2;

                        if (pstWire->chPadType[0] > 0)
                        {
                            theAutoPadMgr.GetRcd(pstWire->chPadType[0] - 1, &stPadRcd);
                            stPadSize.dx = stPadRcd.nPadWidth;
                            stPadSize.dy = stPadRcd.nPadHeight;
                            TablePosToVisionOffset(VISION_LOW_MAG, &stPadSize, &stVisionOffset);

                            st1stPadPos = stStartPos;
                            stPadOffset.dx = pstWire->n1stBondPtPadOffsetX / theServo.GetResolXY();
                            stPadOffset.dy = pstWire->n1stBondPtPadOffsetY / theServo.GetResolXY();
                            TablePosToVisionOffset(VISION_LOW_MAG, &stPadOffset, &stPadOffset);

                            st1stPadPos.dx += stPadOffset.dx;
                            st1stPadPos.dy += stPadOffset.dy;

                            theVision.DrawRect(
                                round(st1stPadPos.dx - stVisionOffset.dx / 2.0),
                                round(st1stPadPos.dy - stVisionOffset.dy / 2.0),
                                round(st1stPadPos.dx + stVisionOffset.dx / 2.0),
                                round(st1stPadPos.dy + stVisionOffset.dy / 2.0),
                                pstWire->chPadType[0] - 1, 0);
                        }

                        if (pstWire->chPadType[1] > 0)
                        {
                            theAutoPadMgr.GetRcd(pstWire->chPadType[1] - 1, &stPadRcd);
                            stPadSize.dx = stPadRcd.nPadWidth;
                            stPadSize.dy = stPadRcd.nPadHeight;
                            TablePosToVisionOffset(VISION_LOW_MAG, &stPadSize, &stVisionOffset);

                            theVision.DrawRect(
                                round(stStartPos.dx - stVisionOffset.dx / 2.0),
                                round(stStartPos.dy - stVisionOffset.dy / 2.0),
                                round(stStartPos.dx + stVisionOffset.dx / 2.0),
                                round(stStartPos.dy + stVisionOffset.dy / 2.0),
                                pstWire->chPadType[0] - 1, 0);
                        }

                        if (nCurWire == i)
                            theVision.UpdateWireLayout(
										round(stStartPos.dx),
										round(stStartPos.dy),
										round(stEndPos.dx),
										round(stEndPos.dy), i, 1);
                        else
                            theVision.UpdateWireLayout(
										round(stStartPos.dx),
										round(stStartPos.dy),
										round(stEndPos.dx),
										round(stEndPos.dy), i, 0);
                    }
                }

                if (nTaught1stBondPt > nTotalWire)
                {
					bool bPointValid;
                    for (i = nTotalWire + 1; i <= nTaught1stBondPt; i++)
                    {
						bPointValid = false;

                        pstWire = theWire.GetRcd(i);
                        st1stBondPt = pstWire->stTchPos[0];

                        st1stBondPt.dx -= pCurTablePos->dx;
                        st1stBondPt.dy -= pCurTablePos->dy;

                        if ((st1stBondPt.dx >= stLeftTopWnd.dx) &&
                            (st1stBondPt.dx <= stBottomRightWnd.dx) &&
                            (st1stBondPt.dy >= stLeftTopWnd.dy) &&
                            (st1stBondPt.dy <= stBottomRightWnd.dy))
                        {
                            bPointValid = true;
                        }

                        if (bPointValid)
                        {
                            TablePosToVisionOffset(VISION_LOW_MAG, &st1stBondPt, &stVisionOffset);
                            stStartPos.dx = -round(stVisionOffset.dx) + VISION_VIEW_WIDTH/2;
                            stStartPos.dy = -round(stVisionOffset.dy) + VISION_VIEW_HEIGHT/2;

                            if (pstWire->chPadType[0] > 0)
                            {
                                theAutoPadMgr.GetRcd(pstWire->chPadType[0] - 1, &stPadRcd);
                                stPadSize.dx = stPadRcd.nPadWidth;
                                stPadSize.dy = stPadRcd.nPadHeight;
                                TablePosToVisionOffset(VISION_LOW_MAG, &stPadSize, &stVisionOffset);

                                st1stPadPos = stStartPos;
                                stPadOffset.dx = pstWire->n1stBondPtPadOffsetX / theServo.GetResolXY();
                                stPadOffset.dy = pstWire->n1stBondPtPadOffsetY / theServo.GetResolXY();
                                TablePosToVisionOffset(VISION_LOW_MAG, &stPadOffset, &stPadOffset);

                                st1stPadPos.dx += stPadOffset.dx;
                                st1stPadPos.dy += stPadOffset.dy;

                                theVision.DrawRect(
                                    round(st1stPadPos.dx - stVisionOffset.dx / 2.0),
                                    round(st1stPadPos.dy - stVisionOffset.dy / 2.0),
                                    round(st1stPadPos.dx + stVisionOffset.dx / 2.0),
                                    round(st1stPadPos.dy + stVisionOffset.dy / 2.0),
                                    pstWire->chPadType[0] - 1, i);
                            }
                        }
                    }
                }

                if (nTaught2ndBondPt > nTotalWire)
                {
					bool bPointValid;
                    for (i = nTotalWire + 1; i <= nTaught2ndBondPt; i++)
                    {
						bPointValid = false;

                        pstWire = theWire.GetRcd(i);
                        st2ndBondPt = pstWire->stTchPos[1];

                        st2ndBondPt.dx -= pCurTablePos->dx;
                        st2ndBondPt.dy -= pCurTablePos->dy;

                        if ((st2ndBondPt.dx >= stLeftTopWnd.dx) &&
                            (st2ndBondPt.dx <= stBottomRightWnd.dx) &&
                            (st2ndBondPt.dy >= stLeftTopWnd.dy) &&
                            (st2ndBondPt.dy <= stBottomRightWnd.dy))
                        {
                            bPointValid = true;
                        }

                        if (bPointValid)
                        {
                            TablePosToVisionOffset(VISION_LOW_MAG, &st1stBondPt, &stVisionOffset);
                            stStartPos.dx = -round(stVisionOffset.dx) + VISION_VIEW_WIDTH/2;
                            stStartPos.dy = -round(stVisionOffset.dy) + VISION_VIEW_HEIGHT/2;

                            if (pstWire->chPadType[1] > 0)
                            {
                                theAutoPadMgr.GetRcd(pstWire->chPadType[1] - 1, &stPadRcd);
                                stPadSize.dx = stPadRcd.nPadWidth;
                                stPadSize.dy = stPadRcd.nPadHeight;
                                TablePosToVisionOffset(VISION_LOW_MAG, &stPadSize, &stVisionOffset);

                                theVision.DrawRect(
                                    round(stStartPos.dx - stVisionOffset.dx / 2.0),
                                    round(stStartPos.dy - stVisionOffset.dy / 2.0),
                                    round(stStartPos.dx + stVisionOffset.dx / 2.0),
                                    round(stStartPos.dy + stVisionOffset.dy / 2.0),
                                    pstWire->chPadType[1] - 1, i);
                            }
                        }
                    }
                }
            }
        }
    }
}
bool CVisionSystem::IsDrawWire(void)   
{
	return m_bDrawWire;
}

void CVisionSystem::SetDrawWire(bool bDraw)
{
    m_bDrawWire = bDraw;
}

void CVisionSystem::DrawCircleforBTO(long lRadius)
{
    if (theMcConfig.IsBypassCamera()) return;

	ClearDrawing();
	theVision.DrawCircle(VISION_VIEW_WIDTH / 2 - 1,
						VISION_VIEW_HEIGHT / 2 - 1, lRadius);
}

bool CVisionSystem::IsEditPatternMask(void)	//v1.5.T138 edit
{
    return m_bEditPatternMask;
}

void CVisionSystem::EditPatternMask(short nPixelX, short nPixelY)
{
    if (m_bEditPatternMask)
    {
        if (theVision.IsMaskBufferValid())
        {
            short nMaskPixelX, nMaskPixelY;
            nMaskPixelX = nPixelX - (ZOOMED_IMG_WIDTH - m_nPatSizeX) / 2;
            nMaskPixelY = nPixelY - (ZOOMED_IMG_HEIGHT - m_nPatSizeY) / 2;

            if ((nMaskPixelX - PATTERN_MASK_BRUSH_SIZE >= 0) &&
                (nMaskPixelX + PATTERN_MASK_BRUSH_SIZE <= m_nPatSizeX) &&
                (nMaskPixelY - PATTERN_MASK_BRUSH_SIZE >= 0) &&
                (nMaskPixelY + PATTERN_MASK_BRUSH_SIZE <= m_nPatSizeY))
            {
                theVision.MaskBufferPoint(nMaskPixelX, nMaskPixelY);
                theVision.DisplayMask(nPixelX, nPixelY);
            }
        }
    }
}

void CVisionSystem::PrepareEditPatternMask(short nPatSizeX, short nPatSizeY)
{
    m_nPatSizeX = nPatSizeX;
    m_nPatSizeY = nPatSizeY;

	int iStatus = theVision.AllocMaskBuffer(m_nPatSizeX, m_nPatSizeY);
    if ((iStatus == FOK) && theVision.IsMaskBufferValid())
        m_bEditPatternMask = true;
    else
        m_bEditPatternMask = false;
}

int CVisionSystem::SetRotateAngle(short nAngle)
{
    if (theMcConfig.IsBypassCamera()) return FOK;
	return theVision.SetRotateAngle(nAngle);
}

void CVisionSystem::SetImageProcess(short nProcess)	//v3.0.T78 rename
{
    if (theMcConfig.IsBypassCamera()) return;
	theVision.SetImageProcess(nProcess);
}

void CVisionSystem::DrawRect(int iStartX, int iStartY, int iEndX, int iEndY, short nColor, short nWireNum)
{
    if (theMcConfig.IsBypassCamera()) return;
	theVision.DrawRect(iStartX, iStartY, iEndX, iEndY, nColor, nWireNum);
}

void CVisionSystem::UpdateWireLayout(int iStartX, int iStartY, int iEndX, int iEndY, short nWireNum, short nColor)
{
    if (theMcConfig.IsBypassCamera()) return;
	theVision.UpdateWireLayout(iStartX, iStartY, iEndX, iEndY, nWireNum, nColor);
}

void CVisionSystem::UpdateBondPtLayout(int iStartX, int iStartY, short nBondPt, short nWireNum, short nColor)
{
    if (theMcConfig.IsBypassCamera()) return;
	theVision.UpdateBondPtLayout(iStartX, iStartY, nBondPt, nWireNum, nColor);
}

void CVisionSystem::BinaryAutoThreshold(short *pnThreshold)
{
    if (theMcConfig.IsBypassCamera()) return;
	theVision.WaitImageTransfer();
	theVision.BinaryAutoThreshold(pnThreshold);
}

int CVisionSystem::LocateLeadCenter(short nOrgX, short nOrgY,
		double dOrgAngle, short nThreshold, short nTipOffset,
		double *pdCenterX, double *pdCenterY)
{
	if (theMcConfig.IsBypassCamera()) return FOK;
	return theVision.LocateLeadCenter(nOrgX, nOrgY, dOrgAngle,
				nThreshold, nTipOffset, pdCenterX, pdCenterY);
}

void CVisionSystem::SetIdleLight(void)
{
	MACHINE_DATA_LIGHT* pstLight = theMcData.GetLightRcd();
	UpdateLightingParameter(pstLight->stIdleLight.nCoaxRedLight,
							pstLight->stIdleLight.bSideRedLight,
							pstLight->stIdleLight.nCoaxBlueLight,
							pstLight->stIdleLight.bSideBlueLight);
}

int CVisionSystem::SetAgcAec(bool bAgc, bool bAec)
{
    if (theMcConfig.IsBypassCamera()) return FOK;
	return theVision.SetAgcAec(bAgc, bAec);
}

double CVisionSystem::GetImageQuality(short nWidth, short nHeight)
{
	return theVision.GetImageQuality(nWidth, nHeight);
}

BYTE CVisionSystem::GetCentrePos(BYTE nAlgorithm, BYTE nGapFill, BYTE nBinarize, BYTE nCircleFit, TBL_POS *pstOffset)
{
	if (pstOffset == NULL) return FALSE;

	ZeroMemory(pstOffset, sizeof(TBL_POS));
	BYTE nWidth = (BYTE)m_nPatSizeX;
	BYTE nHeight = (BYTE)m_nPatSizeY;

	if (nAlgorithm == ALGORITHM_BLOB_PLUS_EDGE)
	{
		if (theVision.GetCentrePos(TRUE, nWidth, nHeight, TRUE, pstOffset) == TRUE)
			return TRUE;
		return theVision.GetCentrePos(FALSE, nWidth, nHeight, nGapFill, nBinarize, nCircleFit, TRUE, pstOffset);
	}
	else if (nAlgorithm == ALGORITHM_EDGE_PLUS_BLOB)
	{
		if (theVision.GetCentrePos(TRUE, nWidth, nHeight, nGapFill, nBinarize, nCircleFit, TRUE, pstOffset) == TRUE)
			return TRUE;
		return theVision.GetCentrePos(FALSE, nWidth, nHeight, TRUE, pstOffset);
	}
	else if (nAlgorithm == ALGORITHM_BLOB)
		return theVision.GetCentrePos(TRUE, nWidth, nHeight, TRUE, pstOffset);
	else if (nAlgorithm == ALGORITHM_EDGE)
		return theVision.GetCentrePos(TRUE, nWidth, nHeight, nGapFill, nBinarize, nCircleFit, TRUE, pstOffset);
	
	return FALSE;
}

void CVisionSystem::DrawRectangle()
{
	if ((m_nPatSizeX == 0) || (m_nPatSizeY == 0)) return;
	theVision.DrawRectangle(m_nPatSizeX, m_nPatSizeY);
}

//v3.0.T122 add
void CVisionSystem::SetShowResult(bool bShow)
{
	theVision.SetShowResult(bShow);
	if (bShow) theBond.StartShowPrTimer();
}

//v3.0.T123 add
bool CVisionSystem::IsShowResult()
{
	return theVision.IsShowResult();
}

//v3.0.T122 add
void CVisionSystem::ToggleShowResult()
{
	theVision.ToggleShowResult();
	if (theVision.IsShowResult()) theBond.StartShowPrTimer();
}

//v3.0.T313 add
double CVisionSystem::GetLowMagFactorX()
{
	return m_dLowMagFactorX;
}

//v3.0.T313 add
double CVisionSystem::GetLowMagFactorY()
{
	return m_dLowMagFactorY;
}
