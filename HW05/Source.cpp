#include <Windows.h>
#include "resource.h"

HINSTANCE hInstance;

struct StaticPart
{
	int FirX;
	int FirY;
	int SecX;
	int SecY;
};

StaticPart TopLeft, TopCenter, TopRight;
StaticPart BottomLeft, BottomCenter, BottomRight;
StaticPart CenterLeft, CenterRight;

RECT WinRect;
RECT StaticRect;
HWND hStatic;
int StaticX, StaticY;
int StaticFirstXMove, StaticSecondXMove, StaticFirstYMove, StaticSecondYMove;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const int RadCurStatic = 30;
	const int SPEED = 8;

	GetWindowRect(hwnd, &WinRect);
	const int WinWidth = WinRect.right - WinRect.left;
	const int WinHeight = WinRect.bottom - WinRect.top;

	const int StaticWidth = 120;
	const int StaticHeight = 50;

	TCHAR szCoordinates[128];
	int MouseX, MouseY;
	switch (uMsg)
	{
	case WM_INITDIALOG:
		hStatic = CreateWindowEx(0, TEXT("STATIC"), TEXT("Try to catch me!"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | WS_EX_CLIENTEDGE,
			WinWidth/2-StaticWidth, WinHeight/2-StaticHeight, StaticWidth, StaticHeight, hwnd, 0, hInstance, 0);

	case WM_MOUSEMOVE:
		GetWindowRect(hStatic, &StaticRect);
		StaticX = StaticRect.left - 10;
		StaticY = StaticRect.top - 33;
		
		StaticFirstXMove = StaticX - RadCurStatic;
		StaticSecondXMove = StaticX + StaticWidth + RadCurStatic;

		StaticFirstYMove = StaticY - RadCurStatic;
		StaticSecondYMove = StaticY + StaticHeight + RadCurStatic;

		MouseX = LOWORD(lParam);
		MouseY = HIWORD(lParam);
		
		TopLeft = {
			StaticX - RadCurStatic, StaticY - RadCurStatic, 
			StaticX, StaticY
		};
		TopCenter = {
			StaticX, StaticY - RadCurStatic, 
			StaticX+StaticWidth, StaticY
		};
		TopRight = {
			StaticX + StaticWidth, StaticY - RadCurStatic, 
			StaticX + StaticWidth + RadCurStatic, StaticY
		};

		BottomLeft = {
			StaticX - RadCurStatic, StaticY + StaticHeight,
			StaticX - RadCurStatic, StaticY + StaticHeight + RadCurStatic
		};
		BottomCenter = {
			StaticX, StaticY + StaticHeight,
			StaticX + StaticWidth, StaticY + StaticHeight + RadCurStatic
		};
		BottomRight = {
			StaticX + StaticWidth, StaticY + StaticHeight,
			StaticX + StaticWidth + RadCurStatic, StaticY + StaticHeight + RadCurStatic
		};

		CenterLeft = {
			StaticX - RadCurStatic, StaticY, 
			StaticX, StaticY + StaticHeight
		};
		CenterRight = {
			StaticX + StaticWidth, StaticY,
			StaticX + StaticWidth + RadCurStatic, StaticY + StaticHeight
		};

		//Top
		if ((MouseX >= TopLeft.FirX && MouseX <= TopLeft.SecX) &&
			(MouseY >= TopLeft.FirY && MouseY <= TopLeft.SecY))
		{
			SetWindowText(hwnd, L"Direction: Down Right");
			MoveWindow(hStatic, StaticX + SPEED, StaticY + SPEED, StaticWidth, StaticHeight, TRUE);
		}
		else if ((MouseX >= TopCenter.FirX && MouseX <= TopCenter.SecX) &&
			(MouseY >= TopCenter.FirY && MouseY <= TopCenter.SecY))
		{
			SetWindowText(hwnd, L"Direction: Down");
			MoveWindow(hStatic, StaticX, StaticY + SPEED, StaticWidth, StaticHeight, TRUE);
		}
		else if ((MouseX >= TopRight.FirX && MouseX <= TopRight.SecX) &&
			(MouseY >= TopRight.FirY && MouseY <= TopRight.SecY))
		{
			SetWindowText(hwnd, L"Direction: Down Left");
			MoveWindow(hStatic, StaticX - SPEED, StaticY + SPEED, StaticWidth, StaticHeight, TRUE);
		}
		//Bottom
		else if ((MouseX >= BottomLeft.FirX && MouseX <= BottomLeft.SecX) &&
			(MouseY >= BottomLeft.FirY && MouseY <= BottomLeft.SecY))
		{
			SetWindowText(hwnd, L"Direction: Up Right");
			MoveWindow(hStatic, StaticX + SPEED, StaticY - SPEED, StaticWidth, StaticHeight, TRUE);
		}
		else if ((MouseX >= BottomCenter.FirX && MouseX <= BottomCenter.SecX) &&
			(MouseY >= BottomCenter.FirY && MouseY <= BottomCenter.SecY))
		{
			SetWindowText(hwnd, L"Direction: Up");
			MoveWindow(hStatic, StaticX, StaticY - SPEED, StaticWidth, StaticHeight, TRUE);
		}
		else if ((MouseX >= BottomRight.FirX && MouseX <= BottomRight.SecX) &&
			(MouseY >= BottomRight.FirY && MouseY <= BottomRight.SecY))
		{
			SetWindowText(hwnd, L"Direction: Up Left");
			MoveWindow(hStatic, StaticX - SPEED, StaticY - SPEED, StaticWidth, StaticHeight, TRUE);
		}
		//Center
		else if ((MouseX >= CenterLeft.FirX && MouseX <= CenterLeft.SecX) &&
			(MouseY >= CenterLeft.FirY && MouseY <= CenterLeft.SecY))
		{
			SetWindowText(hwnd, L"Direction: Right");
			MoveWindow(hStatic, StaticX + SPEED, StaticY, StaticWidth, StaticHeight, TRUE);
		}
		else if ((MouseX >= CenterRight.FirX && MouseX <= CenterRight.SecX) &&
			(MouseY >= CenterRight.FirY && MouseY <= CenterRight.SecY))
		{
			SetWindowText(hwnd, L"Direction: Left");
			MoveWindow(hStatic, StaticX - SPEED, StaticY, StaticWidth, StaticHeight, TRUE);
		}

		return TRUE;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return TRUE;
	}

	return FALSE;
}