#include "MainModalDlg.h"
#include "RegistrationModalDlg.h"

CMainModalDlg* CMainModalDlg::ptr = NULL;
CMainModalDlg::CMainModalDlg(void)
{
	ptr = this;
}
CMainModalDlg::~CMainModalDlg(void)
{

}

void CMainModalDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, IDCANCEL);
}

BOOL CMainModalDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	return TRUE;
}

void CMainModalDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUT_REGISTRATION)
	{
		DialogBox(GetModuleHandle(NULL), 
			MAKEINTRESOURCE(IDD_DIALOG_REGISTRATION), 
			hwnd, 
			CRegistrationModalDlg::DlgProc
		);
	}
	else if (id == IDC_BUT_AUTHORIZATION)
	{
		MessageBox(hwnd, TEXT("Window \"Login\" doesn't work"), TEXT("Error"), MB_OK | MB_ICONERROR);
	}
}

INT_PTR CALLBACK CMainModalDlg::DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}
