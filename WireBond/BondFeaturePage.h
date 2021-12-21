#pragma once

// CBondFeaturePage dialog

class CBondFeaturePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBondFeaturePage)
public:
	CBondFeaturePage();
	virtual ~CBondFeaturePage();
    virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_BOND_FEATURE };
public:
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFocusTransducerIdleTime();
	afx_msg void OnFocusTransducerLengthComp();
	afx_msg void OnFocusHeadTimeOffset();
    afx_msg void OnFocusTsc();
	afx_msg void OnMultiBtos();
	afx_msg void OnConstWireLength();
	afx_msg void OnTsc();
	afx_msg void OnZeroBtos();
	afx_msg void OnCherryBit();
	afx_msg void OnBumpCtrl();
	afx_msg void OnN2h2Enable();
	afx_msg void OnFocusN2h2StableTime();
    afx_msg void OnWireFeedCheck();
    afx_msg void OnTailCtrl();
    afx_msg void OnUphMode();
    afx_msg void OnUnplug();
	afx_msg void OnFocusCheckWfNum();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
private:	//variables
	short m_nTransducerIdleTime;	//minute
	short m_nTransducerLengthComp;	//um
    // Transducer shift compensation enable
    int m_iTscEnable;
    // Transducer shift compensation factor
    float m_fTscFactor;
    // Zero btos enable
    int m_iZeroBtos;
    int m_iCherryBitEnable;
    int m_iBumpBondOption;	//v3.1.T92 rename
    int m_iTailCtrlMode;
	int m_iMultiBtos;
	int m_iN2H2Enable;
	short m_nN2H2StableTime;
    int m_iWireFeedCheck;
    int m_iUnplugEnable;
	short m_nWireFeedTimeout;	//unit: second
	short m_nHeadTimeOffset;
    int m_iConstWireLen;
};
