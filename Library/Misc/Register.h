// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� REGISTER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// REGISTER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#ifndef _REGISTER_H_
#define _REGISTER_H_

#ifdef __cplusplus 
extern "C" { 
#endif 


	typedef struct _REGISTERINFORMATION {
		DWORD dwMachineCode;                  //������
		DWORD dwFirstRunDateTime;             //�״�ע��ʱ��
		DWORD dwLastRunDateTime;              //����������ʱ��
		DWORD dwRegDays;                      //��������
		DWORD dwHDPowerOnHours;               //ע��ʱӲ�̼ӵ�ʱ��(Сʱ��)
	} REGISTERINFORMATION, *PREGISTERINFORMATION, *LPREGISTERINFORMATION;

#ifdef REGISTER_EXPORTS
#define REGISTER_API __declspec(dllexport)
#else
#define REGISTER_API __declspec(dllimport)
#endif

	
	WORD WINAPI CRC16(LPBYTE byteArray, int nArrayLen);
	BOOL WINAPI IsAdministrator();
	BOOL WINAPI GetPhysicalDiskSerial(LPSTR lpBufSerial, DWORD dwSerialLen, int nPhysicalDiskNumber = 0);
	BOOL WINAPI GetRegisterInformation(PREGISTERINFORMATION pRegInfo);
	BOOL WINAPI SaveRegisterInformation(PREGISTERINFORMATION pRegInfo);
	BOOL WINAPI UpdateSoftwareLastRunDateTime();
	BOOL WINAPI Encryption(LPBYTE byteInOut, BOOL bEncrypt);
	
	//////////////////////////////////////////////////////////////////////
	//���ܣ� ¼��ע����
	//��������� szRegCode  ע����ָ�� ע�����ʹ��ע������
	//���������
	//����ֵ��   ¼��ɹ����� 0
	//////////////////////////////////////////////////////////////////////
	REGISTER_API int WINAPI RegisterOp(char* szRegCode);


	//////////////////////////////////////////////////////////////////////
	//���ܣ� ��ȡ��̨�Ļ����룬����ʹ�ø������ɶ�Ӧ��ע����
	//��������� len �����볤�� �̶�Ϊ10
	//��������� szMachineCode  ������ָ�� 
	//����ֵ��   ��ȡ�ɹ����� 0
	//////////////////////////////////////////////////////////////////////
	REGISTER_API int WINAPI GetMachineCode(char* szMachineCode, int len);


	//////////////////////////////////////////////////////////////////////
	//���ܣ� ��ȡ������ע��״̬
	//��������� 
	//��������� 
	//����ֵ��   ��ע����Ч���� 0
	//////////////////////////////////////////////////////////////////////
	REGISTER_API int WINAPI VerifyRegister();


	//////////////////////////////////////////////////////////////////////
	//���ܣ� ��ȡϵͳ ע���ֹ���� �� ʣ������
	//��������� 
	//��������� dwYear  dwMonth dwDays ������     dwLeaveDays ʣ������
	//����ֵ��   ��ȡ�ɹ����� 0
	//////////////////////////////////////////////////////////////////////
	REGISTER_API int WINAPI GetActiveDays(DWORD &dwYear, DWORD &dwMonth, DWORD &dwDays, DWORD &dwLeaveDays);


#ifdef __cplusplus 
} 
#endif

#define RegCodeLength       0x20

//Error Code
#define ERR_NoERR             0
#define ERR_NullParam         -1
#define ERR_WrongRegCode      -2
#define ERR_Administrator     -3
#define ERR_UnRegister        -4
#define ERR_NullRegInfo       -5

#endif