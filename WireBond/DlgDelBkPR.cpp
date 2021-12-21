// DlgDelBkPR.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "DlgDelBkPR.h"
#include "Program.h"
#include "Bond.h"
#include "Wire.h"
#include "Reference.h"
#include "VisionSystem.h"
#include "MsgDef.h"
#include <comutil.h>
#include <memory>

using namespace std;
#pragma comment(lib,"comsuppw")

// CDlgDelBkPR dialog
CDlgDelBkPR::CDlgDelBkPR(CWnd* pParent)
	: CDialog(CDlgDelBkPR::IDD, pParent)
{
	m_bDirty=FALSE;
}

CDlgDelBkPR::~CDlgDelBkPR()
{
}

BEGIN_MESSAGE_MAP(CDlgDelBkPR, CDialog)
	ON_CBN_SELCHANGE(IDC_CB_WIRE_NUM, UpdateStatus)
	ON_COMMAND_RANGE(IDC_CHK_BK_PR_1,IDC_CHK_BK_PR_4,OnBkSelected)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnDelete)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CDlgDelBkPR::InitDialog()
{
	theApp.SetDlgFont(this);	//V1.1.W139 add

	SetDlgItemTextW(IDC_STATIC_SEL_WIRE, theMsg.GetMsg(MSG_SELECT_WIRE));
	SetDlgItemTextW(IDC_CHK_BK_PR_1, theMsg.GetMsg(MSG_BACKUP_PR_1));
	SetDlgItemTextW(IDC_CHK_BK_PR_2, theMsg.GetMsg(MSG_BACKUP_PR_2));
	SetDlgItemTextW(IDC_CHK_BK_PR_3, theMsg.GetMsg(MSG_BACKUP_PR_3));
	SetDlgItemTextW(IDC_CHK_BK_PR_4, theMsg.GetMsg(MSG_BACKUP_PR_4));
	SetDlgItemTextW(IDC_BUTTON_DELETE, theMsg.GetMsg(MSG_DELETE));
	SetDlgItemTextW(IDCANCEL, theMsg.GetMsg(MSG_DONE));
}

void CDlgDelBkPR::GetWireNum()
{
	CWnd* pWnd = GetDlgItem(IDC_CB_WIRE_NUM);
	((CComboBox*)pWnd)->ResetContent();
	pWnd->EnableWindow(FALSE);

	if (thePgm.GetTotalWire() == 0) return;

	WIRE_RECORD stWireRcd;
	REF_RECORD stRefRcd;

	for (short n = 1; n < thePgm.GetTotalWire() + 1; n++)
	{
		theWire.GetRcd(&stWireRcd,n);
		theRef.GetRcd(&stRefRcd, stWireRcd.nRefNum[stWireRcd.nBkPrBondPoint]);

		if (stRefRcd.nBakUpIndex == 0) continue;

		int nPos = ((CComboBox*)pWnd)->AddString(_bstr_t(n));
		((CComboBox*)pWnd)->SetItemData(nPos,n);
	}

	if (((CComboBox*)pWnd)->GetCount() == 0) return;

	pWnd->EnableWindow();
	((CComboBox*)pWnd)->SetCurSel(0);
}

void CDlgDelBkPR::UpdateStatus()
{
	int nPos = 0;

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_DELETE);
	pWnd->EnableWindow(FALSE);

	for(nPos = 0; nPos < MAX_BAK_UP_PR_NUM; nPos++)
	{
		pWnd = GetDlgItem(IDC_CHK_BK_PR_1 + nPos);

		((CButton*)pWnd)->SetCheck(BST_UNCHECKED);
		pWnd->EnableWindow(FALSE);
	}

	pWnd = GetDlgItem(IDC_CB_WIRE_NUM);
	nPos = ((CComboBox*)pWnd)->GetCurSel();
	if (nPos == CB_ERR) return;

	nPos = ((CComboBox*)pWnd)->GetItemData(nPos);

	WIRE_RECORD stWireRcd;
	theWire.GetRcd(&stWireRcd,nPos);

	REF_RECORD stRefRcd;
	theRef.GetRcd(&stRefRcd, stWireRcd.nRefNum[stWireRcd.nBkPrBondPoint]);

	for (nPos = 0; nPos < stRefRcd.nBakUpIndex; nPos++)
		GetDlgItem(IDC_CHK_BK_PR_1 + nPos)->EnableWindow();
}

// CDlgDelBkPR message handlers
BOOL CDlgDelBkPR::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitDialog();
	GetWireNum();
	UpdateStatus();

	return FALSE;
}

void CDlgDelBkPR::OnBkSelected(UINT nID)
{
	BYTE bChecked=FALSE;

	for(nID=0;nID<MAX_BAK_UP_PR_NUM;nID++)
	{
		CWnd* pWnd=GetDlgItem(IDC_CHK_BK_PR_1+nID);

		if(pWnd->IsWindowEnabled()==FALSE)break;
		if(((CButton*)pWnd)->GetCheck()==BST_UNCHECKED)continue;

		bChecked=TRUE;
		break;
	}

	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(bChecked);
}

void CDlgDelBkPR::OnDelete()
{
	CWnd* pWnd = GetDlgItem(IDC_CB_WIRE_NUM);

	int nPos = ((CComboBox*)pWnd)->GetCurSel();
	nPos = ((CComboBox*)pWnd)->GetItemData(nPos);

	WIRE_RECORD stWireRcd;
	theWire.GetRcd(&stWireRcd, nPos);

	REF_RECORD stRefRcd;
	theRef.GetRcd(&stRefRcd, stWireRcd.nRefNum[stWireRcd.nBkPrBondPoint]);

	for (int nIndex = nPos = 0; nPos < MAX_BAK_UP_PR_NUM; nIndex++, nPos++)
	{
		pWnd = GetDlgItem(IDC_CHK_BK_PR_1 + nPos);

		if (pWnd->IsWindowEnabled() == FALSE) break;
		if (((CButton*)pWnd)->GetCheck() == BST_UNCHECKED) continue;

		m_bDirty = TRUE;

		theVisionSys.ClearPatternRcd(
			(short)stRefRcd.stBakUpPr[nIndex].lPatID[0]);
		if (stRefRcd.nLocateMode == REF_LOCATE_DUAL_POINT)
			theVisionSys.ClearPatternRcd(
				(short)stRefRcd.stBakUpPr[nIndex].lPatID[1]);

		ZeroMemory(&stRefRcd.stBakUpPr[nIndex], sizeof(BACK_UP_PR));

		for (int n = nIndex + 1; n < stRefRcd.nBakUpIndex; n++)
			memcpy(&stRefRcd.stBakUpPr[n-1], &stRefRcd.stBakUpPr[n], sizeof(BACK_UP_PR));

		nIndex--;
		stRefRcd.nBakUpIndex--;
	}

	theRef.SetRcd(&stRefRcd, stWireRcd.nRefNum[stWireRcd.nBkPrBondPoint]);

	if (stRefRcd.nBakUpIndex == 0) GetWireNum();
	UpdateStatus();
}

HBRUSH CDlgDelBkPR::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor==CTLCOLOR_DLG||nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(theApp.m_bkColor);
		return theApp.m_DlgBkBrush;
	}

    return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

BYTE CDlgDelBkPR::IsDirty()
{
	return m_bDirty;
}
