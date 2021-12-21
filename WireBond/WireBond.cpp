/***************************************************************
WireBond.cpp : implementation file
****************************************************************/

#include "stdafx.h"
#include "LimitSingleInstance.h"
#include "WireBond.h"
#include "MsgManager.h"
#include "KeyPadHandler.h"
#include "MachineData.h"
#include "DeviceData.h"
#include "Program.h"
#include "Reference.h"
#include "Wire.h"
#include "BondPara.h"
#include "BallPara.h"
#include "LoopPara.h"
#include "Bond.h"
#include "VisionView.h"
#include "WireBondDlg.h"
#include "macdef.h"
#include "DigitalIO.h"
#include "SerialComIO.h"
#include "MaterialHandlerThread.h"
#include "ScanKeyThread.h"
#include "InitDlg.h"
#include "VisionSystem.h"
#include "UsgControl.h"
#include "MsgDef.h"
#include "MachineConfig.h"
#include "BondStatistics.h"
#include "AutoPadMgr.h"
#include "EfoProfile.h"
#include "MachineConfig.h"
#include "TowerLight.h"
#include "CanController.h"
#include "Register.h"
#include "CuOffload.h"
#include "Debug.h"
#include "User.h"
#include "servo.h"
#include "ForceSwitch.h"
#include "Fnk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

WCHAR wcSoftwareVersion[] = _T("v3.1.T580sc");

// The one and only CLimitSingleInstance object.
CLimitSingleInstance gSingleInstanceObj(TEXT("Global\\{Wire Bonder}"));

// CWireBondApp

BEGIN_MESSAGE_MAP(CWireBondApp, CWinApp)
	//ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CWireBondApp construction

CWireBondApp::CWireBondApp()
: m_bkColor(0)
{
}

// The one and only CWireBondApp object
CWireBondApp	theApp;
CKeyPadHandler	theKeyPad;
CLogTrace		theLog(wcSoftwareVersion);
CJpeg			theJpegHelp;	//V1.1.W147 add

void CWireBondApp::RecordCurrentTime(short nFilename)
{
	char  cFileName[28] ;
	if (nFilename == FILE_COLDSTART)
		strcpy(cFileName, "D:\\WBDATA\\McColdStart.dat");
	else
	{
		if (nFilename == FILE_COLDEND)
			strcpy(cFileName, "D:\\WBDATA\\McColdEnd.dat");
		else
			return;
	}
	// current date/time based on current system
	time_t tNow = time(0);

	// convert to string form
	char* dt = ctime(&tNow);
	FILE* pFile;
	if (fopen_s(&pFile, cFileName, "wt") == 0)
	{
		fprintf_s(pFile, "%s\n", dt);
		fclose(pFile);
	} 
}

