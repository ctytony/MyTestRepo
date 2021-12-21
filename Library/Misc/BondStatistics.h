//BondStatistics.h v1.0.T26

#pragma once

// CBondStatistics command target

#include "DataStruct.h"
#include "HTimer.h"

#pragma pack(push, 1)
#pragma warning( disable : 4351 )
struct BOND_STATISTICS_RCD
{
    SYSTEMTIME  stStatsStartTime;
    long        lOutputUnit;
    long        lBondedWire;
    long        lCapillaryCount;
    time_t      tProductionTime;	//v1.0.T24 rename
    time_t      tStandbyTime;
    time_t      tLastStartTime;
    short       nMachineState;
    short       nAssistCount;
    short       nFailureCount;
    short       nNsopCount;
    short       nNsolCount;
    short       nEfoOpenCount;
    short       nShortTailCount;
    short       nLeadPrErrorCount;
    short       nDiePrErrorCount;
    short       nMhsCount;
    short       nSkipDieCount;
	short       nWireFeedCount;	//v1.0.W3 add
    char        chReserve[46];	//v1.0.W3 edit 48->46

    //must init before use, or by default as 0
	BOND_STATISTICS_RCD():
		stStatsStartTime(),
		lOutputUnit(),
		lBondedWire(),
		lCapillaryCount(),
		tProductionTime(),		//v1.0.T24 rename
		tStandbyTime(),
		tLastStartTime(),
		nMachineState(),
		nAssistCount(),
		nFailureCount(),
		nNsopCount(),
		nNsolCount(),
		nEfoOpenCount(),
		nShortTailCount(),
		nLeadPrErrorCount(),
		nDiePrErrorCount(),
		nMhsCount(),
		nSkipDieCount(),
		nWireFeedCount(){}
};
#pragma warning( default : 4351 )
#pragma pack(pop)

enum EN_MACHINE_STATE	//v1.0.T23 rename
{
	MCSTATE_STANDBY,
	MCSTATE_PRODUCTION,
};

enum EN_STAT_ERROR		//v1.0.T23 rename
{
	STATERR_OK,
	STATERR_NSOP,
	STATERR_NSOL,
	STATERR_EFO_OPEN,
	STATERR_SHORT_TAIL,
	STATERR_LEAD_PR,
	STATERR_DIE_PR,
	STATERR_MHS,
	STATERR_SKIP_DIE,
	STATERR_WIRE_FEED,	//v1.0.W3 add
	STATERR_MAX,
};

class CBondStatistics : public CObject
{
public:
	CBondStatistics();
	virtual ~CBondStatistics();
public:	//functions
    BOND_STATISTICS_RCD *GetRcd();	//v1.0.T18 add
    void ResetRcd(EN_STAT_ERROR nErr = STATERR_OK);	//v1.0.W3 edit, v1.0.T17 edit
    void SetMachineState(EN_MACHINE_STATE nState, EN_STAT_ERROR nErr = STATERR_OK);
    void SetBondedUnit(short nBondedUnit, short nBondedWire, short nCapCount);	//v1.0.T24 rename
    void SaveRcd(void);	//v1.0.T17 edit
    void LoadRcd(void);	//v1.0.T17 edit
    void ResetCapillaryCount(void);
    void ResetWireUsageCount(void);
    bool IsCapillaryWearOut(void);	//v1.0.T17 edit
    void SetUnitStartTime(short nStartUnit);
    void SetUnitEndTime(short nLeadFrameUnits);
    long GetNetUph(void);
private:	//variable
	void LogStat(void);			//v1.0.T22 add
    // Unit number of start bonding
    short m_nStartUnit;
    BOND_STATISTICS_RCD m_stStat;	//v1.0.T16 edit
	CHTimer m_UnitTimer;		//v1.0.T8 add
    short m_nUpdatedUnitNum;
    long m_lNetUph;
private:	//function
    void SetTimeDuration(void);	//v1.0.T24 add
    void GetRcd(BOND_STATISTICS_RCD *pstStat);	//v1.0.T17 edit
};

extern CBondStatistics theStat;	//v1.0.T19 edit
