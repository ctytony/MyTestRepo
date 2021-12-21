/***************************************************************
KeyPadHandler.cpp : implementation file
**************************************************************/

#include "stdafx.h"
#include "MsgDef.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "KeyPadDlg.h"
#include "Bond.h"
#include "MachineConfig.h"
#include "servo.h"

// CKeyPadHandler

CKeyPadHandler::CKeyPadHandler()
: m_bKeyInValid(false)
{
	for (short i = 0; i < MAX_KEY_NUM; i++)
	{
		m_anKeyPadStatus[i].bKeyDown  = false;
		m_anKeyPadStatus[i].bProhibit = false;
	}
}

CKeyPadHandler::~CKeyPadHandler()
{
}

// Get user key in data from key pad
int CKeyPadHandler::GetUserKeyInData(long *plKeyInData, long lRef, long lLowLmt, long lHighLmt, bool bRangeMask, bool bPassword)
{
	CString str;
	str.Format(_T("%d"), lRef);
	int nCancel = PopupKeyPadDlg(str, lLowLmt, lHighLmt, bPassword);

	if (m_bKeyInValid)
	{
		long lKeyInData;
		if (wcscmp(m_acEditBuffer, _T("empty")) != 0)
			lKeyInData = _wtol(m_acEditBuffer);
		else
			lKeyInData = *plKeyInData;
	    
		if ((lLowLmt == lHighLmt) && (bRangeMask == false))
			*plKeyInData = lKeyInData;
		else
		{
			if ((lKeyInData < lLowLmt) || (lKeyInData > lHighLmt)
				|| ((lLowLmt == lHighLmt) && bRangeMask && (lKeyInData != lLowLmt)))
			{
				CStringList sList;
				sList.AddTail(theMsg.GetMsg(MSG_PARA_OUT_OF_RANGE));
				theMsg.ShowMsg(WARNING_MSG, sList);
				m_bKeyInValid = false;
			}
			else
				*plKeyInData = lKeyInData;
		}
	}

	return (nCancel == CANCEL_SELECTED ? CANCEL_SELECTED : 0);
}

int CKeyPadHandler::GetUserKeyInData(int *pnKeyInData, long lRef, long lLowLmt, long lHighLmt, bool bRangeMask)
{
	long lKeyinData = *pnKeyInData;
	int nCancel = GetUserKeyInData(&lKeyinData, lRef, lLowLmt, lHighLmt, bRangeMask);
	if (m_bKeyInValid) *pnKeyInData = (int)lKeyinData;
	return (nCancel == CANCEL_SELECTED ? CANCEL_SELECTED : 0);
}

int CKeyPadHandler::GetUserKeyInData(short *pnKeyInData, long lRef, long lLowLmt, long lHighLmt, bool bRangeMask, bool bPassword)
{
	long lKeyinData = *pnKeyInData;
	int nCancel = GetUserKeyInData(&lKeyinData, lRef, lLowLmt, lHighLmt, bRangeMask, bPassword);
	if (m_bKeyInValid) *pnKeyInData = (short)lKeyinData;
	return (nCancel == CANCEL_SELECTED ? CANCEL_SELECTED : 0);
}

int CKeyPadHandler::GetUserKeyInData(double *pdKeyInData, double dRef, long lLowLmt, long lHighLmt, bool bRangeMask)
{
	CString str;
	str.Format(_T("%0.1f"), dRef);
	int nCancel = PopupKeyPadDlg(str, lLowLmt, lHighLmt);

	if (m_bKeyInValid)
	{
		double dKeyInData;
		if (wcscmp(m_acEditBuffer, _T("empty")) != 0)
			dKeyInData = _wtof(m_acEditBuffer);
		else 
			dKeyInData = *pdKeyInData;

		if ((lLowLmt == lHighLmt) && (bRangeMask == false))
			*pdKeyInData = dKeyInData;
		else
		{
			if ((dKeyInData < lLowLmt) || (dKeyInData > lHighLmt)
				|| ((lLowLmt == lHighLmt) && bRangeMask && (dKeyInData != (double)lLowLmt)))
			{
				CStringList sList;
				sList.AddTail(theMsg.GetMsg(MSG_PARA_OUT_OF_RANGE));
				theMsg.ShowMsg(WARNING_MSG, sList);
				m_bKeyInValid = false;
			}
			else
				*pdKeyInData = dKeyInData;
		}
	}

	return (nCancel == CANCEL_SELECTED ? CANCEL_SELECTED : 0);
}

int CKeyPadHandler::GetUserKeyInData(wchar_t *pcString, unsigned short nLength)
{
	PopupKeyPadDlg();

	if (m_bKeyInValid)
	{
		if (wcslen(m_acEditBuffer) > nLength)
		{
			CStringList sList;
			sList.AddTail(theMsg.GetMsg(MSG_FILE_NAME_TOO_LONG));
			theMsg.ShowMsg(WARNING_MSG, sList);
			m_bKeyInValid = false;
		}
		else
			wcscpy_s(pcString, _countof(m_acEditBuffer), m_acEditBuffer);
	}

	return 0;
}

