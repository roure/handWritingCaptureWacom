
// wacomGUI2View.cpp : implementation of the CwacomGUI2View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "wacomGUI2.h"
#endif

#include "wacomGUI2Doc.h"
#include "wacomGUI2View.h"

#include "WacomFacade.h"
extern WacomFacade * tablet;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CwacomGUI2View
char*	gpszProgramName = "WacomGUI Demo";

IMPLEMENT_DYNCREATE(CwacomGUI2View, CView)

BEGIN_MESSAGE_MAP(CwacomGUI2View, CView)
	ON_MESSAGE(WT_PACKET, OnWTPacket)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CwacomGUI2View construction/destruction

CwacomGUI2View::CwacomGUI2View()
	: cleared(0)
	, drawed(0)
{
	// TODO: add construction code here
	csr.x = -1;
	pWTMutex = new CMutex( TRUE, NULL, NULL );
	hCtx = 0;

	numSamples=0;
	accPressure=0;
	dispPressure=0;
	xPressure=0;

}

CwacomGUI2View::~CwacomGUI2View()
{
	delete pWTMutex;
	if( hCtx )
	{
		gpWTClose( hCtx );
	}

}

BOOL CwacomGUI2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CwacomGUI2View drawing

void CwacomGUI2View::OnDraw(CDC* pDC)
{
	CwacomGUI2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    CRect rect;
	
	csr.x = -1;

	list<point> * lst = pDoc->getDrawingPoints();
	list<point>::iterator i = lst->begin();
	while( i != lst->end() ) {
		if( i->x >= 0)
			pDC->LineTo(i->x,i->y);
		else
			pDC->MoveTo(abs(i->x),abs(i->y));
		i++;
	}

	if ((theApp.getState()==LOGGED) || (theApp.getState()==INEXERCICE)) { 
		drawSessionLabel();
		drawPressureMarginWindow();
	}
	// TODO: add draw code for native data here
}


// CwacomGUI2View diagnostics

#ifdef _DEBUG
void CwacomGUI2View::AssertValid() const
{
	CView::AssertValid();
}

void CwacomGUI2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CwacomGUI2Doc* CwacomGUI2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CwacomGUI2Doc)));
	return (CwacomGUI2Doc*)m_pDocument;
}
#endif //_DEBUG


// CwacomGUI2View message handlers

int CwacomGUI2View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Open a Wintab context
	tablet->getTabletContext(m_hWnd);

	return 0;
}

LRESULT CwacomGUI2View::OnWTPacket(WPARAM wSerial, LPARAM hCtx)
{
	// Process packets in order, one at a time
	CSingleLock lock( pWTMutex, TRUE );

	CwacomGUI2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return TRUE;

	
	// Read the packet
	gpWTPacket( (HCTX)tablet->context, wSerial, &pkt );

	if ((theApp.getState()==LOGGED) && pkt.pkButtons) theApp.setState(INEXERCICE); 

	if ((theApp.getState()==LOGGED) || (theApp.getState()==INEXERCICE)) {
		paintCursor();
		drawPressure();
	}

	if (theApp.getState()==INEXERCICE) {
		pDoc->addPacket(pkt);
		if (cleared && !drawed) 
			drawed=1;
	}

	return TRUE;
}



void CwacomGUI2View::paintCursor(void)
{

	CDC *pDC = GetDC();
	
	// Get window size
	RECT window_rect;
	GetWindowRect( &window_rect );
	POINT size;
	size.x = window_rect.right - window_rect.left;
	size.y = window_rect.bottom - window_rect.top;

	// Erase the old cursor
	if( csr.x >= 0 ) {
		CRgn r;
		r.CreateRectRgn( csr.x - 2, csr.y - 2, csr.x + 2, csr.y + 2 );
		pDC->InvertRgn( &r );
	}

	csr.x = (size.x * pkt.pkX) / tablet->lcMine.lcInExtX;
	csr.y = size.y - (size.y * pkt.pkY) / tablet->lcMine.lcInExtY;

	if( pkt.pkButtons ) {
		CwacomGUI2Doc *pDoc = GetDocument();
		list<point> * lst = pDoc->getDrawingPoints();

		if( prev_pkButtons ) {

			list<point>::iterator i = lst->end();
			i--;
			pDC->MoveTo(abs(i->x),abs(i->y));

			lst->push_back(csr);
			pDC->LineTo(csr);
		} else {
			POINT pt;
			pt.x = -csr.x;
			pt.y = -csr.y;
			lst->push_back(pt);
		}
	}

	prev_pkButtons = pkt.pkButtons;
	

	// Draw a new cursor
	CRgn r;
	r.CreateRectRgn( csr.x - 2, csr.y - 2, csr.x + 2, csr.y + 2 );
	pDC->InvertRgn( &r );

	ReleaseDC( pDC );

}

