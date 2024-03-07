#pragma once
#include "header.h"

class cChangeWord
{
public:
	cChangeWord(void);
	~cChangeWord(void);

	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	static cChangeWord* ptr;

	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);

	HWND hDialog, hEditChangeWord;
	WCHAR szText[STR_SIZE];
};

