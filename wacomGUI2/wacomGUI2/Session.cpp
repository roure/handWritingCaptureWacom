#include "StdAfx.h"
#include "Session.h"


Session::Session(void)
{
}


Session::~Session(void)
{
}


string Session::getId(void)
{
	return id;
}


string Session::getAge(void)
{
	return age;
}


string Session::getSex(void)
{
	return sex;
}


string Session::getHanded(void)
{
	return handed;
}


string Session::getStudies(void)
{
	return studies;
}


string Session::getComments(void)
{
	return comments;
}


string Session::getSession(void)
{
	return sessionId;
}


string Session::getHistoryNumber(void)
{
	return historyNumber;
}

void Session::setId(string name)
{
	id=name;
}

void Session::setHistoryNumber(string param)
{
	historyNumber=param;
}


void Session::setAge(string param)
{
	age=param;
}


void Session::setHanded(string param)
{
	handed=param;
}


void Session::setStudies(string param)
{
	studies=param;
}


void Session::setComments(string param)
{
	comments=param;
}


void Session::setSessionId(string param)
{
	sessionId=param;
}



void Session::fill(string *user)
{
	id=user[0];
	historyNumber=user[1];
	age=user[2];
	sex=user[3];
	handed=user[4];
	studies=user[5];
	comments=user[6];
	sessionId=user[7];
}


void Session::clearButId(string userId)
{
	id=userId;
	historyNumber="";
	age="";
	sex="";
	handed="";
	studies="";
	comments="";
	sessionId="";
}


string * Session::getText(void)
{
	string * user = new string[8];
	
	user[0]= id;
	user[1]=historyNumber;
	user[2]=age;
	user[3]=sex;
	user[4]=handed;
	user[5]=studies;
	user[6]=comments;
	user[7]=sessionId;

	return user;
}


void Session::clear(void)
{
	id="";
	historyNumber="";
	age="";
	sex="";
	handed="";
	studies="";
	comments="";
	sessionId="";
}
