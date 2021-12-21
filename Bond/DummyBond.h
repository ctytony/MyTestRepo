#pragma once

#define AUTO_UNPLUG_TIMES	5
#define DUMMY_BOND_PITCH	100 //um

// CDummyBond command target

class CDummyBond : public CObject
{
public:
	CDummyBond();
	virtual ~CDummyBond();
    int PerformDummyBond(char cFabType, TBL_POS stBondPos, bool bPopMsg = true);	//v3.1.T207 edit
    void SetUsgInfo(void);
    void SendUsgInfo(void);
    int GenerateDummyBondInfo(void);
private:
	WORD m_anUsgTime1[USG_1ST_BOND_SEG_NUM];
	BYTE m_anUsgPower1[USG_1ST_BOND_SEG_NUM];
	WORD m_anUsgTime2[USG_2ND_BOND_SEG_NUM];
	BYTE m_anUsgPower2[USG_2ND_BOND_SEG_NUM];
	BYTE m_anIndex[2];
    char m_cFabType;
    //Auto unplug index
    short m_nDummyBondPointIndex;
public:
    // Capillary auto unplug
    int CapillaryUnplug(void);
};

extern CDummyBond theDummyBond;