//This function is to check whether last time the machine quit as normal,
//by comparing two files update time
bool CWireBondApp::IsAbnormalExit()
{
	if (!theMcConfig.IsCheckAbnormalShutdown()) return false;	//v3.1.T218 add

	CString sDir;
	sDir.Format(_T("%s%s"), _T(DIR_WBDATA), _T("McColdStart.dat"));
	HANDLE hFileStart = CreateFile((LPCTSTR)sDir, GENERIC_READ,
		NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	sDir.Format(_T("%s%s"), _T(DIR_WBDATA), _T("McColdEnd.dat"));
	HANDLE hFileEnd = CreateFile((LPCTSTR)sDir, GENERIC_READ,
		NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);;

	if ((hFileStart == INVALID_HANDLE_VALUE)
		|| (hFileEnd == INVALID_HANDLE_VALUE))
	{
		CloseHandle(hFileStart);
		CloseHandle(hFileEnd);
		return false;
	}
	FILETIME tStart, tEnd;
	GetFileTime(hFileStart, NULL, NULL, &tStart);
	GetFileTime(hFileEnd, NULL, NULL, &tEnd);
	CloseHandle(hFileStart);
	CloseHandle(hFileEnd);
		
	if (CompareFileTime(&tStart, &tEnd) == 1)	//start file time is later than end file time = 1
		return true;
	
	return false;
}

//v3.0.T13 add
void CWireBondApp::InitPowerOnData()
{
	CString asFileName[2] = {_T("PowerOn.dat"),_T("WhData.dat")};
	CString sSrcDir, sDestDir;
	sSrcDir = _T(DIR_WBDATA);
	sDestDir = _T(DIR_MCDATA);

	for (short i=0; i<2; i++)
	{
		if (CopyFile((LPCWSTR)(sSrcDir + asFileName[i]), 
				(LPCWSTR)(sDestDir + asFileName[i]), false) == false)
		{
			::MessageBox(NULL, _T("Copy failed!"), (LPCWSTR)asFileName[i],  MB_OK);
		} 
	}	
}

//v2.0.T122 add
void CWireBondApp::InitMcdata()
{
	CString asFileName[2] = {_T("McData.dat"),_T("CtrlData.dat")};
	CString sSrcDir, sDestDir;
	sSrcDir = _T(DIR_WBDATA);
	sDestDir = _T(DIR_MCDATA);

	for (short i=0; i<2; i++)
	{
		if (CopyFile((LPCWSTR)(sSrcDir + asFileName[i]), 
				(LPCWSTR)(sDestDir + asFileName[i]), false) == false)
		{
			::MessageBox(NULL, _T("Copy failed!"), (LPCWSTR)asFileName[i],  MB_OK);
		} 
	}	
}

//v2.0.T121 add
void CWireBondApp::BackupMcdata()
{
	CString asFileName[3] = {_T("McData.dat"),_T("CtrlData.dat"),_T("WhData.dat")};	//v3.0.T14 edit
	CString sSrcDir = _T(DIR_MCDATA);
	CString sDestDir = _T(DIR_MCDATA_BACKUP);

	CString sMsg;
	for (short i=0; i<3; i++)	//v3.0.T14 edit
	{
		if (CopyFile((LPCWSTR)(sSrcDir + asFileName[i]), 
				(LPCWSTR)(sDestDir + asFileName[i]), false) == false)
		{
			//v3.0.T4 delete
			//::MessageBox(NULL, _T("Copy failed!"), (LPCWSTR)asFileName[i],  MB_OK);
			//v3.0.T4 add
			sMsg.Format(_T("BackupMcdata - copy %s failed"), asFileName[i]);
			theLog.WriteLine(sMsg, true);
		}
	}
}

//v2.0.T121 add
bool CWireBondApp::RestoreMcdata()
{
	bool bStatus = true;

	CString sDestDir = _T(DIR_MCDATA);
	CString sSrcDir = _T(DIR_MCDATA_BACKUP);
	CString sSrchFileName = sSrcDir;
	sSrchFileName.Append(_T("*.dat"));

	CString sMsg;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(sSrchFileName, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE ) 
	{
		//v3.0.T6 delete
		//MessageBox(NULL, _T("No *.dat file found "), NULL, MB_OK);
		//v3.0.T6 add
		sMsg.Format(_T("RestoreMcdata - no .dat file"));
		theLog.WriteLine(sMsg, true);

		bStatus = false; goto quit;
	}

	do
	{
		if (CopyFile(
			(LPCWSTR)(sSrcDir + FindFileData.cFileName), 
			(LPCWSTR)(sDestDir + FindFileData.cFileName), false) == false)
		{	//copy all .dat files from backup folder to mcdata folder
			//v3.0.T6 delete
			//sSrchFileName = (LPCWSTR)FindFileData.cFileName;
			//sSrchFileName.Append(_T(" Copy failed!"));
			//MessageBox(NULL, sSrchFileName, NULL,  MB_OK);
			//v3.0.T6 add
			sMsg.Format(_T("RestoreMcdata - copy %s failed"), (LPCWSTR)FindFileData.cFileName);
			theLog.WriteLine(sMsg, true);

			bStatus = false; goto quit;
		}
	} while (FindNextFile(hFind, &FindFileData));

quit:
	FindClose(hFind);
	return bStatus;
}

//The function copy machine data and Vision data to backup folder
void CWireBondApp::BackupWbdata()
{
	CString asFileName[5] = {_T("McData.dat"),_T("CtrlData.dat"),_T("WhData.dat"),_T("PowerOn.dat"),_T("SpeedParam.dat")};
	CString sSrcDir = _T(DIR_WBDATA);
	CString sDestDir = _T(DIR_BACKUP);

	CString sMsg;
	for (short i=0; i<5; i++)
	{	//copy 5 important WBdata files to backup folder
		if (CopyFile((LPCWSTR)(sSrcDir + asFileName[i]), 
				(LPCWSTR)(sDestDir + asFileName[i]), false) == false)
		{
			//v3.0.T4 delete
			//::MessageBox(NULL, _T("Copy failed!"), (LPCWSTR)asFileName[i],  MB_OK);
			//v3.0.T4 add
			sMsg.Format(_T("BackupWbdata - copy %s failed"), asFileName[i]);
			theLog.WriteLine(sMsg, true);
		} 
	}	

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	sSrcDir = _T(DIR_WBDATA_VISION);
	sDestDir = _T(DIR_BACKUP_VISION);
	CString sSrchFileName = sSrcDir;
	sSrchFileName.Append(_T("*.*"));
	hFind = FindFirstFile(sSrchFileName, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{	//copy files under Vision folder to backup folder
			CopyFile(
				(LPCWSTR)(sSrcDir + FindFileData.cFileName), 
				(LPCWSTR)(sDestDir + FindFileData.cFileName), false) ;
		} while(FindNextFile(hFind, &FindFileData));
	}

	FindClose(hFind);
}

//restore WBdata files when abnormal exit happens
bool CWireBondApp::RestoreWbdata()
{
	bool bStatus = true;

	CString sDestDir = _T(DIR_WBDATA);
	CString sSrcDir = _T(DIR_BACKUP);
	CString sSrchFileName = sSrcDir;
	sSrchFileName.Append(_T("*.dat"));

	CString sMsg;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(sSrchFileName, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		//v3.0.T6 delete
		//MessageBox(NULL, _T("No *.dat file found "), NULL, MB_OK);
		//v3.0.T6 add
		sMsg.Format(_T("RestoreWbdata - no .dat file"));
		theLog.WriteLine(sMsg, true);

		bStatus = false; goto quit;
	}

	do
	{
		if (CopyFile(
			(LPCWSTR)(sSrcDir + FindFileData.cFileName), 
			(LPCWSTR)(sDestDir + FindFileData.cFileName), false) == false)
		{	//copy 5 data files from backup folder to WBdata folder
			//v3.0.T6 delete
			//sSrchFileName = (LPCWSTR)FindFileData.cFileName;
			//sSrchFileName.Append(_T(" Copy failed!"));
			//MessageBox(NULL, sSrchFileName, NULL,  MB_OK);
			//v3.0.T6 add
			sMsg.Format(_T("RestoreWbdata - copy %s failed"), (LPCWSTR)FindFileData.cFileName);
			theLog.WriteLine(sMsg, true);

			bStatus = false; goto quit;
		}
	} while (FindNextFile(hFind, &FindFileData));
   
	sSrcDir = _T(DIR_BACKUP_VISION);
	sDestDir = _T(DIR_WBDATA_VISION);
	sSrchFileName = sSrcDir;
	sSrchFileName.Append(_T("*.*"));
	hFind = FindFirstFile(sSrchFileName, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		//v3.0.T6 delete
		//MessageBox(NULL, _T("No Vision file found "), NULL, MB_OK);
		//v3.0.T6 add
		sMsg.Format(_T("RestoreWbdata - no vision file"));
		theLog.WriteLine(sMsg, true);

		bStatus = false; goto quit;
	}
		
	do
	{
		if (CopyFile(
			(LPCWSTR)(sSrcDir + FindFileData.cFileName),
			(LPCWSTR)(sDestDir + FindFileData.cFileName), false) == false)
		{
			//v3.0.T6 add
			sMsg.Format(_T("RestoreWbdata - copy %s failed"), (LPCWSTR)FindFileData.cFileName);
			theLog.WriteLine(sMsg, true);
			bStatus = false; goto quit;
		}
	} while (FindNextFile(hFind, &FindFileData));

quit:
	FindClose(hFind);
	return bStatus;
}

// CWireBondApp initialization
BOOL CWireBondApp::InitInstance()
{
    if (gSingleInstanceObj.IsAnotherInstanceRunning()) return FALSE;
	
    // InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();

	AfxEnableControlContainer();

	theMsg.Init();		//V1.2.W12 add

	// manu bk color
	m_bkColor = RGB(255, 255, 230);

    m_brNull.CreateStockObject(NULL_BRUSH);					//V1.1.W133 add
    m_DlgBkBrush.CreateSolidBrush(m_bkColor);
    m_DlgBkBrushA.CreateSolidBrush(RGB(0, 192, 20));

    m_ChineseFont.CreateFontW(22, 0, 0, 0, 500, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    m_EnglishFont.CreateFontW(22, 0, 0, 0, 500, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	//VerifyRegister - Software Key checking
	/*
	if (VerifyRegister() != 0)
	{
		MessageBox(NULL, _T("Wrong software key . Please send the Machie ID in next message box to request WbSoftware.Key from vendor for registion of the software" ),_T("Information"),MB_OK);
		int len = 10;
		char* szMachineCode = new char[len + 1];
		sprintf(szMachineCode, "%s", "");
		GetMachineCode(szMachineCode, len);
		wchar_t wcMachineCode[20] = {0};
		MultiByteToWideChar(0, 0, szMachineCode, len, wcMachineCode, len);
		delete[] szMachineCode; 
		MessageBox(NULL, wcMachineCode, _T("Machie ID"), MB_OK);
		//return FALSE;
	}
	else
	{
		//MessageBox(NULL,_T("correct software key installed"),_T("ok"),MB_OK);
		DWORD dwYear,dwMonth, dwDays, dwLeaveDays;
		GetActiveDays(dwYear, dwMonth, dwDays, dwLeaveDays);
		WCHAR wzLeaveDays[50]; 
		//swprintf_s(wzLeaveDays, 50, L"software license remaining %d days.", dwLeaveDays);
		//MessageBox(NULL, wzLeaveDays, L"Information", MB_OK);  
		if (dwLeaveDays <= 0)
		{
			swprintf_s(wzLeaveDays, 50, L"Software license expired. Please send the Machie ID to request WbSoftware.Key from vendor for registion of the software");
			MessageBox(NULL, wzLeaveDays, L"Information", MB_OK); 
			//return FALSE;
		}
	}
	*/

    CInitDlg* pInitDlg = new CInitDlg();
    pInitDlg->Create(CInitDlg::IDD);

    int iStatus = FOK;
    theDebug.ClearInfo();	//v3.0.T96 edit

	CreateDirectoryA(DIR_WBDATA, NULL);
	CreateDirectoryA(DIR_VISION, NULL);
	CreateDirectoryA(DIR_VISION_IMAGE, NULL);	//v3.0.T93 add
	CreateDirectoryA(DIR_RECIPE, NULL);

	//v3.0.T7 add
	CreateDirectoryA(DIR_BACKUP, NULL);
	CreateDirectoryA(DIR_BACKUP_VISION, NULL);

	//v2.0.T113 add
	CreateDirectoryA(DIR_MCDATA, NULL);
	CreateDirectoryA(DIR_MCDATA_BACKUP, NULL);

	//v3.0.T171 add
	CreateDirectoryA(DIR_DATALOG, NULL);
	CreateDirectoryA(DIR_DATALOG_VISION, NULL);
	CreateDirectoryA(DIR_DATALOG_IMAGE, NULL);

	//for testing variable size
	//int nSize = sizeof(char);

	//for testing random function
	//srand((int)time(0));
	//int i, j;
	//for (i=0; i<10; i++)
	//{
	//	j = rand();	//0..32767 (RAND_MAX)
	//	j = 0;
	//}

	//load config
	theMcConfig.LoadConfig();
	theMcConfig.LoadAccessControl();		//v3.0.T320 add

	/* v3.1.T527 delete
	if (theMcConfig.GetWireSpoolType())		//v3.0.L302 add
	{
		short nDist = theMcConfig.GetWireSpoolDist();
		short nSpeed = theMcConfig.GetWireSpoolSpeed();
		theStepperIO.SetWireSpoolPara(CANADDR_OFFLOAD, 5, MOVEREL, nDist);
		theStepperIO.SetWireSpoolPara(CANADDR_OFFLOAD, 5, SETVEL, nSpeed);
	}
	*/

    pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_LOAD_MACHINE_CONFIG));
	Sleep_x1ms(1000);

	//v3.1.T192 edit
	if (theMcConfig.IsBypassAll())
	{
		theDebug.SetDebugMode(DEBUG_LOG_TRACE);
		theUser.SetUserLevel(USER_ENGINEER);
	}
	else
		theUser.SetUserLevel(USER_OPERATOR);

	//init device parameter
    pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_DEVICE_PARA));
	Sleep_x1ms(1000);
	theDevice.Init();

	//check software exit normally or not
    pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_CHECK_ABNORMAL_EXIT));
	Sleep_x1ms(1000);
	if (theMcConfig.IsCheckAbnormalShutdown()	//v3.1.T251 add
		&& IsAbnormalExit())	//v3.1.T246 add
	{ 
		MessageBox(NULL, _T("Abnormal exit last time!\nNeed copy backup WBdata......"), NULL,  MB_OK);

		//v3.0.T6 edit
		theLog.WriteLine(_T("Restore WBDATA"), true);
		RestoreWbdata();
		if (theMcConfig.IsMcdata() && theMcConfig.IsRestoreMcdata())
			RestoreMcdata();

		//reset Servo and Stepper Motor
		if (!theMcConfig.IsBypassServo())	//v3.1.T132 edit
		{
			theServo.Init();	
			theServo.ClearAllBuffer();		
			theServo.DisableServoController();
		}

		if (!theMcConfig.IsBypassMHS())		//v3.1.T172 add
		{
			theStepper.Init();
			theStepper.DisableStepper();
		}
	}
	RecordCurrentTime(FILE_COLDSTART);

	pInitDlg->SetDlgItemText(IDC_STATIC_VERSION, theApp.GetSoftwareVersion());
	pInitDlg->RedrawWindow();

	if (!theMcConfig.IsBypassMHS())		//v3.1.T172 add
    {
		//init stepper
		pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_STEPPER));
		Sleep_x1ms(1000);
		if (theStepper.Init() != FOK)
			theDebug.AddInfo(_T("Init stepper fail!"));
    }

	//init servo
    pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_SERVO));
	Sleep_x1ms(1000);
	theServo.Init();

	//init machine parameter
    pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_MACHINE_PARA));
	Sleep_x1ms(1000);
	theApp.ArchiveMachineData(ARCHIVE_LOAD);

	if (!theMcConfig.IsBypassPrLight())	//v3.0.T392 add
    {
		//init CAN Channel 1
	    pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_CAN_CHANNEL_1));
		Sleep_x1ms(1000);
		iStatus = theCanController.Init(1);	//v1.5.T50 edit
		if (iStatus != FOK)
			theDebug.AddInfo(_T("Init Lighting Card fail!"));
    }

	if (!theMcConfig.IsBypassUSG())	//v3.0.T392 add
	{
		//init usg
	    pInitDlg->ShowInitMsg(theMsg.GetMsg(MSG_INIT_USG));
		Sleep_x1ms(1000);
		theSerialCommIO.InitUSG();
		Sleep_x1ms(20);
		theUSG.Init();

		if (theMcConfig.GetUsgType() == 1) theFnk.Init();	//v3.0.T344 add
	}

	//v3.1.T172 delete
	//if ((theMcConfig.GetMhsType() != MHS_NONE)	//v3.0.T158 add
	//	&& !theMcConfig.IsBypassAll())			//v3.0.T392 add
	if (!theMcConfig.IsBypassMHS())			//v3.1.T172 add
    {
		Sleep_x1ms(500);
		theStepperIO.ResetStepCard();

		if (theStepperIO.UpdateInputState(CANADDR_ONLOAD) == CAN_SEND_ERROR)
		{
			pInitDlg->DestroyWindow();
			return FALSE;
		}
		else if (theStepperIO.UpdateInputState(CANADDR_OFFLOAD) == CAN_SEND_ERROR)
		{
			pInitDlg->DestroyWindow();
			return FALSE;
		}
		else if (theStepperIO.UpdateInputState(CANADDR_INDEX) == CAN_SEND_ERROR)
		{
			pInitDlg->DestroyWindow();
			return FALSE;
		}

		Sleep_x1ms(1000);
		theStepper.PowerOn();
		Sleep_x1ms(1000);

		theStepperIO.EnableStepper(CANADDR_ONLOAD, true);
		theStepperIO.EnableStepper(CANADDR_OFFLOAD, true);
		theStepperIO.EnableStepper(CANADDR_INDEX, true);

		theMHS.Init();

		//v3.1.T527 add
		if (theMcConfig.GetWireSpoolType())
		{
			short nDist = theMcConfig.GetWireSpoolDist();
			short nSpeed = theMcConfig.GetWireSpoolSpeed();
			theStepperIO.SetWireSpoolPara(CANADDR_OFFLOAD, 5, MOVEREL, nDist);
			theStepperIO.SetWireSpoolPara(CANADDR_OFFLOAD, 5, SETVEL, nSpeed);
		}
    }
  
    theScanKeyThread.Init();
	Sleep_x1ms(10);

    pInitDlg->DestroyWindow();

	theJpegHelp.Load(m_hInstance,IDR_JPEG_HELP);	//V1.1.W147 add

    CWireBondDlg dlg;
	m_pMainWnd = &dlg;
    dlg.DoModal();

	return FALSE;
}