CwacomGUI2View * CwacomGUI2View::GetView()
{
      CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

      CView * pView = pFrame->GetActiveView();

      if ( !pView )
         return NULL;

      // Fail if view is of wrong kind
      // (this could occur with splitter windows, or additional
      // views on a single document
      if ( ! pView->IsKindOf( RUNTIME_CLASS(CwacomGUI2View) ) )
         return NULL;

      return (CwacomGUI2View *) pView;
}

void CwacomGUI2View::saveExercice(void)
{
	CwacomGUI2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->saveExercice();
	clearWindow();

}


void CwacomGUI2View::clearWindow(void)
{
	CDC *pDC = GetDC();

	CRect rect;
	GetClientRect(rect);
	CBrush brush;
	// repaint the window with solid black background
	brush.CreateSolidBrush(RGB(255,255,255));
	pDC->FillRect(rect,&brush);

	drawSessionLabel();
	drawPressureMarginWindow();

	// reset cursor
	csr.x=csr.y=-1;
	cleared=1;
	ReleaseDC( pDC );
}


void CwacomGUI2View::deleteExercice(void)
{
	CwacomGUI2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	clearWindow();
	pDoc->deleteExercice();
}


void CwacomGUI2View::drawSessionLabel(void)
{
    CwacomGUI2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CDC *pDC = GetDC();
	
	CRect rect;

	CString * s=pDoc->getSessionLabel();

    GetClientRect( &rect );
    pDC->SetTextAlign( TA_BASELINE | TA_LEFT );
	pDC->Rectangle(rect.left + (rect.right - rect.left)*9/10-10, rect.top + 80, rect.left + (rect.right - rect.left)*9/10+120,rect.top + 150);
	pDC->TextOut( rect.left + (rect.right - rect.left)*9/10, rect.top + 100, 
                  s[0], s[0].GetLength() );
	pDC->TextOut( rect.left + (rect.right - rect.left)*9/10, rect.top + 120, 
                  s[1], s[1].GetLength() );
	pDC->TextOut( rect.left + (rect.right - rect.left)*9/10, rect.top + 140, 
                  s[2], s[2].GetLength() );

	ReleaseDC( pDC );
	delete []s;
}


void CwacomGUI2View::drawPressure(void)
{
    CwacomGUI2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CDC *pDC = GetDC();
	
	int maxSamples=25;

	if (numSamples >=maxSamples) {
		numSamples=0;
		accPressure=0;
	}
	accPressure += pkt.pkNormalPressure;
	numSamples++;
	if (xPressure==0) xPressure=winPressurePosX;
	
	//drawPressureMarginWindow();

	CString sstr;
	sstr.Format(_T("Pressure: %i\n"), winPressurePosY + winPressureHeigth - (winPressureHeigth)*dispPressure/1023);
	if (numSamples==maxSamples) {

		pDC->MoveTo(xPressure, winPressurePosY + winPressureHeigth - (winPressureHeigth)*dispPressure/1023);
		dispPressure = accPressure/maxSamples;
		pDC->LineTo(++xPressure, winPressurePosY + winPressureHeigth - (winPressureHeigth)*dispPressure/1023);
		if (xPressure >= winPressurePosX + winPressureWidth) 
			xPressure = winPressurePosX;
		
		//erasing window
		// create and select a white pen
	    CPen penWhite;
		penWhite.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		CPen* pOldPen = pDC->SelectObject(&penWhite);
		pDC->Rectangle(xPressure==winPressurePosX? winPressurePosX : xPressure+1 ,winPressurePosY, xPressure+11<=winPressurePosX + winPressureWidth? xPressure+11 : winPressurePosX + winPressureWidth,
					   winPressurePosY+ winPressureHeigth+1);
		pDC->SelectObject(pOldPen);
	}
    
    pDC->SetTextAlign( TA_BASELINE | TA_LEFT );
	pDC->TextOut( winPressurePosX + 10, winPressurePosY - 20, 
	              sstr, sstr.GetLength() );
			

	ReleaseDC( pDC );
}


void CwacomGUI2View::login(string * userData)
{
   CwacomGUI2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->login(userData);
	drawSessionLabel();
	drawPressureMarginWindow();
}

void CwacomGUI2View::drawPressureMarginWindow(void) {
	CDC *pDC = GetDC();

	
	CRect rect;

	GetClientRect( &rect );
	winPressurePosX=rect.left + (rect.right - rect.left)*9/10-10; 
	winPressurePosY=rect.top + 200;
	winPressureWidth= 120;
	winPressureHeigth= 250;
	 
	// draw margin rectnagle
	pDC->MoveTo(winPressurePosX - 2, winPressurePosY - 2);
	pDC->LineTo(winPressurePosX + winPressureWidth + 2, winPressurePosY - 2);
	pDC->LineTo(winPressurePosX + winPressureWidth + 2, winPressurePosY+ winPressureHeigth +2);
	pDC->LineTo(winPressurePosX - 2, winPressurePosY + winPressureHeigth +2);
	pDC->LineTo(winPressurePosX - 2, winPressurePosY - 2);
}

