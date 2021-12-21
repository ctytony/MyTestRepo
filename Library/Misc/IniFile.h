/* 

 v2.18.5.w3  This is just a class, wrapped for handling *.ini more easy 

*/

#pragma once
#include "afx.h"
#include "afxdlgs.h"

#define MAX_LINE_SIZE          256


class CiniFile :public CFile
{
private:


	CString m_sPath, m_sTitle,m_sExt;
	CString m_sPathTitleExt;

public:
	CiniFile(CString sPath, CString sFileName);
    CiniFile(void);
	~CiniFile(void);
    bool save(bool bSave);
    bool get(CString lpSection, CString lpKey, CString  *sReturn);
    bool get(CString lpSection, CString lpKey, long *lReturn);
    bool get(CString lpSection, CString lpKey, short *nReturn);
    bool get(CString lpSection, CString lpKey, int *nReturn);
    bool get(CString lpSection, CString lpKey, double *diReturn);
    bool get(CString lpSection, CString lpKey, bool *nReturn);


	bool set(CString lpSection, CString lpKey, CString  sReturn);
    bool set(CString lpSection, CString lpKey, long lReturn);
    bool set(CString lpSection, CString lpKey, short nReturn);
	bool set(CString lpSection, CString lpKey, int nReturn);
	bool set(CString lpSection, CString lpKey, bool bReturn);
    bool set(CString lpSection, CString lpKey, double diReturn);
    bool set(CString lpSection, CString lpKey, float diReturn);

    bool getTitle( CString  *sReturn);
	bool getPath( CString  *sReturn);
	bool getExt( CString  *sReturn);
	bool getPathTitleExt( CString  *sReturn);

	bool setPathTitleExt( CString  sReturn);
};
