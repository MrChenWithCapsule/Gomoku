#pragma once
#include "ChessBroad.h"
class Player
{
  public:
    // Get the current player's decision.
    virtual Position get_pos() = 0;

    // Infrom the current pleyer of the other player's step.
    virtual void update(Position last) = 0;

    virtual ~Player()
    {
    }
};