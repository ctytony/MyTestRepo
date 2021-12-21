// ForceSwitchMgr.cpp : 实现文件
//

#include "stdafx.h"
#include "WireBond.h"
#include "ForceSwitch.h"
#include "Program.h"

// CForceSwitch

CForceSwitch::CForceSwitch()
{
	SetDefaultRcd(&m_stForceSwitch);
}

CForceSwitch::~CForceSwitch()
{
}


// CForceSwitch 成员函数

CForceSwitch theForceSwitch;

void CForceSwitch::SetDefaultRcd(FORCE_SWITCH_PARA* pstForceSwitchPara)
{
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 3; j++)
			pstForceSwitchPara->iForceTime[i][j] = 0;

	pstForceSwitchPara->iForceSwitch    = 0;
	pstForceSwitchPara->nMode1  = 0;
	pstForceSwitchPara->nMode2  = 0;
	pstForceSwitchPara->nMode3  = 0;
	pstForceSwitchPara->nSpeed1 = 0;
	pstForceSwitchPara->nSpeed2 = 0;
	pstForceSwitchPara->nSpeed3 = 0;

	pstForceSwitchPara->nTailTime          = 8;
	pstForceSwitchPara->nTailBreakDist     = 20;
	pstForceSwitchPara->nTailBreakTime     = 6;
	pstForceSwitchPara->nFab2TailTime      = 8;
	pstForceSwitchPara->nFab2TailBreakDist = 20;
	pstForceSwitchPara->nFab2TailBreakTime = 6;
}

FORCE_SWITCH_PARA* CForceSwitch::GetRcd(void)
{
	return &m_stForceSwitch;
}

void CForceSwitch::GetRcd(FORCE_SWITCH_PARA* pstForceSwitchPara)
{
	*pstForceSwitchPara = m_stForceSwitch;
}

void CForceSwitch::SetRcd(FORCE_SWITCH_PARA* pstForceSwitchPara)
{
	m_stForceSwitch = *pstForceSwitchPara;
}

void CForceSwitch::SaveRcd(void)
{
	theDataMgr.CopyToBuffer((char *)(&m_stForceSwitch), sizeof(FORCE_SWITCH_PARA));
}

void CForceSwitch::LoadRcd(void)
{
	if (thePgm.GetVersion() < 99)
		theForceSwitch.SetDefaultRcd(&m_stForceSwitch);
	else
		theDataMgr.CopyFromBuffer((char *)(&m_stForceSwitch), sizeof(FORCE_SWITCH_PARA));
}
