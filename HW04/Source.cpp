#include <windows.h>
#include <vector>
#include <tchar.h>
#include "resource.h"
using namespace std;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

void SwapNumbers(int* num1, int* num2)
{
	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

vector<HWND> hStatics;
TCHAR szCoordinates[128];
HINSTANCE hInst;

int DownX, DownY;
int UpX, UpY;
int width, height;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	hInst = hInstance;
	// создаём главное окно приложения на основе модального диалога
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0); // закрываем модальный диалог
		return TRUE;


	case WM_LBUTTONDOWN:
		DownX = LOWORD(lParam);
		DownY = HIWORD(lParam);
		return TRUE;

	case WM_LBUTTONUP:
		UpX = LOWORD(lParam);
		UpY = HIWORD(lParam);
		

		if (UpX > DownX)
			width = UpX - DownX;
		else
		{
			width = DownX - UpX;
			SwapNumbers(&UpX, &DownX);
		}

		if (UpY > DownY)
			height = UpY - DownY;
		else
		{
			height = DownY - UpY;
			SwapNumbers(&UpY, &DownY);
		}

		if (width > 10 && height > 10)
		{
			hStatics.push_back(CreateWindowEx(0, TEXT("STATIC"), 0,
				WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | WS_EX_CLIENTEDGE,
				DownX, DownY, width, height, hWnd, 0, hInst, 0));
		}
		return TRUE;
	}
	return FALSE;
}