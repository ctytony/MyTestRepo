// StepCardControl.cpp : 实现文件
//

#include "stdafx.h"
//#include "Materialhandler.h"
#include "StepCardControl.h"

CFastLock::CFastLock()
{
	FastLockInit(&m_Lock);
}

CFastLock::~CFastLock()
{
	FastLockClose(&m_Lock);
}

void CFastLock::Acquire()
{
	FastLockAcquire(&m_Lock);
}

void CFastLock::Release()
{
	FastLockRelease(&m_Lock);
}

CFastLock CStepCardControl::m_sFastLock;

bool PeekAndPump()
{
	MSG	msg;
	while(::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
	{
		if (!GetMessage(&msg,NULL,0,0))
			return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return true;
}
// CInputList
CInputList::CInputList()
{
	m_OnLoaderMagClamperHome.m_port=0;
	m_OnLoaderMagClamperHome.m_signal=NCLOSE;
	m_OnLoaderMagHolderHome.m_port=1;
	m_OnLoaderMagHolderHome.m_signal=NCLOSE;
	m_OnLoaderMagHolderLimit.m_port=2;
	m_OnLoaderMagHolderLimit.m_signal=NCLOSE;
	m_OnLoaderMagPullyHome.m_port=3;
	m_OnLoaderMagPullyHome.m_signal=NOPEN;
	m_OnLoaderMagPullyLimit.m_port=4;
	m_OnLoaderMagPullyLimit.m_signal=NOPEN;

	m_OnLoaderMagBufferPresentSensor.m_port=5;
	m_OnLoaderMagBufferPresentSensor.m_signal=NOPEN;
	m_OnLoaderMagDetectedSensor.m_port=6;
	m_OnLoaderMagDetectedSensor.m_signal=NOPEN;
	m_OnLoaderMagDropSensor.m_port=7;
	m_OnLoaderMagDropSensor.m_signal=NOPEN;
	m_OnLoaderPullyMagExistSensor.m_port=8;
	m_OnLoaderPullyMagExistSensor.m_signal=NOPEN;
	m_InjectSensor.m_port=9;
	m_InjectSensor.m_signal=NOPEN;
	m_InjectPusherHome.m_port=15;
	m_InjectPusherHome.m_signal=NCLOSE;
	m_InjectPusherLimit.m_port=14;
	m_InjectPusherLimit.m_signal=NCLOSE;
	


	m_OffLoaderMagClamperHome.m_port=16;
	m_OffLoaderMagClamperHome.m_signal=NOPEN;
	m_OffLoaderMagHolderHome.m_port=17;
	m_OffLoaderMagHolderHome.m_signal=NOPEN;
	m_OffLoaderMagHolderLimit.m_port=18;
	m_OffLoaderMagHolderLimit.m_signal=NOPEN;
	m_OffLoaderMagPullyHome.m_port=19;
	m_OffLoaderMagPullyHome.m_signal=NOPEN;
	m_OffLoaderMagPullyLimit.m_port=20;
	m_OffLoaderMagPullyLimit.m_signal=NOPEN;
	m_OffLoaderMagBufferPresentSensor.m_port=21;
	m_OffLoaderMagBufferPresentSensor.m_signal=NOPEN;
	m_OffLoaderMagDetectedSensor.m_port=22;
	m_OffLoaderMagDetectedSensor.m_signal=NOPEN;
	m_OffLoaderMagDropSensor.m_port=23;
	m_OffLoaderMagDropSensor.m_signal=NOPEN;
	m_OffLoaderPullyMagExistSensor.m_port=24;
	m_OffLoaderPullyMagExistSensor.m_signal=NOPEN;
	m_EjectSensor.m_port=25;
	m_EjectSensor.m_signal=NOPEN;
	m_VaccumSnr.m_port=26;
	m_VaccumSnr.m_signal=NOPEN;


	m_IndexerHome.m_port=32;
	m_IndexerHome.m_signal=NOPEN;

	m_IndexerLimit.m_port=33;
	m_IndexerLimit.m_signal=NOPEN;

	m_WindowClampHome.m_port=34;
	m_WindowClampHome.m_signal=NOPEN;

	m_AlignSensor.m_port=35;
	m_AlignSensor.m_signal=NOPEN;

	m_EjectJamSensor.m_port=36;
	m_EjectJamSensor.m_signal=NOPEN;

	m_JoyStickPos_X.m_port=43;
	m_JoyStickPos_X.m_signal=NOPEN;

	m_JoyStickNeg_X.m_port=42;
	m_JoyStickNeg_X.m_signal=NOPEN;

	m_JoyStickPos_Y.m_port=40;
	m_JoyStickPos_Y.m_signal=NOPEN;

	m_JoyStickNeg_Y.m_port=41;
	m_JoyStickNeg_Y.m_signal=NOPEN;

	m_ThermalSensorX.m_port=48;
	m_ThermalSensorX.m_signal=NOPEN;

	m_ThermalSensorY.m_port=49;
	m_ThermalSensorY.m_signal=NOPEN;

	m_HeaterDetector.m_port=50;
	m_HeaterDetector.m_signal=NOPEN;

}



//COutputList
COutputList::COutputList()
{
	m_InjectPusherDir.m_port=0;
	m_InjectPusherDir.m_signal=NOPEN;
	m_InjectPusherPower.m_port=1;
	m_InjectPusherPower.m_signal=NOPEN;
	m_LFHoler.m_port=20;
	m_LFHoler.m_signal=NOPEN;
	m_InjectClaw.m_port=16;
	m_InjectClaw.m_signal=NOPEN;
	m_InputClaw.m_port=17;
	m_InputClaw.m_signal=NOPEN;
	m_OutputClaw.m_port=18;
	m_OutputClaw.m_signal=NOPEN;
	m_EjectClaw.m_port=19;
	m_EjectClaw.m_signal=NOPEN;
	m_Vacuum.m_port=22;
	m_Vacuum.m_signal=NOPEN;


	m_GreenLight.m_port=25;
	m_GreenLight.m_signal=NOPEN;
	m_YellowLight.m_port=26;
	m_YellowLight.m_signal=NOPEN;
	m_RedLight.m_port=27;
	m_RedLight.m_signal=NOPEN;
	m_StepCardPowerCtrl.m_port=28;
	m_StepCardPowerCtrl.m_signal=NOPEN;
	m_PanelLight.m_port=29;
	m_RedLight.m_signal=NOPEN;
	m_N2h2Port.m_port=30;
	m_N2h2Port.m_signal=NOPEN;
	m_AirTensioner.m_port=31;
	m_AirTensioner.m_signal=NOPEN;
	
}

// CStepCardControl

CStepCardControl::CStepCardControl()
{
	//m_pWSerialPort=new CWSerialPort();

	m_Addr=0;
	m_AxisID=0;
	m_iSendAndRecCount=0;
	m_InjectPushStep=0;

	InitInputPort();
	InitOutputPort();

	m_bSendAndRecFinish=true;
	InitStepParameter();
	for (int i = 0; i < AXIS_NUM; i++)
	{
		m_bIsAxisMoving[i-1]=false;
	}
	m_MagezinePullStep[0]=0;
	m_MagezinePullStep[1]=0;

}

CStepCardControl::~CStepCardControl()
{
	//if (m_pWSerialPort)
	//{
	//	delete m_pWSerialPort;
	//	m_pWSerialPort=NULL;
	//}

#ifdef _KANDE_CAN
	VCI_CloseDevice(DEV_PCICAN200,0);
#else
	VCI_CloseDevice(VCI_PCI9820I,0);
#endif
}

CStepCardControl m_StepCardControl;

void CStepCardControl::SetSerialAddress(unsigned short add)
{
	m_Addr=add;
}

HANDLE CStepCardControl::OpenDevice(DWORD DeviceType,DWORD instance,DWORD Error)
{
	//DWORD Error;
	HANDLE hDevice=(HANDLE)VCI_OpenDevice(DeviceType,instance,Error);

	if (hDevice == INVALID_HANDLE_VALUE)
		Error = GetLastError();

	return hDevice;

}

BOOL CStepCardControl::InitializeCan()
{
#ifdef _KANDE_CAN
	DWORD Error=0;
	m_hDevice=OpenDevice(DEV_PCICAN200,0,Error);
	if (m_hDevice<=0)
	{
		AfxMessageBox(_T("Open Fail!"));
		//return FALSE;
	}

	VCI_INIT_CONFIG Config;
	Config.AccCode=0;
	Config.AccMask=0xffffffff;
	Config.Mode=0x00;
	Config.Filter=0x00;
	Config.kCanBaud=15;
	Config.Timing0=0x00;
	Config.Timing1=0x14;
	//Config.CanRx_IER=1;
	//初始化通道1
	if(!VCI_InitCAN(DEV_PCICAN200,0,0,&Config))
		return FALSE;
#else

	DWORD Error=0;
	m_hDevice=OpenDevice(VCI_PCI9820I,0,Error);
	if (m_hDevice<=0)
	{
		AfxMessageBox(_T("Open Fail!"));
		//return FALSE;
	}

	VCI_INIT_CONFIG Config;
	Config.AccCode=0x00;
	Config.AccMask=0xffffffff;
	Config.Mode=0x00;
	Config.Filter=0x00;
	Config.Timing0=0x00;
	Config.Timing1=0x14;
	//初始化通道1
	if(!VCI_InitCAN(VCI_PCI9820I,0,0,&Config))
	{
		AfxMessageBox(_T("初始化CAN失败!"));
		return FALSE;
	}

	if(VCI_StartCAN(VCI_PCI9820I,0,0)!=1)
	{
		AfxMessageBox(_T("初始化CAN失败！"));	
	}

#endif



	return TRUE;
}

int CStepCardControl::SendCanData(char addr, char axisID, char command, long data)
{
	CScopedLock<CFastLock> slock(&CStepCardControl::m_sFastLock);
#ifdef _KANDE_CAN
	bool flag;
	VCI_CAN_OBJ Sendbuf;
	VCI_CAN_OBJ pRcvbuf[50];
	VCI_ERR_INFO errinfo;
	DWORD dLength;
	m_Addr=axisID/AXIS_OF_CARD;
	m_AxisID=axisID%AXIS_OF_CARD+1;
	Sendbuf.ExternFlag=0;
	Sendbuf.DataLen=8;
	Sendbuf.RemoteFlag=0;
	Sendbuf.SendType=0;
	Sendbuf.ID[0]=0x00;
	Sendbuf.ID[1]=m_Addr<<5;// 0x60 ID=3
	Sendbuf.ID[2]=0x00;
	Sendbuf.ID[3]=0x00;
	Sendbuf.Data[0]=command;
	Sendbuf.Data[1]=m_AxisID;
	Sendbuf.Data[2]=data%256;
	Sendbuf.Data[3]=(data>>8)%256;
	Sendbuf.Data[4]=(data>>16)%256;
	Sendbuf.Data[5]=(data>>24)%256;
	Sendbuf.Data[6]=0x00;
	Sendbuf.Data[7]=0x00;
	m_Timer_Can.StartTime();

	if (m_bSendAndRecFinish)
	{
		flag=VCI_Transmit(DEV_PCICAN200,0,0,&Sendbuf);
		if (flag)
			m_bSendAndRecFinish=FALSE;
		else
			AfxMessageBox(_T("发送失败!"));
	}
	else
		return CAN_ACTION_NORMAL;
	dLength = VCI_ReceiveEx(DEV_PCICAN200,0,0,pRcvbuf,1000);
	
	if(dLength<=0)
	{
		//注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
		//千万不能省略这一步（即使你可能不想知道错误码是什么）
		VCI_ReadErrInfo(DEV_PCICAN200,0,0,&errinfo);
		AfxMessageBox(_T("接收失败!"));
		m_bSendAndRecFinish=true;
		return SEND_ERROR;
	} 
	else
	{
		if (dLength>=2)
			AfxMessageBox(_T("2帧数据!"));
	}
	m_Time=m_Timer_Can.EndTime();

	Process_RecData(pRcvbuf[0]);

	if(m_bSendAndRecFinish == false)
	{
		AfxMessageBox(_T("校验失败！"));
		m_bSendAndRecFinish = true;
		return SEND_ERROR;
	}

#else
	bool flag;
	VCI_CAN_OBJ Sendbuf;
	VCI_CAN_OBJ pRcvbuf[50];
	VCI_ERR_INFO errinfo;
	DWORD dLength;
	m_Addr=axisID/AXIS_OF_CARD;
	m_AxisID=axisID%AXIS_OF_CARD+1;
	Sendbuf.ExternFlag=0;
	Sendbuf.DataLen=8;
	Sendbuf.RemoteFlag=0;
	Sendbuf.SendType=0;
	Sendbuf.ID=m_Addr;
	Sendbuf.Data[0]=command;
	Sendbuf.Data[1]=m_AxisID;
	Sendbuf.Data[2]=data%256;
	Sendbuf.Data[3]=(data>>8)%256;
	Sendbuf.Data[4]=(data>>16)%256;
	Sendbuf.Data[5]=(data>>24)%256;
	Sendbuf.Data[6]=0x00;
	Sendbuf.Data[7]=0x00;
	m_Timer_Can.StartTime();
	if (m_bSendAndRecFinish)
	{
		flag=VCI_Transmit(VCI_PCI9820I,0,0,&Sendbuf,1);
		if (flag)
			m_bSendAndRecFinish=FALSE;
		else
			AfxMessageBox(_T("发送失败!"));
	}
	else
		return CAN_ACTION_NORMAL;
	//m_Timer_Can.StartTime();
	dLength = VCI_Receive(VCI_PCI9820I,0,0,pRcvbuf,50,20);

	if(dLength<=0)
	{
		//注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
		//千万不能省略这一步（即使你可能不想知道错误码是什么）
		VCI_ReadErrInfo(VCI_PCI9820I,0,0,&errinfo);
		AfxMessageBox(_T("接收失败!"));
		m_bSendAndRecFinish=true;
		return SEND_ERROR;
	} 
	else
	{
		if (dLength>=2)
			AfxMessageBox(_T("2帧数据!"));
	}
	m_Time=m_Timer_Can.EndTime();
	Process_RecData(pRcvbuf[0]);
	if(m_bSendAndRecFinish == false)
	{
		AfxMessageBox(_T("校验失败！"));
		m_bSendAndRecFinish = true;
		return SEND_ERROR;
	}

#endif


	//VCI_ClearBuffer(VCI_PCI9820I,0,1);
	return CAN_ACTION_NORMAL;
}

DWORD CStepCardControl::VCI_ReceiveEx(DWORD DeviceType,DWORD DeviceInd,DWORD CANInd,PVCI_CAN_OBJ pReceive, int WaitTime )
{
#ifdef _KANDE_CAN
	DWORD dLength;
	CHTimer m_Timer_Rec;
	m_Timer_Rec.StartTime();
	while(1)
	{
		//WaitForSingleObject(m_hEvent,INFINITE);//一直等待
		//接收
		pReceive->ID[1]=0x00;
		dLength=VCI_Receive(DEV_PCICAN200,DeviceInd,CANInd,pReceive);
		if(dLength==-1||dLength==0)
		{
			if (m_Timer_Rec.EndTime()>WaitTime)
				return REC_TIMEOUT;
			else
				continue;
		}
		else if (dLength==2)
		{
			AfxMessageBox(_T("2帧"));
			break;
		}
		else
			break;
	}
#endif

	return CAN_ACTION_NORMAL;


}
// CStepCardControl 成员函数
bool CStepCardControl::InitStepCard()
{
	if(!InitializeCan())
		return true;
	else
	{
		SetAllStepSpeedParameter();
		return false;
	}
}

void CStepCardControl::InitStepParameter()
{
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].Pulses = 1600;    
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].ScrewPitch = 2000;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].m_Scale =  m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].ScrewPitch / m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].Pulses;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].Acc = 500000;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].Dcc = 500000;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].Vel = 30000;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].StopDcc = 2000000;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].FastHomeVel = 30000;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].SlowHomeVel = 30000;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].SlowHomeDistance = 6500;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].HomeOffset = 1000;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].LimitSignal = 0;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].HomeSignal = 1;
	m_StepParameter[TOP_ONLOAD_MAG_CLAMPPER_EX].HomeDirection = 0;

	m_StepParameter[TOP_ONLOAD_HOLDER_EX].Pulses = 1600;    
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].ScrewPitch = 6000;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].m_Scale = m_StepParameter[TOP_ONLOAD_HOLDER_EX].ScrewPitch / m_StepParameter[TOP_ONLOAD_HOLDER_EX].Pulses;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].Acc = 2000000;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].Dcc = 2000000;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].Vel = 120000;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].StopDcc = 20000000;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].FastHomeVel = 70000;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].SlowHomeVel = 60000;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].SlowHomeDistance = 5000;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].HomeOffset = 2000;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].LimitSignal = 1;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].HomeSignal = 0;
	m_StepParameter[TOP_ONLOAD_HOLDER_EX].HomeDirection = 0;

	m_StepParameter[TOP_ONLOAD_PULLY_EX].Pulses = 1600;    
	m_StepParameter[TOP_ONLOAD_PULLY_EX].ScrewPitch = 70000;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].m_Scale = m_StepParameter[TOP_ONLOAD_PULLY_EX].ScrewPitch / m_StepParameter[TOP_ONLOAD_PULLY_EX].Pulses;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].Acc = 3000000;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].Dcc = 3000000;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].Vel = 150000;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].StopDcc = 15000000;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].FastHomeVel = 150000;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].SlowHomeVel = 150000;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].SlowHomeDistance = 5000;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].HomeOffset = -500;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].LimitSignal = 1;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].HomeSignal = 0;
	m_StepParameter[TOP_ONLOAD_PULLY_EX].HomeDirection = 0;



	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].Pulses = 1600;    
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].ScrewPitch = 2000;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].m_Scale =  m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].ScrewPitch / m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].Pulses;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].Acc = 500000;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].Dcc = 500000;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].Vel = 30000;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].StopDcc = 2000000;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].FastHomeVel = 30000;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].SlowHomeVel = 30000;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].SlowHomeDistance = 6500;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].HomeOffset = 1000;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].LimitSignal = 0;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].HomeSignal = 1;
	m_StepParameter[TOP_OFFLOAD_MAG_CLAMPPER_EX].HomeDirection = 0;

	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].Pulses = 1600;    
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].ScrewPitch = 6000;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].m_Scale = m_StepParameter[TOP_OFFLOAD_HOLDER_EX].ScrewPitch / m_StepParameter[TOP_OFFLOAD_HOLDER_EX].Pulses;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].Acc = 2000000;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].Dcc = 2000000;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].Vel = 120000;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].StopDcc = 20000000;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].FastHomeVel = 70000;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].SlowHomeVel = 60000;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].SlowHomeDistance = 5000;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].HomeOffset = 2000;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].LimitSignal = 1;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].HomeSignal = 0;
	m_StepParameter[TOP_OFFLOAD_HOLDER_EX].HomeDirection = 0;

	m_StepParameter[TOP_OFFLOAD_PULLY_EX].Pulses = 1600;    
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].ScrewPitch = 70000;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].m_Scale = m_StepParameter[TOP_OFFLOAD_PULLY_EX].ScrewPitch / m_StepParameter[TOP_OFFLOAD_PULLY_EX].Pulses;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].Acc = 3000000;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].Dcc = 3000000;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].Vel = 150000;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].StopDcc = 15000000;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].FastHomeVel = 150000;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].SlowHomeVel = 150000;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].SlowHomeDistance = 5000;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].HomeOffset = -500;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].LimitSignal = 1;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].HomeSignal = 0;
	m_StepParameter[TOP_OFFLOAD_PULLY_EX].HomeDirection = 0;

	m_StepParameter[TOP_DEVICE_INDEXER_EX].Pulses = 1600;    
	m_StepParameter[TOP_DEVICE_INDEXER_EX].ScrewPitch = 25000;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].m_Scale = m_StepParameter[TOP_DEVICE_INDEXER_EX].ScrewPitch / m_StepParameter[TOP_DEVICE_INDEXER_EX].Pulses;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].Acc = 8000000;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].Dcc = 8000000;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].Vel = 300000;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].StopDcc = 80000000;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].FastHomeVel = 80000;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].SlowHomeVel = 50000;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].SlowHomeDistance = 10000;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].HomeOffset = 2000;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].LimitSignal = 1;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].HomeSignal = 0;
	m_StepParameter[TOP_DEVICE_INDEXER_EX].HomeDirection = 0;

	m_StepParameter[TOP_WINDOW_CLAMPER_EX].Pulses = 1600;    
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].ScrewPitch = 16000;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].m_Scale = m_StepParameter[TOP_WINDOW_CLAMPER_EX].ScrewPitch / m_StepParameter[TOP_WINDOW_CLAMPER_EX].Pulses;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].Acc = 3000000;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].Dcc = 3000000;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].Vel = 80000;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].StopDcc = 50000000;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].FastHomeVel = 10000;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].SlowHomeVel = 7000;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].SlowHomeDistance = 500;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].HomeOffset = 500;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].LimitSignal = 1;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].HomeSignal = 0;
	m_StepParameter[TOP_WINDOW_CLAMPER_EX].HomeDirection = 0;

}

