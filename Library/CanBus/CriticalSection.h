//CriticalSection.h v1.0.T1

#pragma once

template<class CCS> class CScopedCS
{
public:
	CScopedCS(CCS* pCS)
	{
		m_pCS = pCS;
		m_pCS->Enter();
	};
	~CScopedCS()
	{
		if (m_pCS != NULL) m_pCS->Leave();
	};
	CScopedCS& operator = (CScopedCS const&);
private:
	CCS* m_pCS;
};

class CCriticalSection
{
public:
	CCriticalSection();
	~CCriticalSection();
	void Enter();
	void Leave();
private:
	CRITICAL_SECTION m_CS;
};

