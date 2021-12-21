//VisionSystem.h v3.0.T78

#pragma once

#include "VisionDef.h"
#include "Vision.h"
#include "VisionDisplay.h"

#define VISION_CALIB_STEP			420.0	//V1.1.W132 edit
#define VISION_CALIB_TOL			10

#define VISION_SEARCH_SIZE_X		512
#define VISION_SEARCH_SIZE_Y		384

#define VISION_SEARCH_BASE_WIDTH	86
#define VISION_SEARCH_BASE_HEIGHT	64
#define VISION_MAX_SEARCH_RANGE		8

#define FOXCONN_VISION_SEARCH_WIDTH  120	//v3.0.T321 add
#define FOXCONN_VISION_SEARCH_HEIGHT 100	//v3.0.T321 add

enum VISION_ERROR_CODE
{
	VISION_ERROR_NONE,
	VISION_ERROR_NO_PATTERN,
	VISION_ERROR_QUALITY_REJECT,
};

enum VISION_MAG_TYPE
{
	VISION_LOW_MAG,
	VISION_HIGH_MAG
};

// CVisionSystem command target

class CVisionSystem : public CObject
{
public:
	CVisionSystem();
	virtual ~CVisionSystem();
public:	//functions
	bool IsShowResult();			//v3.0.T123 add
	void ToggleShowResult();		//v3.0.T122 add
	void SetShowResult(bool bShow);	//v3.0.T122 add
    void BinaryAutoThreshold(short *pnThreshold);
    int LocateLeadCenter(short nOrgX, short nOrgY,
		double dOrgAngle, short nThreshold, short nTipOffset,
		double *pdCenterX, double *pdCenterY);
	void SetIdleLight(void);	//v2.0.T103 rename
    int SetAgcAec(bool bAgc, bool bAec);
    bool IsEditPatternMask(void);
    void EditPatternMask(short nPixelX, short nPixelY);
    void PrepareEditPatternMask(short nPatSizeX, short nPatSizeY);
    int SetRotateAngle(short nAngle);
    void GetSearchSize(short *nSrchSizeX, short *nSrchSizeY);
	int SetCameraParameters(byte nRedGain, short nContrast, short nBrightness, short nkeepPos = 0);
	void SetImageProcess(short nProcess);	//v3.0.T78 rename
	void DrawRect(int iStartX, int iStartY, int iEndX, int iEndY, short nColor, short nWireNum = 0);
	void UpdateWireLayout(int iStartX, int iStartY, int iEndX, int iEndY, short nWireNum, short nColor);
	void UpdateBondPtLayout(int iStartX, int iStartY, short nBondPt, short nWireNum, short nColor);
    void ClearDrawing(void);
	void DrawLine(TBL_POS *pCurTablePos, short nCurWire);
    void SetPassScore(short nPatRcd, short nPassScore);	//v3.0.T76 rename
    void ClearAllPatternRcd(void);		//v3.0.T76 rename
	void ClearAllIndexPrPattern(void);	//v3.0.T76 rename
    void ClearPatternRcd(short nPatRcd);
    int SavePatternRcd(void);	//v3.0.T70 edit
    int LoadPatternRcd(void);	//v3.0.T70 edit
    void GetPatternRcd(short nPatRcd, ST_PATTERN_RECORD *pstPatRcd);	//v3.0.T76 rename
    CString GetVersion(void);
    int SetCameraExposureTime(short nTime_x1us);
    bool IsDrawWire(void);			//v2.0.T104 rename
	void SetDrawWire(bool bDraw);	//v2.0.T104 rename
	void DrawCircleforBTO(long lRadius);
	void ClearBondVisionPattern(void);
	int Init(HWND hDispWnd);	//v1.6.T339 edit
	void GetVisionFactor(double *pdHighFactorX, double *pdHighFactorY, double *pdLowFactorX, double *pdLowFactorY, double *pdFactorCos, double *pdFactSin);
	void UpdateVisionFactor(int iCalibMag, double dMagFactX, double dMagFactY, double dFactCos, double dFactSin);
    void FreeVisionResource(void);
    void GetLightingParameter(short *pnCoaxRedLight, short *pnSideRedLight, short *pnCoaxBlueLight, short *pnSideBlueLight);
    int UpdateLightingParameter(short nCoaxRedLight, short nSideRedLight, short nCoaxBlueLight, short nSideBlueLight,bool bPowerOn = false);
    void GetBinaryThreshold(short *pnThreshold);
    void UpdateBinaryThreshold(short nThreshold);
    void ChangeLensMag(int nLensMag);
    void SetSearchSize(short nSrchSizeX, short nSrchSizeY);
    int LearnVisionPattern(bool bRotate, short nPatWidth, short nPatHeight, short *pnPatRcd, bool bIndexPR = false);	//v3.0.T73 edit
    int SearchVisionPattern(short nPatRcd, short nSrchRange, short nPassScore, PR_SRCH_RESULT *pstSrchRes, short nBackUpID = -1);
    int SearchVisionPattern(short nPatRcd, short nSrchRangeX, short nSrchRangeY, short nPassScore, PR_SRCH_RESULT *pstSrchRes, short nBackUpID = -1);

