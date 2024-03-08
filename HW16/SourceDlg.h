#pragma once
#include "header.h"

class CSourceDlg
{
public:
	static CSourceDlg* ptr;
	static const int TEXT_SIZE = 128;

	CSourceDlg(void);
	~CSourceDlg(void);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	void Cls_OnClose(HWND hwnd);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnSize(HWND hwnd, UINT state, int cx, int cy);
	void OnTrayIcon(WPARAM wp, LPARAM lp);

	HWND hDialog;
	HICON hIcon;
	PNOTIFYICONDATA pNID;
};
