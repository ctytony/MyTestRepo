/***************************************************************
ServoTune.cpp : implementation file
****************************************************************/

#pragma once

// CPrAlign command target

class CPrAlign : public CObject
{
public:
	CPrAlign();
	virtual ~CPrAlign();
    // Update manual alignment point for edit 
    int UpdateManualAlignPoint(void);
    // Perform inteligent lead locate with or without PR
    int PerformInteliLeadLocate(bool bWithPR); //@1
    // Get next wire number for ILL
    int GetNextILLWire(short nCurWireNum, short nBondPt, short* pnNextWireNum);
};

extern CPrAlign thePrAlign;
