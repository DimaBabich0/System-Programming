#define WM_ICON WM_APP
#define ID_TRAYICON WM_USER
#include "SourceDlg.h"

CSourceDlg* CSourceDlg::ptr = NULL;
CSourceDlg::CSourceDlg(void)
{
	ptr = this;
	pNID = new NOTIFYICONDATA;
}
CSourceDlg::~CSourceDlg(void)
{
	delete pNID;
}
void CSourceDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL CSourceDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	// ������� ����������� ��������� ����������
	hDialog = hwnd;

	// ������� ���������� ���������� ����������
	HINSTANCE hInst = GetModuleHandle(NULL);

	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)); // ��������� ������
	SetClassLong(hDialog, GCL_HICON, LONG(hIcon)); // ������������� ������ � ������� ���� ����������
	memset(pNID, 0, sizeof(NOTIFYICONDATA)); //��������� ���������
	pNID->cbSize = sizeof(NOTIFYICONDATA); //������ ���������
	pNID->hIcon = hIcon; //��������� ���������������� ������
	pNID->hWnd = hwnd; //���������� ����, ������� ����� �������� ������������ ���������,
	// ��������������� � ������� � ����.	
	lstrcpy(pNID->szTip, TEXT("���������")); // ���������

	pNID->uCallbackMessage = WM_ICON; // ���������������� ���������
	// ������� ���������� ���� ������������� ��� ������� ������������
	// ��������� ����, ���������� �������� �������� � ���� hWnd. ��� ���������
	// ����������, ����� ���������� "�������" ��������� � ��������������, ���
	// ����������� ������, ��� ������ ���������� ��� �������������� � �������
	// ����������. �������� ��������� wParam �������� ��� ���� �������������
	// ������ � ����, ��� ��������� �������, � �������� ��������� lParam - 
	// "�������" ��� ������������ ���������, ��������������� � ��������.
	// ������ �������: ������ ����� �� ������ � ����.

	pNID->uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO;
	// NIF_ICON - ���� hIcon �������� ���������� �������� (��������� ������� ������ � ����).
	// NIF_MESSAGE - ���� uCallbackMessage �������� ���������� ��������
	// (��������� �������� ��������� �� ������ � ����).
	// NIF_TIP - ���� szTip �������� ���������� �������� (��������� ������� ����������� ��������� ��� ������ � ����).
	// NIF_INFO - ���� szInfo �������� ���������� �������� (��������� ������� Balloon ��������� ��� ������ � ����).
	lstrcpy(pNID->szInfo, TEXT("���������� ������������� ������ ������� �������������"));
	lstrcpy(pNID->szInfoTitle, TEXT("���������!"));
	pNID->uID = ID_TRAYICON; // ��������������� ������������� ������
	return TRUE;
}

DWORD WINAPI Thread(LPVOID lp)
{
	CSourceDlg* p = (CSourceDlg*)lp;
	HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);// ������� ������ �������������
	return 0;
}

void CSourceDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}

void CSourceDlg::Cls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	if (state == SIZE_MINIMIZED)
	{
		ShowWindow(hwnd, SW_HIDE); // ������ ����
		Shell_NotifyIcon(NIM_ADD, pNID); // ��������� ������ � ����
	}
}

// ���������� ����������������� ���������
void CSourceDlg::OnTrayIcon(WPARAM wp, LPARAM lp)
{
	// WPARAM - ������������� ������
	// LPARAM - ��������� �� ���� ��� ������������ ���������
	if (lp == WM_LBUTTONDBLCLK)
	{
		Shell_NotifyIcon(NIM_DELETE, pNID); // ������� ������ �� ����
		ShowWindow(hDialog, SW_NORMAL); // ��������������� ����
		SetForegroundWindow(hDialog); // ������������� ���� �� �������� ����
	}
}

INT_PTR CALLBACK CSourceDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
		HANDLE_MSG(hwnd, WM_SIZE, ptr->Cls_OnSize);
	}
	// ���������������� ���������
	if (message == WM_ICON)
	{
		ptr->OnTrayIcon(wParam, lParam);
		return TRUE;
	}
	return FALSE;
}
