#pragma once


// CDlgCuEliteWHSetupPage 对话框

class CDlgCuEliteWHSetupPage : public CDialog
{
	DECLARE_DYNAMIC(CDlgCuEliteWHSetupPage)

public:

	CDlgCuEliteWHSetupPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCuEliteWHSetupPage();

// 对话框数据
	enum { IDD = IDD_CUELITE_WH_SETUP };

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnStart();
	afx_msg void OnStartTchTable();
	afx_msg void OnNext();

	afx_msg void OnFrontTrackPosition();
	afx_msg void OnBackTrackPosition();
	afx_msg void OnFrontTrackEdit();
	afx_msg void OnBackTrackEdit();
	afx_msg void OnOk();
	
	afx_msg void OnFrontHome();
	afx_msg void OnBackHome();
	afx_msg void OnFrontStandby();
	afx_msg void OnBackStandby();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	BYTE RemovePlateAndClamper(BYTE nStepper);

	LRESULT OnRefresh(WPARAM wParam,LPARAM lParam);

private:

	long m_lIndexLogicZero;
	long m_lIndexRightLimit;

	int m_nWhTbX;	//V3.3.194 edit
	int m_nWhTbY;	//V3.3.194 edit
	int m_nLeadFrameTrackType;
	long m_lFrontTrackStandby;
	long m_lBackTrackStandby;

	BYTE m_nCurCtrl;
};
