#include "StdAfx.h"
#include "Jpeg.h"

CJpeg::CJpeg()
{
	m_pIPicture = NULL;
}

CJpeg::~CJpeg()
{
	UnLoad();
}

BYTE CJpeg::Load(HINSTANCE hInst, UINT nRes)
{
	UnLoad();

	int nSize = 0;

	HRSRC hResInfo = FindResource(hInst, MAKEINTRESOURCE(nRes), _T("JPEG"));
	if (hResInfo == NULL) return FALSE;

	HANDLE hRes = LoadResource(hInst, hResInfo);
	if (hRes == NULL) return FALSE;

	LPSTR lpBuf = (LPSTR)LockResource(hRes);
	if (lpBuf != NULL)
		nSize = SizeofResource(hInst, hResInfo);

	if (nSize > 0)
	{
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
		LPVOID pData = GlobalLock(hGlobal);

		memcpy(pData,lpBuf,nSize);

		IStream* pStream = NULL;

		if (CreateStreamOnHGlobal(hGlobal,TRUE,&pStream) == S_OK)
		{
			OleLoadPicture(pStream,nSize,TRUE,IID_IPicture,(LPVOID*)&m_pIPicture);
			pStream->Release();
		}

		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);
	}

	UnlockResource(hRes); 
	FreeResource(hRes);

	return TRUE;
}

void CJpeg::UnLoad()
{
	if (m_pIPicture == NULL) return;

	m_pIPicture->Release();
	m_pIPicture = NULL;
}

void CJpeg::Paint(HDC hDC, CRect& rc)
{
	if (m_pIPicture == NULL) return;

	long hmWidth, hmHeight;
	m_pIPicture->get_Width(&hmWidth);
	m_pIPicture->get_Height(&hmHeight);
	m_pIPicture->Render(hDC, rc.left, rc.top, rc.Width(), rc.Height(),
						0, hmHeight, hmWidth, -hmHeight, NULL);
}
