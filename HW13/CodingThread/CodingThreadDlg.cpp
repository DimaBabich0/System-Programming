#include "CodingThreadDlg.h"

HANDLE hMutex;

CodingThreadDlg* CodingThreadDlg::ptr = NULL;
CodingThreadDlg::CodingThreadDlg(void)
{
	ptr = this;
}
CodingThreadDlg::~CodingThreadDlg(void)
{
	delete ptr;
}
void CodingThreadDlg::Cls_OnClose(HWND hwnd)
{
	ReleaseMutex(hMutex);
	EndDialog(hwnd, 0);
}

DWORD WINAPI Сoding_Thread(LPVOID lp)
{
	CodingThreadDlg* ptr = (CodingThreadDlg*)lp;

	ifstream in(TEXT("coding.txt"), ios::binary | ios::in);
	if (!in)
	{
		MessageBox(ptr->hDialog, TEXT("Ошибка открытия файла!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
		return 1;
	}

	hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{19589593-A9BB-40BF-A2DC-E635198F78DD}"));
	DWORD dwAnswer = WaitForSingleObject(hMutex, INFINITE);

	char buf[64];
	while (!in.eof())
	{
		in.getline(buf, 64);
		WCHAR wbuf[100];
		MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, 100);
		SendMessage(ptr->hEdit, EM_REPLACESEL, TRUE, (LPARAM)wbuf);
		SendMessage(ptr->hEdit, EM_REPLACESEL, TRUE, (LPARAM)TEXT("\n"));
	}
	in.close();

	ReleaseMutex(hMutex);

	MessageBox(ptr->hDialog, TEXT("Чтение файла завершено"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
	return 0;
}

BOOL CodingThreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	hButton = GetDlgItem(hwnd, IDC_BUTTON1);
	hEdit = GetDlgItem(hwnd, IDC_EDIT1);
	CreateThread(NULL, 0, Сoding_Thread, this, 0, NULL);
	return TRUE;
}

void CodingThreadDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUTTON1)
	{
		Cls_OnClose(hwnd);
	}
}

INT_PTR CALLBACK CodingThreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}