#pragma once

enum enUserLevel {USER_OPERATOR, USER_PROCESS, USER_SERVICE, USER_ENGINEER};

class CUser
{
public:
	CUser(void);
	~CUser(void);
public:	//functions
    void SetUserLevel(int iUserLevel);
    int GetUserLevel(void);
	int GetPassword(short nUserLevel);
private:	//variables
    int m_iUserLevel;
};

extern CUser theUser;
