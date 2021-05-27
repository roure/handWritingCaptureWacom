
// wacomGUI2.h : main header file for the wacomGUI2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

enum StateEnum {NOLOGGED, LOGGED, INEXERCICE};

// CwacomGUI2App:
// See wacomGUI2.cpp for the implementation of this class
//

class CwacomGUI2App : public CWinApp
{
public:
	CwacomGUI2App();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int	ExitInstance();
	StateEnum getState(void);
	void CwacomGUI2App::setState(StateEnum se);

// Implementation
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
private:
	StateEnum stateEnum;
};

extern CwacomGUI2App theApp;
