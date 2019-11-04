#pragma once
#include "ChessBroad.h"
class Player
{
  public:
    // Get the current player's decision.
    virtual Position get_pos(const ChessBroad &current, Position last) = 0;
    virtual ~Player()
    {
    }
};