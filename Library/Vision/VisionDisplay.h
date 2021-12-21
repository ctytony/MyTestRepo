//VisionDisplay.h v1.1.T18

#pragma once

#include "MacDef.h"
#include "DataStruct.h"

enum VISION_DISPLAY_MODE
{
    VISION_DISPLAY_NORMAL_MODE,
    VISION_DISPLAY_TEACH_MODE,
};

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct VISION_DISPLAY_CONFIG
{
    bool bPrAlignPoint;
    bool bBondPoint;
    bool bWire;
    bool bPad;
    bool bWireNum;
    char cReserve[15];

    //must init before use, or by default as 0
	VISION_DISPLAY_CONFIG():
	    bPrAlignPoint(),
	    bBondPoint(),
	    bWire(),
	    bPad(),
		bWireNum(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

class CVisionDisplay : public CObject
{
public:
	CVisionDisplay();
	virtual ~CVisionDisplay();
public:	//functions
    void EnableDisplay(bool bEnable);
    void UpdateVisionDisplay(bool bForceUpdate = false);	//update vision display corresponding to display mode
    void UpdateDieDisplay(TBL_POS wndStPos, TBL_POS wndEndPos, TBL_POS stTablePos);
    void SetDisplayBondPt(short nBondPt);
    void SetDisplayMode(char cDisplayMode);	//for normal or teaching mode
    void SetDisplayWire(short nWireNum);
	void SetMatrixRowOffset(char nRowOffset);	//V1.0.W9 add
private:	//variables
    bool m_bEnableDisplay;
    char m_cDisplayMode;
	char m_nRowOffset;		//V1.0.W9 add
    short m_nCurBondPt;
    short m_nCurWire;
    TBL_POS m_stOrgTblPos;
    VISION_DISPLAY_CONFIG m_stDisplayConfig;
};

extern CVisionDisplay theVisionDisplay;
