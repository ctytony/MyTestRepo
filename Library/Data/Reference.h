//Reference.h v1.1.T11

#pragma once

#include "DataStruct.h"

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct REF_RECORD
{
	short       nRefNum;
    short       nRefType;
    bool        bCtactLvlValid;
    long        lCtactLvl;
    TBL_POS     stManuAlignPos[2];				//Manual alignment position
    TBL_POS     stVisionAlignPos[2];			//Vision Alignment position
    LIGHT_PARA  stManuAlignLightPara[2];
    LIGHT_PARA  stVisionLightPara[2];
    ULONG       ulManuAlignPatternID[2];
    ULONG       ulVisionAlignPatternID[2];
    short       nManuAlignMag[2];
    short       nVisionAlignMag[2];
    short       nManuSrchSizeX[2];
    short       nManuSrchSizeY[2];
    short       nVisionSrchSizeX[2];
    short       nVisionSrchSizeY[2];
    double      dManuSinAln;
    double      dManuCosAln;
    double      dVisionSinAln;
    double      dVisionCosAln;
    short       nLocateMode;					//Dual Point/Single Point
    bool        bVisionBkupLight[2];
    LIGHT_PARA  stVisionBkupLightPara[2];
    short       nVisionAlgorithm[2];			//grey level/binary
	char        cReserved[128];
	TBL_POS     stBtos;
	short       nBakUpIndex;
	BACK_UP_PR  stBakUpPr[MAX_BAK_UP_PR_NUM];	//85*4
	char        cReserve[10];

    //must init before use, or by default as 0
	REF_RECORD():
	    nRefNum(),
	    nRefType(),
	    bCtactLvlValid(),
	    lCtactLvl(),
	    stManuAlignPos(),
	    stVisionAlignPos(),
	    stManuAlignLightPara(),
	    stVisionLightPara(),
	    ulManuAlignPatternID(),
	    ulVisionAlignPatternID(),
	    nManuAlignMag(),
	    nVisionAlignMag(),
	    nManuSrchSizeX(),
	    nManuSrchSizeY(),
	    nVisionSrchSizeX(),
	    nVisionSrchSizeY(),
	    dManuSinAln(),
	    dManuCosAln(),
	    dVisionSinAln(),
	    dVisionCosAln(),
	    nLocateMode(),
	    bVisionBkupLight(),
	    stVisionBkupLightPara(),
	    nVisionAlgorithm(),
	    stBtos(),
	    nBakUpIndex(),
		stBakUpPr(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

#define CTACT_SRCH_START_LEVEL  1000

enum enRefLocateMode {REF_LOCATE_DUAL_POINT, REF_LOCATE_SINGLE_POINT};

// CReference command target

class CReferenceMgr : public CObject
{
public:
	CReferenceMgr();
	virtual ~CReferenceMgr();
public:	//functions
	void VerifyData(void);				//v1.0.T17 add, v1.0.T18 edit
	REF_RECORD* GetRcd(short nRefNum);	//v1.0.T5 add
    short GetTotalRefNum(void);
    void AddRcd(short nLocateMode);		//v1.0.T9 add
    int UpdateManuAlignPoint(short nRefNum, TBL_POS stPoint1, TBL_POS stPoint2);
    int GetRcd(REF_RECORD *pstRefRcd, short nRefNum);	//v1.0.T8 add
    void ClearAllRcd(void);	//v1.0.T20 rename
    void SaveRcd(void);		//v1.0.T20 rename
    void LoadRcd(void);		//v1.0.T20 rename
    int SetRcd(REF_RECORD *pstRefRcd, short nRefNum);	//v1.0.T8 add
    // Calculate reference angle for manual alignment and PR alignment
    void CalcRefAngle(short nRefNum, short nVisionMode);	//v1.0.T20 rename
    bool IsCtactValid(void);		//v1.0.T15 rename
	void ResetCtactValid(void);		//v1.0.T15 add
	int UpdateAllBtos(TBL_POS stBtos);
    long GetMinCtactLevel(void);
    void DeleteLastRcd();	//v1.0.T20 rename
	TBL_POS GetMultiPrPos(void);
	TBL_POS GetMultiPrPos(int nStartRef, int nEndRef);
private:	//variables
    short m_nTotalRef;		//v1.0.T14 rename
    REF_RECORD *m_pstRef;	//v1.0.T14 rename
};

extern CReferenceMgr theRef;