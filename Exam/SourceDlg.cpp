#include "SourceDlg.h"
#include "ChangeWord.h"

CRITICAL_SECTION cs;
const WCHAR szPathTxtBannedWords[] = TEXT("BannedWords.txt");
const WCHAR szStartFolder[] = TEXT("C:");
const WCHAR szCensureReplace[] = TEXT("***");

DWORD WINAPI Thread(LPVOID lp)
{
	cSourceDlg* dlg = (cSourceDlg*)lp;

	//get exe path
	WCHAR path[STR_SIZE];
	GetModuleFileNameW(NULL, path, STR_SIZE);
	//make path to "result" folder
	wstring szPath = path;
	int dLastSplashPath = szPath.find_last_of(TEXT("\\"));

	for (int i = 0; i < dlg->aFilesName.size(); ++i)
	{
		//variable for words
		wstring szLine;
		char buf[STR_SIZE];
		WCHAR wbuf[STR_SIZE];

		//get file name
		int dLastSlashPos = dlg->aFilesName[i].find_last_of(TEXT("\\"));
		int dDotPos = dlg->aFilesName[i].find(TEXT("."));
		wstring szNameFile = dlg->aFilesName[i].substr(dLastSlashPos + 1, dDotPos - dLastSlashPos - 1);

		wstring szSaveFilePath = szPath.substr(0, dLastSplashPath + 1);
		szSaveFilePath.append(TEXT("Result\\"));
		szSaveFilePath.append(szNameFile);
		szSaveFilePath.append(TEXT(".txt"));

		//open files
		wofstream write(szSaveFilePath, ios::out);
		ifstream read(dlg->aFilesName[i], ios::in);
		if (!read)
		{
			dlg->MessageInfo(TEXT("Can't open file"));
			continue;
		}

		while (!read.eof())
		{
			for (int j = 0; j < dlg->aBannedWords.size(); ++j)
			{
				read.getline(buf, STR_SIZE);
				MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, STR_SIZE);
				szLine = wbuf;

				size_t pos = 0;
				while ((pos = szLine.find(dlg->aBannedWords[j], pos)) != wstring::npos)
				{
					dlg->aBannedWordsStats[j]++;
					szLine.replace(pos, dlg->aBannedWords[j].length(), szCensureReplace);
					pos += lstrlen(szCensureReplace);
				}
				write << szLine << endl;
			}
		}
		write.close();
		read.close();

		//Progress bar
		SendMessage(dlg->hProgressBar, PBM_STEPIT, 0, 0);
		Sleep(1000);
	}
	
	dlg->ShowStats();
	dlg->ResetProgressBar();
	EnableWindow(dlg->hButProgressStart, TRUE);
	EnableWindow(dlg->hButProgressPause, FALSE);
	EnableWindow(dlg->hButProgressStop, FALSE);
	return 0;
}

cSourceDlg* cSourceDlg::ptr = NULL;
cSourceDlg::cSourceDlg(void)
{
	InitializeCriticalSection(&cs);
	ptr = this;
}
void cSourceDlg::Cls_OnClose(HWND hwnd)
{
	EnterCriticalSection(&cs);
	wofstream in(szPathTxtBannedWords);
	if (!in)
	{
		return;
	}

	in.imbue(locale("en_US.utf-8"));
	for (int i = 0; i < aBannedWords.size(); i++)
	{
		in << aBannedWords[i] << endl;
	}
	in.close();

	LeaveCriticalSection(&cs);

	DeleteCriticalSection(&cs);
	CloseHandle(hThread);

	EndDialog(hwnd, 0);
}

BOOL cSourceDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	CheckRunCopyApplication();
	SetDlgVariable();
	ResetProgressBar();
	CopyBannedWordFromFile();
	DragAcceptFiles(hDialog, TRUE);

	return TRUE;
}
void cSourceDlg::CopyBannedWordFromFile()
{
	ifstream in(szPathTxtBannedWords);
	if (!in)
	{
		MessageError(TEXT("Can't open banned words file"));
		return;
	}

	char buf[STR_SIZE];
	WCHAR wbuf[STR_SIZE];
	while (!in.eof())
	{
		in.getline(buf, STR_SIZE);
		if (strlen(buf) != 0)
		{
			MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, STR_SIZE);
			aBannedWords.push_back(wbuf);
		}
	}
	in.close();

	RedrawBannedWordList();
}

void cSourceDlg::MessageError(const WCHAR* text)
{
	MessageBox(hDialog, text, TEXT("Error"), MB_OK | MB_ICONERROR);
}
void cSourceDlg::MessageInfo(const WCHAR* text)
{
	MessageBox(hDialog, text, TEXT("Info"), MB_OK | MB_ICONINFORMATION);
}

