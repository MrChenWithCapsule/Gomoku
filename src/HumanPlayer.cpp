#include <iostream>

#include "HumanPlayer.h"

Position HumanPlayer::get_pos(const ChessBroad &current, Position last)
{
    int r, c;
    char sep;
    std::cin >> r >> sep >> c;
    return {r - 1, c - 1};
}