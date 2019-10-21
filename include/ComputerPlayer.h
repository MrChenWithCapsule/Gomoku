#pragma once
#include "Player.h"

class ComputerPlayer : public Player
{
  private:
    bool _is_first;

  public:
    ComputerPlayer(bool is_first);
    Position get_pos(const ChessBroad &current, Position last) override;
};