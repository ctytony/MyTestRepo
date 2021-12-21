#pragma once

// CMapEdit
class CMapEdit : public CEdit
{
	DECLARE_DYNAMIC(CMapEdit)

public:

	CMapEdit();
	virtual ~CMapEdit();

    afx_msg void OnSetFocus(CWnd* pOldWnd);

    short GetUnitValue(void);
    void SetParamRange(long lRefValue, long lLowerLimit, long lUpperLimit);
    void SetUnitValue(short nUnitValue);

protected:

	DECLARE_MESSAGE_MAP()

private:

    short m_nUnitValue;
    long m_lUpperLimit;
    long m_lLowerLimit;
    long m_lDefaultValue;
};
