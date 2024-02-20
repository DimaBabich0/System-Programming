#pragma once
#include "header.h"

class CSourceDlg
{
public:
	static CSourceDlg* ptr;
	static const int TEXT_SIZE = 256;
	
	HANDLE hSnapShot;
	PROCESSENTRY32 pe32;
	
	vector<PROCESSENTRY32> aProcesses;

	HWND hDialog;
	HWND hButRefresh, hButTerminate, hButInfo, hButCreate;
	HWND hList;
	HWND hEdit;

	CSourceDlg(void);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);

	void RefreshSnapShot();
	void AddSnapShotDataToList();
	void ClearListBox();
};