void CStepCardControl::InitInputPort()
{
	m_InputValue[ONLOADER_CARD_ADDRESS]=0xffff;
	m_InputValue[OFFLOADER_CARD_ADDRESS]=0xffff;
	m_InputValue[INDEXER_CARD_ADDRESS]=0xffff;
	m_InputValue[IO_CARD_ADDRESS]=0xffff;
}
void CStepCardControl::InitOutputPort()
{
	m_OutputValue[ONLOADER_CARD_ADDRESS]=0xffff;
	m_OutputValue[OFFLOADER_CARD_ADDRESS]=0xffff;
	m_OutputValue[INDEXER_CARD_ADDRESS]=0xffff;
	m_OutputValue[IO_CARD_ADDRESS]=0xffff;
}

bool CStepCardControl::GetPortState(IO_CONFIG Input)
{
	unsigned short temp=0;
	short port_temp=Input.m_port;
	short signal_temp=Input.m_signal;

	unsigned short inputvalue=m_InputValue[port_temp/INPUT_NUM];
	temp=inputvalue & (1 << (port_temp%INPUT_NUM) );

	if(temp^(1 << (port_temp%INPUT_NUM) ))
	{
		if (NOPEN==signal_temp)
			return true;
		else
			return false;
	}
	else
	{
		if (NOPEN==signal_temp)
			return false;
		else
			return true;
		
	}
}

