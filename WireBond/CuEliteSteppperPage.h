#pragma once


// CCuEliteSteppperPage dialog


typedef enum enCuEliteElevator 
{
    ID_ONLOAD_ELEVATOR_MOTOR=0,  
	ID_OFFLOAD_ELEVATOR_MOTOR, 

};

typedef enum enCuEliteMotors 
{
	ID_ONLOAD_Z_AXIS_MOTOR=0,  //elevator
	ID_ONLOAD_X_AXIS_MOTOR,    //side clamp
    ID_ONLOAD_YTWO_AXIS_MOTOR, //front clamp
	ID_ONLOAD_YONE_AXIS_MOTOR, //rear clamp
	ID_OFFLOAD_Z_AXIS_MOTOR,  //elevator
	ID_OFFLOAD_X_AXIS_MOTOR,    //side clamp
    ID_OFFLOAD_YTWO_AXIS_MOTOR, //front clamp
	ID_OFFLOAD_YONE_AXIS_MOTOR, //rear clamp
};

class CCuEliteSteppperPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCuEliteSteppperPage)

public:
	CCuEliteSteppperPage();
	virtual ~CCuEliteSteppperPage();

// Dialog Data
	enum { IDD = IDD_TAB_CUELITE_ELEVATOR_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive( );
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnCbnSelchangeComboSideId();
	afx_msg void OnBnClickedButtonHolderHome();
	afx_msg void OnBnClickedButtonHolderStop();
	afx_msg void OnBnClickedButtonPullyHome();
	afx_msg void OnBnClickedButtonPullyLimit();

private:
	int iElevatorId;
	short m_nCurrentSlot;
    int m_nStepMotorId;
	int m_nElevatorId;
    int  m_bStepperAddr;
    long m_lScale;
	long m_lCurrentMagClampDepth;
	
public:
	afx_msg void OnBnClickedButtonNextSlot();
 //   afx_msg void OnBnClickedButtonStandby();
	afx_msg void OnBnClickedButtonUnload();
//	afx_msg void OnBnClickedButtonPusherHome();
//	afx_msg void OnBnClickedButtonPusherLimit();
	afx_msg void OnEnSetfocusEditCurrentSlot();
	afx_msg void OnEnChangeEditCurrentSlot();
	afx_msg void OnBnClickedButtonPusherHome();
	afx_msg void OnBnClickedButtonPusherLimit();
    afx_msg void OnLbnSelchangeListStepperMotor();
    afx_msg void OnBnClickedButtonForward();
	afx_msg void OnBnClickedButtonBackward();
	afx_msg void OnBnClickedButtonHomeStep();
    afx_msg void OnBnClickedButtonHolderStandBy();
    afx_msg void OnEnSetfocusEditScale();
	//v2.18.3.S1 add
    afx_msg void OnBnClickedButtonMagclampForward();
    afx_msg void OnBnClickedButtonMagClampBackward();
	afx_msg void OnEnSetfocusCurrentDepth();

	afx_msg void OnBnClickedButtonStandBy();
};
