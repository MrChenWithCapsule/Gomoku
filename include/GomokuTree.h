#include <algorithm>
#include <memory>
#include <vector>

#include "ChessBroad.h"

struct Node
{
    int score;
    int searched_depth;
    Position pos;
    std::vector<Node *> childs;

    // searched_depth:
    static constexpr int cut = -1;
    static constexpr int leaf = -2;
};

class GomokuTree
{

  public:
    using NodeAllocator = std::allocator<Node>;

    void update(Position pos);
    // Current player placed a new chess.

    void decide();
    // Try to find the best decision for the current player.

  private:
    ChessBroad current_broad;
    Node *root;
    Node *current;
    NodeAllocator alloc;
};