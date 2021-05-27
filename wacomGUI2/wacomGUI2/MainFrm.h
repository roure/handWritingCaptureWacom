
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "UIcontroller.h"

class CMainFrame : public CFrameWnd
{
	CToolBar myBar; //MFC CToolBar based Toolbar variable
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void OnToolBarButtonSave();
    void OnToolBarButtonCase();
    void OnToolBarButtonDelete();


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnUpdateButtondelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonsave(CCmdUI *pCmdUI);
	afx_msg void OnFileNew();
	afx_msg void OnMenuSave();
	afx_msg void OnMenuDelete();
};


