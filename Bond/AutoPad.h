#pragma once

// CAutoPad command target

class CAutoPad : public CObject
{
public:
	CAutoPad();
	virtual ~CAutoPad();
    // Perform auto pad without PR during teach bond point
    int PerformAutoPadWithoutPR(void);
    // Get wire number of next auto pad
    int GetNextAutoPadWire(short nCurWireNum, short nBondPt, short* pnNextWireNum);
    // Perform auto pad with PR
    int PerformAutoPadWithPR(void);
};

extern CAutoPad theAutoPad;
