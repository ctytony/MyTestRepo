#pragma once

#define MAXAUTOSAMPLE			5
#define AUTO_SAMPLING_DONE		-5
#define MAX_CONTINUE_FALSE_ALM	4
#define MSG_CALLBACK_AUTOSAMPLING_ONOK	5

class CBSDDebug : public CObject
{
public:
	CBSDDebug(void);
	~CBSDDebug(void);
public:		//function
	bool GetEnableFlag(void);
	int OnBSDErrorLogEnable(int nErrorType);
	int ResetSampling(void);
	bool GetAutoSamplingFlag(void);
	int CaculateAvePara(double *p_DieVolt,double *p_LeadVolt);
	short GetFalseAlmCnter(void);
	int ResetFalseAlmcnter(void);
	int IncreFalseAlmCnter(void);
	int GetSingleRslt(double* dDieVolt, double* dLeadVolt);
	int SetErrorLogFlag(bool bEnable);
	int SetAutoSamplingFlag(bool bEnable);
	short GetSamplingCounter(void);
private:	//variable
	bool m_BSDebugEnable;
	bool bAutoSamplingEnableFlag;
	short m_nCounter;
	double dSamplingDieVolt[5];
	double dSamplingLeadVolt[5];
	short nFalseAlmCnter;
};

extern CBSDDebug theBSDDebug;