#pragma once

class CControlState
{
public:
	CControlState(void);
	CControlState(HWND hWnd, BYTE nInitial, BYTE nFinal);
	~CControlState(void);
	void AddControl(HWND hWnd, BYTE nInitial, BYTE nFinal);
	void UpdateFinalState(HWND hWnd, BYTE nFinal);
protected:
	CList<LPSTR, LPSTR> m_list;
};
