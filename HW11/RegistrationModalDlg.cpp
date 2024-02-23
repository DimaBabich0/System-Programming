#include "RegistrationModalDlg.h"
#include "User.h"

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

BOOL CRegistrationModalDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	return TRUE;
}

void CRegistrationModalDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDOK)
	{
		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
		HWND hEditFullName = GetDlgItem(hwnd, IDC_EDIT_FULL_NAME);
		HWND hEditAge = GetDlgItem(hwnd, IDC_EDIT_AGE);

		User n;
		GetWindowText(hEditLogin, n.Login, STR_SIZE);
		GetWindowText(hEditPassword, n.Password, STR_SIZE);
		GetWindowText(hEditFullName, n.FullName, STR_SIZE);
		GetWindowText(hEditAge, n.Age, STR_SIZE);

		if (n.SpaceCheck(n.Login) || n.SpaceCheck(n.Password))
		{
			MessageBox(hwnd, TEXT("Логин и пароль не должны иметь пробелы"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		}
		else if (n.EmptyCheckReg())
		{
			MessageBox(hwnd, TEXT("Все строки должны быть заполнены"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		}
		else if (n.AgeCheck(n.Age))
		{
			MessageBox(hwnd, TEXT("Строка \"возраст\" должна быть числом"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		}
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
						MessageBox(hwnd, TEXT("Аккаунт с таким логином уже существует"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
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
					MessageBox(hwnd, TEXT("Аккаунт успешно создан"), TEXT("Регистрация"), MB_OK | MB_ICONINFORMATION);
					EndDialog(hwnd, IDCANCEL);
					wcout << n.Login << " " << n.Password << " " << n.FullName << " " << n.Age << endl;
				}
				else
				{
					MessageBox(hwnd, TEXT("Файл с данными аккунтов невозможно открыть"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
				}
			}
		}
	}
	else if (id == IDNO)
	{
		EndDialog(hwnd, IDCANCEL);
	}
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
