#include "wintab.h"
#include "Utils.h"
#include <sstream>
using namespace std;

#define PACKETDATA	PK_X | PK_Y | PK_Z | PK_BUTTONS| PK_NORMAL_PRESSURE | PK_TIME | PK_ORIENTATION
#define PACKETMODE	0
#include "pktdef.h"			// NOTE: get from wactab header package


#pragma once
class WacomFacade
{
public:
	WacomFacade(void);
	~WacomFacade(void);
	bool initWintab(void);
	void showProperties(void);
	string getAxisInformation(UINT ax, char * c);
	void unloadWintab(void);
	string getRateInformation(void);
	HCTX getTabletContext(HWND handleWindow);
	HCTX context;
	LOGCONTEXT lcMine;
};

