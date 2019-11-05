#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

#include "HumanPlayer.h"

bool ignore_invalid_chars(std::istream &is)
{
    char ch;
    while (is >> ch)
        if (isdigit(ch))
        {
            is.putback(ch);
            return true;
        }
    return false;
}
HumanPlayer::HumanPlayer(bool is_first) : _is_first{is_first}
{
}
Position HumanPlayer::get_pos(const ChessBroad &current, Position last)
{
    std::string str;
    while (str.empty())
        std::getline(std::cin, str);
    std::istringstream strin{str};

    int r, c;
    if (!ignore_invalid_chars(strin))
        return {-1, -1};
    strin >> r;
    if (!ignore_invalid_chars(strin))
        return {-1, -1};
    strin >> c;

    return {r - 1, c - 1};
}