bool CStepCardControl::GetPortStateByPort(short port,bool signal)
{
	UpdateInputState(port/16);
	unsigned short temp=0;
	short port_temp=port;
	short signal_temp=signal;

	unsigned short inputvalue=m_InputValue[port_temp/INPUT_NUM];
	temp=inputvalue & (1 << (port_temp%INPUT_NUM) );

	if(temp^(1 << (port_temp%INPUT_NUM) ))
	{
		if (NOPEN==signal_temp)
			return true;
		else
			return false;
	}
	else
	{
		if (NOPEN==signal_temp)
			return false;
		else
			return true;

	}
}

bool CStepCardControl::GetOutputPortState(IO_CONFIG Output)
{
	unsigned short temp=0;
	short port_temp=Output.m_port;
	short signal_temp=Output.m_signal;


	unsigned short outputvalue=m_OutputValue[port_temp/OUTPUT_NUM];
	temp=outputvalue & (1 << (port_temp%OUTPUT_NUM) );

	if(temp^(1 << (port_temp%OUTPUT_NUM) ))
	{
		if (NOPEN==signal_temp)
			return true;
		else
			return false;
	}
	else
	{
		if (NOPEN==signal_temp)
			return false;
		else
			return true;

	}
}

void CStepCardControl::OpenPortByPort(short port, short signal)
{
	unsigned short enable=0;
	enable=enable | (1 << (port));
	if ( NOPEN == signal )
		m_OutputValue[port/OUTPUT_NUM] = m_OutputValue[port/OUTPUT_NUM] & ~(1 << (port%OUTPUT_NUM));
	else
		m_OutputValue[port/OUTPUT_NUM] = m_OutputValue[port/OUTPUT_NUM] | (1 << (port%OUTPUT_NUM));

}

