#pragma once

// CKeyPadHandler command target
#define CANCEL_SELECTED  1

// Key pad function relation to charactor
enum EN_KEYPAD_CHARACTER
{
	KEY_WIRE_CLAMP,			//'F' or 'f'
	KEY_WIRE_FEED,			//'E' or 'e'
	KEY_EFO_SPARK,			//'D' or 'd'
	KEY_CHG_CAPILLARY,		//'C' or 'c'
	KEY_USG,				//'B' or 'b'
	KEY_PANEL_LIGHT,		//'A' or 'a'
	KEY_INDEXING,			//'I' or 'i'
	KEY_CLEAR_TRACK,		//'L' or 'l'
	KEY_WINDOW_CLAMP,		//'O' or 'o'
	KEY_SPEED_UP,			//'R' or 'r'
	KEY_SPEED_DOWN,			//'Q' or 'q'
	KEY_IN_MAG_UP,			//'H' or 'h'
	KEY_IN_MAG_DOWN,		//'K' or 'k'
	KEY_IN_MAG_HOME,		//'N' or 'n'
	KEY_OUT_MAG_UP,			//'G' or 'g'
	KEY_OUT_MAG_DOWN,		//'J' or 'j'
	KEY_OUT_MAG_HOME,		//'M' or 'm'
	KEY_BOND,				//'P' or 'p'
	KEY_STOP,				//'S' or 's'
	KEY_0,					//'0'
	KEY_1,					//'1'
	KEY_2,					//'2'
	KEY_3,					//'3'
	KEY_4,					//'4'
	KEY_5,					//'5'
	KEY_6,					//'6'
	KEY_7,					//'7'
	KEY_8,					//'8'
	KEY_9,					//'9'
	KEY_DEL,				//VK_DELETE
	KEY_ESC,				//VK_ESCAPE
	KEY_ENTER,				//VK_RETURN
	KEY_DECIMAL,			//VK_DECIMAL
	KEY_LEFT,				//VK_LEFT
	KEY_RIGHT,				//VK_RIGHT
	KEY_UP,					//VK_UP
	KEY_DOWN,				//VK_DOWN
	KEY_F1,					//VK_F1
	KEY_F2,					//VK_F2
	KEY_F3,					//VK_F3
	KEY_F4,					//VK_F4
	KEY_F5,					//VK_F5
	KEY_F6,					//VK_F6
	KEY_SETUP,				//v3.0.T452 add
	KEY_RECIPE,				//v3.0.T454 add
	MAX_KEY_NUM,
};

struct ST_KEYPAD_STATUS
{
    bool bKeyDown;
    bool bProhibit;
};

class CKeyPadHandler : public CObject
{
public:
	CKeyPadHandler();
	virtual ~CKeyPadHandler();
public:	//functions
    // Get user key in data from key pad
    int GetUserKeyInData(long *plKeyInData, long lRef = 0, long lLowLmt = 0, long lHighLmt = 0, bool bRangeMask = false, bool bPassword = false);
    int GetUserKeyInData(int *pnKeyInData, long lRef = 0, long lLowLmt = 0, long lHighLmt = 0, bool bRangeMask = false);
    int GetUserKeyInData(short *pnKeyInData, long lRef = 0, long lLowLmt = 0, long lHighLmt = 0, bool bRangeMask = false, bool bPassword = false);
    int GetUserKeyInData(double *pdKeyInData, double dRef = 0, long lLowLmt = 0, long lHighLmt = 0, bool bRangeMask = false);
    int GetUserKeyInData(wchar_t *pcString, unsigned short nLength = 255);
    // Update keyin data from key pad dialog
    void UpdateKeyInData(wchar_t *pcString);
    BOOL HotKeyHandler(MSG *pMsg);
    bool IsKeyDown(short nKeyIndex);
    void SetKeyProhibit(short nKeyIndex, bool bProhibit);
    void SetKeyDown(short nKeyIndex, bool bKeyDown);
	bool IsKeyProhibited(short nKeyIndex);	//V1.1.W270 add
private:	//variables
    wchar_t m_acEditBuffer[255];
    bool m_bKeyInValid;
    ST_KEYPAD_STATUS m_anKeyPadStatus[MAX_KEY_NUM];
private:	//functions
    int PopupKeyPadDlg(CString str = _T(""), long lLowLmt = 0, long lHighLmt = 0, bool bPassword = false);
};

extern CKeyPadHandler theKeyPad;
