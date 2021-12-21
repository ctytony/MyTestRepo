// MapEdit.cpp : implementation file

#include "stdafx.h"
#include "WireBond.h"
#include "KeyPadHandler.h"
#include "MapEdit.h"
#include "AdvParaMapDlg.h"

// CMapEdit
IMPLEMENT_DYNAMIC(CMapEdit, CEdit)

CMapEdit::CMapEdit()
{
}

CMapEdit::~CMapEdit()
{
}

BEGIN_MESSAGE_MAP(CMapEdit, CEdit)
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CMapEdit message handlers
void CMapEdit::OnSetFocus(CWnd* pOldWnd)
{
    CEdit::OnSetFocus(pOldWnd);

    CAdvParaMapDlg* pWnd = (CAdvParaMapDlg*)GetParent();
    pWnd->RemoveEditFocus();

	if (theKeyPad.GetUserKeyInData(&m_nUnitValue, m_lDefaultValue, m_lLowerLimit,m_lUpperLimit, true) == CANCEL_SELECTED)
		return;

    pWnd->UpdateUnitValue(GetDlgCtrlID());
}

short CMapEdit::GetUnitValue(void)
{
    return m_nUnitValue;
}

// Set range parameter of edit control
void CMapEdit::SetParamRange(long lRefValue, long lLowerLimit, long lUpperLimit)
{
    m_lUpperLimit = lUpperLimit;
    m_lLowerLimit = lLowerLimit;
    m_lDefaultValue = lRefValue;
}

void CMapEdit::SetUnitValue(short nUnitValue)
{
    m_nUnitValue = nUnitValue;
}