void cSourceDlg::CheckRunCopyApplication()
{
	TCHAR GUID[] = TEXT("{DEC04ADF-66B7-415F-81CA-904EE1EC7AA3}");
	HANDLE hMutex = CreateMutex(NULL, FALSE, GUID);
	DWORD dwAnswer = WaitForSingleObject(hMutex, 0);
	if (dwAnswer == WAIT_TIMEOUT)
	{
		MessageError(TEXT("Cannot run more than one copy of an application"));
		EndDialog(hDialog, 0);
	}
}
void cSourceDlg::SetDlgVariable()
{
	hListFiles = GetDlgItem(hDialog, IDC_LIST_FILES);
	hButLoadFile = GetDlgItem(hDialog, IDC_BUT_LOAD_FILE);
	hButDelFile = GetDlgItem(hDialog, IDC_BUT_DEL_FILE);

	hListBanWords = GetDlgItem(hDialog, IDC_LIST_BAN_WORDS);
	hEditUserBanWord = GetDlgItem(hDialog, IDC_EDIT_USER_BAN_WORD);
	hButAddBanWord = GetDlgItem(hDialog, IDC_BUT_ADD_BAN_WORD);
	hButChangeBanWord = GetDlgItem(hDialog, IDC_BUT_CHANGE_BAN_WORD);
	hButDelBanWord = GetDlgItem(hDialog, IDC_BUT_DEL_BAN_WORD);

	hProgressBar = GetDlgItem(hDialog, IDC_PROGRESS_BAR);
	hButProgressStart = GetDlgItem(hDialog, IDC_BUT_PROGRESS_START);
	hButProgressPause = GetDlgItem(hDialog, IDC_BUT_PROGRESS_PAUSE);
	hButProgressStop = GetDlgItem(hDialog, IDC_BUT_PROGRESS_STOP);

	EnableWindow(hButProgressPause, FALSE);
	EnableWindow(hButProgressStop, FALSE);
}
void cSourceDlg::ResetProgressBar()
{
	SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 200));
	SendMessage(hProgressBar, PBM_SETSTEP, 1, 0);
	SendMessage(hProgressBar, PBM_SETPOS, 0, 0);
	SendMessage(hProgressBar, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 255)));
	SendMessage(hProgressBar, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 255, 0)));
}

void cSourceDlg::AddFileToList()
{
	WCHAR szPathFile[MAX_PATH] = { 0 };
	OPENFILENAME open = { sizeof(OPENFILENAME) };
	open.hwndOwner = hDialog;
	open.lpstrFilter = TEXT("Text Files(*.txt)\0*.txt");
	open.lpstrFile = szPathFile;
	open.nMaxFile = MAX_PATH;
	open.lpstrInitialDir = szStartFolder;
	open.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST;

	if (GetOpenFileName(&open))
	{
		ifstream in(szPathFile, ios::in);
		if (!in)
		{
			MessageError(TEXT("Can't open file"));
			return;
		}
		in.close();
		
		CheckAndAddFile(szPathFile);
	}
}
void cSourceDlg::DeleteFileFromList()
{
	int dSelectedItem = (int)SendMessage(hListFiles, LB_GETCURSEL, 0, 0);
	if (dSelectedItem != LB_ERR)
	{
		SendMessage(hListFiles, LB_DELETESTRING, dSelectedItem, 0);
		aFilesName.erase(aFilesName.begin() + dSelectedItem);
		MessageInfo(TEXT("File deleted from the list"));
	}
	else
		MessageError(TEXT("File from the list isn't selected"));
}
void cSourceDlg::CheckAndAddFile(WCHAR* szPathFile)
{
	int dIndex = SendMessage(hListFiles, LB_FINDSTRINGEXACT, -1, (LPARAM)szPathFile);
	if (dIndex == LB_ERR)
	{
		wstring str = szPathFile;
		size_t pos = str.find(szPathTxtBannedWords);
		if (pos != string::npos)
		{
			MessageError(TEXT("File with banned words can't be added to the list"));
			return;
		}

		SendMessage(hListFiles, LB_ADDSTRING, 0, (LPARAM)szPathFile);
		aFilesName.push_back(szPathFile);
	}
	else
		MessageError(TEXT("This file has already been added to the list"));
}

