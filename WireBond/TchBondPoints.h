#pragma once

enum TCH_BOND_POINT_MODE
{
	TCH_NEW_BOND_POINT,
	TCH_GROUP_BOND_POINT,
	TCH_EDIT_BOND_POINT,
	TCH_INSERT_BOND_POINT,
};

// CTchBondPoints dialog
class CTchBondPoints : public CDialog
{
	DECLARE_DYNAMIC(CTchBondPoints)

public:

	CTchBondPoints(short nBondPt, int nBondOn1, int nBondOn2, short nTchMode, short nCurrentPt, int lOperateMode, CWnd* pParent = NULL);   // standard constructor
	virtual ~CTchBondPoints();

// Dialog Data
	enum { IDD = IDD_TCH_SINGLE_BOND_PT };
    virtual BOOL OnInitDialog();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    // Teach new bond point
    void TeachNewBondPoint(void);
    // Teach a group of bond points
    void TeachGroupBondPoint(void);
    // Edit bond point
    void EditBondPoint(void);
    // Insert bond point
    void InsertBondPoint(void);
    // Search pad pattern for current bond point
    int SearchPadPattern(TBL_POS* pstSrchOffset);
    afx_msg void OnComboPadType();
	afx_msg void OnFirstPoint();
	afx_msg void OnSecondPoint();
    afx_msg void OnDestroy();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

    virtual void PostNcDestroy();
    virtual void OnOK();
    virtual void OnCancel();

	void SaveAutoPadCentering();
	void OnAlgorithm();

	afx_msg void OnAutoPadCentering();
	afx_msg void OnGapFill();

private:

   	CFont m_font;

    // Teaching bond point
    short m_nBondPt;
    // Current bond point
    short m_nCurBondPt;
    // Reference of 1st bond teaching point
    int m_iBondOn1;
    // Reference of 2nd bond teaching point
    int m_iBondOn2;
    // Pad type of current teaching bond point
    int m_iPadType;
    // Number of taught points
    short m_nTaughtPoints;
    // Teaching point
    short m_nTeachingPoint;
    // Teach mode
    short m_nTchMode;
    // Error handler mode
    bool bErrorHandlerMode;
    // Receipe operation mode
    int m_lOperateMode;
    // Step of teach group points
    short m_nTchGroupStep;
    // 1st point of group
    TBL_POS stGroup1stPoint;
    // 2nd point of group
    TBL_POS stGroup2ndPoint;
    // Last point of group
    TBL_POS stGroupLastPoint;
};
