#pragma once
#include "header.h"

class CSourceDlg
{
public:
	static CSourceDlg* ptr;

	CSourceDlg(void);
	~CSourceDlg(void);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);

	HWND hDialog, hButton, hEditFileName, hEditNumCopy, hSpin;
};
