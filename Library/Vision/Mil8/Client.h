//Client.h v1.9.T1

#pragma once

#include "PatternMgr.h"
#include "windows.h"

#define  VISION_DATA_VERSION 1
#define  MAX_ANGLE_TOL_VALUE 20	//v1.8.T25 resume

#define IMAGE_THRESHOLD_VALUE 140L //v1.8.L59 add
#define MIN_BLOB_AREA         300L //v1.8.L59 add
#define MIN_BLOB_RADIUS       2L   //v1.8.L59 add

//v1.8.T2 add
enum EN_COLOR
{
	COLOR_GOLD,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_YELLOW,
	COLOR_MAGENTA,
	COLOR_CYAN,
	COLOR_LIGHT_GREEN,
	COLOR_LIGHT_BLUE,
	COLOR_DARK_RED,
	COLOR_DARK_GREEN,
	COLOR_DARK_BLUE,
	COLOR_DARK_YELLOW,
	COLOR_DARK_MAGENTA,
	COLOR_DARK_CYAN,
};

#pragma pack(push, 1)

struct VISION_PARAMETER
{
    short nRotateAngle;
    char  chRev[200];
};

#pragma pack(pop)

//enum CCD_TYPE{MIL_PCI_CCD, MSZ300_USB_CCD};	//v1.7.T27 delete

class CClient
{
public:
    CClient(void);
    ~CClient(void);
public:	//variables
	MIL_ID m_MilSystem;	//v1.8.T49 add
    MIL_ID m_GrabImage;	//v1.8.T54 rename
	MIL_ID m_FlipImage;	//v1.8.T54 rename
	MIL_ID m_DestImage;	//v1.8.T54 rename
	MIL_ID m_TempImage;	//v1.8.T54 rename
    MIL_ID m_MaskImage;	//v1.8.T54 rename
public:	//functions
	void LoadImage();				//v1.8.T57 add		
	short SaveImage();				//v1.8.T46 add, v1.8.T48 edit
	bool IsShowResult();			//v1.8.T43 add
	void ToggleShowResult();		//v1.8.T42 add
	void SetShowResult(bool bShow);	//v1.8.T40 add
    int Init(HWND hClientWnd);		//v1.8.T57 edit
    void FreeClientResource(void);
    void DrawCursor(void);
    void DrawRectangle(short nWidth, short nHeight);
	void DrawRect(int iStartX, int iStartY, int iEndX, int iEndY, short nColor, short nWireNum = 0);
    void ClearOverlay(bool bShowCursor = true);	//v1.8.T33 add
    void ShowOverlay(bool bEnable);
    void BinarizeImage(short nThreshold);
    int LearnPattern(bool bRotateEnable, short nPatWidth, short nPatHt, short *pnPatRcd, bool bIndexPR);	//v1.8.T12 edit
    int SearchPattern(short nPatRcd, short nSrchWidth, short nSrchHt, ST_SEARCH_PATTERN_RESULT* pstSrchRes, short nBackUpID);	//v1.8.T13 rename, v1.8.T16 edit
	//v1.9.T1 add
	int SearchPattern(short nPatRcd, short nSrchStartX, short nSrchStartY, short nSrchWidth,
        short nSrchHt, ST_SEARCH_PATTERN_RESULT* pstSrchRes, short nBackUpID, int showNum = -1);
    int SearchMultiPattern(short nPatNum, short nPatRcd, short* nSrchStartX, short* nSrchStartY, short* nSrchWidth,
        short* nSrchHt, ST_SEARCH_PATTERN_RESULT* pstSrchRes, short nBackUpID); //use MpatFindModel
    int SearchMultiPattern2(short nPatNum, short* nPatRcd, short nSrchStartX, short nSrchStartY, short nSrchWidth,
        short nSrchHt, ST_SEARCH_PATTERN_RESULT* pstSrchRes, short nBackUpID); //use MpatFindMultipleModel

	void ClearPatternRcd(short nPatRcd);	//v1.8.T10 edit, v1.8.T15 edit
    void SetPatternSearchRange(short nPatRcd, short nWidth, short nHeight);	//v1.8.T16 edit
    int SavePatternRcd(void);	//v1.8.T9 edit
    int LoadPatternRcd(void);	//v1.8.T9 edit
    void ClearAllPatternRcd(short nClearType);	//v1.8.T10 edit
    int DrawPatternOnOverlay(short nPatRcd);	//v1.8.T16 edit
    void SetPassScore(short nPatRcd, short nPassScore);	//v1.8.T16 edit, v1.8.T17 rename
    void GetPatternRcd(short nPatRcd, ST_PATTERN_RECORD *pstPatRcd);	//v1.8.T17 rename
    void SetDisplayImage(bool bEnable);
	// Display the currrent Ref wirelay out on the overlay
	void UpdateWireLayout(int iStartX, int iStartY, int iEndX, int iEndY, short nWireNum, short nColor);
	// Display bond point layout on the overlay
	void UpdateBondPtLayout(int iStartX, int iStartY, short nBondPt, short nWireNum, short nColor);
	void DrawCircle(long lXCenter, long lYCenter, long lRadius);
    int AllocMaskBuffer(short nWidth, short nHeight);
    bool IsMaskBufferValid(void);
    // Mask buffer image point
    int MaskBufferPoint(short nPixelX, short nPixelY);
    // Display mask on overlay
    int DisplayMask(short nPixelX, short nPixelY);
    int SetRotateAngle(short nAngle);
    short BinaryAutoThreshold(void);
    int LocateLeadCenter(short nOrgX, short nOrgY, double dOrgAngle, short nThreshold, 
                         short nTipOffset, double* pdCenterX, double* pdCenterY);
    void DisplayText(long XStart, long YStart, LPCTSTR psFormat, ...);
	void DrawBlob(MIL_ID midResult, long lOffX, long lOffY, long lSizeX, long lSizeY);	//V6.4 add
	void DrawEdge(MIL_ID midResult, long lOffX, long lOffY, long lSizeX, long lSizeY);	//V6.2 add
	int IsDieInverted(short nPatRcd, ST_SEARCH_PATTERN_RESULT* pstSrchRes);  //v1.8.L59 add, v1.8.T60 rename
protected:
	static DWORD WINAPI ThreadProc(LPVOID lpVoid);	//V7.4 add
private:	///variables
	MIL_ID m_MilApp;	//v1.8.T49 add
	bool m_bShowResult;	//v1.8.T40 add
    CString m_sDispStr;	//v1.8.T35 add
    char m_acStr[500];	//v1.8.T35 add
	MIL_ID m_MilDisplay;
    MIL_ID m_MilOverlayImage;
	MIL_ID m_MilROI;	//v1.7.T26 edit
	// Pattern match result
    MIL_ID m_MilResult;
    CPatternMgr m_PatternMgr;
    // Handler of display window
    HWND m_hClientWnd;
    short m_nRotateAngle;
    short m_nImageID;
	BYTE m_bRunning;	//V7.4 add
private:	//function
	//read results and draw a box around model occurrence
	void GetSrchResult(long lPatID, ST_SEARCH_PATTERN_RESULT *pstSrchRes);	//v1.8.T31 add, v1.8.T36 edit
	void GetSrchResult(long lPatID, long lResID, ST_SEARCH_PATTERN_RESULT *pstSrchRes);	//v1.9.T1 add
	void SetColor(EN_COLOR nColor);		//v1.8.T2 add
};