CFont* CWireBondApp::GetFont()
{
	if (theMcConfig.GetLanguage() == 1)	//v1.6.T130 add
		return &m_ChineseFont;
	else
		return &m_EnglishFont;
}

bool CWireBondApp::_CopyFile(const TCHAR *_pFrom,const TCHAR *_pTo)
{
	TCHAR pTo[MAX_PATH]={0};
	_tcscpy_s(pTo,_pTo);
	
	TCHAR pFrom[MAX_PATH]={0};
	_tcscpy_s(pFrom,_pFrom);

	SHFILEOPSTRUCT FileOp = {0}; 
	FileOp.fFlags = FOF_NOCONFIRMATION;
	FileOp.pFrom  = pFrom; 
	FileOp.pTo    = pTo; 
	FileOp.wFunc  = FO_COPY; 
	return (SHFileOperation(&FileOp) == 0);     
}

BOOL CWireBondApp::PreTranslateMessage(MSG* pMsg)
{
    if ((pMsg->message == WM_KEYDOWN) || (pMsg->message == WM_KEYUP))
    {
        if ((theMcConfig.GetKeyPadType() >= 1)
			&& (theMcConfig.GetKeyPadType() <= 3))	//v2.0.T35 edit
        {
            if (theKeyPad.HotKeyHandler(pMsg) == TRUE) return TRUE;
        }
    }

    return CWinApp::PreTranslateMessage(pMsg);
}

