#pragma once
#include "GomokuTree.h"
#include "Player.h"

class ComputerPlayer : public Player
{
  private:
    const bool _is_first;
    GomokuTree _tree;

  public:
    ComputerPlayer(bool is_first);
    Position get_pos() override;
    void update(Position last) override;
};