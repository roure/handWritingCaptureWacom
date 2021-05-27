
// wacomGUI2Doc.cpp : implementation of the CwacomGUI2Doc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "wacomGUI2.h"
#endif

#include "wacomGUI2Doc.h"
#include "wacomGUI2.h"

#include <propkey.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CwacomGUI2Doc

IMPLEMENT_DYNCREATE(CwacomGUI2Doc, CDocument)

BEGIN_MESSAGE_MAP(CwacomGUI2Doc, CDocument)
END_MESSAGE_MAP()


// CwacomGUI2Doc construction/destruction

CwacomGUI2Doc::CwacomGUI2Doc()
	: session(NULL)
	, xml(NULL)
	, exercice(0)
{
	// TODO: add one-time construction code here

	session = new Session();
	xml = new XMLfacade();
	pointList = new list<PACKET>;
	drawList = new list<point>;

	//session->setId(getIncrementedFormatedId(xml->getLastUserId()));
	
}

CwacomGUI2Doc::~CwacomGUI2Doc()
{
	if (pointList->size()>0) saveExercice(); 
	
	delete session;
	delete xml;
	delete pointList;
	delete drawList;
}

BOOL CwacomGUI2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CwacomGUI2Doc serialization

void CwacomGUI2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

   // SDI document implementation file
 CwacomGUI2Doc * CwacomGUI2Doc::GetDoc()
{
    CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
    return (CwacomGUI2Doc *) pFrame->GetActiveDocument();
}


#ifdef SHARED_HANDLERS

// Support for thumbnails
void CwacomGUI2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CwacomGUI2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CwacomGUI2Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CwacomGUI2Doc diagnostics

#ifdef _DEBUG
void CwacomGUI2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CwacomGUI2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CwacomGUI2Doc commands


void CwacomGUI2Doc::setUserId(CString user)
{
	session->setId(user.GetString());
}


void CwacomGUI2Doc::loadUserById(string id)
{
	string * user, idFormated;
	CString idFormatted;

	idFormated = getFormatedId(id);
	if ( idFormated.compare("")!=0) {
		user = xml->getUserDataByNumId(idFormated);
		session->fill(user);
		delete [] user;
	}
	else {
		session->clear(); //clearButId(id)
	}
}

void CwacomGUI2Doc::loadUserByHistory(string id)
{
	string * user, idFormated;
	CString idFormatted;

	idFormated = getFormatedId(id);
	if ( idFormated.compare("")!=0) {
		user = xml->getUserDataByHistory(idFormated);
		session->fill(user);
		delete [] user;
	}
	else {
		session->clear();
	}
}


string CwacomGUI2Doc::getFormatedId(string id)
{
	return getFormatedIdAux(id,false);
}

string CwacomGUI2Doc::getIncrementedFormatedId(string id)
{
	return getFormatedIdAux(id,true);
}


string CwacomGUI2Doc::getFormatedIdAux(string id, bool increment)
{
	CString idFormatted;
	int number;
	string res;
	istringstream is;
	int ct;

	is=istringstream(id);

	if ( (is >> number) ) {
		ct=is.tellg();
		if (ct == id.size()) { //all the string is parsed 123a
			if (increment) number++;
			idFormatted.Format("%05d", number);	
			res=idFormatted.GetString();
		}
		else res="";
	}
	else res="";

	return res;
}


string * CwacomGUI2Doc::getSessionText(void)
{
	string * tmp;
	tmp = session->getText();

	return tmp;
}


void CwacomGUI2Doc::login(string * userData)
{
	userData[0]=getFormatedId(userData[0]);
	userData[1]=getFormatedId(userData[1]);
	if (session->getSession().compare("") != 0) {
		userData[7]=getIncrementedFormatedId(session->getSession());
	}
	else userData[7]="00001";

	session->fill(userData);

	exercice =1;

	if (xml->existsUser(userData[0])) xml->updateUser(userData);
	else xml->addUser(userData);
	
	CwacomGUI2App *  app = (CwacomGUI2App * )::AfxGetApp();
	app->setState(LOGGED);
	xml->writeXMLfile(); 
}





string CwacomGUI2Doc::getNextUserId(void)
{
	return getIncrementedFormatedId(xml->getLastUserId());
}


bool CwacomGUI2Doc::currentUserExists(void)
{
	return session->getId().compare("") != 0;
}


void CwacomGUI2Doc::addPacket(PACKET pkt)
{
	pointList->push_back(pkt);
}


void CwacomGUI2Doc::clearPackets(void)
{
	pointList->clear();
}


list<PACKET> * CwacomGUI2Doc::getPackets(void)
{
	return pointList;
}


list<point> * CwacomGUI2Doc::getDrawingPoints(void)
{
	return drawList;
}

void CwacomGUI2Doc::clearDrawingPoints(void)
{
	drawList->clear();
}


void CwacomGUI2Doc::saveExercice(void)
{
	CwacomGUI2App *  app = (CwacomGUI2App * )::AfxGetApp();
	app->setState(LOGGED);

	drawList->clear();
	clearLastOnAirPoints();
	saveFile();
	pointList->clear();
	exercice ++;
}


void CwacomGUI2Doc::saveFile(void)
{
	CString fileName;
	string fn;

	fileName.Format(_T("data/user%s"), session->getId().c_str());
	fn = fileName.GetString();
	mkdir(fn.c_str());

	fileName.Format(_T("data/user%s/session%s"), session->getId().c_str(),session->getSession().c_str());
	fn = fileName.GetString();
	mkdir(fn.c_str());

	fileName.Format(_T("data/user%s/session%s/u%ss%s_hw%05d.svc"), session->getId().c_str(),session->getSession().c_str(),session->getId().c_str(),session->getSession().c_str(),exercice);
	ofstream myfile (fileName);
	if (myfile.is_open())
	{
		myfile << pointList->size() << endl;
		
		list<PACKET>::iterator i = pointList->begin();
		while( i != pointList->end()) {
			myfile << i->pkX << " " << i->pkY << " " << i->pkTime << " " << i->pkButtons << " " 
				<< i->pkOrientation.orAzimuth << " " << i->pkOrientation.orAltitude << " " << i->pkNormalPressure << " " << endl;
			i++;
		}

		myfile.close();
	}
	else cout << "Unable to open file";
}


void CwacomGUI2Doc::clearLastOnAirPoints(void)
{
	list<PACKET>::reverse_iterator ini;
	list<PACKET>::iterator end;
	ini = pointList->rbegin();
	end = pointList->end();
	while( ini != pointList->rend() && ini->pkButtons==0) {
		ini++;
	}

	pointList->erase(ini.base(),end);
}


void CwacomGUI2Doc::deleteExercice(void)
{
	CwacomGUI2App *  app = (CwacomGUI2App * )::AfxGetApp();
	app->setState(LOGGED);

	drawList->clear();
	pointList->clear();
}


CString * CwacomGUI2Doc::getSessionLabel(void)
{
	CString * s = new CString[3];
	
	s[0].Format(_T("User:       %s"),session->getId().c_str());
	s[1].Format(_T("Session:  %s"),session->getSession().c_str());
	s[2].Format(_T("Exercice: %d"),exercice);
	
	return s;
}