void CWireBondApp::ArchiveProgram(CString sPgmName, ARCHIVE_MODE nArchiveMode, bool bLoadWH)	//V1.1.W172 add
{
    if (nArchiveMode == ARCHIVE_SAVE)
        SaveRecipeToFile(sPgmName, false);
    else if (nArchiveMode == ARCHIVE_LOAD)
    {
		LoadRecipeFromFile(sPgmName, false, bLoadWH);

		if (!theMcConfig.IsBypassCamera())
			theVisionSys.LoadPatternRcd();	//v3.0.T70 edit
    }
    else if (nArchiveMode == ARCHIVE_CLEAR)
	{
        thePgm.ClearProgram();  
	}
    
    ArchivePowerOnData(ARCHIVE_SAVE); 
}

int CWireBondApp::SaveRecipeToFile(CString sFilePath, bool bPowerOn)  
{
    if (theDataMgr.AllocBufferMemory(RECIPE_BUF_SIZE) != 0)
        return DATA_MGR_MALLOC_MEM_FAIL; 
	thePgm.SaveRcd();
    theRef.SaveRcd();	//v2.0.T67 rename
    theWire.SaveRcd();
    theAutoPadMgr.SaveRcd(); 
	theEfoProfile.SaveProfile();
    theBondPara.SaveRcd();
    theBallPara.SaveRcd();
    theLoopPara.SaveRcd();
    theServo.SaveMotionParameter();
    theServo.SaveWireClampProfile();
	theForceSwitch.SaveRcd();   //v1.6.T126 add
    
	switch (theMcConfig.GetMhsType())	//v3.0.T153 edit
	{
	case MHS_TOP_LED: case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
		theDevice.SaveTopRcd();
		break;
	case MHS_CU_ELITE:
		theDevice.SaveCuEliteRcd();
		break;
	}
   
    if (bPowerOn)
    {
        theBond.SaveBondData();
        theStat.SaveRcd();
    }
	else 
	{
		if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
			|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
			|| (theMcConfig.GetMhsType() == MHS_R2R)
			|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
	    {
		    theMHS.SaveMhsRcd();
		}
	} 

    theDataMgr.SaveDataToFile(sFilePath);
    return 0;
}

