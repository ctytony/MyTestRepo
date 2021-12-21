//DataManager.h v1.0.T6

#pragma once

#define RECIPE_BUF_SIZE		18000000	//v1.0.T4 rename
#define DATA_BUF_SIZE		640000		//v1.0.T4 rename

// CDataManager command target

enum DATA_MGR_ERROR
{
    DATA_MGR_OK,
    DATA_MGR_OPEN_FILE_FAIL,
    DATA_MGR_READ_FILE_FAIL,
    DATA_MGR_MALLOC_MEM_FAIL,
};

class CDataManager : public CObject
{
public:
	CDataManager();
	virtual ~CDataManager();
public:	//functions
    int SaveDataToFile(CString sFileName);
    int LoadDataFromFile(CString sFileName, int *nFileLen);
    int AllocBufferMemory(long lBufSize);
    int FreeBufferMemory(void);
    int CopyToBuffer(char *pcSource, int lDataLen);
    int CopyFromBuffer(char *pcDest, int lDataLen);
    void ZipPrFile(CString sTargetDir, CString sRecipeAddr, CString sFileName, CString sDir);
    void UnZipPrFile(CString sTargetDir, CString sDir);
    int DeleteVisionPattern(CString sDir);
    void DeleteFile(CString sFilePath);
private:	//variables
    char *m_pcDataBuf;	//v1.0.T3 rename
    int m_nBufAddr;
private:	//functions
	void CopyBuffer(char *pcDataBuf, int *pnBufAddr);
};

extern CDataManager theDataMgr;
