#pragma once

#include "IniFile.h"

// CCuEliteDeviceOffsetSetupPage dialog

#define   ID_DEVOFFSET_X_AXIS_MOTOR 0 //v2.18

typedef enum enDevOffsetIndexMotor 
{
    ID_DEVOFFSET_FRONTRAIL_MOTOR,                  
    ID_DEVOFFSET_REARRAIL_MOTOR, 
    ID_DEVOFFSET_FRONT_REAR_RAIL_MOTOR, 
};   

typedef enum enCuEliteDevOffsetElevator 
{

    ID_DEVOFFSET_ONLOAD_X_AXIS_MOTOR,
  	ID_DEVOFFSET_OFFLOAD_X_AXIS_MOTOR,
	ID_DEVOFFSET_ONLOAD_Y_AXIS_MOTOR,
	ID_DEVOFFSET_OFFLOAD_Y_AXIS_MOTOR,

};

typedef enum enCuEliteDevOffsetMotors 
{
	//v2.18.3.S1 edit
    ID_DEVOFFSET_ONLOAD_YTWO_AXIS_MOTOR,
	ID_DEVOFFSET_ONLOAD_YONE_AXIS_MOTOR,
	ID_DEVOFFSET_OFFLOAD_YTWO_AXIS_MOTOR,
	ID_DEVOFFSET_OFFLOAD_YONE_AXIS_MOTOR,
	ID_DEVOFFSET_ONLOAD_YONE_YTWO_AXIS_MOTOR,
	ID_DEVOFFSET_OFFLOAD_YONE_YTWO_AXIS_MOTOR

};

class CCuEliteDeviceOffsetSetupPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteDeviceOffsetSetupPage)

public:
	CCuEliteDeviceOffsetSetupPage();
	virtual ~CCuEliteDeviceOffsetSetupPage();

// Dialog Data
	enum { IDD = IDD_TAB_CUELITE_DEVICEOFFSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
     virtual void OnOK();	//v2.18.3.S1 add


	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();


	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnCbnSelchangeComboSideId();
public:

private:
	//v2.18.5.w3 add { //for  variable display usage
	int m_nElevatorId;  
    int m_nStepMotorId;
    int m_nRailMotorId; 
    int m_nClampMotorId; 
    int  m_bStepperAddr;

    long m_lTrackStandby; 
    long m_lLengthStandby;
    long m_lWidthStandby; 
    long m_lDepthStandby; 

    long m_lExpectedDist; 
    long m_lExpectedDist2; 
 
	CString m_sID;

	CiniFile m_newFile, m_refFile;

    //}

	//v2.18.3.S1 add
    long m_lMagDevLength;
    long m_lMagDevWidth;
    long m_lLFTrackDevWidth;
    long m_lMagSysLength;
    long m_lMagSysWidth;
   //v2.18.5.w4 delete  long m_lLFTrackSysWidth;
   //v2.18.5.w4 delete  long m_lMagLengthOffset;
   //v2.18.5.w4 delete  long m_lMagWidthOffset;
    long m_lLFTrackWidthOffset;
    long m_lLFTrackOffset;
    long m_lMagYOneOffset;
    long m_lMagYTwoOffset;

private:
    long m_lLFFrontRailCurPos;
    long m_lLFRearRailCurPos;
    long m_lOnLoadMagZineYOneCurPos;
    long m_lOnLoadMagZineYTwoCurPos;
    long m_lOffLoadMagZineYOneCurPos;
    long m_lOffLoadMagZineYTwoCurPos;

	
public:
    void setRefFile(void);

   afx_msg void OnBnClickedButtonDoneDevOffset();
	//v2.18.3.S1 add
   afx_msg void OnLbnSelchangeLFTrackStepperMotor();
   afx_msg void OnLbnSelchangeYAxisStepperMotor();

   afx_msg void OnEnChangeEditTrackWidth3();


   afx_msg void OnEnChangeEditMagLength();
   afx_msg void OnEnChangeEditMagWidth();
   afx_msg void OnEnChangeEditTrackWidth();


   afx_msg void OnBnClickedEditTrackMotor();
   afx_msg void OnBnClickedEditXaxisMotor();
   afx_msg void OnBnClickedEditYaxisMotor();

   //afx_msg void OnEnSetfocusEditMagLength();
   //afx_msg void OnEnSetfocusEditMagWidth();
   afx_msg void OnEnSetfocusEditLFTrackWidth();

   //v2.18.5.w3 add {
   afx_msg void OnBnClickedButtonTrackHome();
   afx_msg void OnBnClickedButtonTrackStandby();
   afx_msg void OnBnClickedButtonTrackTeach();

   afx_msg void OnBnClickedButtonMagLengthHome();
   afx_msg void OnBnClickedButtonMagLengthStandby();
   afx_msg void OnBnClickedButtonMagLengthTeach();

   afx_msg void OnBnClickedButtonMagWidthHome();
   afx_msg void OnBnClickedButtonMagWidthStandby();

   afx_msg void OnBnClickedButtonMagWidthTeach();
 

   afx_msg void OnBnClickedButtonMagClampOpen();
   afx_msg void OnBnClickedButtonMagClampClose();
   afx_msg void OnBnClickedButtonMagClampTeach();


   afx_msg void OnEnSetfocusEditTrackDist();
   afx_msg void OnEnSetfocusEditMagLength();
   afx_msg void OnEnSetfocusEditMagWidth();
   afx_msg void OnEnSetfocusEditMagClampDepth();

   afx_msg void OnCbnSelchangeTrackMotor();
   afx_msg void OnCbnSelchangeElevatorMotor();
   afx_msg void OnCbnSelchangeMagWidthMotor();
   afx_msg void OnCbnSelchangeMagClampMotor();

   void  loadStandbyFromID_CUELITEini();
   void  saveStandbyToID_CUELITEini();
   //}
   afx_msg void OnBnClickedButtonMagWidthStandby2();
   afx_msg void OnBnClickedButtonMagWidthTeach2();
   afx_msg void OnEnSetfocusEditFile3();
   afx_msg void OnBnClickedButtonLoadDatabase();
   afx_msg void OnBnClickedButtonInite2();
};