void CStepCardControl::ClosePortByPort(short port, short signal)
{
	unsigned short enable=0;
	enable=enable | (1 << (port));
	if ( NOPEN == signal )
		m_OutputValue[port/OUTPUT_NUM] = m_OutputValue[port/OUTPUT_NUM] | (1 << (port%OUTPUT_NUM));
	else
		m_OutputValue[port/OUTPUT_NUM] = m_OutputValue[port/OUTPUT_NUM] & ~(1 << (port%OUTPUT_NUM));
}

void CStepCardControl::OpenPort(IO_CONFIG Output)
{
	unsigned short enable=0;
	short port=Output.m_port;
	short signal=Output.m_signal;
	enable=enable | (1 << (port));
	if ( NOPEN == signal )
		m_OutputValue[port/OUTPUT_NUM] = m_OutputValue[port/OUTPUT_NUM] & ~(1 << (port%OUTPUT_NUM));
	else
		m_OutputValue[port/OUTPUT_NUM] = m_OutputValue[port/OUTPUT_NUM] | (1 << (port%OUTPUT_NUM));

}

void CStepCardControl::ClosePort(IO_CONFIG Output)
{
	unsigned short enable=0;
	short port=Output.m_port;
	short signal=Output.m_signal;
	enable=enable | (1 << (port));
	if ( NOPEN == signal )
		m_OutputValue[port/OUTPUT_NUM] = m_OutputValue[port/OUTPUT_NUM] | (1 << (port%OUTPUT_NUM));
	else
		m_OutputValue[port/OUTPUT_NUM] = m_OutputValue[port/OUTPUT_NUM] & ~(1 << (port%OUTPUT_NUM));
}

int CStepCardControl::UpdateOutputState(char addr)
{
	//m_Addr=addr;
	//m_AxisID=0;
	unsigned long output=((long)m_OutputValue[addr] & 0xffffffff)<<16 | 0x0000ffff;
	if (!SendCanData(m_Addr, addr* AXIS_OF_CARD, SETOUTPUT,output))
		return false;
	else
		return -1;

}


int CStepCardControl::SetAllStepSpeedParameter()
{
	for (int i=0; i<AXIS_NUM; i++)
	{
		if(-1 == SetStepSpeedParameter(i,m_StepParameter[i]))
		{
			CString str;
			str.Format(_T("%d"),i);
			AfxMessageBox(_T("第")+str+_T("组参数传输错误！"));
			return -1;
		}
	}
	return false;
}

