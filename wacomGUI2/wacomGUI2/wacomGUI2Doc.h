
// wacomGUI2Doc.h : interface of the CwacomGUI2Doc class
//


#pragma once
#include "Session.h"
#include "XMLfacade.h"
#include <sstream> 
#include <iostream>
#include <fstream>
#include <direct.h>
using namespace std;

#include <list>
#include "wacomFacade.h"
#include "point.h"

class CwacomGUI2Doc : public CDocument
{
protected: // create from serialization only
	CwacomGUI2Doc();
	DECLARE_DYNCREATE(CwacomGUI2Doc)

// Attributes
public:

	
private:
	CString s;

// Operations
public:
	void setString(CString param) {s=param;}
	CString getString() {return s;}
	static CwacomGUI2Doc * GetDoc();
	

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CwacomGUI2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
private:
	Session *session;
	XMLfacade *xml;
public:
	void setUserId(CString user);
public:
	void loadUserById(string id);
	void loadUserByHistory(string id);
	string * getSessionText(void);
	void login(string * userData);
private:
	string getFormatedId(string id);
public:
	string getIncrementedFormatedId(string id);
private:
	string getFormatedIdAux(string id, bool increment);
public:
	string getNextUserId(void);
	bool currentUserExists(void);
	list<PACKET> * pointList;
	void addPacket(PACKET pkt);
	void clearPackets(void);
	void clearDrawingPoints(void);
	list<PACKET> * getPackets(void);
	list<point> * getDrawingPoints(void);
	list<point> * drawList;
	void saveExercice(void);
private:
	int exercice;
	void saveFile(void);
public:
	void clearLastOnAirPoints(void);
	void deleteExercice(void);
	CString * getSessionLabel(void);
};
