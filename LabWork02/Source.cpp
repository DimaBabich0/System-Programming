#include <Windows.h>
#include "resource.h"
#include <tchar.h>
#include <string>
using namespace std;

HINSTANCE hInstance;

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	::hInstance = hInstance;
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const int SIZE_ARR = 4;
	static HWND hCheckBoxes[SIZE_ARR];
	HWND hStaticInfo = GetDlgItem(hwnd, IDC_STATIC_INFO);

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		hCheckBoxes[0] = GetDlgItem(hwnd, IDC_RADIO1);
		hCheckBoxes[1] = GetDlgItem(hwnd, IDC_RADIO2);
		hCheckBoxes[2] = GetDlgItem(hwnd, IDC_RADIO3);
		hCheckBoxes[3] = GetDlgItem(hwnd, IDC_RADIO4);

		SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, BST_CHECKED, 0);
		break;
	}

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED)
		{
			TCHAR FirstText[32], SecondText[32];
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT1), FirstText, 32);
			GetWindowText(GetDlgItem(hwnd, IDC_EDIT2), SecondText, 32);
			if (lstrlen(FirstText) == NULL || lstrlen(SecondText) == NULL)
			{
				MessageBox(hwnd, L"Edit boxes are empty", L"Error", MB_OK | MB_ICONWARNING);
				break;
			}

			int num1 = GetDlgItemInt(hwnd, IDC_EDIT1, NULL, FALSE);
			int num2 = GetDlgItemInt(hwnd, IDC_EDIT2, NULL, FALSE);
			int res;

			if (SendMessage(hCheckBoxes[0], BM_GETCHECK, 0, 0) == BST_CHECKED)
				res = num1 + num2;
			else if (SendMessage(hCheckBoxes[1], BM_GETCHECK, 0, 0) == BST_CHECKED)
				res = num1 - num2;
			else if (SendMessage(hCheckBoxes[2], BM_GETCHECK, 0, 0) == BST_CHECKED)
				res = num1 * num2;
			else if (SendMessage(hCheckBoxes[3], BM_GETCHECK, 0, 0) == BST_CHECKED)
			{
				if(num2 != 0)
					res = num1 / num2;
				else
				{
					MessageBox(hwnd, L"Can't divide to zero", L"Error", MB_OK | MB_ICONWARNING);
					break;
				}
			}
			TCHAR buf[32];
			wsprintf(buf, TEXT("%d"), res);
			SetWindowText(hStaticInfo, buf);
		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return TRUE;
	}

	return FALSE;
}