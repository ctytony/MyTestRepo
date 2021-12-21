/***************************************************************
ParaMapBase.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "MenuConfig.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "Program.h"
#include "ParaMapBase.h"
#include "WireBondDlg.h"
#include "WireGroupDlg.h"

// CParaMapBase dialog

IMPLEMENT_DYNAMIC(CParaMapBase, CDialog)

CParaMapBase::CParaMapBase(short nParaType, CWnd* pParent /*=NULL*/)
	: CDialog(CParaMapBase::IDD, pParent)
    , m_nParaMapType(nParaType)
    , m_sHelpMsg(_T(""))
    , m_nParaApplyTo(1)
{
}

CParaMapBase::~CParaMapBase()
{
}

void CParaMapBase::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_STATIC_HELP_MSG, m_sHelpMsg);
    DDX_CBIndex(pDX, IDC_COMBO_APPLY_TO, m_nParaApplyTo);
}

BEGIN_MESSAGE_MAP(CParaMapBase, CDialog)
    ON_WM_CTLCOLOR()
    ON_BN_CLICKED(IDC_BUTTON_GROUPING, OnGrouping)
    ON_CBN_SELCHANGE(IDC_COMBO_APPLY_TO, OnApplyTo)
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)		//V1.5.W27 add
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CParaMapBase message handlers
BOOL CParaMapBase::OnInitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
	SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	SetDlgItemText(IDC_BUTTON_GROUPING,theMsg.GetMsg(MSG_GROUPING));
	SetDlgItemText(IDC_STATIC_APPLY_TO,theMsg.GetMsg(MSG_APPLY_TO));

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_APPLY_TO);
	pComboBox->AddString(theMsg.GetMsg(MSG_GROUP));
	pComboBox->AddString(theMsg.GetMsg(MSG_SINGLE));
	pComboBox->AddString(theMsg.GetMsg(MSG_ALL));

	short nTotalWire = thePgm.GetUnitTotalWire();
	CPoint pt;
	pt.x = MAP_PARA_COLUMN1_LEFT;
	pt.y = MAP_PARA_LABEL_TOP;

	for (char i = 1; i <= 8; i++)
	{
		m_paTopLabel[i-1] = new CStatic();
		m_paTopLabel[i-1]->Create(CString((char)(i+0x30)), ES_CENTER, Rect, this, IDC_STATIC_H_NUM_01 + i);

		if (i <= nTotalWire)
		{
			m_paTopLabel[i-1]->SetWindowPos(NULL, pt.x, pt.y, MAP_PARA_COLUMN_WIDTH, MAP_PARA_COLUMN_HEIGHT, SWP_NOZORDER);
			m_paTopLabel[i-1]->SetFont(theApp.GetFont());
			pt.x += MAP_PARA_COLUMN_WIDTH + MAP_PARA_COLUMN_GAP;
			m_paTopLabel[i-1]->ShowWindow(SW_SHOWNORMAL);
		}
		else
			m_paTopLabel[i-1]->ShowWindow(SW_HIDE);
	}

	m_dlgAdvParaMap.SetParaMapType(m_nParaMapType);
	m_dlgAdvParaMap.Create(IDD_ADV_PARA_MAP, this);
	m_dlgAdvParaMap.SetWindowPos(NULL, Rect.left, Rect.top + 40,
		TREE_DLG_WIDTH - 6, TREE_DLG_HEIGHT - 305, SWP_NOZORDER);	//v3.1.T47 edit
	m_dlgAdvParaMap.ShowWindow(SW_SHOWNORMAL);
	m_dlgAdvParaMap.SetParaApplyMode(m_nParaApplyTo);

	return CDialog::OnInitDialog();
}

HBRUSH CParaMapBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetBkColor(theApp.m_bkColor);
        return theApp.m_DlgBkBrush;
    }
    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CParaMapBase::SetHelpMessage(CString& sHelpMsg)
{
    if (sHelpMsg.IsEmpty() == false)
        m_sHelpMsg = sHelpMsg;
}

void CParaMapBase::OnDestroy()
{
	m_dlgAdvParaMap.DestroyWindow();
	for (char i = 1; i <= 8; i++) delete m_paTopLabel[i-1];
	((CWireBondDlg*)theApp.m_pMainWnd)->UpdateInfoView(INFOVIEW_WIRE_INFO);
	CDialog::OnDestroy();
}

void CParaMapBase::OnGrouping()
{
    CWireGroupDlg dlgWireGroup;
    dlgWireGroup.DoModal();
    m_dlgAdvParaMap.Invalidate();
}

void CParaMapBase::OnApplyTo()
{
    UpdateData();
    m_dlgAdvParaMap.SetParaApplyMode(m_nParaApplyTo);
}

void CParaMapBase::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

LRESULT CParaMapBase::CloseDialog(WPARAM wParam, LPARAM lParam)		//V1.5.W27 add
{
	return DestroyWindow()*DIALOG_CLOSED;
}
