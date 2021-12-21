//MsgManager.h v3.0.T292

#pragma once

#include "WarnMsgDlg.h"

class CMsgManager
{
public:
	CMsgManager(void);
	~CMsgManager(void);
public:		//function
	void SetProcessKeyboard(bool bProcess);	//v2.0.T33 add
	void Init();	//V1.2.W12 add
	void GetMsg(short nMsgID, WCHAR *pcMsg);	//v3.0.T199 rename
	CString GetMsg(short nMsgID);		//V1.1.W145 add, v3.0.T199 rename
	CString GetEngMsg(short nMsgID);	//v2.0.T80 add
    // Show message box for help, error, or warning
    int ShowMsg(short nMsgType, short nMsgNum, short *pnMsgIndex, bool bStopValid = false, bool bSkipValid = false, bool bCenter = false);	//v3.0.T200 rename
    int ShowMsg(short nMsgType, CStringList& sList, bool bStopValid = false, bool bSkipValid = false);	//v3.0.T200 rename
    int ShowMhsMsg(short nMsgType, CStringList& sList, short nCallBackId, short nClearTrackId);	//v3.0.T200 rename
    int ShowMsgS(short nMsgType, CStringList& sList, bool bStopValid = false, bool bSkipValid = false, bool CtrLight = false);	//v3.0.T200 rename
	int ShowDynamicBtnMsg(CStringList& sList, short nOkBtnTextID, short nCancelBtnTextID);
    int ShowMsgX(short nMsgType, CStringList& sList, bool bStopValid = false, bool bSkipValid = false, short nMode = 0);	//v3.0.T200 rename
	void SetMsgLock(bool bLock);
	bool IsMsgLock(void);
	BYTE IsEqual(short nMsg, LPTSTR psMsg);	//V1.1.W154 add
private:	//variable
	CString m_asString[50];
	CWarnMsgDlg m_MhsMsgDlg;
	bool m_bProcessKeyboard;	//v2.0.T33 move from public
	bool m_bMsgLock;			//one message a time is shown
};

extern CMsgManager theMsg;	//v3.0.T198 rename