int CWireBondApp::LoadRecipeFromFile(CString sFilePath, bool bPowerOn, bool bLoadWH) 
{
    int lFileLength;
    int ret = theDataMgr.LoadDataFromFile(sFilePath, &lFileLength);
    if (ret != 0) return ret; 
    thePgm.ClearProgram();
    theVisionSys.ClearAllPatternRcd(); 

	thePgm.LoadRcd();
	theRef.LoadRcd();	//v2.0.T67 rename
	theWire.LoadRcd();
	theAutoPadMgr.LoadRcd();

	theEfoProfile.LoadProfile();
	theBondPara.LoadRcd();
	theBallPara.LoadRcd();
	theLoopPara.LoadRcd();
	theServo.LoadMotionParameter();
	theServo.LoadWireClampProfile();
	theForceSwitch.LoadRcd();   //v1.6.T126 add
	thePgm.SetVersion();

	switch (theMcConfig.GetMhsType())	//v3.0.T153 edit
	{
	case MHS_TOP_LED: case MHS_R2R:
	case MHS_TOP_ELITE:	//v3.0.T410 add
		theDevice.LoadTopRcd();
		break;
	case MHS_CU_ELITE:
		theDevice.LoadCuEliteRcd();
		break;
	}

    if (bPowerOn)
    {
        theBond.LoadBondData();
        theStat.LoadRcd();
    }
	else if (bLoadWH)
	{
		if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
			|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
			|| (theMcConfig.GetMhsType() == MHS_R2R)
			|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
	    {
	        theMHS.LoadMhsRcd(); 

			if ((theMcConfig.GetMhsType() == MHS_TOP_LED)
				|| (theMcConfig.GetMhsType() == MHS_CU_ELITE)
				|| (theMcConfig.GetMhsType() == MHS_TOP_ELITE))	//v3.0.T410 add
			{
				theServo.MoveBondHeadToSafePos();
				Sleep(1000);
				theMHS.WarmReset();
				((CWireBondDlg*)m_pMainWnd)->ShowUsgInfoDialog();	//v3.0.T179 rename
			}
		}
	}

    theDataMgr.FreeBufferMemory();
    return 0;
}

