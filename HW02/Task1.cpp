#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> 
#include <tchar.h> 

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    const int ArraySize = 3;
    const int TextSize = 128;

    TCHAR TextArray[ArraySize][TextSize] = {
       _TEXT("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."),
       _TEXT("Bibendum est ultricies integer quis auctor elit sed. Risus quis varius quam quisque id diam vel."),
       _TEXT("Quis blandit turpis cursus in hac habitasse platea dictumst.")
    };

    TCHAR Text[TextSize];
    for (int i = 0; i < ArraySize; i++)
    {
        _swprintf(Text, TEXT("Page: #%d"), i + 1);
        MessageBox(0, TextArray[i], Text, MB_OK);
    }

    int Counter = 0;
    for (int i = 0; i < ArraySize; i++)
        Counter += lstrlen(TextArray[i]);
    Counter /= ArraySize;

    TCHAR BufCount[64];
    _swprintf(BufCount, TEXT("%d"), Counter);
    MessageBox(0, BufCount, TEXT("Average text by window"), MB_OK);
}