void cSourceDlg::AddBannedWordToList()
{
	WCHAR szBuffer[STR_SIZE];
	GetWindowText(hEditUserBanWord, szBuffer, sizeof(szBuffer));
	if (lstrlen(szBuffer))
	{
		int index = SendMessage(hListBanWords, LB_FINDSTRINGEXACT, -1, (LPARAM)szBuffer);
		if (index == LB_ERR)
		{
			SendMessage(hListBanWords, LB_ADDSTRING, 0, LPARAM(szBuffer));
			SetWindowText(hEditUserBanWord, NULL);
			aBannedWords.push_back(szBuffer);
		}
		else
			MessageError(TEXT("Such a banned word is already on the list"));
	}
	else
		MessageError(TEXT("Empty banned word field"));
}
void cSourceDlg::DeleteBannedWordFromList()
{
	int dSelectedItem = (int)SendMessage(hListBanWords, LB_GETCURSEL, 0, 0);
	if (dSelectedItem != LB_ERR)
	{
		SendMessage(hListBanWords, LB_DELETESTRING, dSelectedItem, 0);
		aBannedWords.erase(aBannedWords.begin() + dSelectedItem);
	}
	else
		MessageError(TEXT("Word from the list isn't selected"));
}
void cSourceDlg::ChangeBannedWordFromList()
{
	int dSelectedItem = (int)SendMessage(hListBanWords, LB_GETCURSEL, 0, 0);
	if (dSelectedItem != LB_ERR)
	{
		cChangeWord dlg;
		wcscpy_s(dlg.szText, aBannedWords[dSelectedItem].c_str());
		WCHAR szBuf[STR_SIZE];

		const INT_PTR dpResult = DialogBox(GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDD_DIALOG_CHANGE_WORD),
			hDialog,
			cChangeWord::DlgProc
		);

		if (dpResult == IDOK)
		{
			int index = SendMessage(hListBanWords, LB_FINDSTRINGEXACT, -1, (LPARAM)dlg.szText);
			if (index == LB_ERR)
			{
				aBannedWords[dSelectedItem] = dlg.szText;
				RedrawBannedWordList();
			}
			else
				MessageError(TEXT("Such a banned word is already on the list"));
		}
		else if (dpResult == IDCANCEL)
			MessageError(TEXT("Modified word couldn't be retrieved"));
	}
	else
		MessageError(TEXT("Word from the list isn't selected"));
}
void cSourceDlg::RedrawBannedWordList()
{
	SendMessage(hListBanWords, LB_RESETCONTENT, 0, 0);
	for (int i = 0; i < aBannedWords.size(); i++)
		SendMessage(hListBanWords, LB_ADDSTRING, 0, (LPARAM)aBannedWords[i].c_str());
}
void cSourceDlg::DragAndDropFile(HDROP hDrop)
{
	//delete after presentaion szPathFile
	WCHAR fName[MAX_PATH];
	WCHAR szPathFile[STR_SIZE];
	int dAmount = DragQueryFile(hDrop, 0xffffffff, NULL, 0);
	
	for (int i = 0; i < dAmount; i++)
	{
		DragQueryFile(hDrop, i, fName, MAX_PATH);
		wcscpy_s(szPathFile, fName);

		CheckAndAddFile(szPathFile);
	}
	DragFinish(hDrop);
}

void cSourceDlg::ShowStats()
{
	wstring szStat;
	szStat.append(TEXT("Stats about banned words: (word - amount)\n"));
	for (int i = 0; i < aBannedWordsStats.size(); i++)
	{
		WCHAR buf[STR_SIZE];
		wsprintf(buf, TEXT("%s - %d\n"), aBannedWords[i].c_str(), aBannedWordsStats[i]);
		szStat.append(buf);
	}
	MessageInfo(szStat.c_str());
}

void cSourceDlg::StartReplaceBannedWords()
{
	ResetProgressBar();

	if (aFilesName.size() == 0)
	{
		MessageError(TEXT("No files added to the program"));
		return;
	}
	else if (aBannedWords.size() == 0)
	{
		MessageError(TEXT("No words for search"));
		return;
	}

	vector <int> temp(aBannedWords.size());
	aBannedWordsStats = temp;

	bThreadWorks = TRUE;

	EnableWindow(hButProgressStart, FALSE);
	EnableWindow(hButProgressPause, TRUE);
	EnableWindow(hButProgressStop, TRUE);

	cSourceDlg dlg(*this);

	SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, aFilesName.size()));
	hThread = CreateThread(NULL, 0, Thread, this, 0, NULL);
}

void cSourceDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
		case IDC_BUT_LOAD_FILE:
		{
			AddFileToList();
			break;
		}
		case IDC_BUT_DEL_FILE:
		{
			DeleteFileFromList();
			break;
		}
		case IDC_BUT_ADD_BAN_WORD:
		{
			AddBannedWordToList();
			break;
		}
		case IDC_BUT_DEL_BAN_WORD:
		{
			DeleteBannedWordFromList();
			break;
		}
		case IDC_BUT_CHANGE_BAN_WORD:
		{
			ChangeBannedWordFromList();
			break;
		}
		case IDC_BUT_PROGRESS_START:
		{
			StartReplaceBannedWords();
			break;
		}
		case IDC_BUT_PROGRESS_PAUSE:
		{
			if (bThreadWorks)
			{
				SuspendThread(hThread);
				SetWindowText(hButProgressPause, TEXT("Resume"));
			}
			else
			{
				ResumeThread(hThread);
				SetWindowText(hButProgressPause, TEXT("Pause"));
			}
			bThreadWorks = !bThreadWorks;
			break;
		}
		case IDC_BUT_PROGRESS_STOP:
		{
			TerminateThread(hThread, 0);
			EnableWindow(hButProgressStart, TRUE);
			EnableWindow(hButProgressPause, FALSE);
			EnableWindow(hButProgressStop, FALSE);

			MessageInfo(TEXT("Progress is stopped"));
			ResetProgressBar();
		}
		default:
			break;
	}
}

INT_PTR CALLBACK cSourceDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	if (message == WM_DROPFILES)
	{
		ptr->DragAndDropFile((HDROP)wParam);
	}
	return FALSE;
}
