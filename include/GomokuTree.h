#pragma once
#include <algorithm>
#include <memory>
#include <vector>

#include "ChessBroad.h"

struct Node
{
    int score = 0;
    int searched_depth = not_searched;
    Position pos;
    std::vector<Node *> childs;

    // searched_depth:
    static constexpr int cut = -1;
    static constexpr int not_searched = 0;
};

class GomokuTree
{

  public:
    using NodeAllocator = std::allocator<Node>;
    using AllocatorTraits = std::allocator_traits<NodeAllocator>;

    GomokuTree();
    void update(Position pos);
    // Current player placed a new chess.

    Position decide();
    // Try to find the best decision for the current player.

  private:
    void find_possible_position(Node *target, int target_depth);
    void cut_childs(Node *target, Node *except);
    void search(Node *target, int target_depth, int depth_limit);
    bool is_first();

    ChessBroad _current_broad;
    Node *_current;
    NodeAllocator _alloc;

    int _current_depth = 0;
    /*
     * _current_depth % 2 == 0: first player's tern
     * else is second's.
     */
};