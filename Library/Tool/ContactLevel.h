//ContactLevel.h v1.0.T32

#pragma once

#define CTACT_LOG_NUM	10

#pragma pack(push, 1)

struct ST_CTACT_LOG			//v1.0.T20 add
{
	BYTE	cIdx;	//v1.0.T30 edit
	bool	abValid[CTACT_LOG_NUM];
	int		anCtactLvl[CTACT_LOG_NUM];
};

struct ST_CTACT_STAT		//v1.0.T3 edit
{
    BYTE	cSampleSize;	//v1.0.T30 edit
    int		nCtactLvl;		//v1.0.T19 edit
    float	fMean;			//v1.0.T19 edit
    //float	fStdDev;		//v1.0.T22 delete
};

#pragma pack(pop)

//#define CTACT_WIRE_BUFFER	500  //Only support total wire number 499
//#define CTACT_SAMPLE_SIZE	2    //Minimum 2 sample is requested for valid contact level
//#define CTACT_SAFE_OFFSET	100  //Offset for safe contact level when statistics invalid

// CCtactLvlMgr command target

class CCtactLvlMgr : public CObject
{
public:
	CCtactLvlMgr();
	virtual ~CCtactLvlMgr();
private:
    ST_CTACT_STAT m_astStat[CTACT_WIRE_BUFFER][2];		//v1.0.T21 add
    ST_CTACT_LOG m_astAddLog[CTACT_WIRE_BUFFER][2];		//v1.0.T20 add
    ST_CTACT_LOG m_astGetLog[CTACT_WIRE_BUFFER][2];		//v1.0.T20 add
	void LogAdd(short nWireNum, char cBondPt, int nCtactLvl, bool bValid);	//v1.0.T25 edit
	void LogGet(short nWireNum, char cBondPt, int nCtactLvl, bool bValid);
public:
    void ClearRcd(void);	//v1.0.T6 edit
    int AddRcd(short nWireNum, char cBondPt, int nCtactLvl);	//v1.0.T6 edit, v1.0.T31 edit
	bool GetCtactLvl(short nWireNum, char cBondPt, int *pnCtactLvl);	//v1.0.T6 edit
    ST_CTACT_STAT *GetRcd(short nWireNum, char cBondPt);	//v1.0.T23 edit
    ST_CTACT_LOG *GetAddLog(short nWireNum, char cBondPt);	//v1.0.T24 edit
    ST_CTACT_LOG *GetGetLog(short nWireNum, char cBondPt);	//v1.0.T24 edit
};

extern CCtactLvlMgr theCtact;
