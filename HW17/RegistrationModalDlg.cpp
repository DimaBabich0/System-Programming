#include "RegistrationModalDlg.h"
#include "User.h"

CRegistrationModalDlg dlg;
HMODULE hMod;
HHOOK hHook;
User n;

LRESULT CALLBACK KeyboardProc(int idCode, WPARAM wParam, LPARAM lParam)
{
	if (HC_ACTION == idCode)
	{
		HWND hWnd = GetFocus();
		WCHAR buf[STR_SIZE];
		GetClassName(hWnd, buf, sizeof(buf));
		if (lstrcmpi(buf, TEXT("EDIT")) == 0 && (lParam & 0x80000000))
		{
			buf[0] = L'\0';
			if ((wParam >= 32 && wParam <= 126))
				wsprintf(buf, TEXT("%c"), wParam);

			if (hWnd == dlg.hEditLogin)
				wcscat_s(n.Login, buf);
			else if (hWnd == dlg.hEditPassword)
				wcscat_s(n.Password, buf);
			else if (hWnd == dlg.hEditFullName)
				wcscat_s(n.FullName, buf);
			else if (hWnd == dlg.hEditAge)
				wcscat_s(n.Age, buf);
		}
	}
	return CallNextHookEx(hHook, idCode, wParam, lParam);
}

CRegistrationModalDlg* CRegistrationModalDlg::ptr = NULL;
CRegistrationModalDlg::CRegistrationModalDlg(void)
{
	ptr = this;
}
CRegistrationModalDlg::~CRegistrationModalDlg(void)
{

}
void CRegistrationModalDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, IDCANCEL);
}

void MessageError(const WCHAR* text)
{
	MessageBox(0, text, TEXT("Error"), MB_OK | MB_ICONERROR);
}

BOOL CRegistrationModalDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	dlg.hEditLogin = GetDlgItem(hwnd, IDC_EDIT_REG_LOGIN);
	dlg.hEditPassword = GetDlgItem(hwnd, IDC_EDIT_REG_PASSWORD);
	dlg.hEditFullName = GetDlgItem(hwnd, IDC_EDIT_REG_FULL_NAME);
	dlg.hEditAge = GetDlgItem(hwnd, IDC_EDIT_REG_AGE);
	hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL, GetCurrentThreadId());
	n.Login[0] = L'\0';
	n.Password[0] = L'\0';
	n.FullName[0] = L'\0';
	n.Age[0] = L'\0';
	return TRUE;
}

void CRegistrationModalDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUT_REG_ENTER)
	{
		if (n.SpaceCheck(n.Login) || n.SpaceCheck(n.Password))
			MessageError(TEXT("Fields \"Login\" and \"Password\" shouldn't have spaces"));
		else if (n.EmptyCheckReg())
			MessageError(TEXT("All fields must be filled"));
		else if (n.AgeCheck(n.Age))
			MessageError(TEXT("The field \"Age\" should be a number"));
		else
		{
			bool isLoginFound = false;

			wifstream file("data.txt");
			if (!file.is_open())
			{
				wofstream createDataFile("data.txt");
			}
			wstring line;
			while (getline(file, line))
			{
				size_t pos = line.find(L' ');
				if (pos != wstring::npos)
				{
					wstring dataLogin = line.substr(0, pos);
					
					if (dataLogin == n.Login)
					{
						MessageError(TEXT("Account with this login already exists"));
						isLoginFound = true;
						file.close();
						return;
					}
				}
			}
			file.close();

			if (!isLoginFound)
			{
				wofstream out("data.txt", fstream::app);
				if (out.is_open())
				{
					out << n.Login << " " << n.Password << " " << n.FullName << " " << n.Age << endl;
					out.close();
					MessageBox(hwnd, TEXT("Account created successfully"), TEXT("Registration"), MB_OK | MB_ICONINFORMATION);
					EndDialog(hwnd, IDCANCEL);
					wcout << n.Login << " " << n.Password << " " << n.FullName << " " << n.Age << endl;
				}
				else
				{
					MessageError(TEXT("File with account data cannot be opened"));
				}
			}
		}
	}
	else if (id == IDC_BUT_REG_CANCEL)
		EndDialog(hwnd, IDCANCEL);
}

INT_PTR CALLBACK CRegistrationModalDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}
