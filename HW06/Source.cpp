#include <windows.h>
#include <vector>
#include <tchar.h>
#include "resource.h"
using namespace std;

struct ButtonStruct
{
	HWND hwnd;
	int define;
	bool isCrossORNought[2] = {FALSE, FALSE};
};

HINSTANCE hInstance;
HWND hStaticInfo, hButtonCross, hButtonNought, hButtonRestart;
TCHAR buf[32];
HBITMAP hBitmapCross, hBitmapNull;
vector<ButtonStruct> hButtons;
bool isGameStart = FALSE;
bool isCross = FALSE;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

void RestartGame(HWND hwnd)
{
	hButtons.clear();
	for (size_t i = 0; i < 9; i++)
	{
		HWND hButton = GetDlgItem(hwnd, IDC_BUTTON1 + i);
		int num = IDC_BUTTON1 + i;
		hButtons.push_back({ hButton, num });
		SendMessage(hButtons[i].hwnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
	}

	EnableWindow(hButtonCross, TRUE);
	EnableWindow(hButtonNought, TRUE);
	EnableWindow(hButtonRestart, FALSE);

	wsprintf(buf, TEXT("Перед началом выберите фигуру"));
	SetWindowText(hStaticInfo, buf);
	isGameStart = FALSE;
}

void isWin(HWND hwnd, int num)
{
	wsprintf(buf, TEXT("%s выиграли"), (num == 0 ? TEXT("Крестик") : TEXT("Нолик")));
	MessageBox(hwnd, buf, TEXT("Победа"), MB_OK | MB_ICONWARNING);
	EnableWindow(hButtonCross, TRUE);
	EnableWindow(hButtonNought, TRUE);
	EnableWindow(hButtonRestart, FALSE);
	RestartGame(hwnd);
	isGameStart = FALSE;
}

void CheckWin(HWND hwnd)
{
	for (size_t i = 0; i < 2; i++)
	{
		if (hButtons[0].isCrossORNought[i] == TRUE &&
			hButtons[1].isCrossORNought[i] == TRUE &&
			hButtons[2].isCrossORNought[i] == TRUE)
			isWin(hwnd, i);
		else if (hButtons[3].isCrossORNought[i] == TRUE &&
			hButtons[4].isCrossORNought[i] == TRUE &&
			hButtons[5].isCrossORNought[i] == TRUE)
			isWin(hwnd, i);
		else if (hButtons[6].isCrossORNought[i] == TRUE &&
			hButtons[7].isCrossORNought[i] == TRUE &&
			hButtons[8].isCrossORNought[i] == TRUE)
			isWin(hwnd, i);
		else if (hButtons[0].isCrossORNought[i] == TRUE &&
			hButtons[3].isCrossORNought[i] == TRUE &&
			hButtons[6].isCrossORNought[i] == TRUE)
			isWin(hwnd, i);
		else if (hButtons[1].isCrossORNought[i] == TRUE &&
			hButtons[4].isCrossORNought[i] == TRUE &&
			hButtons[7].isCrossORNought[i] == TRUE)
			isWin(hwnd, i);
		else if (hButtons[2].isCrossORNought[i] == TRUE &&
			hButtons[5].isCrossORNought[i] == TRUE &&
			hButtons[8].isCrossORNought[i] == TRUE)
			isWin(hwnd, i);
		else if (hButtons[0].isCrossORNought[i] == TRUE &&
			hButtons[4].isCrossORNought[i] == TRUE &&
			hButtons[8].isCrossORNought[i] == TRUE)
			isWin(hwnd, i);
		else if (hButtons[2].isCrossORNought[i] == TRUE &&
			hButtons[4].isCrossORNought[i] == TRUE &&
			hButtons[6].isCrossORNought[i] == TRUE)
			isWin(hwnd, i);
	}
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);
		hButtonCross = GetDlgItem(hwnd, IDC_CHOICECROSS);
		hButtonNought = GetDlgItem(hwnd, IDC_CHOICENOUGHT);
		hButtonRestart = GetDlgItem(hwnd, IDC_RESTARTGAME);

		hBitmapCross = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		hBitmapNull = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));

		RestartGame(hwnd);
		break;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_CHOICECROSS && HIWORD(wParam) == BN_CLICKED)
		{
			EnableWindow(hButtonCross, FALSE);
			EnableWindow(hButtonNought, FALSE);
			EnableWindow(hButtonRestart, TRUE);
			isGameStart = TRUE;
			isCross = TRUE;
		}
		else if (LOWORD(wParam) == IDC_CHOICENOUGHT && HIWORD(wParam) == BN_CLICKED)
		{
			EnableWindow(hButtonCross, FALSE);
			EnableWindow(hButtonNought, FALSE);
			EnableWindow(hButtonRestart, TRUE);
			isCross = FALSE;
			isGameStart = TRUE;
		}

		if (LOWORD(wParam) == IDC_RESTARTGAME && HIWORD(wParam) == BN_CLICKED)
		{
			RestartGame(hwnd);
		}

		if (isGameStart)
		{
			for (size_t i = 0; i < hButtons.size(); i++)
			{
				if (LOWORD(wParam) == hButtons[i].define && HIWORD(wParam) == BN_CLICKED && 
					(hButtons[i].isCrossORNought[0] == FALSE && hButtons[i].isCrossORNought[1] == FALSE))
				{
					if (isCross)
					{
						SendMessage(hButtons[i].hwnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapCross);
						isCross = FALSE;
						hButtons[i].isCrossORNought[0] = TRUE;
						CheckWin(hwnd);
					}
					else if (!isCross)
					{
						SendMessage(hButtons[i].hwnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmapNull);
						isCross = TRUE;
						hButtons[i].isCrossORNought[1] = TRUE;
						CheckWin(hwnd);
					}
				}
				if (isGameStart)
				{
					wsprintf(buf, TEXT("ХОД: %s"), (isCross == FALSE ? TEXT("Нолик") : TEXT("Крестик")));
					SetWindowText(hStaticInfo, buf);
				}
			}
		}

		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return TRUE;
	}

	return FALSE;
}