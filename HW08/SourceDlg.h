#pragma once
#include "header.h"

struct ButtonStruct
{
	HWND hwnd;
	int define;
};

class CSourceDlg
{
public:
	static CSourceDlg* ptr;
	static const int TEXT_SIZE = 128;

	HWND hDialog, hSpin, hEdit, hStartGame, hListBox, hProgress;
	TCHAR text[TEXT_SIZE];
	int count;
	int nTimeBar;
	int nUserErrors;
	UINT_PTR pTimer;

	vector<ButtonStruct> hButtons;
	vector<int> aGameNumbers;
	vector<int> aUserNumbers;

	bool bGameStart = FALSE;

	CSourceDlg(void);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnTimer(HWND hwnd, UINT id);
	void Cls_OnClose(HWND hwnd);

	void FinishGame(HWND hwnd);
};
