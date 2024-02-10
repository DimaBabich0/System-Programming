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

struct StaticStruct
{
	HWND hwnd;
	int downX;
	int downY;
	int width;
	int height;
};

vector<StaticStruct> hStatics;
TCHAR szCoordinates[128];
HINSTANCE hInst;

int DownX, DownY;
int UpX, UpY;
int width, height;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0);
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
			HWND hStatic = CreateWindowEx(0, TEXT("STATIC"), 0, 
				WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | WS_EX_CLIENTEDGE, 
				DownX, DownY, width, height, hWnd, 0, hInst, 0);
			hStatics.push_back({ hStatic, DownX, DownY, width, height });
		}
		return TRUE;

	case WM_RBUTTONDOWN:
		DownX = LOWORD(lParam);
		DownY = HIWORD(lParam);

		for (int i = hStatics.size() - 1; i >= 0; --i)
		{
			const StaticStruct& staticInfo = hStatics[i];
			if (DownX >= staticInfo.downX && DownX <= staticInfo.downX + staticInfo.width &&
				DownY >= staticInfo.downY && DownY <= staticInfo.downY + staticInfo.height)
			{
				wsprintf(szCoordinates, TEXT("[Static ID:%d] [X: %d] [Y: %d] [Width: %d] [Height: %d]"), 
					i, staticInfo.downX, staticInfo.downY, staticInfo.width, staticInfo.height);
				SetWindowText(hWnd, szCoordinates);
				break;
			}
			else
			{
				SetWindowText(hWnd, L"");
			}
		}
		return TRUE;

	case WM_RBUTTONDBLCLK:
		DownX = LOWORD(lParam);
		DownY = HIWORD(lParam);

		for (int i = hStatics.size() - 1; i >= 0; --i)
		{
			const StaticStruct& staticInfo = hStatics[i];
			if (DownX >= staticInfo.downX && DownX <= staticInfo.downX + staticInfo.width &&
				DownY >= staticInfo.downY && DownY <= staticInfo.downY + staticInfo.height)
			{
				DestroyWindow(hStatics[i].hwnd);
				hStatics.erase(hStatics.begin() + i);
				break;
			}
		}
		return TRUE;
	}
	return FALSE;
}