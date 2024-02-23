#include "MainModalDlg.h"

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	CMainModalDlg dlg;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, CMainModalDlg::DlgProc);
}