int CStepCardControl::SetStepSpeedParameter(unsigned short axis_index, AxisParameter axisParameter)
{
	// TODO: Add your control notification handler code here
	char Send_data[100]={};
	char rec_data[100]={};
	//m_Addr=axis_index/AXIS_OF_CARD;
	//m_AxisID=axis_index%AXIS_OF_CARD+1;
	int a,b,c,d;
	
	SendCanData(m_Addr,axis_index,SETACC,axisParameter.Acc / axisParameter.m_Scale);
	SendCanData(m_Addr,axis_index,SETDCC,axisParameter.Dcc / axisParameter.m_Scale);
	SendCanData(m_Addr,axis_index,SETVEL,axisParameter.Vel / axisParameter.m_Scale);
	SendCanData(m_Addr,axis_index,SETSTOPDCC,axisParameter.StopDcc / axisParameter.m_Scale);
	SendCanData(m_Addr,axis_index,HOMEFASTVEL,axisParameter.FastHomeVel / axisParameter.m_Scale);
	SendCanData(m_Addr,axis_index,HOMESLOWVEL,axisParameter.SlowHomeVel / axisParameter.m_Scale);
	SendCanData(m_Addr,axis_index,HOMEBACK,axisParameter.SlowHomeDistance / axisParameter.m_Scale);
	SendCanData(m_Addr,axis_index,HOMEOFFSET,axisParameter.HomeOffset / axisParameter.m_Scale);


	if (axisParameter.LimitSignal==0)
	{
		char temp=0x03;
		Send_data[0]=temp<<(axis_index % AXIS_OF_CARD*2);
		Send_data[1]=0;
		Send_data[2]=temp<<(axis_index % AXIS_OF_CARD*2);
		Send_data[3]=0;
	}
	else
	{
		char temp=0x03;
		Send_data[2]=temp<<(axis_index % AXIS_OF_CARD*2);
		temp=0x00;
		Send_data[0]=temp<<(axis_index % AXIS_OF_CARD*2);
		Send_data[1]=0;
		Send_data[3]=0;
	}
	a=Send_data[0] & 0x00ff;
	b=Send_data[1] << 8 & 0xff00;
	c=Send_data[2] << 16 & 0x00ff0000;
	d=Send_data[3] << 24 & 0xff000000;
	SendCanData(m_Addr,axis_index,LMTSENSE,a|b|c|d);


	if (axisParameter.HomeSignal==0)
	{
		char temp=0x01;
		Send_data[2]=temp<<(axis_index % AXIS_OF_CARD);
		temp=0x00;
		Send_data[0]=temp<<(axis_index % AXIS_OF_CARD);
		Send_data[1]=0;
		Send_data[3]=0;

	}
	else
	{
		char temp=0x01;
		Send_data[0]=temp<<(axis_index % AXIS_OF_CARD);
		Send_data[2]=temp<<(axis_index % AXIS_OF_CARD);
		Send_data[1]=0;
		Send_data[3]=0;
	}
	a=Send_data[0] & 0x00ff;
	b=Send_data[1] << 8 & 0xff00;
	c=Send_data[2] << 16 & 0x00ff0000;
	d=Send_data[3] << 24 & 0xff000000;
	SendCanData(m_Addr,axis_index,HOMEEDGE,a|b|c|d);


	if (axisParameter.HomeDirection==0)
	{
		char temp=0x01;
		Send_data[2]=temp<<(axis_index % AXIS_OF_CARD);
		temp=0x00;
		Send_data[0]=temp<<(axis_index % AXIS_OF_CARD);
		Send_data[1]=0;
		Send_data[3]=0;
	}
	else
	{
		char temp=0x01;
		Send_data[0]=temp<<(axis_index % AXIS_OF_CARD);
		Send_data[2]=temp<<(axis_index % AXIS_OF_CARD);
		Send_data[1]=0;
		Send_data[3]=0;
	}
	a=Send_data[0] & 0x00ff;
	b=Send_data[1] << 8 & 0xff00;
	c=Send_data[2] << 16 & 0x00ff0000;
	d=Send_data[3] << 24 & 0xff000000;
	SendCanData(m_Addr,axis_index,HOMEDIR,a|b|c|d);


	char temp=0x01;
	Send_data[2]=temp<<(axis_index % AXIS_OF_CARD);
	temp=0x00;
	Send_data[0]=temp<<(axis_index % AXIS_OF_CARD);
	Send_data[1]=0;
	Send_data[3]=0;

	a=Send_data[0] & 0x00ff;
	b=Send_data[1] << 8 & 0xff00;
	c=Send_data[2] << 16 & 0x00ff0000;
	d=Send_data[3] << 24 & 0xff000000;
	SendCanData(m_Addr, axis_index, SEARCHEDGE,a|b|c|d);
	
	return false;
}

int CStepCardControl::SetMotorSpeedProfile(unsigned short axis_index, STEPPER_CARD_SPEED_SETTING axis_speedsetting)
{
	// TODO: Add your control notification handler code here
	char Send_data[100]={};
	char rec_data[100]={};
	//m_Addr=axis_index/AXIS_OF_CARD;
	//m_AxisID=axis_index%AXIS_OF_CARD+1;

	SendCanData(m_Addr,axis_index,SETACC,axis_speedsetting.Acc / m_StepParameter[axis_index].m_Scale);
	SendCanData(m_Addr,axis_index,SETDCC,axis_speedsetting.Dcc / m_StepParameter[axis_index].m_Scale);
	SendCanData(m_Addr,axis_index,SETVEL,axis_speedsetting.Vel / m_StepParameter[axis_index].m_Scale);
	SendCanData(m_Addr,axis_index,SETSTOPDCC,axis_speedsetting.StopDcc / m_StepParameter[axis_index].m_Scale);

	return false;
}

void CStepCardControl::Process_RecData(VCI_CAN_OBJ RecData)
{
	long a,b,c,d;
	unsigned char axis=0;
	a=0;
	b=0;
	c=0;
	d=0;
	BYTE addr;
#ifdef _KANDE_CAN
	addr=RecData.ID[1]>>5;
#else
	addr=RecData.ID;
#endif
	
	BYTE command=RecData.Data[0];
	BYTE axisid=RecData.Data[1];

	if(addr == m_Addr)
	{
		//if (axisid == m_AxisID)
		{
			switch(command)
			{
			case GETSTATUS:
				if (RecData.Data[2]==0)
					m_bIsAxisMoving[m_Addr*AXIS_OF_CARD+0]=false;
				else
					m_bIsAxisMoving[m_Addr*AXIS_OF_CARD+0]=true;

				if (RecData.Data[3]==0)
					m_bIsAxisMoving[m_Addr*AXIS_OF_CARD+1]=false;
				else
					m_bIsAxisMoving[m_Addr*AXIS_OF_CARD+1]=true;

				if (RecData.Data[4]==0)
					m_bIsAxisMoving[m_Addr*AXIS_OF_CARD+2]=false;
				else
					m_bIsAxisMoving[m_Addr*AXIS_OF_CARD+2]=true;

				if (RecData.Data[5]==0)
					m_bIsAxisMoving[m_Addr*AXIS_OF_CARD+3]=false;
				else
					m_bIsAxisMoving[m_Addr*AXIS_OF_CARD+3]=true;

				m_bSendAndRecFinish=true;
				break;

			case GETPOSITION:
				a=RecData.Data[2] & 0x00ff;
				b=RecData.Data[3] << 8 & 0xff00;
				c=RecData.Data[4] << 16 & 0x00ff0000;
				d=RecData.Data[5] << 24 & 0xff000000;
				m_AxisPos[m_Addr*AXIS_OF_CARD+axisid-1]=a|b|c|d;
				//a=data[4] & 0x00ff;
				//b=data[5] & 0xff00 * 256;
				//c=data[6] & 0x00ff * 65536;
				//d=data[7] & 0xff00 * 16776960;
				//m_AxisPos[m_Addr*AXIS_OF_CARD+data[3]-1]=(a+b+c+d);
				m_bSendAndRecFinish=true;
				break;

			case GETSEARCHPOS:
				a=RecData.Data[2] & 0x00ff;
				b=RecData.Data[3] << 8 & 0xff00;
				c=RecData.Data[4] << 16 & 0x00ff0000;
				d=RecData.Data[5] << 24 & 0xff000000;
				m_SearchPos=a|b|c|d;
				m_bSendAndRecFinish=true;
				break;

			case GETINPUT:
				a=RecData.Data[2] & 0x00ff;
				b=RecData.Data[3] << 8 & 0xff00;
				c=RecData.Data[4] << 16 & 0x00ff0000;
				d=RecData.Data[5] << 24 & 0xff000000;
				m_InputValue[m_Addr]= a|b|c|d;
				m_bSendAndRecFinish=true;
				break;
			case SETACC:
			case SETDCC:
			case SETVEL:
			case SETSTOPDCC:
			case HOMEFASTVEL:
			case HOMESLOWVEL:
			case HOMEBACK:
			case HOMEOFFSET:
			case LMTSENSE:
			case HOMEEDGE:
			case HOMEDIR:
			case STOPMOVE:
			case SEARCHEDGE:
			case MOVEREL:
			case MOVEABS:
			case HOME:
			case SETOUTPUT:
			case ENABLELIMINT:
			case SEARCH:
				m_bSendAndRecFinish=true;
				break;
			
			default:
				AfxMessageBox(_T("接收错误!"));
				break;
			}
		}			
	}
}


