// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wacomGUI2.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "wacomGUI2Doc.h"
#include "wacomGUI2View.h"
#include <iostream>

// LoginDlg dialog

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(LoginDlg::IDD, pParent)
	, caseBoxContent(_T(""))
	, b_sex(0)
	, b_hand(0)
//	, listStudiesTxt(_T(""))
, historyNumContent(_T(""))
, CommentsContent(_T(""))
, doc(NULL)
, ageValue(_T(""))
{
	doc = CwacomGUI2Doc::GetDoc();

	caseBoxContent = doc->getNextUserId().c_str();

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CASE, caseBoxContent);
	DDX_Radio(pDX, IDC_RADIO_SEX, b_sex);
	DDX_Radio(pDX, IDC_RADIO_HAND, b_hand);
	DDX_Control(pDX, IDC_COMBOLST, studiesCombo);
	DDX_Text(pDX, IDC_HISTORY, historyNumContent);
	DDX_Text(pDX, IDC_EDITCOMMENTS, CommentsContent);
	DDX_Text(pDX, IDC_AGE, ageValue);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LoginDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_CASE, &LoginDlg::OnEnChangeCase)
	ON_EN_CHANGE(IDC_HISTORY, &LoginDlg::OnEnChangeHistory)
	ON_EN_KILLFOCUS(IDC_CASE, &LoginDlg::OnEnKillfocusCase)
END_MESSAGE_MAP()


// LoginDlg message handlers


void LoginDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	//if (!isInt()) MessageBox();

	doc->loadUserById(caseBoxContent.GetString());

	string * userData = getDialogData();
	CwacomGUI2View * view = CwacomGUI2View::GetView();
	view->login(userData);

	delete [] userData;

	CDialogEx::OnOK();
}


BOOL LoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	//studiesCombo.SetDroppedWidth(5);
	studiesCombo.AddString("None");
	studiesCombo.AddString("Primary");
	studiesCombo.AddString("Secondary");
	studiesCombo.AddString("University");


	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void LoginDlg::OnEnChangeCase()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);

	doc->loadUserById(caseBoxContent.GetString());
	fillDialogNoId();

	UpdateData(FALSE);
}


void LoginDlg::OnEnChangeHistory()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);

	//doc->loadUserByHistory(historyNumContent.GetString());
	//fillDialogNoHistory();

	UpdateData(FALSE);
}


void LoginDlg::fillDialog(string * session)
{
	ageValue = session[2].c_str();

	if (session[3].compare("male")==0) {
		b_sex=0;
	}
	else {
		b_sex=1;
	}

	if (session[4].compare("right")==0) {
		b_hand=0;
	}
	else {
		b_hand=1;
	}

	if (session[5].compare("")==0) {
		studiesCombo.SetCurSel(0);
	}
	else {
		studiesCombo.SelectString(0,session[5].c_str());	
	}
	CommentsContent = session[6].c_str();
	//studiesCombo
}



void LoginDlg::fillDialogNoId(void)
{
	string * session = doc->getSessionText();
	historyNumContent = session[1].c_str();
	fillDialog(session);
	delete [] session;
}


void LoginDlg::fillDialogNoHistory(void)
{
	string * session = doc->getSessionText();
	caseBoxContent = session[0].c_str();
	fillDialog(session);
	delete [] session;
}



string * LoginDlg::getDialogData(void)
{
	string * userData = new string[8]; 

	userData[0] = caseBoxContent.GetString();
	userData[1] = historyNumContent.GetString();
	userData[2] = ageValue.GetString();
	
	if (b_sex == 0) userData[3]="male";
	else userData[3]="female";
	
	if (b_hand==0) userData[4] = "right";
	else userData[4]="left";

	CString str;
	int idx = studiesCombo.GetCurSel();
	if( idx > 0 ) {
		studiesCombo.GetLBText( idx, str );
		userData[5] = str.GetString();
	}
	else userData[5] = "";

	userData[6] = CommentsContent.GetString();

	return userData;
}


void LoginDlg::OnEnKillfocusCase()
{
	// TODO: Add your control notification handler code here
		UpdateData(TRUE);

	doc->loadUserById(caseBoxContent.GetString());
	fillDialogNoId();
	if (!doc->currentUserExists()) caseBoxContent = doc->getNextUserId().c_str();

	UpdateData(FALSE);

}
