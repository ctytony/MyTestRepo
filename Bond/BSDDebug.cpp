#include "StdAfx.h"
#include "BSDDebug.h"
#include "DataStruct.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "MsgManager.h"

CBSDDebug theBSDDebug;

CBSDDebug::CBSDDebug(void)
: nFalseAlmCnter(0)
{
	m_BSDebugEnable = false;
	bAutoSamplingEnableFlag = false;
	m_nCounter = 0;
	nFalseAlmCnter = 0;
}

CBSDDebug::~CBSDDebug(void)
{
}

int CBSDDebug::CaculateAvePara(double *p_DieVolt, double *p_LeadVolt)
{
	int i;

	*p_DieVolt = 0;
	*p_LeadVolt = 0;

	for (i=0; i<MAXAUTOSAMPLE; i++)
	{
		*p_DieVolt += dSamplingDieVolt[i];
		*p_LeadVolt += dSamplingLeadVolt[i];
	}

	*p_DieVolt  = *p_DieVolt / 5.0;
	*p_LeadVolt = *p_LeadVolt / 5.0;

	return 0;
}

int CBSDDebug::ResetSampling(void)
{
	int i;
	//bAutoSamplingEnableFlag=false;
	m_nCounter = 0;

	for (i=0; i<MAXAUTOSAMPLE; i++)
	{
		dSamplingDieVolt[i] = 0;
		dSamplingLeadVolt[i] = 0;
	}
	
	return 0;
}

/*
int CBSDDebug::OnBSDErrorLogEnable(int nErrorType)
{
	
		char FileNameBuff[30];
		memset(FileNameBuff,0,30);

		char FilePrefix[10];
		memset(FilePrefix,0,10);
		
		switch(nErrorType)
		{
		case 1:
			strcpy_s(FilePrefix,10,"d:\\NSOP");
		case 2:
			strcpy_s(FilePrefix,10,"d:\\NSOL");
		case 4:
			strcpy_s(FilePrefix,10,"d:\\TailShrt");
				
		}

	    bsd_api_gen_debug_file(FilePrefix,FileNameBuff, 30);

		
	return 0;
}
*/

int CBSDDebug::SetErrorLogFlag(bool bEnable)
{
	m_BSDebugEnable = bEnable;
	return 0;
}

int CBSDDebug::SetAutoSamplingFlag(bool bEnable)
{
	bAutoSamplingEnableFlag = bEnable;
	return 0;
}

bool CBSDDebug::GetEnableFlag(void)
{
	return m_BSDebugEnable;
}

bool CBSDDebug::GetAutoSamplingFlag(void)
{
	return bAutoSamplingEnableFlag;
}

short CBSDDebug::GetSamplingCounter(void)
{
	return m_nCounter;
}

short CBSDDebug::GetFalseAlmCnter(void)
{
	return nFalseAlmCnter;
}

int CBSDDebug::ResetFalseAlmcnter(void)
{
	nFalseAlmCnter = 0;
	return 0;
}

int CBSDDebug::IncreFalseAlmCnter(void)
{
	nFalseAlmCnter++;
	return 0;
}

int CBSDDebug::GetSingleRslt(double* dDieVolt, double* dLeadVolt)
{
	*dDieVolt = dSamplingDieVolt[m_nCounter-1];
	*dLeadVolt = dSamplingLeadVolt[m_nCounter-1];
	return 0;
}
