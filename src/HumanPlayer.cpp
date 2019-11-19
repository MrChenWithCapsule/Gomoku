#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

#include "HumanPlayer.h"

bool ignore_invalid_chars(std::istream &is)
{
    char ch;
    while (is >> ch)
        if (isdigit(ch) != 0)
        {
            is.putback(ch);
            return true;
        }
    return false;
}
HumanPlayer::HumanPlayer(bool is_first) : _is_first{is_first}
{
}
Position HumanPlayer::get_pos()
{
    std::string str;
    std::cout << "Input row, column: " << std::flush;
    while (str.empty())
        std::getline(std::cin, str);
    std::istringstream strin{str};

    int r;
    int c;
    if (!ignore_invalid_chars(strin))
        return {-1, -1};
    strin >> r;
    if (!ignore_invalid_chars(strin))
        return {-1, -1};
    strin >> c;

    return {r - 1, c - 1};
}

void HumanPlayer::update(Position last)
{
    auto another_player_name = _is_first ? "Player 2" : "Player 1";
    std::cout << another_player_name << " placed a chess at: " << last.row + 1
              << ", " << last.column + 1 << std::endl;
}
