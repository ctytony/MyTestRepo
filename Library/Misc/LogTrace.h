//LogTrace.h v1.0.T24

#ifndef __LOGTRACE_H__
#define __LOGTRACE_H__

//v1.0.T24 delete
//#define DIR_DATALOG			"D:\\DATALOG\\"			//v1.0.T12 add
//#define DIR_DATALOG_VISION	"D:\\DATALOG\\VISION\\"	//v1.0.T16 add
//#define DIR_DATALOG_IMAGE		"D:\\DATALOG\\IMAGE\\"	//v1.0.T22 add

//v1.0.T14 add
enum EN_LOG_MODE
{
	LOG_MACHINE_DATA,
	LOG_VERIFY_RECIPE,
	LOG_STATISTIC,		//v1.0.T19 add
	LOG_CALI_POSITION,	//v1.0.T23 add
};

class CLogTrace
{
public:
	CLogTrace(CString sSoftwareVersion);
	~CLogTrace();
private:	//variable
	EN_LOG_MODE m_nLogMode;		//v1.0.T14 add
	CString m_sFilePrefix[4];	//v1.0.T14 add, v1.0.T23 edit
	CString m_sSoftwareVersion;
	CString m_sAppName;
	CString m_sLastMsg;
	bool m_bEnableWrite;
	bool m_bTimeStamp;
	bool m_bOriEnableWrite;		//v1.0.T14 add
	bool m_bOriTimeStamp;		//v1.0.T14 add
private:	//function
	CString GetFileName();			//v1.0.T18 add
	CString GetBackupFileName();	//v1.0.T18 add
public:		//function
	void BackupLogFile();		//v1.0.T15 rename
	void SetLogMode(EN_LOG_MODE nMode);	//v1.0.T14 add
	void WriteLine(CString sMsg, bool bForceWrite = false);
	void WriteString(LPCTSTR sFormat, LPCTSTR sString, bool bForceWrite = false);
	void WriteLong(LPCTSTR sFormat, long lValue, bool bForceWrite = false);
	//e.g. theLog.WriteLong(_T("%d"), lValue);
	void WriteLong2(LPCTSTR sFormat, long lValue1, long lValue2, bool bForceWrite = false);
	//e.g. theLog.WriteLong2(_T("%d/%d"), lValue1, lValue2);
	void WriteLong4(LPCTSTR sFormat, long lValue1, long lValue2, long lValue3, long lValue4, bool bForceWrite = false);
	//e.g. theLog.WriteLong4(_T("%d/%d/%d/%d"), lValue1, lValue2, lValue3, lValue4);
	void WriteDouble(LPCTSTR sFormat, double fValue, bool bForceWrite = false);
	//e.g. theLog.WriteDouble(_T("%f"), fValule);
	void WriteDouble2(LPCTSTR sFormat, double fValue1, double fValue2, bool bForceWrite = false);
	//e.g. theLog.WriteDouble2(_T("%f/%f"), fValule1, fValue2);
	void WriteDouble3(LPCTSTR sFormat, double fValue1, double fValue2, double fValue3, bool bForceWrite = false);	//v1.0.T10 add
	//e.g. theLog.WriteDouble3(_T("%f/%f"), fValule1, fValue2, fValue3);
	void SetActive(bool bForceWrite, bool bTimeStamp);
};

extern CLogTrace theLog;

#endif // __LOGTRACE_H__
