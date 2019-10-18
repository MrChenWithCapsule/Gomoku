#include <iostream>

#include "HumanPlayer.h"

HumanPlayer::HumanPlayer(bool is_first) : _is_first{is_first}
{
}
Position HumanPlayer::get_pos(const ChessBroad &current, Position last)
{
    int r, c;
    char sep;
    std::cin >> r >> sep >> c;
    return {r - 1, c - 1};
}