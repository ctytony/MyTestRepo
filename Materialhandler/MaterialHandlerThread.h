#pragma once

//#define WH_VERSION_VERTICAL    1
#define WH_VERSION_VERTICAL    2

#define WH_VERSION_R2R			1
#define WH_VERSION_TOP			1
#define WH_VERSION_CUELITE		1
#define WH_VERSION_VELITE		1

// CMaterialHandlerThread command target

#define VALVE_MOTION_TIME		300    //300ms
#define MAX_CLEAR_TRACK_TIMES	6

enum enMhsError
{
    MHS_NO_ERROR,
    MHS_CLAW_MISSING_STEPS,
    MHS_ONLOAD_EMPTY,
    MHS_ONLOAD_TRACK_JAM,
    MHS_OFFLOAD_KICKER_JAM,
    MHS_OFFLOAD_PUSHER_JAM,
    MHS_OFFLOAD_TRACK_JAM,
    MHS_OFFLOAD_FORK_FULL,
    MHS_ONLOAD_DOUBLE_LEAD_FRAME,
	MHS_END_ALL_LF,

	MHS_TOP_ONLOAD_NO_LF,
	MHS_TOP_ONLOAD_NO_MAG,
	MHS_TOP_OFFLOAD_NO_MAG,
	MHS_TOP_SRCH_TIME_OUT,
	MHS_TOP_EJECT_DIST_NOT_ENOUGH,
	MHS_TOP_ONLOADER_NO_RESPONSE,
	MHS_TOP_ONLOAD_MAG_PULLY_FULL,
	MHS_TOP_OFFLOAD_MAG_PULLY_FULL,
	MHS_TOP_ONLOAD_PULLY_MOTOR_JAM,
	MHS_TOP_OFFLOAD_PULLY_MOTOR_JAM,
	MHS_TOP_OFFLOAD_TIME_OUT,
	MHS_TOP_BH_NOT_IN_SAFE_POS,					//v3.1.T219 rename
	MHS_TOP_ONLOAD_PUSH_KICKER_WRONG,
    MHS_TOP_ONLOAD_MAG_BUF_EMPTY,
	MHS_TOP_OFFLOAD_MAG_BUF_EMPTY,
	MHS_TOP_WINDOW_CLAMPER_NOT_CLOSED,			//V1.5.W11 add
	MHS_TOP_WINDOW_CLAMPER_NOT_OPENED,			//V1.5.W11 add

	//for R2R MHS
	MHS_R2R_ONLOAD_STOP,
	MHS_R2R_OFFLOAD_STOP,
	MHS_R2R_ONLOAD_ERROR,
	MHS_R2R_OFFLOAD_ERROR,

    //for CuElite MHS
    MHS_CUELITE_ONLOAD_NO_LF,
	MHS_CUELITE_ONLOAD_NO_MAG,
	MHS_CUELITE_OFFLOAD_NO_MAG,
	MHS_CUELITE_SRCH_TIME_OUT,
	MHS_CUELITE_EJECT_DIST_NOT_ENOUGH,
	MHS_CUELITE_ONLOADER_NO_RESPONSE,
	MHS_CUELITE_ONLOAD_MAG_BUF_EMPTY,
	MHS_CUELITE_OFFLOAD_MAG_BUF_EMPTY,
	MHS_CUELITE_ONLOAD_PULLY_MOTOR_JAM,
	MHS_CUELITE_OFFLOAD_PULLY_MOTOR_JAM,
	MHS_CUELITE_OFFLOAD_TIME_OUT,
	MHS_CUELITE_BOND_HEAD_NOT_SEFTY,
	MHS_CUELITE_INJECTOR_ERROR,
    MHS_CUELITE_OFFLOAD_JAM,
	MHS_CUELITE_ONLOAD_MAG_DROP,
	MHS_CUELITE_OFFLOAD_MAG_DROP,
	MHS_CUELITE_ONLOAD_MAG_PULLY_FULL,
	MHS_CUELITE_OFFLOAD_MAG_PULLY_FULL,
	MHS_CUELITE_ONLOAD_PUSH_KICKER_WRONG,