int CStepCardControl::DisableStepController(short bOperationMode)
{
	return false;
}

int CStepCardControl::SendPortData(char addr, char axisID, char command, long data)
{
	char Send_data[9]={};
	char rec_data[9]={};

	Send_data[0]='$';
	Send_data[1]=addr;
	Send_data[2]=command;
	Send_data[3]=axisID;
	Send_data[4]=data%256;
	Send_data[5]=(data>>8)%256;
	Send_data[6]=(data>>16)%256;
	Send_data[7]=(data>>24)%256;
	Send_data[8]='#';

	//if (m_pWSerialPort)
	//{
	//	do
	//	{
	//		if (m_iSendAndRecCount>3)
	//		{
	//			m_iSendAndRecCount=0;
	//			m_bSendAndRecFinish=true;
	//			return -1;
	//		}
	//		m_pWSerialPort->WritePort(Send_data,9);
	//		m_bSendAndRecFinish=false;
	//		m_iSendAndRecCount++;
	//		m_pWSerialPort->ReadPort(rec_data,9);
	//		if (0 != rec_data[0])
	//			Process_RecData(rec_data,9);
	//	} while (!m_bSendAndRecFinish);
	//	m_iSendAndRecCount=0;
	//}
	return false;
}

int CStepCardControl::CheckStepBusyState(unsigned short usAxis)
{
	if(!SendCanData(m_Addr, usAxis, GETSTATUS, 0))
		return m_bIsAxisMoving[usAxis];
	else
		return -1;
}

int CStepCardControl::MoveMotorSteps(unsigned short usAxis, long lMoveDist, short nMotionMode )
{
	long Pos=lMoveDist;
	if (nMotionMode==SEARCH)
	{
		Pos=lMoveDist;
		if (!SendCanData(m_Addr, usAxis, nMotionMode, Pos))
			return false;
		else
			return -1;
	}
	else
	{
		Pos=lMoveDist;
		if (!SendCanData(m_Addr, usAxis, nMotionMode, Pos))
		{
			m_bIsAxisMoving[usAxis]=true;
			return false;
		}
		else
			return -1;
	}
	return false;
}

int CStepCardControl::MoveMotorStepsEx(unsigned short usAxis, long lMoveDist, short nMotionMode )
{
	int rtn;
	switch (m_MotionMoveStep)
	{
	case 0:
		if (!MoveMotorSteps(usAxis,lMoveDist,nMotionMode))
		{
			m_MotionMoveStep=10;
		}
		else
		{
			return -1;
		}
		break;
	case 10:
		rtn = CheckStepBusyState(usAxis);
		if (!rtn)
		{
			m_MotionMoveStep=0;
			return false;
		}
		else if (-1 == rtn)
		{
			return -1;
		}
		break;
	}
	return true;
}

int CStepCardControl::ReadMotorStopReason(unsigned short usAxis, BOOL* bHomeBit,BOOL* bNLimitBit,BOOL* bPLimitBit)
{
	switch(usAxis)
	{
	case TOP_ONLOAD_MAG_CLAMPPER_EX:
		UpdateInputState(ONLOADER_CARD_ADDRESS);
		*bNLimitBit=GetPortState(m_InputList.m_OnLoaderMagClamperHome);
		break;
	case TOP_ONLOAD_HOLDER_EX:
		UpdateInputState(ONLOADER_CARD_ADDRESS);
		*bNLimitBit=GetPortState(m_InputList.m_OnLoaderMagHolderHome);
		*bPLimitBit=GetPortState(m_InputList.m_OnLoaderMagHolderLimit);
		break;
	case TOP_ONLOAD_PULLY_EX:
		UpdateInputState(ONLOADER_CARD_ADDRESS);
		*bNLimitBit=GetPortState(m_InputList.m_OnLoaderMagPullyHome);
		*bPLimitBit=GetPortState(m_InputList.m_OnLoaderMagPullyLimit);
		break;
	case TOP_OFFLOAD_MAG_CLAMPPER_EX:
		UpdateInputState(OFFLOADER_CARD_ADDRESS);
		*bNLimitBit=GetPortState(m_InputList.m_OffLoaderMagClamperHome);
		break;
	case TOP_OFFLOAD_HOLDER_EX:
		UpdateInputState(OFFLOADER_CARD_ADDRESS);
		*bNLimitBit=GetPortState(m_InputList.m_OffLoaderMagHolderHome);
		*bPLimitBit=GetPortState(m_InputList.m_OffLoaderMagHolderLimit);
		break;
	case TOP_OFFLOAD_PULLY_EX:
		UpdateInputState(OFFLOADER_CARD_ADDRESS);
		*bNLimitBit=GetPortState(m_InputList.m_OffLoaderMagPullyHome);
		*bPLimitBit=GetPortState(m_InputList.m_OffLoaderMagPullyLimit);
		break;
	case TOP_DEVICE_INDEXER_EX:
		UpdateInputState(INDEXER_CARD_ADDRESS);
		*bNLimitBit=GetPortState(m_InputList.m_IndexerHome);
		*bPLimitBit=GetPortState(m_InputList.m_IndexerLimit);
		break;
	}


	return true;
}

int CStepCardControl::CheckStepMotionStatus(unsigned short usAxis, long* plDestinationDist)
{
	char Send_data[10]={};
	char rec_data[10]={};
	//m_Addr=usAxis/AXIS_OF_CARD;
	//m_AxisID=usAxis%AXIS_OF_CARD+1;

	if (!SendCanData(m_Addr, usAxis, GETPOSITION, 0))
	{
		*plDestinationDist=m_AxisPos[usAxis];
		return false;
	}
	else
		return -1;
	return false;
}

int CStepCardControl::CheckStepMotionSearchDis(unsigned short usAxis, long* plDestinationDist)
{
	char Send_data[10]={};
	char rec_data[10]={};
	//m_Addr=usAxis/AXIS_OF_CARD;
	//m_AxisID=usAxis%AXIS_OF_CARD+1;

	if (!SendCanData(m_Addr, usAxis, GETSEARCHPOS, 0))
	{
		*plDestinationDist=m_SearchPos;
		return false;
	}
	else
		return -1;
	return false;
}

double CStepCardControl::GetStepResolution(unsigned short usAxis)
{
	return m_StepParameter[usAxis].m_Scale;
}

int CStepCardControl::SearchStepHome(unsigned short usAxis)
{
	char Send_data[10]={};
	char rec_data[10]={};
	//m_Addr=usAxis/AXIS_OF_CARD;
	//m_AxisID=usAxis%AXIS_OF_CARD+1;
	if (!SendCanData(m_Addr, usAxis, HOME, 0))
	{
		m_bIsAxisMoving[usAxis]=true;
		return false;
	}
	else
		return -1;

	return false;
}

int CStepCardControl::StopStep(unsigned short usAxis)
{
	char Send_data[10]={};
	char rec_data[10]={};
	//m_Addr=usAxis/AXIS_OF_CARD;
	//m_AxisID=usAxis%AXIS_OF_CARD+1;

	if (!SendCanData(m_Addr, usAxis, STOPMOVE, 0))
		return false;
	else
		return -1;

	return false;
}

