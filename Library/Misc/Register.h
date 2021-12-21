// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 REGISTER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// REGISTER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifndef _REGISTER_H_
#define _REGISTER_H_

#ifdef __cplusplus 
extern "C" { 
#endif 


	typedef struct _REGISTERINFORMATION {
		DWORD dwMachineCode;                  //机器码
		DWORD dwFirstRunDateTime;             //首次注册时间
		DWORD dwLastRunDateTime;              //软件最后运行时间
		DWORD dwRegDays;                      //试用天数
		DWORD dwHDPowerOnHours;               //注册时硬盘加电时间(小时数)
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
	//功能： 录入注册码
	//输入参数： szRegCode  注册码指针 注册码可使用注册机获得
	//输出参数：
	//返回值：   录入成功返回 0
	//////////////////////////////////////////////////////////////////////
	REGISTER_API int WINAPI RegisterOp(char* szRegCode);


	//////////////////////////////////////////////////////////////////////
	//功能： 获取机台的机器码，厂商使用该码生成对应的注册码
	//输入参数： len 机器码长度 固定为10
	//输出参数： szMachineCode  机器码指针 
	//返回值：   获取成功返回 0
	//////////////////////////////////////////////////////////////////////
	REGISTER_API int WINAPI GetMachineCode(char* szMachineCode, int len);


	//////////////////////////////////////////////////////////////////////
	//功能： 获取机器的注册状态
	//输入参数： 
	//输出参数： 
	//返回值：   若注册有效返回 0
	//////////////////////////////////////////////////////////////////////
	REGISTER_API int WINAPI VerifyRegister();


	//////////////////////////////////////////////////////////////////////
	//功能： 获取系统 注册截止日期 和 剩余活动天数
	//输入参数： 
	//输出参数： dwYear  dwMonth dwDays 年月日     dwLeaveDays 剩余天数
	//返回值：   获取成功返回 0
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