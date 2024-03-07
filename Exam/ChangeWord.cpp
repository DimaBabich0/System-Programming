#include "ChangeWord.h"
#include "SourceDlg.h"

cChangeWord* cChangeWord::ptr = NULL;
cChangeWord::cChangeWord(void)
{
	ptr = this;
}
cChangeWord::~cChangeWord(void)
{

}

void cChangeWord::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, IDCANCEL);
}

BOOL cChangeWord::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	hEditChangeWord = GetDlgItem(hDialog, IDC_EDIT_CHANGE_WORD);
	SetWindowText(hEditChangeWord, szText);
	return TRUE;
}

void cChangeWord::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUT_ACCEPT)
	{
		GetWindowText(hEditChangeWord, szText, sizeof(szText));
		if (lstrlen(szText))
		{
			EndDialog(hwnd, IDOK);
		}
		else
			MessageBox(hwnd, TEXT("Empty changed word field"), TEXT("Error"), MB_OK | MB_ICONERROR);
	}
	else if (id == IDC_BUT_CANCEL)
	{
		EndDialog(hwnd, IDCANCEL);
	}
}

INT_PTR CALLBACK cChangeWord::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}