	MHS_VE_OFFLOAD_MAG_ARM_EMPTY,
	MHS_VE_OFFLOAD_MAG_ARM_FULL,
	MHS_VE_OFFLOAD_MAG_DROP_FAIL,
	MHS_VE_OFFLOAD_MAG_KICKER_FULL,
	MHS_VE_OFFLOAD_LF_OUTPUT_JAM,

	MHS_ERROR_MAX,
};

enum enMhsWarning
{
    MHS_NO_WARNING,
    MHS_WARNING_ONLOAD_FORK_NOT_EXIST,
    MHS_WARNING_OFFLOAD_INNER_FORK_NOT_EXIST,
    MHS_WARNING_OFFLOAD_INNER_FORK_FULL,
};

class CMaterialHandlerThread : public CObject
{
public:
	CMaterialHandlerThread();
	virtual ~CMaterialHandlerThread();
public:	//variables
	bool m_bIndexBusy;
	long m_lOnloadRefreshIOStartTime;
	long m_lOffloadRefreshIOStartTime;
	long m_lOnloadFullStartTime;
	long m_lOffloadFullStartTime;
public:	//functions
	void VerifyData();	//v3.0.T58 add
	void ClearLastLf(void);
    int BeginThread(void);
	UINT VledWin32Timing();
	UINT VEliteWin32Timing();
    void Init(void);
	void SetThreadActive(bool bActive);
	bool IsThreadActive(void);
	void SaveMhsRcd(void);
    void LoadMhsRcd(void);
    void SetAutoIndex(bool bAutoIndex);
    bool IsAutoIndex(void);
    bool IsDryRun(void);
	void SetDryRun(bool bDryRun);
    void InitAutoIndex(void);
    int WarmReset(void);
    int ClearTrack();
    //Report material handler error
    //add bool bWorkerThread
    //because worker thread CANNOT display modeless dialog directly, 
    //mode dialog is no issue
    void ReportError(short* panErrorCode, bool bWorkerThread = false);
    void SetStopAll(bool bStopAll);
    bool IsStopAll(void);
    void SaveMhsPowerOnData(void);
    void LoadMhsPowerOnData(void);
    //Check material handler error status
    //add bool bWorkerThread
    //because worker thread CANNOT display modeless dialog directly, 
    //mode dialog is no issue
    short ShowError(bool bWorkerThread = false);
	void SetSensorEnable(bool bSensorValid);
	void SetThreadStop(bool bStop);
	bool IsThreadStop(void);
	int ResetWorkHolder(void);
	void SetLastLf(bool bLastLf);
	bool IsLastLf(void);
	int LastLeadframeHandle(void);
	bool IsOxidationProtect(void);
	void SetOxidationProtect(bool bProtect);
	short GetCurBondUnit(void);
	void ResetCurBondUnit();
    void UpdateDebugInfo(void);
	short GetTopStopReason(void);
    short GetCuEliteStopReason(void);
	short GetR2RStopReason(void);
	bool IsSameMhsError(short anErrorList[], short nError,int nTotalError);
    short GetMhsDataVersion(void);
    int IndexLeadFrame(void);
    void ToggleLfClamper(void);
    int SaveWorkHolderData(CString szWorkHolderFileName);
    int LoadWorkHolderData(CString szWorkHolderFileName); 
	int CloseWindowClamp(void);
    int PreCloseWindowClamp(void);
	bool IsBusy(void);
	void SetBusy(bool bBusy);
	bool IsRequestIndex(void);
	void SetRequestIndex(bool bIndex);
private:	//variables
	static DWORD WINAPI ThreadProc(LPVOID lpVoid);
    CWinThread *m_pMhsThread;
	CWinThread *m_pIndexThread;
    bool m_bThreadActive;
    bool m_bThreadStop;
    bool m_bRequestIndex;
    bool m_bBusy;
    bool m_bAutoIndex;
	bool m_bOxidationProtect;
	short m_nWhVersion;
    bool m_bDryRun;
	bool m_bLastLf;
	bool m_bStopAll;			// Stop all moving parts for emergency case
	bool m_bSensorEnable;
};

extern CMaterialHandlerThread theMHS;
