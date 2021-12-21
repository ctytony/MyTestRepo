#pragma once

// CDlgCuEliteElevSetupPage 对话框
class CDlgCuEliteElevSetupPage : public CDialog
{
	DECLARE_DYNAMIC(CDlgCuEliteElevSetupPage)

public:

	CDlgCuEliteElevSetupPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCuEliteElevSetupPage();

// 对话框数据
	enum { IDD = IDD_CUELITE_ELEV_SETUP };

	afx_msg void OnOnloaderMagClamperFrontEdit();
	afx_msg void OnOnloaderMagClamperBackEdit();
	afx_msg void OnOnloaderMagClamperLengthEdit();
	afx_msg void OnOffloaderMagClamperFrontEdit();
	afx_msg void OnOffloaderMagClamperBackEdit();
	afx_msg void OnOffloaderMagClamperLengthEdit();
	afx_msg void OnOnloaderMagClamperDepth();
	afx_msg void OnEditOnloaderMagClamperFrontPosition();
	afx_msg void OnEditOnloaderMagClamperBackPosition();
	afx_msg void OnEditOnloaderMagLengthPosition();
	afx_msg void OnOffloaderMagClamperDepth();
	afx_msg void OnEditOffloaderMagClamperFrontPosition();
	afx_msg void OnEditOffloaderMagClamperBackPosition();
	afx_msg void OnEditOffloaderMagLengthPosition();
	long m_lOnloaderMagClamperDepth;
	long m_lOffloaderMagClamperDepth;
	long m_lOnloaderMagClamperFrontStandby;
	long m_lOffloaderMagClamperFrontStandby;
	long m_lOnloaderMagClamperBackStandby;
	long m_lOffloaderMagClamperBackStandby;
	long m_lOnloaderMagLengthStandby;
	long m_lOffloaderMagLengthStandby;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOk();
	afx_msg void OnDestroy();

protected:

	BYTE m_nCurCtrl;

	CUELITE_ONLOAD_RCD* m_lpstOnloadRcd;
	CUELITE_OFFLOAD_RCD* m_lpstOffloadRcd;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	LRESULT OnRefresh(WPARAM wParam,LPARAM lParam);
};
