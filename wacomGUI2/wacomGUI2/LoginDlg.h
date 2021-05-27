#pragma once
#include "afxwin.h"
#include "wacomGUI2Doc.h"
// LoginDlg dialog

class LoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~LoginDlg();

// Dialog Data
	enum { IDD = IDD_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString caseBoxContent;
	int b_sex;
	int b_hand;
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox studiesCombo;
	CString historyNumContent;
	CString CommentsContent;
	afx_msg void OnEnChangeCase();
	afx_msg void OnEnChangeHistory();
private:
	CwacomGUI2Doc * doc;
	void fillDialog(string * session);
	void fillDialogNoId(void);
	void fillDialogNoHistory(void);
public:
	CString ageValue;
	string * getDialogData(void);
	afx_msg void OnEnKillfocusCase();
};
