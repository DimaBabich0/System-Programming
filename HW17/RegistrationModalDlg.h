#pragma once
#include "header.h"

class CRegistrationModalDlg
{
public:
	CRegistrationModalDlg(void);
	~CRegistrationModalDlg(void);

	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static CRegistrationModalDlg* ptr;

	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);

	HWND hEditLogin, hEditPassword, hEditFullName, hEditAge;
};
