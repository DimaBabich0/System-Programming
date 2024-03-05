#include "ParentProcessDlg.h"

ParentProcessDlg* ParentProcessDlg::ptr = NULL;
ParentProcessDlg::ParentProcessDlg(void)
{
	ptr = this;
}
ParentProcessDlg::~ParentProcessDlg(void)
{
	
}
void ParentProcessDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

void MessageAboutError(DWORD dwError)
{
	LPVOID lpMsgBuf = NULL;
	TCHAR szBuf[300]; 
	BOOL fOK = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	if(lpMsgBuf != NULL)
	{
		wsprintf(szBuf, TEXT("Ошибка %d: %s"), dwError, lpMsgBuf); 
		MessageBox(NULL, szBuf, TEXT("Сообщение об ошибке"), MB_OK | MB_ICONSTOP); 
		LocalFree(lpMsgBuf);
	}
}

void ParentProcessDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (IDC_BUTTON1 == id)
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
		TCHAR buf[128];
		GetWindowText(hEdit, buf, 32);
		int dAmountCopys = _wtoi(buf);
		if (dAmountCopys <= 0)
		{
			MessageBox(hwnd, TEXT("Число копий меньше или равно нулю"), TEXT("Ошибка"), MB_OK | MB_ICONWARNING);
			return;
		}
		else if (dAmountCopys > 10)
		{
			MessageBox(hwnd, TEXT("Число копий больше 10. Поменяйте число на меньшее"), TEXT("Ошибка"), MB_OK | MB_ICONWARNING);
			return;
		}

		vector <wstring> aFiles;
		CreateSemaphore(NULL, 1, dAmountCopys, TEXT("{2525FD5F-12E6-47c0-838A-7C5CA1EBD169}"));
		STARTUPINFO st = {sizeof(st)};
		PROCESS_INFORMATION pr;
		TCHAR filename[20];
		wsprintf(filename, TEXT("%s"), TEXT("CodingThread.exe"));
		if (!CreateProcess(NULL, filename, NULL, NULL, 0, 0, NULL, NULL, &st, &pr))
		{
			MessageAboutError(GetLastError());
			return;
		}
		CloseHandle(pr.hThread);
		CloseHandle(pr.hProcess);

		for(int i = 1; i <= dAmountCopys; i++)
		{
			TCHAR buf[30];

			ZeroMemory(&st, sizeof(st));
			st.cb = sizeof(st);

			wsprintf(buf, TEXT("copymusic_%d.txt"), i);
			aFiles.push_back(buf);

			wsprintf(buf, TEXT("DecodingThread.exe %d"), i);
			if (!CreateProcess(NULL, buf, NULL, NULL, 0, 0, NULL, NULL, &st, &pr))
			{
				MessageAboutError(GetLastError());
			}
			CloseHandle(pr.hThread);
			CloseHandle(pr.hProcess);	
		}

		Sleep(1000);

		int dTotalCharacters = 0;
		int dTotalSize = 0;
		for (size_t i = 0; i < aFiles.size(); i++)
		{
			string line;
			ifstream inSize(aFiles[i], ifstream::ate | ifstream::binary);
			dTotalSize += inSize.tellg();

			ifstream inRead(aFiles[i]);
			while (getline(inRead, line))
			{
				dTotalCharacters += line.size();
			}
			inSize.close();
			inRead.close();
		}

		swprintf(buf, TEXT("Количество копий: %d\nОбщее количество символов во всех файлах - %d символа\nОбщий размер всех копий - %.2f мегабайт\n"),
			dAmountCopys, dTotalCharacters, (float)(dTotalSize / 1024.f)/1024.f);
		MessageBox(hwnd, buf, TEXT("Статистика"), MB_OK | MB_ICONINFORMATION);
	}
}

BOOL CALLBACK ParentProcessDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}