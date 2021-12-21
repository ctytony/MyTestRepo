#pragma once

// CServoTune command target

class CServoTune : public CObject
{
public:
	CServoTune();
	virtual ~CServoTune();
    // Verify bond head performance
    int VerifyBondHeadPerformance(short nDebugMode);
    // Verify XY table performance
    int VerifyTablePerformance(void);
    // Inform stop tuning
    int InformStopTuning(bool bStopTune);
private:
    // Stop tuning flag
    bool m_bStopTuning;
};

extern CServoTune theServoTune;
