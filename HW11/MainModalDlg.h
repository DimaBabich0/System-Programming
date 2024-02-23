#pragma once
#include "header.h"

class CMainModalDlg
{
public:
	CMainModalDlg(void);
	~CMainModalDlg(void);

	static INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static CMainModalDlg* ptr;

	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
};