int CStepCardControl::GetStepperHomeSensor(unsigned short usAxis, BOOL* bHomeBit,BOOL* bNLimitBit,BOOL* bPLimitBit)
{
	char Send_data[10]={};
	char rec_data[10]={};
	//m_Addr=usAxis/AXIS_OF_CARD;
	//m_AxisID=0;

	switch(usAxis)
	{
	case TOP_ONLOAD_MAG_CLAMPPER_EX:
		UpdateInputState(ONLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OnLoaderMagClamperHome);
		*bNLimitBit=false;
		*bPLimitBit=false;
		break;
	case TOP_ONLOAD_HOLDER_EX:
		UpdateInputState(ONLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OnLoaderMagHolderHome);
		*bNLimitBit=false;
		*bPLimitBit=GetPortState(m_InputList.m_OnLoaderMagHolderLimit);;
		break;
	case TOP_ONLOAD_PULLY_EX:
		UpdateInputState(ONLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OnLoaderMagPullyHome);
		*bNLimitBit=false;
		*bPLimitBit=GetPortState(m_InputList.m_OnLoaderMagPullyLimit);;
		break;
	case TOP_OFFLOAD_MAG_CLAMPPER_EX:
		UpdateInputState(OFFLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OffLoaderMagClamperHome);
		*bNLimitBit=false;
		*bPLimitBit=false;
		break;
	case TOP_OFFLOAD_HOLDER_EX:
		UpdateInputState(OFFLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OffLoaderMagHolderHome);
		*bNLimitBit=false;
		*bPLimitBit=GetPortState(m_InputList.m_OffLoaderMagHolderLimit);
		break;
	case TOP_OFFLOAD_PULLY_EX:
		UpdateInputState(OFFLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OffLoaderMagPullyHome);
		*bNLimitBit=false;
		*bPLimitBit=GetPortState(m_InputList.m_OffLoaderMagPullyLimit);
		break;
	case TOP_DEVICE_INDEXER_EX:
		UpdateInputState(INDEXER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_IndexerHome);
		*bNLimitBit=false;
		*bPLimitBit=GetPortState(m_InputList.m_IndexerLimit);
		break;
	case TOP_WINDOW_CLAMPER_EX:
		UpdateInputState(INDEXER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_WindowClampHome);
		*bNLimitBit=false;
		*bPLimitBit=false;
		break;
	}
	return false;
}

int CStepCardControl::GetHomeSensor(unsigned short usAxis, BOOL* bHomeBit)
{
	char Send_data[10]={};
	char rec_data[10]={};
	//m_Addr=usAxis/AXIS_OF_CARD;
	//m_AxisID=usAxis%AXIS_OF_CARD+1;

	switch(usAxis)
	{
	case TOP_ONLOAD_MAG_CLAMPPER_EX:
		UpdateInputState(ONLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OnLoaderMagClamperHome);
		break;
	case TOP_ONLOAD_HOLDER_EX:
		UpdateInputState(ONLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OnLoaderMagHolderHome);
		break;
	case TOP_ONLOAD_PULLY_EX:
		UpdateInputState(ONLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OnLoaderMagPullyHome);
		break;
	case TOP_OFFLOAD_MAG_CLAMPPER_EX:
		UpdateInputState(OFFLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OffLoaderMagClamperHome);
		break;
	case TOP_OFFLOAD_HOLDER_EX:
		UpdateInputState(OFFLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OffLoaderMagHolderHome);
		break;
	case TOP_OFFLOAD_PULLY_EX:
		UpdateInputState(OFFLOADER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_OffLoaderMagPullyHome);
		break;
	case TOP_DEVICE_INDEXER_EX:
		UpdateInputState(INDEXER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_IndexerHome);
		break;
	case TOP_WINDOW_CLAMPER_EX:
		UpdateInputState(INDEXER_CARD_ADDRESS);
		*bHomeBit=GetPortState(m_InputList.m_WindowClampHome);
		break;
	}
	return false;
}


void CStepCardControl::SetJogMoveStep(long lMoveStep)
{
	m_lJogMoveStep=lMoveStep;

}

long CStepCardControl::GetJogMoveStep(unsigned short usAxis)
{
	return m_lJogMoveStep;

}

int CStepCardControl::StartJogStepper(unsigned short usAxis, short nMotionDir)
{
	long Pos;
	if ( POSITIVE == nMotionDir )
		Pos=m_lJogMoveStep;
	else
		Pos=0 - m_lJogMoveStep;

	if (!SendCanData(m_Addr, usAxis, MOVEREL, Pos))
		m_bIsAxisMoving[usAxis]=true;

	while (1)
	{
		int rtn=CheckStepBusyState(usAxis);
		if (-1 == rtn)
			return -1;
		else if ( false == rtn )
			return false;
		else
			PeekAndPump();
	}
	return false;
}

int CStepCardControl::SetSection(unsigned short usAxis, long lAcc, long lDcc)
{
	SendCanData(m_Addr,usAxis,SETDCC,lAcc);
	SendCanData(m_Addr,usAxis,SETDCC,lDcc);
	return false;
}

int CStepCardControl::SetOutputPort(short nOutputId, short nState)
{
	return 0;
}

int CStepCardControl::PowerOnStepper(void)
{
	m_StepCardControl.OpenPort(m_StepCardControl.m_OutputList.m_StepCardPowerCtrl);
	m_StepCardControl.UpdateOutputState(IO_CARD_ADDRESS);
	return 0;
}

int CStepCardControl::PowerOffStepper(void)
{
	m_StepCardControl.ClosePort(m_StepCardControl.m_OutputList.m_StepCardPowerCtrl);
	m_StepCardControl.UpdateOutputState(IO_CARD_ADDRESS);
	return 0;
}

int CStepCardControl::CheckStepperPowerStatus(bool *pbStepperOn)
{
	return 0;
}

int CStepCardControl::UpdateInputState(char addr)
{
	if (!SendCanData(m_Addr, addr*AXIS_OF_CARD, GETINPUT, 0))
		return false;
	else
		return -1;
}

int CStepCardControl::GetInjectPushState()
{
	return m_InjectPushStep;
}

void CStepCardControl::ResetInjectPushState()
{
	m_InjectPushStep = DC_MOTOR_IDLE;
	m_StepCardControl.ClosePort(m_StepCardControl.m_OutputList.m_InjectPusherPower);
	m_StepCardControl.ClosePort(m_StepCardControl.m_OutputList.m_InjectPusherDir);
	m_StepCardControl.UpdateInputState(ONLOADER_CARD_ADDRESS);
	
}