	//v3.1.T489 add
	int SearchVisionPattern(short nPatRcd, short nSrchStartX,short nSrchStartY,
		short nSrchWidth,short nSrchHt, short nPassScore, PR_SRCH_RESULT *pstSrchRes, short nBackUpID = -1, int showNum = -1);
	//whj: multi Model multi region
	int SearchMultiVisionPattern(short &nPatNum, short* nPatRcd, short* nSrchStartX, short* nSrchStartY, short* nSrchWidth, short* nSrchHt, 
        short* nPassScore, PR_SRCH_RESULT* pstSrchRes, short nBackUpID = -1);
	//single Model multi region
    int SearchMultiVisionPattern2(short& nPatNum, short nPatRcd, short* nSrchStartX, short* nSrchStartY, short* nSrchWidth, short* nSrchHt,
        short* nPassScore, PR_SRCH_RESULT* pstSrchRes, short nBackUpID = -1); //use SearchMultiPattern
	//multi Model single region
    int SearchMultiVisionPattern3(short& nPatNum, short* nPatRcd, short nSrchStartX, short nSrchStartY, short nSrchWidth, short nSrchHt,
        short nPassScore, PR_SRCH_RESULT* pstSrchRes, short nBackUpID = -1); //use SearchMultiPattern2
	int GetSrchRegion(short* nSrchStartX, short* nSrchStartY, short* nSrchWidth, short* nSrchHt, short nNumOfCol = 3, short nNumOfLine = 1);
	int GetSrchRegion(short& nSrchStartX, short& nSrchStartY, short& nSrchWidth, short& nSrchHt,int index = 1);

	void EnableVideo(bool bEnable);
    void GrabImage(void);
    void VisionOffsetToTablePos(int nLensMag, PR_SRCH_RESULT *pstSrchResult, TBL_POS *pstVisionOffset);
    void TablePosToVisionOffset(int nLensMag, TBL_POS *pstTblPos, TBL_POS *pstVisionOffset);
    int ShowVisionPattern(short nPatRcd);
	double GetImageQuality(short nWidth, short nHeight);
	BYTE GetCentrePos(BYTE nAlgorithm, BYTE nGapFill, BYTE nBinarize, BYTE nCircleFit, TBL_POS *pstOffset);
	void DrawRectangle();
	double GetLowMagFactorX();	//v3.0.T313 add
	double GetLowMagFactorY();	//v3.0.T313 add
private:	//variables
	int m_iCalibMag;	//Calibration Magnification
	double m_dHighMagFactorX;
	double m_dHighMagFactorY;
	double m_dLowMagFactorX;
	double m_dLowMagFactorY;
    short m_nCurCoaxRedLight;
    short m_nCurSideRedLight;
    short m_nCurSideBlueLight;
    short m_nCurCoaxBlueLight;
    short m_nBinaryThreshold;
    short m_nPatSizeX;
    short m_nPatSizeY;
    bool m_bVideoState;
	bool m_bDrawWire;	//v2.0.T104 rename
    bool m_bEditPatternMask;
    // Cosin value of angle between XY table and work holder
    double m_dVisionFactorCos;
    // Sin value of angle between table XY and work holder
    double m_dVisionFactorSin;
};

extern CVisionSystem theVisionSys;