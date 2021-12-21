
//#if _MSC_VER >= 1000
#pragma once
//#endif // _MSC_VER >= 1000
#include "DigitalIO.h"  // 20081030




#define EFO_STATUS_TRIGGER   0x1
#define EFO_STATUS_SUCCESS   0x2

#define EFO_NO_TRIGGER  0
#define EFO_TRIGGER_NOT_GOOD  1
#define EFO_TRIGGER_AND_GOOD  2

class EFO_Monitor : public CObject
{
public:
	EFO_Monitor();
	virtual ~EFO_Monitor();
	
	void EFO_start_monitor();
	void EFO_stop_monitor();
	void EFO_resume_monitor();
	void EFO_pause_monitor();
	void EFO_reset_status();
	void EFO_prepare_trigger();
	int EFO_check_trigger();

	UINT IntEventThread();
	int iStatusEFO;
	unsigned int uiCountTriggerEFO;
	unsigned int uiCountSuccessEFO;
	unsigned int uiTrigCountBeforeTrigger;
	unsigned int uiSuccessCountBeforeTrigger;

	void EFO_enable_1739_int(LONG lDriverHandle1739);


private:
	// iStatusEFO = 0 : Fail
	//         = 1 : Triggered but not success
	//         = 2 : Abnormal success
	//         = 3 : Trigger and success
	BOOL m_bRunEFO_MonitorThread;

	void EFO_CheckStatusByDigitalIO();
	CWinThread*     m_pThreadIntPCI;
};

