#define _CRT_SECURE_NO_WARNINGS
#define _UNICODE
#include <iostream>
#include <tchar.h>
#include <windows.h>
using namespace std;

void Task1()
{
	_TCHAR str[] = _TEXT("H e l l o");
	wcout << "Before: " << str << endl;
	for (size_t i = 0; i < wcslen(str); i++)
	{
		if (str[i] == ' ')
		{
			str[i] = '\t';
		}
	}
	wcout << "After: " << str << endl;
}

void Task2()
{
	_TCHAR str[] = _TEXT("Hello !? 123");
	wcout << "Text: " << str << endl;

	int CounterLetter = 0;
	int CounterNumber = 0;
	int CounterSymbol = 0;

	for (int i = 0; i < wcslen(str); ++i)
	{
		if (iswalpha(str[i]))
			CounterLetter++;
		else if (iswdigit(str[i]))
			CounterNumber++;
		else if (iswpunct(str[i]))
			CounterSymbol++;
	}

	wcout << "Letters: " << CounterLetter << endl;
	wcout << "Numbers: " << CounterNumber << endl;
	wcout << "Symbols: " << CounterSymbol << endl;
}

void Task3()
{
	_TCHAR str[64];
	cout << "Enter sentence: ";
	wcin.getline(str, 64);
	int num = 0;
	bool newWord = false;
	for (size_t i = 0; i < wcslen(str); i++)
	{
		if (str[i] != ' ')
		{
			if (newWord == false)
			{
				num++;
				newWord = true;
			}
		}
		else if (str[i] == ' ')
		{
			if (newWord == true)
				newWord = false;
		}
	}
	cout << "Counter words: " << num << endl;
}

void Task4()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	const int SIZE = 64;
	_TCHAR str[SIZE];
	_TCHAR letters[SIZE] = L"ауоиэы€юеЄ";

	wcout << L"¬ведите предложение с гласными буквами: ";
	wcin.getline(str, SIZE);

	int vowelCount = 0;

	for (int i = 0; i < wcslen(str); ++i)
	{
		wchar_t ch = towlower(str[i]);
		for (int j = 0; j < wcslen(letters); j++)
		{
			if (ch == letters[j])
			{
				vowelCount++;
				break;
			}
		}
	}

	wcout << L" оличество гласных букв: " << vowelCount << endl;
}

void Task5()
{
	_TCHAR str[] = _TEXT("level");
	wcout << "Text: " << str << " - ";

	int size = wcslen(str);

	for (int i = 0; i < size / 2; i++)
	{
		if (tolower(str[i]) != tolower(str[size - i - 1]))
		{
			wcout << "not a palindrome";
			return;
		}
	}
	wcout << "palindrome";
}

void Task6()
{
	_TCHAR str[] = _TEXT("Hello world");
	wcout << "Before deleting: " << str << endl;

	wcout << "Enter number position for deleting: ";
	int pos;
	cin >> pos;

	int size = wcslen(str);

	if (pos >= 0 && pos <= size)
	{
		for (int i = pos - 1; i < size - 1; i++)
		{
			str[i] = str[i + 1];
		}

		str[size - 1] = _TEXT('\0');
		wcout << "After deleting: " << str << endl;
	}
	else
	{
		wcout << "Invalid position." << endl;
	}
}

void Task7()
{
	_TCHAR str[] = _TEXT("Hello world");
	wcout << "Before deleting: " << str << endl;

	_TCHAR ch;
	wcout << "Enter symbol: ";
	wcin >> ch;

	bool isFind = 0;
	int j = 0;
	for (int i = 0; i < wcslen(str); i++)
	{
		if (str[i] != ch)
			str[j++] = str[i];
		else
			isFind = 1;
	}

	if (isFind)
	{
		str[j] = L'\0';
		wcout << "After deleting: " << str << endl;
	}
	else
		wcout << "Symbol not found";
}

void Task8()
{
	_TCHAR str[] = _TEXT("Helo world");
	wcout << "Before deleting: " << str << endl;

	_TCHAR ch;
	wcout << "Enter symbol: ";
	wcin >> ch;

	int pos;
	wcout << "Enter position: ";
	wcin >> pos;

	int size = wcslen(str);

	if (pos >= 0 && pos <= size)
	{
		_TCHAR newStr[32];
		int j = 0;

		for (int i = 0; i < pos; i++)
		{
			newStr[j++] = str[i];
		}
		newStr[j++] = ch;

		for (int i = pos; i < size; i++)
		{
			newStr[j++] = str[i];
		}

		newStr[j] = L'\0';
		wcout << "After inserting: " << newStr << endl;
	}
	else
	{
		wcout << "Invalid position." << endl;
	}
}

int main()
{
	Task1();
	Task2();
	//Task3();
	//Task4();
	//Task5();
	//Task6();
	//Task7();
	//Task8();
}