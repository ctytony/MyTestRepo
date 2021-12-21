/***************************************************************
FileOpenDlg.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "dlgs.h"
#include "WireBond.h"
#include "KeyPadHandler.h"
#include "FileOpenDlg.h"
#include <memory>

// CFileOpenDlg

IMPLEMENT_DYNAMIC(CFileOpenDlg, CFileDialog)

CFileOpenDlg::CFileOpenDlg(BOOL bOpenFileDialog,
	LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags,
	LPCTSTR lpszFilter, CWnd* pParentWnd) :
	CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName,
					dwFlags, lpszFilter, pParentWnd)
{
    GetOFN().nMaxFileTitle = FILE_NAME_MAX_CHARS;
    GetOFN().lpstrFile = m_sBuffer.GetBuffer(FILE_NAME_MAX_CHARS);
    GetOFN().nMaxFile = FILE_NAME_MAX_CHARS;
    if (GetOFN().lpstrFile == NULL) 
    {
		AfxMessageBox(AFX_IDP_FAILED_MEMORY_ALLOC); 
        return; 
    }
}

CFileOpenDlg::~CFileOpenDlg()
{
	m_sBuffer.ReleaseBuffer();
}

BEGIN_MESSAGE_MAP(CFileOpenDlg, CFileDialog)
END_MESSAGE_MAP()

void CFileOpenDlg::SetFileName(LPWSTR lpszFile)
{
	::OpenClipboard(NULL);
	EmptyClipboard();

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (lstrlen(lpszFile)+1) * sizeof(TCHAR));
	LPTSTR lpsz = (LPTSTR)GlobalLock(hGlobal);

	lstrcpy(lpsz, lpszFile);
	GlobalUnlock(hGlobal);
	SetClipboardData(CF_UNICODETEXT, hGlobal);

	GlobalFree(hGlobal);
	CloseClipboard();

	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.dwFlags = 0;

	ip.ki.wVk = VK_CONTROL;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.wVk = 'V';
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
 
	ip.ki.wVk = VK_CONTROL;
	SendInput(1, &ip, sizeof(INPUT));
}

// CFileOpenDlg message handlers
void CFileOpenDlg::OnFileNameChange()
{
    if (GetFileTitle().IsEmpty() && FindWindow(NULL, L"Key Pad") == NULL)
    {
		std::auto_ptr<wchar_t> lpszBuf(new wchar_t[FILE_NAME_MAX_CHARS]);
		lstrcpy(lpszBuf.get(), _T("Empty"));

        theKeyPad.GetUserKeyInData(lpszBuf.get(), FILE_NAME_MAX_CHARS);
		SetFileName(lpszBuf.get());
        //SetControlText(cmb13, lpszBuf.get());
    }
                        
    CFileDialog::OnFileNameChange();
}
