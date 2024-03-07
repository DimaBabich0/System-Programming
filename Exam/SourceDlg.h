#pragma once
#include "header.h"

class cSourceDlg
{
public:
	static cSourceDlg* ptr;
	
	cSourceDlg(void);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	
	void CopyBannedWordFromFile();

	void MessageError(const WCHAR* text);
	void MessageInfo(const WCHAR* text);

	void CheckRunCopyApplication();
	void SetDlgVariable();

	void AddFileToList();
	void DeleteFileFromList();

	void AddBannedWordToList();
	void DeleteBannedWordFromList();
	void ChangeBannedWordFromList();
	void RedrawBannedWordList();

	void DragAndDropFile(HDROP hDrop);
	
	void CheckAndAddFile(WCHAR* szPathFile);

	void StartReplaceBannedWords();

	void ResetProgressBar();

	void ShowStats();

	HWND hDialog;
	HWND hListFiles, hButLoadFile, hButDelFile;
	HWND hListBanWords, hEditUserBanWord, hButAddBanWord, hButChangeBanWord, hButDelBanWord;
	HWND hProgressBar, hButProgressStart, hButProgressPause, hButProgressStop;
	
	BOOL bThreadWorks;

	HANDLE hThread;

	vector <wstring> aBannedWords;
	vector <int> aBannedWordsStats;
	vector <wstring> aFilesName;
};
