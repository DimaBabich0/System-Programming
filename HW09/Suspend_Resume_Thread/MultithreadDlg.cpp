#include "MultithreadDlg.h"

const int SIZE_STR = 32;

int GetRandNum(int min, int max)
{
	return rand() % ((max + 1) - min) + min;
}

CMultithreadDlg* CMultithreadDlg::ptr = NULL;
CMultithreadDlg::CMultithreadDlg(void)
{
	ptr = this;
}
CMultithreadDlg::~CMultithreadDlg(void)
{

}
void CMultithreadDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

DWORD WINAPI Thread1(LPVOID lp)
{
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	HWND hStatic = (HWND)lp;
	while(TRUE)
	{
		bool isNum = TRUE;
		int n = GetRandNum(1, 20000);
		for (int i = 2; i <= sqrt(n); i++) {
			if (n % i == 0) {
				isNum = FALSE;
			}
		}
		if (isNum)
		{
			wchar_t buffer[SIZE_STR];
			wsprintfW(buffer, L"%d", n);
			SetWindowText(hStatic, buffer);
		}
		Sleep(250);
	}
	return 0;
}

DWORD WINAPI Thread2(LPVOID lp)
{
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
	HWND hStatic = (HWND)lp;
	while (TRUE)
	{
		int n = GetRandNum(1, 20000);
		if (n % 2 == 0)
		{
			wchar_t buffer[SIZE_STR];
			wsprintfW(buffer, L"%d", n);
			SetWindowText(hStatic, buffer);
		}
		Sleep(250);
	}
	return 0;
}

DWORD WINAPI Thread3(LPVOID lp)
{
	SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_IDLE);
	HWND hStatic = (HWND)lp;
	while (TRUE)
	{
		int n = GetRandNum(0, 20000);
		wchar_t buffer[SIZE_STR];
		wsprintfW(buffer, L"%d", n);
		SetWindowText(hStatic, buffer);
		Sleep(250);
	}
	return 0;
}

BOOL CMultithreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	srand(time(0));

	hPlay1 = GetDlgItem(hwnd, IDC_PLAY1);
	hPlay2 = GetDlgItem(hwnd, IDC_PLAY2);
	hPlay3 = GetDlgItem(hwnd, IDC_PLAY3);

	hStatic1 = GetDlgItem(hwnd, IDC_STATICINFO1);
	hStatic2 = GetDlgItem(hwnd, IDC_STATICINFO2);
	hStatic3 = GetDlgItem(hwnd, IDC_STATICINFO3);

	Th1 = CreateThread(NULL, 0, Thread1, hStatic1, CREATE_SUSPENDED, NULL);
    Th2 = CreateThread(NULL, 0, Thread2, hStatic2, 0, NULL);
	Th3 = CreateThread(NULL, 0, Thread3, hStatic3, 0, NULL);
	return TRUE;
}

void CMultithreadDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if(id == IDC_PLAY1)
	{
		static BOOL flag = FALSE;
		if(flag)
		{
			SuspendThread(Th1);
			SetWindowText(hPlay1, TEXT("Пуск"));
		}
		else
		{
			ResumeThread(Th1);
			SetWindowText(hPlay1, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if(id == IDC_PLAY2)
	{
		static BOOL flag = TRUE;
		if(flag)
		{
			SuspendThread(Th2);
			SetWindowText(hPlay2, TEXT("Пуск"));
		}
		else
		{
			ResumeThread(Th2);
			SetWindowText(hPlay2, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if(id == IDC_PLAY3)
	{
		static BOOL flag = TRUE;
		if(flag)
		{
			SuspendThread(Th3);
			SetWindowText(hPlay3, TEXT("Пуск"));
		}
		else
		{
			ResumeThread(Th3);
			SetWindowText(hPlay3, TEXT("Пауза"));
		}
		flag = !flag;
	}
	else if(id == IDC_STOP1)
	{
		TerminateThread(Th1, 0);
		CloseHandle(Th1);
		EnableWindow(hPlay1, FALSE);
	}
	else if(id == IDC_STOP2)
	{
		TerminateThread(Th2, 0);
		CloseHandle(Th2);
		EnableWindow(hPlay2, FALSE);
	}
	else if(id == IDC_STOP3)
	{
		TerminateThread(Th3, 0);
		CloseHandle(Th3);
		EnableWindow(hPlay3, FALSE);
	}
}

BOOL CALLBACK CMultithreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}