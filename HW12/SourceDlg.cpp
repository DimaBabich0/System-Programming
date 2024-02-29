#include "SourceDlg.h"

CRITICAL_SECTION cs;

const int TEXT_SIZE = 128;
vector<wstring> aFileNames;
WCHAR szFileName[TEXT_SIZE];
int dCopys;

CSourceDlg* CSourceDlg::ptr = NULL;
CSourceDlg::CSourceDlg(void)
{
	ptr = this;
	InitializeCriticalSection(&cs);
}
CSourceDlg::~CSourceDlg(void)
{
	DeleteCriticalSection(&cs);
}
void CSourceDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL CSourceDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	hDialog = hwnd;
	hEditFileName = GetDlgItem(hwnd, IDC_EDIT_FILE_NAME);
	hEditNumCopy = GetDlgItem(hwnd, IDC_EDIT_NUM_COPY);
	hButton = GetDlgItem(hwnd, IDC_BUTTON1);

	hSpin = GetDlgItem(hwnd, IDC_SPIN1);
	SendMessage(hSpin, UDM_SETRANGE32, 0, 5);
	SendMessage(hSpin, UDM_SETBUDDY, WPARAM(hEditNumCopy), 0);
	SetWindowText(hEditNumCopy, TEXT("0"));
	return TRUE;
}

DWORD WINAPI CreateFiles(LPVOID lp)
{
	EnterCriticalSection(&cs);

	wstring szCopyFileName;
	szCopyFileName.append(szFileName);
	szCopyFileName.append(TEXT(".txt"));

	ifstream readFile(szCopyFileName, ios::in);
	if (!readFile.is_open())
	{
		szFileName[0] = NULL;
		MessageBox(NULL, TEXT("Failed to open the file to read"), TEXT("Error"), MB_OK | MB_ICONERROR);
		LeaveCriticalSection(&cs);
		return 1;
	}

	char cBuf[100];
	for (size_t i = 1; i <= dCopys; i++)
	{
		wstring szCopyFileName;
		szCopyFileName.append(szFileName);
		szCopyFileName.append(TEXT("Copy"));
		szCopyFileName.append(to_wstring(i));
		szCopyFileName.append(TEXT(".txt"));

		wofstream createFile(szCopyFileName);
		aFileNames.push_back(szCopyFileName);

		readFile.clear();
		readFile.seekg(0, ios::beg);
		while (!readFile.eof())
		{
			readFile.getline(cBuf, sizeof(cBuf));
			createFile << cBuf;
		}
		createFile.close();
	}
	readFile.close();

	LeaveCriticalSection(&cs);
	MessageBox(NULL, TEXT("Copies created"), TEXT("Succes"), MB_OK | MB_ICONINFORMATION);
	return 0;
}

DWORD WINAPI ReadAndWrite(LPVOID lp)
{
	EnterCriticalSection(&cs);
	char cBuf[TEXT_SIZE];
	string strBuf;

	if (wcslen(szFileName) == 0)
	{
		MessageBox(0, TEXT("Failed to open the file to write"), TEXT("Error"), MB_OK | MB_ICONERROR);
		LeaveCriticalSection(&cs);
		return 1;
	}
	wstring szResultFileName;
	szResultFileName.append(szFileName);
	szResultFileName.append(TEXT("Result.txt"));

	wofstream writeRes(szResultFileName);

	for (size_t i = 0; i < aFileNames.size(); i++)
	{
		ifstream readCopy(aFileNames[i], ios::in);

		while (!readCopy.eof())
		{
			readCopy.getline(cBuf, sizeof(cBuf));
			writeRes << cBuf << "\n";
		}
		readCopy.close();
	}
	writeRes.close();
	LeaveCriticalSection(&cs);
	MessageBox(NULL, TEXT("File copys created"), TEXT("Succes"), MB_OK | MB_ICONINFORMATION);
	return 0;
}

void CSourceDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUTTON1)
	{
		aFileNames.clear();

		GetWindowText(hEditFileName, szFileName, TEXT_SIZE);
		if (wcslen(szFileName) == 0)
		{
			MessageBox(hwnd, TEXT("Строка с названием файла пустая"), TEXT("Error"), MB_OK | MB_ICONERROR);
			return;
		}

		dCopys = GetDlgItemInt(hwnd, IDC_EDIT_NUM_COPY, NULL, FALSE);
		if (dCopys <= 0)
		{
			MessageBox(hwnd, TEXT("Количество копий меньше или равно нулю"), TEXT("Error"), MB_OK | MB_ICONERROR);
			return;
		}

		HANDLE hThread = CreateThread(NULL, 0, CreateFiles, 0, 0, NULL);
		CloseHandle(hThread);
		hThread = CreateThread(NULL, 0, ReadAndWrite, 0, 0, NULL);
		CloseHandle(hThread);
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