#pragma once
#include "ChessBroad.h"
#include "Player.h"

class ComputerPlayer : public Player
{
    Position get_position(Position last_position) override;
};