//v3.0.T13 edit
void CWireBondApp::ArchivePowerOnData(ARCHIVE_MODE nArchiveMode)  
{
	CString sDir;
	if (theMcConfig.IsMcdata())
		sDir = _T(DIR_MCDATA);
	else
		sDir = _T(DIR_WBDATA);

	CString sFilePath;
	if (nArchiveMode == ARCHIVE_SAVE)
	{
		sFilePath.Format(_T("%s%s"), sDir, _T("PowerOn.dat"));
		SaveRecipeToFile(sFilePath, true);
		Sleep(10);
		sFilePath.Format(_T("%s%s"), sDir, _T("WhData.dat"));
		theMHS.SaveWorkHolderData(sFilePath);
	}
	else if (nArchiveMode == ARCHIVE_LOAD)
	{
		sFilePath.Format(_T("%s%s"), sDir, _T("PowerOn.dat"));
		if (LoadRecipeFromFile(sFilePath, true, true) != 0)
		{
			if (theMcConfig.IsMcdata())
			{
				InitPowerOnData();
				BackupMcdata();
				if (LoadRecipeFromFile(sFilePath, true, true) != 0)
					::AfxMessageBox(_T("Load PowerOnData failed"));
			}
			else
				::AfxMessageBox(_T("Load PowerOnData failed"));
		};
		Sleep(10);
		sFilePath.Format(_T("%s%s"), sDir, _T("WhData.dat"));
		theMHS.LoadWorkHolderData(sFilePath);
	}
}

