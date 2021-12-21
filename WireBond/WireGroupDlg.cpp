// WireGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "Program.h"
#include "Wire.h"
#include "WireGroupDlg.h"
#include "AdvParaMapDlg.h"

// CWireGroupDlg dialog
IMPLEMENT_DYNAMIC(CWireGroupDlg, CDialog)

CWireGroupDlg::CWireGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWireGroupDlg::IDD, pParent)
    , m_nStartWire(1)
    , m_nEndWire(1)
    , m_nGroupNum(1)
{
}

CWireGroupDlg::~CWireGroupDlg()
{
}

void CWireGroupDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_START_WIRE, m_nStartWire);
    DDX_Text(pDX, IDC_EDIT_END_WIRE, m_nEndWire);
    DDX_Text(pDX, IDC_EDIT_GROUP, m_nGroupNum);
}

BEGIN_MESSAGE_MAP(CWireGroupDlg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnConfirm)
    ON_WM_CTLCOLOR()
    ON_EN_SETFOCUS(IDC_EDIT_START_WIRE, OnFocusStartWire)
    ON_EN_SETFOCUS(IDC_EDIT_END_WIRE, OnFocusEndWire)
    ON_EN_SETFOCUS(IDC_EDIT_GROUP, OnFocusGroup)
END_MESSAGE_MAP()

// CWireGroupDlg message handlers
BOOL CWireGroupDlg::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

    m_aGroupBrush[0].CreateSolidBrush(BOND_GROUP_COLOR_00);
    m_aGroupBrush[1].CreateSolidBrush(BOND_GROUP_COLOR_01);
    m_aGroupBrush[2].CreateSolidBrush(BOND_GROUP_COLOR_02);
    m_aGroupBrush[3].CreateSolidBrush(BOND_GROUP_COLOR_03);
    m_aGroupBrush[4].CreateSolidBrush(BOND_GROUP_COLOR_04);
    m_aGroupBrush[5].CreateSolidBrush(BOND_GROUP_COLOR_05);
    m_aGroupBrush[6].CreateSolidBrush(BOND_GROUP_COLOR_06);
    m_aGroupBrush[7].CreateSolidBrush(BOND_GROUP_COLOR_07);
    m_aGroupBrush[8].CreateSolidBrush(BOND_GROUP_COLOR_08);
    m_aGroupBrush[9].CreateSolidBrush(BOND_GROUP_COLOR_09);
    m_aGroupBrush[10].CreateSolidBrush(BOND_GROUP_COLOR_10);
    m_aGroupBrush[11].CreateSolidBrush(BOND_GROUP_COLOR_11);
    m_aGroupBrush[12].CreateSolidBrush(BOND_GROUP_COLOR_12);
    m_aGroupBrush[13].CreateSolidBrush(BOND_GROUP_COLOR_13);
    m_aGroupBrush[14].CreateSolidBrush(BOND_GROUP_COLOR_14);

    m_nEndWire = thePgm.GetUnitTotalWire();

    return CDialog::OnInitDialog();
}

HBRUSH CWireGroupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }

    if (nCtlColor == CTLCOLOR_EDIT&&pWnd->GetDlgCtrlID() == IDC_EDIT_GROUP)
    {
        if(0==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_00);
        else if(1==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_01);
        else if(2==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_02);
        else if(3==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_03);
        else if(4==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_04);
        else if(5==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_05);
        else if(6==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_06);
        else if(7==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_07);
        else if(8==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_08);
        else if(9==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_09);
        else if(10==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_10);
        else if(11==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_11);
        else if(12==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_12);
        else if(13==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_13);
        else if(14==m_nGroupNum)pDC->SetBkColor(BOND_GROUP_COLOR_14);

        return m_aGroupBrush[m_nGroupNum];
    }

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CWireGroupDlg::OnFocusStartWire()
{
	GetDlgItem(IDOK)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nStartWire, 1, 1, thePgm.GetUnitTotalWire());

	UpdateData(false);
}

void CWireGroupDlg::OnFocusEndWire()
{
	GetDlgItem(IDOK)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nEndWire, 1, 1, thePgm.GetUnitTotalWire());

	UpdateData(false);
}

void CWireGroupDlg::OnFocusGroup()
{
	GetDlgItem(IDOK)->SetFocus();

	theKeyPad.GetUserKeyInData(&m_nGroupNum, 1, 0, 14);

	UpdateData(false);
}

void CWireGroupDlg::OnConfirm()
{
    short i, j, nTotalWire, nUnitTotalWire;
    WIRE_RECORD stWireRcd;

    nTotalWire = theWire.GetTotalWireNum();
    nUnitTotalWire = thePgm.GetUnitTotalWire();

    for (i = m_nStartWire; i <= m_nEndWire; i++)
    {
        theWire.GetRcd(&stWireRcd, i);
        stWireRcd.chBondGroup = (char)m_nGroupNum;
        theWire.SetRcd(&stWireRcd, i);

        for (j = nUnitTotalWire + 1; j <= nTotalWire; j++)
        {
            if ((j-1) % nUnitTotalWire + 1 == i)
            {
                theWire.GetRcd(&stWireRcd, j);
                stWireRcd.chBondGroup = (char)m_nGroupNum;
                theWire.SetRcd(&stWireRcd, j);
            }
        }
    }

    GetParent()->Invalidate();

    if (m_nEndWire < nUnitTotalWire)
    {
        m_nStartWire = m_nEndWire + 1;
        m_nEndWire = nUnitTotalWire;
        m_nGroupNum++;
        UpdateData(FALSE);
    }
}
