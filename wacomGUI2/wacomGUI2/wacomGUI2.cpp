
// wacomGUI2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "wacomGUI2.h"
#include "MainFrm.h"

#include "wacomGUI2Doc.h"
#include "wacomGUI2View.h"

#include "wacomfacade.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

WacomFacade * tablet;
UIcontroller * uiController;

// CwacomGUI2App

BEGIN_MESSAGE_MAP(CwacomGUI2App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CwacomGUI2App::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CwacomGUI2App construction

CwacomGUI2App::CwacomGUI2App()
{
	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Handwriting Capture.Signal Processing Group, Tecnocampus Mataró Maresme.V1.0"));
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CwacomGUI2App object

CwacomGUI2App theApp;


// CwacomGUI2App initialization

BOOL CwacomGUI2App::InitInstance()
{
	CWinApp::InitInstance();

	tablet = new WacomFacade;
	tablet->initWintab();
	//tablet->showProperties();
	uiController = new UIcontroller();
	uiController->setApp(this);

	stateEnum=NOLOGGED;


	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Signal Processing Group, Tecnocampus Mataró Maresme"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CwacomGUI2Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CwacomGUI2View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	uiController->setApp(this);


	// The one and only window has been initialized, so show and update it
	m_pMainWnd->SetWindowText("Handwriting Capture");
	m_pMainWnd->ShowWindow(SW_MAXIMIZE); //(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand


	return TRUE;
}

int CwacomGUI2App::ExitInstance()
{
	tablet->unloadWintab();
	delete tablet;
	delete uiController;
	return 0;
}

StateEnum CwacomGUI2App::getState(void)
{
	return stateEnum;
}

void CwacomGUI2App::setState(StateEnum se) {
	stateEnum = se;
}

// CwacomGUI2App message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CwacomGUI2App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CwacomGUI2App message handlers




