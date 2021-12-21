#pragma once

class CJpeg
{
public:
	CJpeg();
	~CJpeg();
public:
	BYTE Load(HINSTANCE hInst, UINT nRes);
	void UnLoad();
	void Paint(HDC hDC, CRect& rc);
protected:
	IPicture* m_pIPicture;
};
