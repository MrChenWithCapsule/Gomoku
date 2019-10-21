#pragma once
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
bool operator<(const Node &left, const Node &right)
{
    return left.pos < right.pos;
}

class GomokuTree
{

  public:
    using NodeAllocator = std::allocator<Node>;

    GomokuTree();
    void update(Position pos);
    // Current player placed a new chess.

    Position decide();
    // Try to find the best decision for the current player.

  private:
    void find_possible_position(Node *target, int target_depth);
    void cut_childs(Node *target, Node *except);

    ChessBroad _current_broad;
    Node *_root;
    Node *_current;
    NodeAllocator _alloc;
    int _node_n = 0;
    int _current_depth = 0;
};