#pragma once

// CForceSwitch ÃüÁîÄ¿±ê

#pragma pack(push, 1)
#pragma warning(disable : 4351)
struct FORCE_SWITCH_PARA
{
	short iForceTime[30][3];  //v1.6.P136sc   edit
	int   iForceSwitch;		//v1.6.P136sc edit
	short nSpeed1;
	short nSpeed2;
	short nSpeed3;
	short nMode1;
	short nMode2;
	short nMode3;
	short nTailTime;			//unit sample
	short nTailBreakDist;		//unit um
	short nFab2TailTime;
	short nFab2TailBreakDist;
	short nFab2TailBreakTime;
	short nTailBreakTime;
	char  cReserve[156];

	FORCE_SWITCH_PARA():
		iForceTime(),       //v1.6.P136sc   edit
	    iForceSwitch(),     //v1.6.P136sc   edit
		nSpeed1(),
		nSpeed2(),
		nSpeed3(),
		nMode1(),
		nMode2(),
		nMode3(),
		nTailTime(),
		nTailBreakDist(),
		nFab2TailTime(),
		nFab2TailBreakDist(),
		nFab2TailBreakTime(),
		nTailBreakTime(){}
};
#pragma warning(default : 4351)
#pragma pack(pop)

class CForceSwitch : public CObject
{
public:
	CForceSwitch();
	virtual ~CForceSwitch();
public:		//function
	FORCE_SWITCH_PARA* GetRcd(void);
	void GetRcd(FORCE_SWITCH_PARA* pstForceSwitchPara);
	void SetRcd(FORCE_SWITCH_PARA* pstForceSwitchPara);
	void SaveRcd(void);
	void LoadRcd(void);
private:	//function
	void SetDefaultRcd(FORCE_SWITCH_PARA* pstForceSwitchPara);
private:	//variable
	FORCE_SWITCH_PARA m_stForceSwitch;
};

extern CForceSwitch theForceSwitch;
