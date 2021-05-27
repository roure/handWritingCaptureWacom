#include "wintab.h"

using namespace std;

// wacomGUI2View.h : interface of the CwacomGUI2View class
//

#pragma once


class CwacomGUI2View : public CView
{
	CMutex *pWTMutex;
	HCTX hCtx;
	POINT csr;
	unsigned prev_pkButtons;
protected: // create from serialization only
	CwacomGUI2View();
	DECLARE_DYNCREATE(CwacomGUI2View)

// Attributes
public:
	CwacomGUI2Doc* GetDocument() const;

// Operations
public:
	static CwacomGUI2View * GetView();
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CwacomGUI2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	afx_msg LRESULT OnWTPacket(WPARAM, LPARAM);

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMFC_DEMOView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void paintCursor(void);
	PACKET pkt;
public:
	void saveExercice(void);
private:
	void clearWindow(void);
public:
	int cleared;
	int drawed;
	void deleteExercice(void);
private:
	void drawSessionLabel(void);
	void drawPressure(void);
	void drawPressureMarginWindow(void);
	int numSamples;
	int accPressure;
	int dispPressure;
	int xPressure;
	int winPressurePosX, winPressurePosY, winPressureWidth, winPressureHeigth; 

public:
	void login(string * userData);
};

#ifndef _DEBUG  // debug version in wacomGUI2View.cpp
inline CwacomGUI2Doc* CwacomGUI2View::GetDocument() const
   { return reinterpret_cast<CwacomGUI2Doc*>(m_pDocument); }
#endif

