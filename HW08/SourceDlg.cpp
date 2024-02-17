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

void CSourceDlg::FinishGame(HWND hwnd)
{
	KillTimer(hwnd, pTimer);
	bGameStart = FALSE;
}

int getRandNum(int min, int max)
{
	return rand() % ((max + 1) - min) + min;
}

BOOL CSourceDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	srand(time(NULL));

	hDialog = hwnd;
	hSpin = GetDlgItem(hDialog, IDC_SPIN1);
	hEdit = GetDlgItem(hDialog, IDC_EDIT1);
	hListBox = GetDlgItem(hDialog, IDC_LIST1);
	hStartGame = GetDlgItem(hDialog, IDC_STARTGAME);

	hProgress = GetDlgItem(hDialog, IDC_PROGRESS1);
	SendMessage(hProgress, PBM_SETSTEP, 1, 0);
	SendMessage(hProgress, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 255)));
	SendMessage(hProgress, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 255, 0)));

	for (size_t i = 0; i < 16; i++)
	{
		HWND hButton = GetDlgItem(hwnd, IDC_BUTTON1 + i);
		int num = IDC_BUTTON1 + i;
		hButtons.push_back({ hButton, num });
		EnableWindow(hButton, FALSE);
	}

	SendMessage(hSpin, UDM_SETRANGE32, 0, 120);
	SendMessage(hSpin, UDM_SETBUDDY, WPARAM(hEdit), 0);

	SetWindowText(hEdit, TEXT("0"));
	return TRUE;
}

//действия (кнопки и тд)
void CSourceDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if(id == IDC_STARTGAME)
	{
		// Получим текущую позицию счетчика
		int nTime = SendMessage(hSpin, UDM_GETPOS32, 0, 0);
		if (nTime <= 0)
		{
			MessageBox(hwnd, TEXT("Время меньше или равно нулю"), TEXT("Ошибка"), MB_OK | MB_ICONWARNING);
			return;
		}
		nTimeBar = nTime * 4;
		SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, nTimeBar));
		SendMessage(hProgress, PBM_SETPOS, 0, 0); // Установка текущей позиции индикатора
		SetTimer(hwnd, pTimer, 250, NULL);

		for (size_t i = 0; i < aUserNumbers.size(); i++)
		{
			SendMessage(hListBox, LB_DELETESTRING, 0, 0);
		}

		aGameNumbers.clear();
		aUserNumbers.clear();

		for (size_t i = 0; i < hButtons.size(); i++)
		{
			aGameNumbers.push_back(getRandNum(1, 100));
			EnableWindow(hButtons[i].hwnd, TRUE);
			wsprintf(text, TEXT("%d"), aGameNumbers[i]);
			SetWindowText(hButtons[i].hwnd, text);
		}

		sort(aGameNumbers.begin(), aGameNumbers.end());
		bGameStart = TRUE;
		nUserErrors = 0;
	}

	if (bGameStart)
	{
		for (size_t i = 0; i < hButtons.size(); i++)
		{
			if (id == hButtons[i].define)
			{
				GetWindowText(hButtons[i].hwnd, text, TEXT_SIZE);
				int num = _tstoi(text);
				if (aGameNumbers[aUserNumbers.size()] == num)
				{
					aUserNumbers.push_back(num);
					EnableWindow(hButtons[i].hwnd, FALSE);
					SendMessage(hListBox, LB_ADDSTRING, 0, LPARAM(text));
				}
				else
					nUserErrors++;

				if (aGameNumbers.size() == aUserNumbers.size())
				{
					FinishGame(hwnd);
					int progress = SendMessage(hProgress, PBM_GETPOS, 0, 0);
					float time = ((float)nTimeBar - (float)progress) / 4.f;
					swprintf_s(text,
						TEXT("Все числа собранны\nКоличество оставшегося времени: %.2f сек.\nКоличество ошибок: %d"), 
						time, nUserErrors);
					MessageBox(hwnd, text, TEXT("Вы выиграли"), MB_OK | MB_ICONINFORMATION);
					return;
				}
			}
		}
	}
}

//работа таймера
void CSourceDlg::Cls_OnTimer(HWND hwnd, UINT id)
{
	int progress = SendMessage(hProgress, PBM_GETPOS, 0, 0);
	if (progress >= nTimeBar)
	{
		FinishGame(hwnd);
		MessageBox(hwnd, TEXT("Время вышло"), TEXT("Вы проиграли"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	SendMessage(hProgress, PBM_STEPIT, 0, 0);
}

INT_PTR CALLBACK CSourceDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_TIMER, ptr->Cls_OnTimer);
	}
	return FALSE;
}