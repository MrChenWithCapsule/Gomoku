#include "ChessBroad.h"

class GomokuTree
{
  public:
    void update(Position pos, Chess ch);
    // Someone placed a new chess.

    void decide();
    // Try to find the best decision for the current player.
};