int CStepCardControl::InjectPush()
{
	static int Inject_error=false;
	switch (m_InjectPushStep)
	{
	case DC_MOTOR_IDLE:
//		if (!UpdateInputState(ONLOADER_CARD_ADDRESS) )
		{
			//if(GetPortState(m_InputList.m_OnLoaderMagDetectedSensor.m_port) && GetPortState(m_InputList.m_OnLoaderMagBufferPresentSensor.m_port))
			//{
			//	return -1;
			//}
			//else
			{
				OpenPort(m_OutputList.m_InjectPusherPower);
				ClosePort(m_OutputList.m_InjectPusherDir);
				m_Timer.StartTime();
				UpdateOutputState(ONLOADER_CARD_ADDRESS);
				m_InjectPushStep=DC_MOTOR_MOVE_TO_LIMIT;
				Inject_error=false;
			}
		}
		
		break;
	case DC_MOTOR_MOVE_TO_LIMIT:
		UpdateInputState(ONLOADER_CARD_ADDRESS);
		if(!GetPortState(m_InputList.m_InjectPusherHome) )
		{
			OpenPort(m_OutputList.m_InjectPusherDir);
			UpdateOutputState(ONLOADER_CARD_ADDRESS);
			m_Timer.StartTime();
			m_InjectPushStep=DC_MOTOR_MOVE_TO_HOME;
		}
		else if (m_Timer.EndTime() > 3000)
		{
			ClosePort(m_OutputList.m_InjectPusherPower);
			ClosePort(m_OutputList.m_InjectPusherDir);
			UpdateOutputState(ONLOADER_CARD_ADDRESS);
			//m_InjectPushStep=DC_MOTOR_IDLE;
			m_InjectPushStep=DC_MOTOR_MOVE_TO_HOME; //测试用
			Inject_error=-1;
			//return -1;
		}
		break;
	case DC_MOTOR_MOVE_TO_HOME:
		UpdateInputState(ONLOADER_CARD_ADDRESS);
		if(!GetPortState(m_InputList.m_InjectPusherLimit) )
		{
			ClosePort(m_OutputList.m_InjectPusherPower);
			ClosePort(m_OutputList.m_InjectPusherDir);
			UpdateOutputState(ONLOADER_CARD_ADDRESS);
			m_InjectPushStep=DC_MOTOR_IDLE;
			return Inject_error;
		}
		else if (m_Timer.EndTime() > 3000)
		{
			ClosePort(m_OutputList.m_InjectPusherPower);
			ClosePort(m_OutputList.m_InjectPusherDir);
			UpdateOutputState(ONLOADER_CARD_ADDRESS);
			m_InjectPushStep=DC_MOTOR_IDLE;
			return -1;
		}
		break;
	}
	return true;
}

int CStepCardControl::MagezinePull(unsigned short addr, int mode)
{
	int rtn=0;
	short axis_id;
	IO_CONFIG sensor_home,sensor_limit,sensor_drop;
	if(addr==ONLOADER_CARD_ADDRESS)
		axis_id=TOP_ONLOAD_PULLY_EX;
	else
		axis_id=TOP_OFFLOAD_PULLY_EX;
	switch (m_MagezinePullStep[addr])
	{
	case 0:
		if ( TRIGGER_TO_HOME == mode )
			m_MagezinePullStep[addr]=30;
		else
			m_MagezinePullStep[addr]=10;
		break;
	case 10:
		if(m_StepCardControl.CheckStepBusyState(axis_id) == FALSE)
		{
			if (addr==ONLOADER_CARD_ADDRESS)
				sensor_limit=m_StepCardControl.m_InputList.m_OnLoaderMagPullyLimit;
			else
				sensor_limit=m_StepCardControl.m_InputList.m_OffLoaderMagPullyLimit;
			m_StepCardControl.UpdateInputState(addr);
			if(!m_StepCardControl.GetPortState(sensor_limit))
			{
				m_StepCardControl.MoveMotorSteps(axis_id, MAG_PULL_DISTANCE / m_StepParameter[axis_id].m_Scale, MOVEABS); 
				m_MagezinePullStep[addr]=20;
			}
			else
			{
				m_MagezinePullStep[addr]=20;
				m_Timer.StartTime();
			}
		}
		break;
	case 20:
		if (!CheckStepBusyState(axis_id))
		{
			if (addr==ONLOADER_CARD_ADDRESS)
				sensor_limit=m_StepCardControl.m_InputList.m_OnLoaderMagPullyLimit;
			else
				sensor_limit=m_StepCardControl.m_InputList.m_OffLoaderMagPullyLimit;

			m_StepCardControl.UpdateInputState(addr);
			if(m_StepCardControl.GetPortState(sensor_limit))
			{
				if ( TRIGGER_TO_LIMIT == mode )
				{
					m_MagezinePullStep[addr]=0;
					return false;
				}
				else
					m_MagezinePullStep[addr]=30;

			}
			else
			{
				if (addr==ONLOADER_CARD_ADDRESS)
					SearchStepHome(TOP_ONLOAD_PULLY_EX);
				else
					SearchStepHome(TOP_OFFLOAD_PULLY_EX);
				m_MagezinePullStep[addr]=0;
				return -1;
			}
		}
		break;
	case 30:
		if (!CheckStepBusyState(axis_id))
		{
			if (addr==ONLOADER_CARD_ADDRESS)
				sensor_drop=m_StepCardControl.m_InputList.m_OnLoaderMagDropSensor;
			else
				sensor_drop=m_StepCardControl.m_InputList.m_OffLoaderMagDropSensor;
			m_StepCardControl.UpdateInputState(addr);
			if(m_StepCardControl.GetPortState(sensor_drop))
			{
				m_MagezinePullStep[addr]=0;
				return -2;
			}
			else
			{
				m_StepCardControl.SearchStepHome(axis_id);
				m_MagezinePullStep[addr]=40;
			}
		}
		break;
	case 40:
		if (!CheckStepBusyState(axis_id))
		{
			if (addr==ONLOADER_CARD_ADDRESS)
				sensor_home=m_StepCardControl.m_InputList.m_OnLoaderMagPullyHome;
			else
				sensor_home=m_StepCardControl.m_InputList.m_OffLoaderMagPullyHome;
			m_StepCardControl.UpdateInputState(addr);
			if(m_StepCardControl.GetPortState(sensor_home))
			{
				m_MagezinePullStep[addr]=0;
				return false;
			}
			else
			{
				m_MagezinePullStep[addr]=0;
				return -1;
			}
		}
		break;
	}
	return true;
}

void CStepCardControl::ResetMagPullyState()
{
	m_InjectPushStep = DC_MOTOR_IDLE;
}

int CStepCardControl::StopAllStep(unsigned short addr)
{
	// TODO: Add your control notification handler code here

	SendCanData(m_Addr,addr*AXIS_OF_CARD+1,STOPMOVE);
	SendCanData(m_Addr,addr*AXIS_OF_CARD+2,STOPMOVE);
	SendCanData(m_Addr,addr*AXIS_OF_CARD+3,STOPMOVE);
	return false;
}

void CStepCardControl::CloseAllOutput()
{
	ClosePort(m_StepCardControl.m_OutputList.m_GreenLight);
	ClosePort(m_StepCardControl.m_OutputList.m_YellowLight);
	ClosePort(m_StepCardControl.m_OutputList.m_RedLight);
	ClosePort(m_StepCardControl.m_OutputList.m_LFHoler);
	ClosePort(m_StepCardControl.m_OutputList.m_InjectClaw);
	ClosePort(m_StepCardControl.m_OutputList.m_InputClaw);
	ClosePort(m_StepCardControl.m_OutputList.m_OutputClaw);
	ClosePort(m_StepCardControl.m_OutputList.m_EjectClaw);
	ClosePort(m_StepCardControl.m_OutputList.m_StepCardPowerCtrl);
	UpdateOutputState(INDEXER_CARD_ADDRESS);
	UpdateOutputState(IO_CARD_ADDRESS);
}