//SerialComIO.h v1.0.T7

#pragma once

// CSerialComIO command target

enum LIGHT_CHN 
{
    CHN_COAX_RED_LIGHT = 1, 
    CHN_SIDE_RED_LIGHT = 2,
    CHN_SIDE_BLUE_LIGHT = 3,
    CHN_COAX_BLUE_LIGHT = 4 
};

class CSerialCommIO : public CObject
{
public:
	CSerialCommIO();
	virtual ~CSerialCommIO();
public:
    int InitLight(void);
    int UpdateLightingParameter(short nChannel, short nLevel);
	int InitUSG(void);
	int Digital_IO_Send(BYTE* p_BufferSend, DWORD BufferByteLength);
	int Digital_IO_Rev(BYTE* pBufferRev,DWORD dwBytesRead);
private:
    HANDLE m_hComLight;
	HANDLE m_hComUSG;
};

extern CSerialCommIO theSerialCommIO;