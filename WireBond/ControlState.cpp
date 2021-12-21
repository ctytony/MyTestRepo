#include "StdAfx.h"
#include "ControlState.h"

CControlState::CControlState(void)
{
}

CControlState::CControlState(HWND hWnd, BYTE nInitial, BYTE nFinal)
{
	AddControl(hWnd, nInitial, nFinal);
}

CControlState::~CControlState(void)
{
	while (m_list.GetCount())
	{
		LPSTR lpszBuf = m_list.RemoveHead();
		EnableWindow(*(HWND*)lpszBuf, *(BYTE*)(lpszBuf + sizeof(HWND)));
		delete lpszBuf;
	}
}

void CControlState::AddControl(HWND hWnd, BYTE nInitial, BYTE nFinal)
{
	EnableWindow(hWnd, nInitial);

	LPSTR lpszBuf = new char[sizeof(HWND) + sizeof(BYTE)];
	*(HWND*)lpszBuf = hWnd;
	*(BYTE*)(lpszBuf + sizeof(HWND)) = nFinal;
	m_list.AddTail(lpszBuf);
}

void CControlState::UpdateFinalState(HWND hWnd, BYTE nFinal)
{
	for (BYTE n = 0; n < m_list.GetCount(); n++)
	{
		LPSTR lpszBuf = m_list.GetAt(m_list.FindIndex(n));
		if (*(HWND*)lpszBuf != hWnd) continue;

		*(LPBYTE)(lpszBuf + sizeof(HWND)) = nFinal;
		break;
	}
}
