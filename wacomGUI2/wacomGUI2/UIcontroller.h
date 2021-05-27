#pragma once

#include "wacomGUI2.h"

class UIcontroller
{
public:
	UIcontroller(void);
	~UIcontroller(void);
	void setApp(CwacomGUI2App * paramApp);
private:
	CwacomGUI2App *app;
public:
	BOOL decideDeleteEnabled(void);
	BOOL decideSaveEnabled(void);
};

