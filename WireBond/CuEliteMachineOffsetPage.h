#pragma once

#include "IniFile.h"

// CCuEliteMachineOffsetPage dialog

class CCuEliteMachineOffsetPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteMachineOffsetPage)

public:
	CCuEliteMachineOffsetPage();   // standard constructor
	virtual ~CCuEliteMachineOffsetPage();

// Dialog Data
	enum { IDD = IDD_TAB_CUELITE_MACHINEOFFSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

private: 
	long m_lOnLoadMagazineElevatorOffset;
	long m_lOnLoadMagazineLengthOffset; 
	long m_lOnLoadMagazineYOneOffset;  
	long m_lOnLoadMagazineYTwoOffset;  
	long m_lOffLoadMagazineElevatorOffset;
	long m_lOffLoadMagazineLengthOffset;
	long m_lOffLoadMagazineYOneOffset; 
	long m_lOffLoadMagazineYTwoOffset; 
    long m_lLFIndexerOffset;
	long m_lLFFrontTrackOffset;        
	long m_lLFRearTrackOffset;        
    long m_lLFWindowClamperOffset;

	long m_lOnLoadMagazineElevatorStandby;
	long m_lOnLoadMagazineLengthStandby; 
	long m_lOnLoadMagazineYOneStandby;  
	long m_lOnLoadMagazineYTwoStandby;  
	long m_lOffLoadMagazineElevatorStandby;
	long m_lOffLoadMagazineLengthStandby;
	long m_lOffLoadMagazineYOneStandby; 
	long m_lOffLoadMagazineYTwoStandby; 
    long m_lLFIndexerStandby;
	long m_lLFFrontTrackStandby;        
	long m_lLFRearTrackStandby;        
    long m_lLFWindowClamperStandby;

	long m_lOnLoadMagazineElevatorStandbyRef;
	long m_lOnLoadMagazineLengthStandbyRef; 
	long m_lOnLoadMagazineYOneStandbyRef;  
	long m_lOnLoadMagazineYTwoStandbyRef;  
	long m_lOffLoadMagazineElevatorStandbyRef;
	long m_lOffLoadMagazineLengthStandbyRef;
	long m_lOffLoadMagazineYOneStandbyRef; 
	long m_lOffLoadMagazineYTwoStandbyRef; 
    long m_lLFIndexerStandbyRef;
	long m_lLFFrontTrackStandbyRef;        
	long m_lLFRearTrackStandbyRef;        
    long m_lLFWindowClamperStandbyRef;


	CString m_sID;

	CiniFile m_newFile, m_refFile;


	void  saveOffsetToID_CUELITEini();

public:


	virtual BOOL OnInitDialog();

	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnEnSetfocusEditOnloadElevator();
	afx_msg void OnEnSetfocusEditOnloadFrontClamp();
	afx_msg void OnEnSetfocusEditOnloadRearClamp();
	afx_msg void OnEnSetfocusEditOnloadSideClamp();
	afx_msg void OnEnSetfocusEditIndexFrontRail();
	afx_msg void OnEnSetfocusEditIndexRearRail();
	afx_msg void OnEnSetfocusEditIndexIndexer();
	afx_msg void OnEnSetfocusEditIndexWinClamp();
	afx_msg void OnEnSetfocusEditOffloadElevator();
	afx_msg void OnEnSetfocusEditOffloadFrontClamp();
	afx_msg void OnEnSetfocusEditOffloadRearClamp();
	afx_msg void OnEnSetfocusEditOffloadSideClamp();
	afx_msg void OnEnSetfocusEditFile2();
	afx_msg void OnBnClickedButtonLoadDatabase();
	afx_msg void OnEnSetfocusEditOnloadFClamp2();
};
