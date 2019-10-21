#pragma once
#include <cstdlib>
#include <iosfwd>

class ClearScreen
{
} clear;

std::ostream &operator<<(std::ostream &os, ClearScreen)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    return os;
}
