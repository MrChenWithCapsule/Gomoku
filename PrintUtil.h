#pragma once
#include <cstdlib>

#include "ChessBroad.h"

struct PrintBroad
{
    BroadType &broad;
};
inline std::ostream &operator<<(std::ostream &os, PrintBroad &printer)
{
    os << R"(
   c 1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
  r|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
)";
    for (int i = 0; i < broad_size; ++i)
    {
        os << std::setw(3) << i + 1 << '|';
        for (char ch : printer.broad[i])
            os << ' ' << ch << " |";
        os << "\n   |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|\n";
    }
    os << std::flush;
    return os;
}

struct ClearScreen
{
};
inline std::ostream &operator<<(std::ostream &os, ClearScreen)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    return os;
}