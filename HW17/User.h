#pragma once
#include "header.h"

class User
{
public:
    User() {}
    WCHAR Login[STR_SIZE];
    WCHAR Password[STR_SIZE];
    WCHAR FullName[STR_SIZE];
    WCHAR Age[STR_SIZE];

    bool SpaceCheck(const wchar_t* str)
    {
        while (*str)
        {
            if (*str == L' ')
            {
                return true;
            }
            ++str;
        }
        return false;
    }
    bool AgeCheck(const wchar_t* str)
    {
        while (*str)
        {
            if (!isdigit(*str))
            {
                return true;
            }
            ++str;
        }
        return false;
    }
    bool EmptyCheckReg()
    {
        if (wcslen(Login) == 0 || wcslen(Password) == 0 || wcslen(FullName) == 0 || wcslen(Age) == 0)
        {
            return true;
        }
        return false;
    }
    bool EmptyCheckLog()
    {
        if (wcslen(Login) == 0 || wcslen(Password) == 0)
        {
            return true;
        }
        return false;
    }
};