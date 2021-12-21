// UsgDebugPage.cpp : implementation file
//

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "UsgDebugPage.h"
#include "MacDef.h"
#include "MsgDef.h"
#include "WireBondDlg.h"
#include "MenuConfig.h"
#include "UsgControl.h"
#include "SerialComIO.h"
#include "Busy.h"

// CUsgDebugPage dialog
IMPLEMENT_DYNAMIC(CUsgDebugPage, CDialog)

CUsgDebugPage::CUsgDebugPage(CWnd* pParent /*=NULL*/)
	: CDialog(CUsgDebugPage::IDD, pParent)
	, m_szString(_T(""))
{
	m_bBusy=FALSE;	//V1.4.W41 add
}

CUsgDebugPage::~CUsgDebugPage()
{
}

void CUsgDebugPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REV_DATA, m_szString);
}

BEGIN_MESSAGE_MAP(CUsgDebugPage, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_REV_DATA, OnRevData)
	ON_MESSAGE(WM_CLOSEDIALOG, CloseDialog)	//V1.4.W41 add
END_MESSAGE_MAP()

// CUsgDebugPage message handlers
BOOL CUsgDebugPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	theApp.SetDlgFont(this);	//V1.1.W139 add

	CRect Rect;
	GetParent()->GetWindowRect(&Rect);
	SetWindowPos(NULL, Rect.left + TREE_DLG_LEFT, Rect.top + TREE_DLG_TOP, TREE_DLG_WIDTH, TREE_DLG_HEIGHT, SWP_NOZORDER);

	((CComboBox*)GetDlgItem(IDC_COMBO_PROFILE))->SetCurSel(0);

	BYTE bRevData[255];
	memset(bRevData, 0, 255);

	return theSerialCommIO.Digital_IO_Rev(bRevData, 255);
}

HBRUSH CUsgDebugPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CUsgDebugPage::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	delete this;
}

void CUsgDebugPage::OnRevData()
{
	CBusy bs(&m_bBusy);	//V1.4.W41 add

	BYTE bRevData[255];

	*(LPINT)bRevData = ((CComboBox*)GetDlgItem(IDC_COMBO_PROFILE))->GetCurSel();
	theUSG.RequestGetProfile(bRevData[0]);

	Sleep_x1ms(200);

	memset(bRevData, 0, 255);

	int nReturnNum = theSerialCommIO.Digital_IO_Rev(bRevData, 255);

	m_szString.Empty();

	if(nReturnNum > 0)
	{
		for(int i=0;i<nReturnNum; i++)
		{
			if(i%3 == 0)
				m_szString.AppendFormat(_T("Time%d:"),i/3);
			else if(i%3 == 1)
			    m_szString.AppendFormat(_T("%d "),MAKEWORD(bRevData[i-1],bRevData[i]));
			else if(i%3 == 2)
			{
				m_szString.AppendFormat(_T("Power%d:%d "),i/3, bRevData[i]);
				m_szString.Append(_T("\r\n"));
			}
		}
	}

	UpdateData(FALSE);
}

LRESULT CUsgDebugPage::CloseDialog(WPARAM wParam, LPARAM lParam)	//V1.4.W41 add
{
	return (m_bBusy==FALSE&&DestroyWindow())*DIALOG_CLOSED;
}
