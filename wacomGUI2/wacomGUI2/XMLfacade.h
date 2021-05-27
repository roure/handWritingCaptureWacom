#pragma once

#include <string>
using namespace std;
#include <mxml.h>
#include <sstream> 
#include <iostream>
#include <fstream>
#include <direct.h>

using namespace std;

class XMLfacade
{
public:
	XMLfacade(void);
	~XMLfacade(void);
private:
	string fileName;
	mxml_node_t *tree;
public:
	mxml_node_t * readXMLfile(void);
	void writeXMLfile(void);
	mxml_node_t * findUserByNumId(void);
public:
	mxml_node_t * findUserByHistoryNumber(void);
	string * getUserDataByNumId(string id);
private:
	string * getUserData(mxml_node_t * user);
	mxml_node_t * findUserByTag(string userName, string tag);
public:
	mxml_node_t * createDocument(void);
	void addUser(string * user);
private:
	mxml_node_t * createUser(string * userData);
	string * fillInUserData(mxml_node_t * user);
public:
	string * getUserDataByHistory(string id);
	void updateUser(string * userData);
private:
	void removeSpaceNodes(void);
	string *tag;
public:
	bool existsUser(string userId);
	string getLastUserId(void);
private:
	mxml_node_t * createVoidXML(void);
};

	const char *				/* O - Whitespace string or NULL */
	whitespace_cb(mxml_node_t *node,	/* I - Element node */
              int         where);	/* I - Open or close tag? */

