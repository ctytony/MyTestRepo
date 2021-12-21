//AutoPadMgr.h v1.0.T12

#pragma once

#define MAX_AUTO_PAD_TYPE  10

enum PAD_PATTERN
{
    PAD_PATTERN_RECTANGLE,
    PAD_PATTERN_CIRCLE,
};

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct AUTO_PAD_TYPE
{
    bool        bPadTypeValid;
    char        chPadShape;
    short       nPadWidth;		//Enc Count
    short       nPadHeight;		//Enc Count
    LIGHT_PARA  stPrLightPara;
    ULONG       ulPrPatternID;
    char        cReserve[100];

    //must init before use, or by default as 0
	AUTO_PAD_TYPE():
	    bPadTypeValid(),
	    chPadShape(),
	    nPadWidth(),
	    nPadHeight(),
	    stPrLightPara(),
		ulPrPatternID(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

class CAutoPadMgr : public CObject
{
public:
	CAutoPadMgr();
	virtual ~CAutoPadMgr();
public:	//functions
	void VerifyData(void);			//v1.0.T11 add
    short GetTotalAutoPad(void);	//v1.0.T7 edit
    void GetRcd(short nPadId, AUTO_PAD_TYPE* pstAutoPad);	//v1.0.T8 edit
    void SetRcd(short nPadId, AUTO_PAD_TYPE* pstAutoPad);	//v1.0.T8 edit
    void IncTotalAutoPad(void);		//v1.0.T7 edit
    void SaveRcd(void);
    void LoadRcd(void);
    int ClearAutoPadRcd(void);
private:	//variables
    AUTO_PAD_TYPE m_astAutoPad[MAX_AUTO_PAD_TYPE];
    short m_nTotalAutoPad;	//v1.0.T7 edit
};

extern CAutoPadMgr theAutoPadMgr;	//v1.0.T9 edit
