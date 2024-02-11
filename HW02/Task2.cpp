#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> 
#include <tchar.h> 

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    bool IsOver = false;
    while (!IsOver)
    {
        int MinRange = 1;
        int MaxRange = 100;
        int CounterTries = 0;

        MessageBox(0, TEXT("Select a number from 1 to 100"), TEXT("Game"), MB_OK);
        bool IsFound = false;

        while (!IsFound)
        {
            int nGuessNum = (MinRange + MaxRange) / 2;
            TCHAR Text[100];

            _stprintf_s(Text, TEXT("Is your number greater than %d?"), nGuessNum);
            int Result = MessageBox(0, Text, TEXT("Game"), MB_YESNO);

            if (Result == IDYES)
                MinRange = nGuessNum + 1;
            else
                MaxRange = nGuessNum - 1;

            if (MinRange > 100 || MaxRange < 1)
            {
                MessageBox(0, TEXT("Your number is outside the range from 1 to 100."), TEXT("Game"), MB_OK);
                IsFound = true;
            }
            else if (MinRange > MaxRange)
            {
                IsFound = true;
                _stprintf_s(Text, TEXT("Your number: %d\nNumber of attempts: %d"), MinRange, CounterTries);
                MessageBox(0, Text, TEXT("Game"), MB_OK);
            }

            CounterTries++;
        }

        int Restart = MessageBox(0, TEXT("Restart game?"), TEXT("Game"), MB_YESNO);
        if (Restart != IDYES)
            IsOver = true;
    }
}