//PatternMgr.h v1.8.T12

#pragma once

#include "Mil.h"

class CPatternMgr
{
public:
    CPatternMgr(void);
    ~CPatternMgr(void);
private:	//variable
    short m_nTotalPattern;	//v1.8.T8 rename
    ST_PATTERN_RECORD m_stPatRcd[MAX_VISION_PATTERN];	//v1.8.T8 rename
public:		//function
    ST_PATTERN_RECORD *GetRcd(short nPatRcd);	//v1.8.T10 add, v1.8.T12 edit
    int AddRcd(long lPatID, short *pnPatRcd, bool bIndexPR);	//v1.8.T10 edit, v1.8.T12 edit
    int ClearRcd(short nPatRcd);	//v1.8.T10 edit, v1.8.T12 edit
    void ClearAllRcd(short nClearType);	//v1.8.T10 edit
    int SaveRcd(void);	//v1.8.T9 edit
    int LoadRcd(MIL_ID lMilSystem);	//v1.8.T9 edit
    int GetPatID(short nPatRcd, MIL_ID *plPatID);	//v1.8.T9 edit, v1.8.T12 edit
    void SetAllPassScore(short nPassScore);	//v5.5
    void SetPassScore(short nPatRcd, short nPassScore);	//v1.8.T11 edit, v1.8.T12 edit
    void GetRcd(short nPatRcd, ST_PATTERN_RECORD *pstPatRcd);	//v1.8.T10 edit, v1.8.T12 edit
};