int CKeyPadHandler::PopupKeyPadDlg(CString str, long lLowLmt, long lHighLmt, bool bPassword)
{
    m_bKeyInValid = false;
    CKeyPadDlg dlg(str, lLowLmt, lHighLmt, bPassword);
    dlg.DoModal();
	return (dlg.m_bCancelSelected ? CANCEL_SELECTED : 0);
}

// Update keyin data from key pad dialog
void CKeyPadHandler::UpdateKeyInData(wchar_t *pcString)
{
    m_bKeyInValid = true;
    wcscpy_s(m_acEditBuffer, pcString);
}

// Hot key handler. Return 1 to skip further handler
BOOL CKeyPadHandler::HotKeyHandler(MSG *pMsg)
{
    BOOL bProcessed = FALSE;

	switch (pMsg->wParam)
	{
	case VK_F1:
	{
		bProcessed = TRUE;
		break;
	}
	case 'E':
	case 'e':
	{
		bProcessed = TRUE;
		break;
	}
	case 'F':
	case 'f':
	{
		if (m_anKeyPadStatus[KEY_WIRE_CLAMP].bProhibit == false)
		{
			if (pMsg->message == WM_KEYDOWN)
				m_anKeyPadStatus[KEY_WIRE_CLAMP].bKeyDown = true;
			else if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_WIRE_CLAMP].bKeyDown = false;
			bProcessed = TRUE;
		}
		break;
	}
	case 'D':
	case 'd':
	{
		if (m_anKeyPadStatus[KEY_EFO_SPARK].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_EFO_SPARK].bKeyDown = true;
			bProcessed = TRUE;
		}
		break;
	}
	case 'A':
	case 'a':
	{
		if (m_anKeyPadStatus[KEY_PANEL_LIGHT].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_PANEL_LIGHT].bKeyDown = true;
			bProcessed = TRUE;
		}
		break;
	}
	case 'I':
	case 'i':
	{
		if (m_anKeyPadStatus[KEY_INDEXING].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_INDEXING].bKeyDown = true;
			bProcessed = TRUE;
		}
		break;
	}
	case 'L':
	case 'l':
	{
		if (m_anKeyPadStatus[KEY_CLEAR_TRACK].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_CLEAR_TRACK].bKeyDown = true;
			bProcessed = TRUE;
		}
		break;
	}
	case 'H':
	case 'h':
	{
		if (m_anKeyPadStatus[KEY_IN_MAG_UP].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_IN_MAG_UP].bKeyDown = true;
			bProcessed = TRUE;
		}
		break;
	}
	case 'K':
	case 'k':
	{
		if (m_anKeyPadStatus[KEY_IN_MAG_DOWN].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_IN_MAG_DOWN].bKeyDown = true;
			bProcessed = TRUE;
		}
		break;
	}
	case 'N':
	case 'n':
	{
		if (m_anKeyPadStatus[KEY_IN_MAG_HOME].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_IN_MAG_HOME].bKeyDown = true;
			bProcessed = TRUE;
		}
		break;
	}
	case 'G':
	case 'g':
	{
		if (m_anKeyPadStatus[KEY_OUT_MAG_UP].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_OUT_MAG_UP].bKeyDown = true;
			bProcessed = TRUE;
		}
		break;
	}
	case 'J':
	case 'j':
	{
		if (m_anKeyPadStatus[KEY_OUT_MAG_DOWN].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_OUT_MAG_DOWN].bKeyDown = true;
			bProcessed = TRUE;
		}
		break;
	}
	case 'M':
	case 'm':
	{
		if (m_anKeyPadStatus[KEY_OUT_MAG_HOME].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_OUT_MAG_HOME].bKeyDown = true;
			bProcessed = TRUE;
		}
		break;
	}
	case 'O':
	case 'o':
	{
		if (m_anKeyPadStatus[KEY_WINDOW_CLAMP].bProhibit == false)
		{
			if (pMsg->message == WM_KEYDOWN)
				m_anKeyPadStatus[KEY_WINDOW_CLAMP].bKeyDown = true;
			else if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_WINDOW_CLAMP].bKeyDown = false;
			bProcessed = TRUE;
		}
		break;
	}
	case 'C':
	case 'c':
	{
		if (m_anKeyPadStatus[KEY_CHG_CAPILLARY].bProhibit == false)
		{
			if (pMsg->message == WM_KEYDOWN)
				m_anKeyPadStatus[KEY_CHG_CAPILLARY].bKeyDown = true;
			else if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_CHG_CAPILLARY].bKeyDown = false;
			bProcessed = TRUE;
		}
		break;
	}
	case 'B':
	case 'b':
	{
		if (m_anKeyPadStatus[KEY_USG].bProhibit == false)
		{
			if (pMsg->message == WM_KEYDOWN)
				m_anKeyPadStatus[KEY_USG].bKeyDown = true;
			else if (pMsg->message == WM_KEYUP)
			{
				//theBond.UsgSelfDefineBond(50000, 200);	//v3.1.T446 delete
				theBond.UsgSelfDefineBond(50000,
					(BYTE)theMcConfig.GetUsgManualPower());	//v3.1.T446 add
				m_anKeyPadStatus[KEY_USG].bKeyDown = false;
			}
			bProcessed = TRUE;
		}
		break;
	}
	case 'P':
	//case 'p':
	{
		if (m_anKeyPadStatus[KEY_BOND].bProhibit == false)
		{
			if (pMsg->message == WM_KEYDOWN)
				m_anKeyPadStatus[KEY_BOND].bKeyDown = true;
			else if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_BOND].bKeyDown = false;
			bProcessed = TRUE;
		}
		break;
	}
	case 'S':
	case 's':
	{
		if (m_anKeyPadStatus[KEY_STOP].bProhibit == false)
		{
			if (pMsg->message == WM_KEYDOWN)
				m_anKeyPadStatus[KEY_STOP].bKeyDown = true;
			else if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_STOP].bKeyDown = false;
			bProcessed = TRUE;
		}
		break;
	}
	case VK_UP:
	{
		if (m_anKeyPadStatus[KEY_UP].bProhibit == false)
		{
			if (pMsg->message == WM_KEYDOWN)
				m_anKeyPadStatus[KEY_UP].bKeyDown = true;
			else if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_UP].bKeyDown = false;
			bProcessed = TRUE;
		}
		break;
	}
	case VK_DOWN:
	{
		if (m_anKeyPadStatus[KEY_DOWN].bProhibit == false)
		{
			if (pMsg->message == WM_KEYDOWN)
				m_anKeyPadStatus[KEY_DOWN].bKeyDown = true;
			else if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_DOWN].bKeyDown = false;
			bProcessed = TRUE;
		}
		break;
	}
	case VK_LEFT:
	{
		if (m_anKeyPadStatus[KEY_LEFT].bProhibit == false)
		{
			if (pMsg->message == WM_KEYDOWN)
				m_anKeyPadStatus[KEY_LEFT].bKeyDown = true;
			else if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_LEFT].bKeyDown = false;
			bProcessed = TRUE;
		}
		break;
	}
	case VK_RIGHT:
	{
		if (m_anKeyPadStatus[KEY_RIGHT].bProhibit == false)
		{
			if (pMsg->message == WM_KEYDOWN)
				m_anKeyPadStatus[KEY_RIGHT].bKeyDown = true;
			else if (pMsg->message == WM_KEYUP)
				m_anKeyPadStatus[KEY_RIGHT].bKeyDown = false;
			bProcessed = TRUE;
		}
		break;
	}
	case 'Q':
	case 'q':
	{
		if (m_anKeyPadStatus[KEY_SPEED_DOWN].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
			{
				m_anKeyPadStatus[KEY_SPEED_DOWN].bKeyDown = false;
				theServo.JogSpeedDown();
			}
			bProcessed = TRUE;
		}
		break;
	}
	case 'R':
	case 'r':
	{
		if (m_anKeyPadStatus[KEY_SPEED_UP].bProhibit == false)
		{
			if (pMsg->message == WM_KEYUP)
			{
				m_anKeyPadStatus[KEY_SPEED_UP].bKeyDown = false;
				theServo.JogSpeedUp();
			}
			bProcessed = TRUE;
		}
		break;
	}
	default:
		break;
	}

	return bProcessed;
}

