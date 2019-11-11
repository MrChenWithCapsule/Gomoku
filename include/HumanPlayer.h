#pragma once
#include "Player.h"
class HumanPlayer : public Player
{
  private:
    bool _is_first;

  public:
    HumanPlayer(bool is_first);
    Position get_pos() override;
    void update(Position last) override;
};