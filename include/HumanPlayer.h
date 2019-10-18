#include "Player.h"
class HumanPlayer : public Player
{
  public:
    Position get_pos(const ChessBroad &current, Position last) override;
};