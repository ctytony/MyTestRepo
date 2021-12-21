#pragma once

class CBusy
{
public:
	CBusy(LPBOOL pbBusy);
	~CBusy();
protected:
	LPBOOL m_pbBusy;
};