bool CKeyPadHandler::IsKeyDown(short nKeyIndex)
{
	bool bKeyDown = m_anKeyPadStatus[nKeyIndex].bKeyDown;
	switch (nKeyIndex)
	{
	case KEY_PANEL_LIGHT:
	case KEY_EFO_SPARK:
	case KEY_INDEXING:
	case KEY_CLEAR_TRACK:
	case KEY_IN_MAG_UP:
	case KEY_IN_MAG_DOWN:
	case KEY_IN_MAG_HOME:
	case KEY_OUT_MAG_UP:
	case KEY_OUT_MAG_DOWN:
		m_anKeyPadStatus[nKeyIndex].bKeyDown = false;	//reset key down status after process
		break;
	case KEY_OUT_MAG_HOME:
		if (theMcConfig.GetKeyPadType() != 3)
			m_anKeyPadStatus[nKeyIndex].bKeyDown = false;	//reset key down status after process
		break;
	default:
		break;
	}
    return (bKeyDown);
}

void CKeyPadHandler::SetKeyDown(short nKeyIndex, bool bKeyDown)
{
    m_anKeyPadStatus[nKeyIndex].bKeyDown = bKeyDown;
}

void CKeyPadHandler::SetKeyProhibit(short nKeyIndex, bool bProhibit)
{
    m_anKeyPadStatus[nKeyIndex].bProhibit = bProhibit;
}

bool CKeyPadHandler::IsKeyProhibited(short nKeyIndex)	//V1.1.W270 add
{
	return m_anKeyPadStatus[nKeyIndex].bProhibit;
}
