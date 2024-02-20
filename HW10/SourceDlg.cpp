#include "SourceDlg.h"

CSourceDlg* CSourceDlg::ptr = NULL;
CSourceDlg::CSourceDlg(void)
{
	ptr = this;
}
void CSourceDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

void CSourceDlg::RefreshSnapShot()
{
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	memset(&pe32, 0, sizeof(PROCESSENTRY32));
	pe32.dwSize = sizeof(PROCESSENTRY32);
}

void CSourceDlg::AddSnapShotDataToList()
{
	RefreshSnapShot();
	if (Process32First(hSnapShot, &pe32))
	{
		ClearListBox();
		aProcesses.clear();

		int pos = SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pe32.szExeFile);
		SendMessage(hList, LB_SETITEMDATA, pos, aProcesses.size());
		aProcesses.push_back(pe32);

		while (Process32Next(hSnapShot, &pe32))
		{
			int pos = SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)pe32.szExeFile);
			SendMessage(hList, LB_SETITEMDATA, pos, aProcesses.size());
			aProcesses.push_back(pe32);
		}
	}
}

void CSourceDlg::ClearListBox()
{
	for (size_t i = 0; i < aProcesses.size(); i++)
		SendMessage(hList, LB_DELETESTRING, 0, 0);
}

//инициализация
BOOL CSourceDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hDialog = hwnd;
	hButRefresh = GetDlgItem(hDialog, IDC_BUT_REFRESH);
	hButTerminate = GetDlgItem(hDialog, IDC_BUT_TERMINATE_PROC);
	hButInfo = GetDlgItem(hDialog, IDC_BUT_INFO);
	hButCreate = GetDlgItem(hDialog, IDC_BUT_CREATE_PROC);
	hList = GetDlgItem(hDialog, IDC_LIST_PROC);
	hEdit = GetDlgItem(hDialog, IDC_EDIT_NAME_PROC);

	AddSnapShotDataToList();

	return TRUE;
}

//действия (кнопки и тд)
void CSourceDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUT_REFRESH)
	{
		AddSnapShotDataToList();
	}
	else if (id == IDC_BUT_INFO)
	{
		int lbItem = SendMessage(hList, LB_GETCURSEL, 0, 0);
		int i = SendMessage(hList, LB_GETITEMDATA, lbItem, 0);
		if (lbItem != LB_ERR)
		{
			WCHAR buf[TEXT_SIZE];
			wsprintf(buf, TEXT("ID: %u\nThreads: %u\nPriority: %u\nName: %s"),
				aProcesses[i].th32ProcessID,
				aProcesses[i].cntThreads,
				aProcesses[i].pcPriClassBase,
				aProcesses[i].szExeFile);
			MessageBox(hDialog, buf, TEXT("Info"), MB_OK | MB_ICONINFORMATION);
			AddSnapShotDataToList();
		}
		else
			MessageBox(hwnd, TEXT("Процесс не выбран"), TEXT("Просмотр информации о процессе"), MB_OK | MB_ICONSTOP);

	}
	else if (id == IDC_BUT_TERMINATE_PROC)
	{
		int lbItem = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
		int i = (int)SendMessage(hList, LB_GETITEMDATA, lbItem, 0);
		if (lbItem != LB_ERR)
		{
			HANDLE hProcess;
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, aProcesses[i].th32ProcessID);
			TerminateProcess(hProcess, 0);
			MessageBox(hwnd, TEXT("Процесс удален"), TEXT("Удаление процесса"), MB_OK | MB_ICONINFORMATION);
			AddSnapShotDataToList();
		}
		else
			MessageBox(hwnd, TEXT("Процесс не выбран"), TEXT("Удаление процесса"), MB_OK | MB_ICONSTOP);
	}
	else if (id == IDC_BUT_CREATE_PROC)
	{
		int length = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
		WCHAR buf[TEXT_SIZE];
		GetWindowText(hEdit, buf, length + 1);

		STARTUPINFO s = { sizeof(STARTUPINFO) };
		PROCESS_INFORMATION p;
		BOOL fs = CreateProcess(NULL, buf, NULL, NULL, FALSE, 0, NULL, NULL, &s, &p);
		if (fs)
		{
			CloseHandle(p.hThread);
			CloseHandle(p.hProcess);
			MessageBox(hwnd, TEXT("Процесс успешно создан"), TEXT("Создание процесса"), MB_OK | MB_ICONINFORMATION);
			AddSnapShotDataToList();
		}
		else
			MessageBox(hwnd, TEXT("Процесс не создан"), TEXT("Создание процесса"), MB_OK | MB_ICONSTOP);
	}
}

INT_PTR CALLBACK CSourceDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}