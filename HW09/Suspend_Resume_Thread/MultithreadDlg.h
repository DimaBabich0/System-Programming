#pragma once
#include "header.h"

class CMultithreadDlg
{
public:
	static CMultithreadDlg* ptr;

	CMultithreadDlg(void);
	~CMultithreadDlg(void);

	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

	HWND hStatic1, hStatic2, hStatic3;
	HWND hPlay1, hPlay2, hPlay3;
	HANDLE Th1, Th2, Th3;
};