int CWireBondApp::SaveMcDataToFile(CString sFilePath) 
{
    if (theDataMgr.AllocBufferMemory(DATA_BUF_SIZE) != 0)
        return DATA_MGR_MALLOC_MEM_FAIL; 

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
	theVisionSys.GetVisionFactor(&(pstMcData->dHighMagFactX),
									&(pstMcData->dHighMagFactY), 
									&(pstMcData->dLowMagFactX), 
									&(pstMcData->dLowMagFactY), 
									&(pstMcData->dVisionFactCos), 
									&(pstMcData->dVisionFactSin));

	theMcData.SaveMachineData();
    theDataMgr.SaveDataToFile(sFilePath);
    return 0;
}

int CWireBondApp::LoadMcDataFromFile(CString sFilePath) 
{
    int lFileLength;
    int ret = theDataMgr.LoadDataFromFile(sFilePath, &lFileLength);
    if (ret != 0) return ret;
    
    theMcData.LoadMachineData();

	MACHINE_DATA* pstMcData = theMcData.GetRcd();
    theVisionSys.UpdateVisionFactor(VISION_HIGH_MAG,
			pstMcData->dHighMagFactX, pstMcData->dHighMagFactY,
			pstMcData->dVisionFactCos, pstMcData->dVisionFactSin);
    theVisionSys.UpdateVisionFactor(VISION_LOW_MAG,
			pstMcData->dLowMagFactX, pstMcData->dLowMagFactY,
			pstMcData->dVisionFactCos, pstMcData->dVisionFactSin);

    theDataMgr.FreeBufferMemory();

    return 0;
}

