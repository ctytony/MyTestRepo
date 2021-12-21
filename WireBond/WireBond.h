//WireBond.h v3.0.T416

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "afxwin.h"
#include "MacDef.h"
#include "ServoController.h"
#include "DataManager.h"
#include "LogTrace.h"

// CWireBondApp:
// See WireBond.cpp for the implementation of this class
//
#define FILE_COLDSTART		1
#define FILE_COLDEND		2

enum ARCHIVE_MODE
{
	ARCHIVE_SAVE,
	ARCHIVE_LOAD,
	ARCHIVE_CLEAR
};

class CWireBondApp : public CWinApp
{
public:
	CWireBondApp();
	virtual BOOL InitInstance();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	DECLARE_MESSAGE_MAP()
public:	//variables
    // Dialog backgroud brush color
    CBrush m_DlgBkBrush, m_brNull;			//V1.1.W133 add
    // Dialog background brush color A
    CBrush m_DlgBkBrushA;
	COLORREF m_bkColor;
public:	//functiions
	void BackupMcdata();	//v2.0.T121 add
	void BackupWbdata();
    void ArchiveMachineData(ARCHIVE_MODE nArchiveMode);
    void ArchiveProgram(CString sPgmName, ARCHIVE_MODE nArchiveMode, bool bLoadWH = true);	//V1.1.W172 edit
    void ArchivePowerOnData(ARCHIVE_MODE nArchiveMode);
	CString GetSoftwareVersion();
	void SetRedLamp(bool bOn);
	CFont* GetFont();
	bool _CopyFile(const TCHAR *_pFrom, const TCHAR *_pTo);
    void RecordCurrentTime(short nFilename);
	void SetDlgFont(CWnd* lpParent, CFont* lpFont = NULL);	//V1.1.W139 add
private:	//variables
    CFont m_ChineseFont;
    CFont m_EnglishFont;
private:	//functions
	void InitPowerOnData();	//v3.0.T13 add
	void InitMcdata();		//v2.0.T122 add
	int LoadControlData(CString sFilePath); 
	int SaveControlData(CString sFilePath);
	int LoadMcDataFromFile(CString sFilePath);
	int SaveMcDataToFile(CString sFilePath);
	int LoadRecipeFromFile(CString sFilePath, bool bPowerOn, bool bLoadWH);	//V1.1.W172 edit
	int SaveRecipeToFile(CString sFilePath, bool bPowerOn);
	bool IsAbnormalExit();
	bool RestoreWbdata();	//v2.0.T113 add
	bool RestoreMcdata();	//v2.0.T121 add
};

extern CWireBondApp theApp;
extern WCHAR wcSoftwareVersion[];
