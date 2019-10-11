#pragma once
#include "ChessBroad.h"
class Player
{
public:
    virtual Position get_position(Position last_position) = 0;
};