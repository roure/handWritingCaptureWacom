#include "StdAfx.h"

#include "UIcontroller.h"

UIcontroller::UIcontroller(void)
	: app(NULL)
{
}


UIcontroller::~UIcontroller(void)
{
}


void UIcontroller::setApp(CwacomGUI2App * paramApp)
{
	app=paramApp;
}


BOOL UIcontroller::decideDeleteEnabled(void)
{
	switch (app->getState()) {
	case INEXERCICE :
		return TRUE;
		break;
	default: 
		return FALSE;
		break;
	}
}


BOOL UIcontroller::decideSaveEnabled(void)
{
	return 0;
}
