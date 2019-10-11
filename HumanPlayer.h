#pragma once
#include <iostream>

#include "ChessBroad.h"
#include "Player.h"

class HumanPlayer : public Player
{
    Position get_position(Position last_position) override
    {
        int r, c;
        char ch;
        std::cin >> r >> ch >> c;
        return {r - 1, c - 1};
    }
};