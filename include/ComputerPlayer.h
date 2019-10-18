#include "Player.h"

class ComputerPlayer : public Player
{
  public:
    Position get_pos(const ChessBroad &current, Position last) override;
};