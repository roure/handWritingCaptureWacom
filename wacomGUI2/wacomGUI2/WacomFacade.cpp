#include "StdAfx.h"
#include "WacomFacade.h"


WacomFacade::WacomFacade(void)
{
}


WacomFacade::~WacomFacade(void)
{
}


bool WacomFacade::initWintab(void)
{
	char            WName[50];        /* String to hold device name */

	if ( !LoadWintab( ) )
	{
		ShowError( "Wintab not available" );
		return false;
	}

	/* check if WinTab available. */
	if (!gpWTInfoA(0, 0, NULL)) {
		ShowError("WinTab Services Not Available.");
		return false;
	}

	/* check if WACOM available. */
    gpWTInfoA(WTI_DEVICES, DVC_NAME, WName);
    if (strncmp(WName,"WACOM",5)) {
		ShowError("Wacom Tablet Not Installed.");
		return false;
    }

	return true;
}



void WacomFacade::showProperties(void)
{
	string result = "Tablet properties: \n";
	

	result += getAxisInformation(DVC_X,"X");

	result += getAxisInformation(DVC_Y,"Y");

	result += getAxisInformation(DVC_Z,"Z");

	result += getAxisInformation(DVC_NPRESSURE,"NORMAL PRESSURE");

	result += getRateInformation();

	MessageBoxA( NULL, result.c_str(), gpszProgramName, MB_OK | MB_ICONINFORMATION );

}



string WacomFacade::getAxisInformation(UINT ax, char * c)
{
	struct          tagAXIS axis; /* The capabilities of tilt */
	UINT infoLength;
	stringstream result;

	infoLength=gpWTInfoA(WTI_DEVICES,ax,&axis);
	if (infoLength) {
		result << c << "min: " << axis.axMin << endl;
		result << c << "max: " << axis.axMax << endl;
		result << c << "units: " << axis.axUnits << endl;
		result << c << "resolution: " << FIX_DOUBLE(axis.axResolution) << endl;
	}
	result << endl;

	return result.str();
}


void WacomFacade::unloadWintab(void)
{
	UnloadWintab();
}


string WacomFacade::getRateInformation(void)
{
	UINT rate;
	stringstream result;
	
	gpWTInfoA(WTI_DEVICES, DVC_PKTRATE, &rate);
	result << "Rate (Hz): " << rate << endl; 
	return result.str();
}


HCTX WacomFacade::getTabletContext(HWND handleWindow)
{
	AXIS            TabletX, TabletY, TabletZ; /* The maximum tablet size */

	/* get default region */
	gpWTInfoA(WTI_DEFCONTEXT, 0, &lcMine);

	/* modify the digitizing region */
	//wsprintf(lcMine.lcName, "TiltTest Digitizing");
	lcMine.lcOptions = CXO_MESSAGES; 
	lcMine.lcPktData = PACKETDATA;
	lcMine.lcPktMode = PACKETMODE;
	lcMine.lcMoveMask = PACKETDATA;
	lcMine.lcBtnUpMask = lcMine.lcBtnDnMask;

    /* Set the entire tablet as active */
	gpWTInfoA(WTI_DEVICES,DVC_X,&TabletX);
	gpWTInfoA(WTI_DEVICES,DVC_Y,&TabletY);
	gpWTInfoA(WTI_DEVICES,DVC_Z,&TabletZ);
	lcMine.lcInOrgX = 0;
	lcMine.lcInOrgY = 0;
	lcMine.lcInOrgZ = 0;
	lcMine.lcInExtX = TabletX.axMax;
	lcMine.lcInExtY = TabletY.axMax;
	lcMine.lcInExtZ = TabletZ.axMax;

	gpWTInfoA(WTI_DEVICES,DVC_PKTRATE,&TabletX);
	lcMine.lcPktRate = TabletX.axMax;

	/*
	//output the data in system coords?
	lcMine.lcSysOrgX = lcMine.lcSysOrgY = 0;

    // output the data in screen coords 
	lcMine.lcOutOrgX = lcMine.lcOutOrgY = 0;
	lcMine.lcOutExtX = GetSystemMetrics(SM_CXSCREEN);
    // move origin to upper left 
	lcMine.lcOutExtY = -GetSystemMetrics(SM_CYSCREEN);
	*/

	/* open the region */
	context= gpWTOpenA(handleWindow, &lcMine, TRUE);
	return context;
}
