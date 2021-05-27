
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "wacomGUI2.h"

#include "MainFrm.h"
#include "LoginDlg.h"
#include "wacomGUI2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern UIcontroller * uiController;

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
    ON_COMMAND(ID_BUTTONCASE,OnToolBarButtonCase)
    ON_COMMAND(ID_BUTTONSAVE,OnToolBarButtonSave)
    ON_COMMAND(ID_BUTTONDELETE,OnToolBarButtonDelete)
	ON_UPDATE_COMMAND_UI(ID_BUTTONDELETE, &CMainFrame::OnUpdateButtondelete)
	ON_UPDATE_COMMAND_UI(ID_BUTTONSAVE, &CMainFrame::OnUpdateButtonsave)
	ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
	ON_COMMAND(ID_Menu_Save, &CMainFrame::OnMenuSave)
	ON_COMMAND(ID_Menu_Delete, &CMainFrame::OnMenuDelete)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION //| FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers
void CMainFrame::OnToolBarButtonSave()
   {
       CwacomGUI2View * view = CwacomGUI2View::GetView();
	   view->saveExercice();
   }

void CMainFrame::OnToolBarButtonCase()
   {
	   	LoginDlg loginDlg;
		loginDlg.DoModal();
	//	int result=loginDlg.DoModal();
	//	if (result==IDOK)  MessageBox("OK pressed");
	//	if (result==IDCANCEL)  MessageBox("CANCEL pressed");
  }

void CMainFrame::OnToolBarButtonDelete()
   {
       CwacomGUI2View * view = CwacomGUI2View::GetView();
	   view->deleteExercice();
   }


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
   {
       if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
           return -1;

       if (!myBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
          | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
      {
            MessageBox("Failed to create toolbar\n");
            return -1; // fail to create
      }

     myBar.LoadToolBar(IDR_TOOLBAR1);
     myBar.EnableDocking(CBRS_ALIGN_ANY);
     EnableDocking(CBRS_ALIGN_ANY);
     DockControlBar(&myBar);
	 return 0;
   }


   void CMainFrame::OnUpdateButtondelete(CCmdUI *pCmdUI)
   {
	   // TODO: Add your command update UI handler code here
	   pCmdUI->Enable (uiController->decideDeleteEnabled());
   }


   void CMainFrame::OnUpdateButtonsave(CCmdUI *pCmdUI)
   {
	   // TODO: Add your command update UI handler code here
	   //the same as delete button
	   pCmdUI->Enable (uiController->decideDeleteEnabled());
   }


   void CMainFrame::OnFileNew()
   {
	   // TODO: Add your command handler code here
  	   	OnToolBarButtonCase();
 }


   void CMainFrame::OnMenuSave()
   {
	   // TODO: Add your command handler code here
	   OnToolBarButtonSave();
   }


   void CMainFrame::OnMenuDelete()
   {
	   // TODO: Add your command handler code here
	   OnToolBarButtonDelete();
   }