int CWireBondApp::SaveControlData(CString sFilePath)   
{
    if (theDataMgr.AllocBufferMemory(DATA_BUF_SIZE) != 0)
        return DATA_MGR_MALLOC_MEM_FAIL; 

    theServo.SaveControlParameter();
    theDataMgr.SaveDataToFile(sFilePath);

   	return 0;
}

int CWireBondApp::LoadControlData(CString sFilePath)  
{
    int lFileLength; 
    int ret = theDataMgr.LoadDataFromFile(sFilePath, &lFileLength);
    if (ret != 0) return ret;
    
    theServo.LoadControlParameter(lFileLength); 
    theDataMgr.FreeBufferMemory();

	return 0;
}

void CWireBondApp::ArchiveMachineData(ARCHIVE_MODE nArchiveMode)   
{
	CString sDir;
	if (theMcConfig.IsMcdata())
		sDir = _T(DIR_MCDATA);
	else
		sDir = _T(DIR_WBDATA);

    CString sFilePath;
    if (nArchiveMode == ARCHIVE_SAVE) 
	{
	    sFilePath.Format(_T("%s%s"), sDir, _T("McData.dat"));
        SaveMcDataToFile(sFilePath);
		Sleep(10);
	    sFilePath.Format(_T("%s%s"), sDir, _T("CtrlData.dat"));
		SaveControlData(sFilePath);
	}
    else if (nArchiveMode == ARCHIVE_LOAD)
	{
	    sFilePath.Format(_T("%s%s"), sDir, _T("McData.dat"));
        if (LoadMcDataFromFile(sFilePath) != 0)
		{
			if (theMcConfig.IsMcdata())
			{
				InitMcdata();
				BackupWbdata();		//v3.0.T7 add
				BackupMcdata();		//v3.0.T7 add
				if (LoadMcDataFromFile(sFilePath) != 0)
					::AfxMessageBox(_T("Load McData failed"));
			}
			else
				::AfxMessageBox(_T("Load McData failed"));
		};
		Sleep(10);
	    sFilePath.Format(_T("%s%s"), sDir, _T("CtrlData.dat"));
		if (LoadControlData(sFilePath) != 0)
			::AfxMessageBox(_T("Load CtrlData failed"));
	}
}

CString CWireBondApp::GetSoftwareVersion()
{
	CString sVersion;

	switch (theMcConfig.GetMhsType())
	{
	case MHS_VLED:			sVersion.Append(_T("VLED")); break;
	case MHS_VLED_ELITE:	sVersion.Append(_T("V-ELITE")); break;
	case MHS_TOP_LED:		sVersion.Append(_T("TOP")); break;
	case MHS_TOP_ELITE:		sVersion.Append(_T("TOP-ELITE")); break;	//v3.0.T410 add
	case MHS_CU_ELITE:		sVersion.Append(_T("ELITE")); break;
	case MHS_FOXCONN:		sVersion.Append(_T("FOXCONN")); break;
	case MHS_R2R:			sVersion.Append(_T("R2R")); break;
	default:				sVersion.Format(_T("?")); break;
	}
	sVersion.AppendFormat(_T(" %s"), wcSoftwareVersion);
	//v1.6.T318 add
	if (theMcConfig.GetAcsBufferType() > 0)
		sVersion.AppendFormat(_T("_Buf%d"), theMcConfig.GetAcsBufferType());

	return sVersion;
}

void CWireBondApp::SetRedLamp(bool bOn)
{
	theTowerLight.RedLight(bOn, theDebug.GetDebugMode());
}

void CWireBondApp::SetDlgFont(CWnd* lpParent,CFont* lpFont)	//V1.1.W139 add
{
	if(lpParent==NULL)return;
	if(lpParent->m_hWnd==NULL)return;

	if (lpFont == NULL) lpFont = GetFont();	//v1.6.T130 edit
	CWnd* lpChild=lpParent->GetWindow(GW_CHILD);

	while(lpChild)
	{
		lpChild->SetFont(lpFont);
		lpChild = lpChild->GetWindow(GW_HWNDNEXT);
	}
}
