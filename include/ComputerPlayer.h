#pragma once
#include "GomokuTree.h"
#include "Player.h"

class ComputerPlayer : public Player
{
  private:
    bool _is_first;
    GomokuTree _tree;

  public:
    ComputerPlayer(bool is_first);
    Position get_pos(const ChessBroad &current, Position last) override;
};