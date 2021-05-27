#pragma once

#include <string>
using namespace std;

#include "Session.h"

class Session
{
public:
	Session(void);
	~Session(void);
private:
	string id;
	string historyNumber;
	string age;
	string sex;
	string handed;
	string studies;
	string comments;
	string sessionId;
public:
	string getId(void);
	string getAge(void);
	string getSex(void);
	string getHanded(void);
	string getStudies(void);
	string getComments(void);
	string getSession(void);
	void setId(string name);
	string getHistoryNumber(void);
	void setHistoryNumber(string param);
	void setAge(string param);
	void setHanded(string param);
	void setStudies(string param);
	void setComments(string param);
	void setSessionId(string param);
	void fill(string *user);
	void clearButId(string userId);
	string * getText(void);
	void clear(void);
};

