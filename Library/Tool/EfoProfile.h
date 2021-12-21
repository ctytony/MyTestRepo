//EfoProfile.h v1.0.T33

#pragma once

struct ST_EFO_PROFILE	//v1.0.T21 rename
{
	WORD nTime1;
	WORD nCurrent1;
	WORD nTime2;
	WORD nCurrent2;
	WORD nTime3;		//v1.0.L17 add
	WORD nCurrent3;		//v1.0.L17 add
	WORD nTailLen_x1um;	//v1.0.T30 add, v1.0.T32 rename
	//char cReserve[8];	//v1.0.L17 edit
	char cReserve[6];	//v1.0.T30 add
};

#define MAX_PROFILE_NUM		10

// CEfoProfileMgr command target

class CEfoProfileMgr : public CObject
{
public:
	CEfoProfileMgr();
	virtual ~CEfoProfileMgr();
public:	//functions
	void VerifyProfile(void);	//v1.0.T19 add
	ST_EFO_PROFILE *GetProfile(short nFabID);	//v1.0.T31 add
    void GetProfile(short nFabID, ST_EFO_PROFILE *pstProfile);
    void SetProfile(short nFabID, ST_EFO_PROFILE *pstProfile);	//v1.0.T7 edit
    void SaveProfile(void);
    void LoadProfile(void);
    void ClearProfile(void);
private:	//variables
    ST_EFO_PROFILE m_astEfoProfile[MAX_PROFILE_NUM];	//v1.0.T5 edit
};

extern CEfoProfileMgr theEfoProfile;
